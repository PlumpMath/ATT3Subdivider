#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Point {
    float x,y,z;
    Point(float q, float w, float e){
    	x = q;
    	y = w;
    	z = e;
    }
};

class ObjConverter
{
public:
	void makebez(const string & objname, const string & bezname);

	string bezFileName(const string & input_filename);
};