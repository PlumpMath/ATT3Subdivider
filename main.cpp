#include <iostream>

#include "PolyVector.h"
#include "BezReader.h"



using namespace std;


int main(int argc, char* argv[])
{
	BezReader bezReader;
	
	bezReader.ReadBezFile(argv[1]);

	

	//test 
	PolyVector teapot;


	//bezReader.BuildPolyVector_Uniform(teapot,0.1);

	bezReader.BuildPolyVector_Adaptive(teapot,0.01,0.1);




	return 0;
}