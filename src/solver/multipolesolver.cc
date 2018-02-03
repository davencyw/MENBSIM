#include "solver/forcesolver.hh"

#include "benchdec.hh"
#include "global.hh"
#include "octree/octree.hh"
#include "octree/octreenode.hh"

#include <stack>

void Multipolesolver::solve(const unsigned int numparticles,
                            const array_t& xpos, const array_t& ypos,
                            const array_t& zpos, const array_t& masses,
                            array_t* forcex, array_t* forcey, array_t* forcez,
                            const precision_t softening, const Extent extent) {
  // create tree first
  CCPP::BENCH::start(B_TREEGEN);
  _xpos = xpos;
  _ypos = ypos;
  _zpos = zpos;
  _masses = masses;
  _extent = extent;
  _numparticles = numparticles;
  _forcex = forcex;
  _forcey = forcey;
  _forcez = forcez;
  createTree(_simenv._octreeleafnodesize);
  CCPP::BENCH::stop(B_TREEGEN);

  CCPP::BENCH::start(B_MULTIPOLE);
  getforceonparticles();
  CCPP::BENCH::stop(B_MULTIPOLE);

  // destroy tree
  CCPP::BENCH::start(B_TREEGEN);
  delete _octree;
  CCPP::BENCH::stop(B_TREEGEN);
}

void Multipolesolver::getforceonparticles() {
  const std::array<oct::Octreenode*, 8> rootchildren(
      *_octree->getroot()->getchildren());
  std::stack<const oct::Octreenode*> rootnodestoprocess;
  for (size_t toplevelnode_i = 0; toplevelnode_i < 8; toplevelnode_i++) {
    rootnodestoprocess.push(rootchildren[toplevelnode_i]);
  }

  // TODO(dave): check this omp implementation
  //#pragma omp parallel for private(nodestoprocess)
  for (unsigned particle_i = 0; particle_i < _numparticles; ++particle_i) {
    std::cout << "pi: " << particle_i << "\n";
    // iterate over highest possible node for each particle
    std::stack<const oct::Octreenode*> nodestoprocess = rootnodestoprocess;
    while (!nodestoprocess.empty()) {
      const oct::Octreenode* currentnode(nodestoprocess.top());
      nodestoprocess.pop();
      if (currentnode->isleaf()) {
        // get direct force evaluations from particles in this leafnode
        getdirectforce(particle_i, currentnode);
      } else {
        // get opening angle
        const precision_t halfwidth(currentnode->gethalfwidth());
        const unsigned int nodedataindex(currentnode->getdataindex());
        const Eigen::Vector3d particle_i_to_nodecom(
            _nodecomx(nodedataindex) - _xpos(particle_i),
            _nodecomy(nodedataindex) - _ypos(particle_i),
            _nodecomz(nodedataindex) - _zpos(particle_i));
        const precision_t openingangle(
            2.0 * std::atan2(halfwidth, particle_i_to_nodecom.norm()));
        if (std::abs(openingangle) < 0) {
          // use this nodes expansion to get force
          // SSA
          // monopole force

          const precision_t distance(particle_i_to_nodecom.norm());
          const precision_t distance3(distance * distance * distance);
          const precision_t distance4(distance3 * distance);
          const precision_t distance3inverse(1.0 / distance3);
          const precision_t distance4inverse(1.0 / distance4);
          const precision_t massdistance3inverse(_masses(particle_i) *
                                                 distance3inverse);
          const precision_t monopoleforcex(-massdistance3inverse *
                                           particle_i_to_nodecom(0) *
                                           _monopole(nodedataindex));
          const precision_t monopoleforcey(-massdistance3inverse *
                                           particle_i_to_nodecom(1) *
                                           _monopole(nodedataindex));
          const precision_t monopoleforcez(-massdistance3inverse *
                                           particle_i_to_nodecom(2) *
                                           _monopole(nodedataindex));

          // quadrapole force
          const mat33_t localquadrapolemoment(_quadrapole[nodedataindex]);
          const Eigen::Vector3d qr(localquadrapolemoment *
                                   particle_i_to_nodecom);
          const precision_t rqr(particle_i_to_nodecom.transpose() * qr);
          const Eigen::Vector3d quadrapolemagnitude =
              (-5.0 / 2.0 * rqr * distance4inverse) * particle_i_to_nodecom;
          const Eigen::Vector3d qaudrapoleforcevector =
              qr * distance4inverse + quadrapolemagnitude;

          const precision_t quadrapoleforcex(qaudrapoleforcevector(0));
          const precision_t quadrapoleforcey(qaudrapoleforcevector(1));
          const precision_t quadrapoleforcez(qaudrapoleforcevector(2));

          // write back to force vector
          (*_forcex)(particle_i) += monopoleforcex + quadrapoleforcex;
          (*_forcey)(particle_i) += monopoleforcey + quadrapoleforcey;
          (*_forcez)(particle_i) += monopoleforcez + quadrapoleforcez;

        } else {
          // push back childnodes of currentnode
          std::array<oct::Octreenode*, 8> currentnodechildren(
              *currentnode->getchildren());
          for (size_t child_i = 0; child_i < 8; child_i++) {
            nodestoprocess.push(currentnodechildren[child_i]);
          }
        }
      }
    }
  }
}

void Multipolesolver::getdirectforce(const unsigned int particle_i,
                                     const oct::Octreenode* currentnode) {
  const std::vector<unsigned int>* particlesinnodeindices(
      currentnode->getindices());
  for (unsigned int particle_j : *particlesinnodeindices) {
    // Get force from particle_j onto particle_i
    // SSA
    if (particle_j != particle_i) {
      const precision_t xj(_xpos(particle_j));
      const precision_t yj(_ypos(particle_j));
      const precision_t zj(_zpos(particle_j));

      const precision_t xi(_xpos(particle_i));
      const precision_t yi(_ypos(particle_i));
      const precision_t zi(_zpos(particle_i));

      const precision_t xjxi(xi - xj);
      const precision_t yjyi(yi - yj);
      const precision_t zjzi(zi - zj);

      const precision_t rmagnitude(
          std::sqrt(xjxi * xjxi + yjyi * yjyi + zjzi * zjzi +
                    _simenv._softeningparam * _simenv._softeningparam));

      const precision_t m1(_masses(particle_i));
      const precision_t m2(_masses(particle_j));

      const precision_t forcemagnitude(m1 * m2 /
                                       (rmagnitude * rmagnitude * rmagnitude));

      const precision_t fx(forcemagnitude * xjxi);
      const precision_t fy(forcemagnitude * yjyi);
      const precision_t fz(forcemagnitude * zjzi);

      (*_forcex)(particle_i) += fx;
      (*_forcey)(particle_i) += fy;
      (*_forcez)(particle_i) += fz;

      std::cout << particle_i << "\t" << particle_j << "\t\t" << fx << "\n";
    }
  }
  std::cout << "\n";
}

void Multipolesolver::createTree(const unsigned int leafnodesize) {
  _octree = new oct::Octree(_extent, _xpos, _ypos, _zpos, leafnodesize);
  _octree->init();
  multipoleExpansion();
}

void Multipolesolver::multipoleExpansion() {
  // create multipole datacontainer
  const unsigned int numnodes(_octree->getnumnodes());
  _monopole = array_t(numnodes);
  _quadrapole = std::vector<mat33_t>(numnodes);
  _nodecomx = array_t(numnodes);
  _nodecomy = array_t(numnodes);
  _nodecomz = array_t(numnodes);
  _monopole.setZero();

  // get leafnodes
  std::vector<const oct::Octreenode*> leafnodes(_octree->getleafnodes());

// compute leafnode multipole expansions
#pragma omp parallel for
  for (unsigned int leafnode_i = 0; leafnode_i < leafnodes.size();
       ++leafnode_i) {
    const oct::Octreenode* leafnode(leafnodes[leafnode_i]);
    const std::vector<unsigned int>* leafnodeindicesinposarray(
        leafnode->getindices());
    const unsigned int leafnodedataindex(leafnode->getdataindex());

    precision_t comx(0.0);
    precision_t comy(0.0);
    precision_t comz(0.0);
    precision_t totalemass(0.0);

    mat33_t quadrapole;
    quadrapole.setZero();

    // leafnode monopole and com
    for (unsigned int index : *leafnodeindicesinposarray) {
      // SSA
      const precision_t mass_i(_masses(index));
      const precision_t xpos(_xpos(index));
      const precision_t ypos(_ypos(index));
      const precision_t zpos(_zpos(index));
      totalemass += mass_i;
      comx += mass_i * xpos;
      comy += mass_i * ypos;
      comz += mass_i * zpos;
    }
    const precision_t totalmassinverse(1.0 / totalemass);
    _monopole(leafnodedataindex) = totalemass;
    _nodecomx(leafnodedataindex) = comx * totalmassinverse;
    _nodecomy(leafnodedataindex) = comy * totalmassinverse;
    _nodecomz(leafnodedataindex) = comz * totalmassinverse;

    getquadrapole(leafnode, quadrapole);

    _quadrapole[leafnodedataindex] = quadrapole;
  }

  const std::array<oct::Octreenode*, 8>* rootchildren(
      _octree->getroot()->getchildren());

#pragma omp parallel for
  for (unsigned int toplevelnode_i = 0; toplevelnode_i < 8; toplevelnode_i++) {
    expandmoments((*rootchildren)[toplevelnode_i]);
  }
}

void Multipolesolver::expandmoments(const oct::Octreenode* const node) {
  if (node->isleaf()) {
    return;
  } else {
    const unsigned int nodedataindex(node->getdataindex());
    const std::array<oct::Octreenode*, 8>* children(node->getchildren());

    precision_t comx(0.0);
    precision_t comy(0.0);
    precision_t comz(0.0);
    precision_t totalemass(0.0);

    for (unsigned int child_i = 0; child_i < 8; child_i++) {
      const oct::Octreenode* childnode_i((*children)[child_i]);
      expandmoments(childnode_i);

      // propagate from child_i to node
      const unsigned int childdataindex(childnode_i->getdataindex());
      // monopole and com
      const precision_t mass_i(_monopole(childdataindex));
      totalemass += mass_i;
      // com of node
      comx += mass_i * _nodecomx(childdataindex);
      comy += mass_i * _nodecomy(childdataindex);
      comz += mass_i * _nodecomz(childdataindex);
    }

    const precision_t totalmassinverse(1.0 / totalemass);
    _monopole(nodedataindex) = totalemass;
    _nodecomx(nodedataindex) = comx * totalmassinverse;
    _nodecomy(nodedataindex) = comy * totalmassinverse;
    _nodecomz(nodedataindex) = comz * totalmassinverse;

    mat33_t localquadrapolemoment = mat33_t::Zero();
    for (unsigned int child_i = 0; child_i < 8; child_i++) {
      const oct::Octreenode* childnode_i((*children)[child_i]);
      const unsigned int childdataindex(childnode_i->getdataindex());

      // get quadrapolemoment recursively!
      Eigen::Vector3d displacement(_nodecomx(childdataindex) - comx,
                                   _nodecomy(childdataindex) - comy,
                                   _nodecomz(childdataindex) - comz);
      mat33_t compositemoment =
          (3.0 * displacement * displacement.transpose() -
           ((displacement.norm() * displacement.norm() * mat33_t::Identity()) *
            _monopole(childdataindex)));

      // quadrapole
      localquadrapolemoment += _quadrapole[childdataindex] + compositemoment;
    }
  }
}

void Multipolesolver::getquadrapole(const oct::Octreenode* node,
                                    mat33_t& quadrapole) {
  const unsigned int nodedataindex(node->getdataindex());
  const std::vector<unsigned int>* nodeindicesinposarray(node->getindices());

  const precision_t comx(_nodecomx(nodedataindex));
  const precision_t comy(_nodecomy(nodedataindex));
  const precision_t comz(_nodecomz(nodedataindex));

  for (unsigned int index : *nodeindicesinposarray) {
    const precision_t mass_i(_masses(index));
    // shifted position, com frame
    const precision_t xpos(_xpos(index) - comx);
    const precision_t ypos(_ypos(index) - comy);
    const precision_t zpos(_zpos(index) - comz);

    // node quadrapole
    const precision_t xpos2(xpos * xpos);
    const precision_t ypos2(ypos * ypos);
    const precision_t zpos2(zpos * zpos);
    const precision_t length(std::sqrt(xpos2 + ypos2 + zpos2));
    const precision_t s00(3.0 * (xpos2)-length);
    const precision_t s11(3.0 * (ypos2)-length);
    const precision_t s22(3.0 * (ypos2)-length);
    quadrapole(0, 0) += s00;
    quadrapole(1, 1) += s11;
    quadrapole(2, 2) += s22;

    const precision_t s10(3.0 * (xpos * ypos));
    const precision_t s20(3.0 * (xpos * zpos));
    const precision_t s21(3.0 * (zpos * ypos));
    quadrapole(1, 0) += s10;
    quadrapole(0, 1) += s10;
    quadrapole(2, 0) += s20;
    quadrapole(0, 2) += s20;
    quadrapole(2, 1) += s21;
    quadrapole(1, 2) += s21;

    quadrapole *= mass_i;
  }
}
