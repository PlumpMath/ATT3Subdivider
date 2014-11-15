#pragma once

#include "Eigen/Dense"

#include <vector>

#include "PolyVector.h"

using namespace std;


#define NUM_CP_AXIS (4)
#define EPSILON (1.1921e-7)

//enum CurveDirection
//{
//	U_Dir,V_Dir
//};


class Patch
{
protected:
	//void bezcurveInterp_(Eigen::Vector3f curve[], float u,Eigen::Vector3f& point, Eigen::Vector3f& dpdu);
	void bezcurveInterp_U(int id, float u, Eigen::Vector3f& p, Eigen::Vector3f& dpdu);
	void bezcurveInterp_V(int id, float u, Eigen::Vector3f& p, Eigen::Vector3f& dpdu);

	void bezcurveInterp(Eigen::Vector3f* curve[], float u,Eigen::Vector3f& point, Eigen::Vector3f& dpdu);

	void bezpatchInterp(float u,float v,Eigen::Vector3f& p, Eigen::Vector3f& n);

	void triangleAdaptiveTessllate(const Eigen::Vector3f & a,const Eigen::Vector3f & an, float ua,float va
									  ,const Eigen::Vector3f & b,const Eigen::Vector3f & bn, float ub,float vb
									  ,const Eigen::Vector3f & c,const Eigen::Vector3f & cn, float uc,float vc
									  ,float threshold, PolyVector & polyVector);
public:
	//vector<Eigen::Vector3f> vec_cp;
	Eigen::Vector3f cp[NUM_CP_AXIS][NUM_CP_AXIS];	


	void uniformSubdividePatch(float step, PolyVector & polyVector);

	void adaptiveSubdividePatch(float step,float threshold, PolyVector & polyVector);



	
};