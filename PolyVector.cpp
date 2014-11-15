#include "PolyVector.h"

PolyVector::PolyVector()
	:cur_poly_id(-1)
{
	//TODO
}


//PolyVector* PolyVector::getInstance()
//{
//	static PolyVector instance;
//	return &instance;
//}

void PolyVector::nextPoly()
{
	cur_poly_id++;
	vec_poly.push_back( vector<LocalGeo>() );
}



void PolyVector::pushBackVertex(const LocalGeo & local)
{
	vec_poly.at( cur_poly_id ).push_back(local);
}