#include "Patch.h"

void Patch::bezcurveInterp_U(int id, float u, Eigen::Vector3f& p, Eigen::Vector3f& dpdu)
{
	Eigen::Vector3f* curve[NUM_CP_AXIS];
	for (int i = 0; i < NUM_CP_AXIS; i++)
	{
		curve[i] = &(cp[id][i]);
	}
	bezcurveInterp(curve,u,p,dpdu);
}

void Patch::bezcurveInterp_V(int id, float u, Eigen::Vector3f& p, Eigen::Vector3f& dpdu)
{
	Eigen::Vector3f* curve[NUM_CP_AXIS];
	for (int i = 0; i < NUM_CP_AXIS; i++)
	{
		curve[i] = &(cp[i][id]);
	}
	bezcurveInterp(curve,u,p,dpdu);
}



void Patch::bezcurveInterp(Eigen::Vector3f* curve[],float u,Eigen::Vector3f& p,Eigen::Vector3f& dpdu)
{
	//interp
	Eigen::Vector3f A(  (*curve[0])*(1.0f-u) + (*curve[1])*u );
	Eigen::Vector3f B(  (*curve[1])*(1.0f-u) + (*curve[2])*u );
	Eigen::Vector3f C(  (*curve[2])*(1.0f-u) + (*curve[3])*u );


	Eigen::Vector3f D(  A*(1.0f-u) + B*u );
	Eigen::Vector3f E(  B*(1.0f-u) + C*u );

	p = D*(1.0f - u) + E*u;

	dpdu = 3 * (E-D);

}


void Patch::bezpatchInterp(float u,float v,Eigen::Vector3f& p, Eigen::Vector3f& n)
{
	Eigen::Vector3f tmp_d;

	Eigen::Vector3f vcurve[NUM_CP_AXIS];
	bezcurveInterp_V(0,u,vcurve[0],tmp_d);
	bezcurveInterp_V(1,u,vcurve[1],tmp_d);
	bezcurveInterp_V(2,u,vcurve[2],tmp_d);
	bezcurveInterp_V(3,u,vcurve[3],tmp_d);

	Eigen::Vector3f ucurve[NUM_CP_AXIS];
	bezcurveInterp_U(0,v,ucurve[0],tmp_d);
	bezcurveInterp_U(1,v,ucurve[1],tmp_d);
	bezcurveInterp_U(2,v,ucurve[2],tmp_d);
	bezcurveInterp_U(3,v,ucurve[3],tmp_d);


	Eigen::Vector3f dpdv;
	Eigen::Vector3f dpdu;

	Eigen::Vector3f* tmp_v_p[NUM_CP_AXIS] = {&vcurve[0],&vcurve[1],&vcurve[2],&vcurve[3]};
	bezcurveInterp(tmp_v_p,v,p,dpdv);

	Eigen::Vector3f* tmp_u_p[NUM_CP_AXIS] = {&ucurve[0],&ucurve[1],&ucurve[2],&ucurve[3]};
	bezcurveInterp(tmp_u_p,u,p,dpdu);

	n = dpdu.cross(dpdv);
	n.normalize();
}


void Patch::uniformSubdividePatch(float step, PolyVector & polyVector)
{
	//uniform divide

	int numdiv = (1.0f + EPSILON) / step;

	float u,v;
	for (int iu = 0; iu < numdiv; iu++)
	{
		u = iu * step;
		for (int iv = 0; iv < numdiv; iv++)
		{
			v = iv * step;

			
			//uniform subdivision
			Eigen::Vector3f p00,p01,p11,p10;
			Eigen::Vector3f n00,n01,n11,n10;
			bezpatchInterp(u,v,p00,n00);
			bezpatchInterp(u+step,v,p01,n01);
			bezpatchInterp(u+step,v+step,p11,n11);
			bezpatchInterp(u,v+step,p10,n10);


			//push that square to vector
			polyVector.nextPoly();
			polyVector.pushBackVertex(LocalGeo(p00,n00));
			polyVector.pushBackVertex(LocalGeo(p01,n01));
			polyVector.pushBackVertex(LocalGeo(p11,n11));
			polyVector.pushBackVertex(LocalGeo(p10,n10));
		}
	}
}





void Patch::adaptiveSubdividePatch(float step,float threshold, PolyVector & polyVector)
{
	//adaptive Tessellation based on triangles
	int numdiv = (1.0f + EPSILON) / step;

	float u,v;
	for (int iu = 0; iu < numdiv; iu++)
	{
		u = iu * step;
		for (int iv = 0; iv < numdiv; iv++)
		{
			v = iv * step;

			//adaptive tessllation based on triangles
			Eigen::Vector3f p00,p01,p11,p10;
			Eigen::Vector3f n00,n01,n11,n10;
			bezpatchInterp(u,v,p00,n00);
			bezpatchInterp(u+step,v,p01,n01);
			bezpatchInterp(u+step,v+step,p11,n11);
			bezpatchInterp(u,v+step,p10,n10);


			
			triangleAdaptiveTessllate
				(p00,n00,u,v
				,p01,n01,u+step,v
				,p10,n10,u,v+step
				,threshold,polyVector);

			triangleAdaptiveTessllate
				(p11,n11,u+step,v+step
				,p01,n01,u+step,v
				,p10,n10,u,v+step
				,threshold,polyVector);
		}
	}
}





void Patch::triangleAdaptiveTessllate(const Eigen::Vector3f & a,const Eigen::Vector3f & an, float ua,float va
									  ,const Eigen::Vector3f & b,const Eigen::Vector3f & bn, float ub,float vb
									  ,const Eigen::Vector3f & c,const Eigen::Vector3f & cn, float uc,float vc
									  //,float u,float v			//this is (u,v) for midpoint on diagonal
									  //,int tri_dir						//direction of triangle
									  ,float threshold, PolyVector & polyVector)
{
	// a is supposed to be big(right) angle
	// a (u,v)
	// b (u+step,v)
	// c (u,v+step)

	float u[3] = {(ub+uc)/2,(ua+ub)/2,(ua+uc)/2};
	float v[3] = {(vb+vc)/2,(va+vb)/2,(va+vc)/2};
	Eigen::Vector3f mid[3] = { (b+c)/2, (a+b)/2 , (a+c)/2 };
	Eigen::Vector3f mid_curve[3];
	Eigen::Vector3f mid_curve_n[3];

	bool e[3];

	for (int i = 0 ;i<3;i++)
	{
		bezpatchInterp(u[i],v[i],mid_curve[i],mid_curve_n[i]);

		e[i] = ( (mid_curve[i] - mid[i]).norm() < threshold );
		
	}

	
	
	
	//flatness test

	if(e[0] && e[1] && e[2])
	{
		//all pass flatness test

		polyVector.nextPoly();
		polyVector.pushBackVertex(LocalGeo(a,an));
		polyVector.pushBackVertex(LocalGeo(b,bn));
		polyVector.pushBackVertex(LocalGeo(c,cn));

		return;
	}
	else if(e[0] && e[1])
	{
		bezpatchInterp(u[2],v[2],mid_curve[2],mid_curve_n[2]);

		triangleAdaptiveTessllate(a,an,ua,va
								,b,bn,ub,vb
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);

		triangleAdaptiveTessllate(c,cn,uc,vc
								,b,bn,ub,vb
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
	}
	else if(e[0] && e[2])
	{

		triangleAdaptiveTessllate(a,an,ua,va
								,c,cn,uc,vc
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,threshold,polyVector);

		triangleAdaptiveTessllate(c,cn,uc,vc
								,b,bn,ub,vb
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,threshold,polyVector);
	}
	else if(e[1] && e[2])
	{
		triangleAdaptiveTessllate(a,an,ua,va
								,b,bn,ub,vb
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,threshold,polyVector);

		triangleAdaptiveTessllate(c,cn,uc,vc
								,a,an,ua,va
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,threshold,polyVector);
	}
	else if(e[0])
	{

		triangleAdaptiveTessllate(a,an,ua,va
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(b,bn,ub,vb
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(b,bn,ub,vb
								,c,cn,uc,vc
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
	}
	else if(e[1])
	{

		triangleAdaptiveTessllate(a,an,ua,va
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(c,cn,uc,vc
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(b,bn,ub,vb
								,a,an,ua,va
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,threshold,polyVector);
	}
	else if(e[2])
	{

		triangleAdaptiveTessllate(a,an,ua,va
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,threshold,polyVector);
		triangleAdaptiveTessllate(b,bn,ub,vb
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,threshold,polyVector);
		triangleAdaptiveTessllate(c,cn,uc,vc
								,a,an,ua,va
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,threshold,polyVector);
	}
	else 
	{
		// all fail flatness test

		triangleAdaptiveTessllate(a,an,ua,va
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(b,bn,ub,vb
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,threshold,polyVector);
		triangleAdaptiveTessllate(c,cn,uc,vc
								,mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
		triangleAdaptiveTessllate(mid_curve[0],mid_curve_n[0],u[0],v[0]
								,mid_curve[1],mid_curve_n[1],u[1],v[1]
								,mid_curve[2],mid_curve_n[2],u[2],v[2]
								,threshold,polyVector);
	}
}