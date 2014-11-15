#pragma once

#include "Patch.h"

#include <vector>

using namespace std;

class BezReader
{
protected:
	int num_patches;
	vector<Patch> vec_patches;	//patches

public:

	void ReadBezFile(const char * filename);


	void BuildPolyVector_Uniform(PolyVector & polyVector,float step);
	void BuildPolyVector_Adaptive(PolyVector & polyVector,float threshold,float step);

};