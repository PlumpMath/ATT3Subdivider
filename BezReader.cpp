#include <fstream>
#include <string>
#include <sstream>
#include "BezReader.h"

void BezReader::ReadBezFile(const char * filename)
{
	ifstream in;
	in.open(filename);

	if(in.is_open())
	{
		in>>num_patches;

		vec_patches = vector<Patch>(num_patches);

		string line;
		line = "";
		for(int patch_id = 0; patch_id<num_patches; patch_id++)
		{
			while(line == "")
			{
				getline(in,line);
			}
			
			stringstream ss(line);
			float x,y,z;

			for (int i = 0;i<NUM_CP_AXIS;i++)
			{
				for (int j = 0;j<NUM_CP_AXIS;j++)
				{
					ss>>x>>y>>z;
					Eigen::Vector3f& cur_vector3f = vec_patches.at(patch_id).cp[i][j];
					cur_vector3f(0) = x;
					cur_vector3f(1) = y;
					cur_vector3f(2) = z;
				}
			}

		}



	}



	in.close();
}





void BezReader::BuildPolyVector_Uniform(PolyVector & polyVector,float step )
{
	for(auto i:vec_patches)
	{
		i.uniformSubdividePatch(step,polyVector);
	}
}

void BezReader::BuildPolyVector_Adaptive(PolyVector & polyVector,float threshold ,float step )
{
	for(auto i:vec_patches)
	{
		i.adaptiveSubdividePatch(step,threshold,polyVector);
	}
}