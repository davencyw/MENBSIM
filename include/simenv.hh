#ifndef __SIMENV_HH__
#define __SIMENV_HH__

#include <string>

struct SimEnv
{
		
	//physical environment
	//____________________

	//timestep
	double _dt;
	//number of steps
	int _nsteps;


	//computing environment
	//____________________
	
	//number of threads
	int _nthreads;
	//omp scheduling
	int _scheduling;
	//cuda enabled
	bool _cuda;


	//data environment
	//____________________

};

#endif //__SIMENV_HH__