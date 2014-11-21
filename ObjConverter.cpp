#include "ObjConverter.h"


void ObjConverter::makebez(const string & objname, const string & bezname)
{
	vector<Point> vec_Vert;
	ifstream file;
	file.open(objname);
        ofstream myfile;
        myfile.open (bezname);
        myfile << "           ";
        myfile << endl;
        int num_f=0;
	if(!file.is_open())
	{
		std::cout << "error: Unable to open Obj file" << std::endl;
	}
	else
	{
                std::cout << "opening Obj file" << std::endl;
		string line;
		while(file.good()) {
			std::vector<std::string> split;
			std::string buf;
			std::getline(file,line);
			std::stringstream ss(line);
			
                        while (ss >> buf) {
				split.push_back(buf);
                                
			}
                        
			if(split.size() == 0) {
				continue;
			}
			if (split[0][0] == '#'){
				continue;
			} 
			else if(split[0] == "g")
			{
				//group...
				continue;
			}
			else if(split[0] == "s")
			{
				//ignore...
				continue;
			}
			else if(split[0] == "mtllib")
			{
				//ignore
				continue;
			}
			else if(split[0] == "usemtl")
			{
				//ignore
				continue;
			}
			else if (split[0] == "v"){
				//vertex
                                

				float x = atof(split[1].c_str());
				float y = atof(split[2].c_str());
				float z = atof(split[3].c_str());

				vec_Vert.push_back(Point(x,y,z));

			} else if (split[0] == "vn"){
				//normal

				float x = atof(split[1].c_str());
				float y = atof(split[2].c_str());
				float z = atof(split[3].c_str());

			}
			else if(split[0] == "vt")
			{
				//texture
				//ignore now
			}
                        
			else if (split[0] == "f")
			{
                                num_f = num_f+1;
				string s;
				int split_size = split.size();
				int num_v = split_size - 1;
				
				int v[50];
				int vt[50];
				int vn[50];
				vn[1] = -1;
				
				int i;
				for (i = 1;i<split_size;i++)
				{
					std::istringstream ss(split.at(i));
					getline(ss, s, '/');
					v[i] = atoi(s.c_str());
					s="-1";
                                        
                                        //std::cout << v[i] << std::endl;

					getline(ss, s, '/');
					vt[i] = atoi(s.c_str());	//texture
					s="-1";

					getline(ss, s, '/');
					vn[i] = atoi(s.c_str());
					s="-1";
				}
                                
                                
				
				for (i = 3; i <= num_v ;i++)
				{
                                        
					myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).z << endl;
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i-1]-1).z << endl;
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).z << endl;
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[1]-1).z;
                                        myfile << "  ";
                                        
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).z;
                                        myfile << "  ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).x;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).y;
                                        myfile << " ";
                                        myfile  << fixed << std::setprecision(5) << vec_Vert.at(v[i]-1).z << endl;
                                        myfile << endl;
                                        
                                        
                                        
//
//                                                
                                        
                                        

//					myfile << ' ';
//					myfile << vec_Vert.at(v[1]-1)).y;
//					myfile << " ";
//					myfile << vec_Vert.at(v[1]-1)).z;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i-1]-1)).x;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i-1]-1)).y;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i-1]-1)).z;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i]-1)).x;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i]-1)).y;
//					myfile << " ";
//					myfile << vec_Vert.at(v[i]-1)).z;
//					myfile << " ";

					
				}

			}
			else
			{
				cout<<"Warning! undefined label!\n";
				abort();
			}




		}
	}
        myfile.seekp(0,ios::beg);
        myfile << num_f<<endl;
        myfile.close();
        
	file.close();
}



string ObjConverter::bezFileName(const string & input_filename)
{
	size_t found = input_filename.find(".obj");

	if (found != std::string::npos)
	{
		string bezfilename(input_filename);
		bezfilename.replace(found,4,".bez");
		makebez(input_filename,bezfilename);
		
		return bezfilename;
	}
	else
	{
		return input_filename;
	}
}