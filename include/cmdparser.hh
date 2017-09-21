#include <cstdlib>
#include <string>
#include "boost/program_options.hpp"

#include "simenv.hh"
#include "global.hh"

void cmdpars(int argc, char const* argv[], SimEnv& simenv){


	//set default simenv
	simenv._N = N;
	simenv._dt = DT;
	simenv._nThreads = NTHREADS;
	simenv._scheduling = SCHEDULE;
	simenv._cuda = CUDA;


	//BOOST PRORGAM OPTIONS
	namespace po = boost::program_options;
	po::options_description desc("Parameters");
	desc.add_options() 
      //("help", "Print help messages") 
      //(",N", po::value<int>(&(simenv._N)) ,"number of bodies") 
      //(",t", po::value<double>(&(simenv._dt))->required() ,"timestep of simulation")
      //("xbod,x", po::value<std::string>(&(simenv._xbodpath))->required(), "file to x coordinates array")
      ("nthreads,n", po::value<int>(&(simenv._nThreads)) ,"number of threads [optional]")
      ("schedule,s", po::value<int>(&(simenv._scheduling)) ,"omp scheduling [optional]")
      ("cuda", "enable cuda support [optional]");


    po::variables_map vm; 
    try 
    { 
      po::store(po::parse_command_line(argc, argv, desc),vm); // can throw 
 
      /** --help option 
       */ 
      if (vm.count("help")) 
      { 
        std::cout << __P_NAME << std::endl 
                  << desc << std::endl; 
        exit(0);
      }
      
      if(vm.count("cuda"))
        simenv._cuda = true;

      po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    } 
    catch(po::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      std::cerr << desc << std::endl; 
      exit(1); 
    } 


    //sanity check input

	//END BOOST PROGRAM OPTIONS

}
