#pragma once

#include <vector>
//#include "Eigen/Dense"

#include "LocalGeo.h"

using namespace std;

//#define pPolyVector PolyVector::getInstance()

class PolyVector
{
protected:
	vector< vector<LocalGeo> >  vec_poly;

	int cur_poly_id;
public:
	PolyVector();
	//PolyVector* getInstance();
	void nextPoly();
	void pushBackVertex(const LocalGeo & local);


	int getPolySize()
	{
		return vec_poly.size();
	}
	int getVertexSize(int i)
	{
		return vec_poly.at(i).size();
	}

	//naive method
	LocalGeo& getVertex(int i,int j)
	{
		return vec_poly.at(i).at(j);
	}
};

