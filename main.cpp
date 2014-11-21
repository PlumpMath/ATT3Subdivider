/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Materials" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include "PolyVector.h"
#include "BezReader.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
/*
#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
*/
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

using namespace std;

float zoom = 1;
int window_w = 600;
int window_h = 600;
float camera_dist = 10;
Eigen::Vector3f pos_camera(0,0,-10);
//for mouse
int lastx=0;
int lasty=0;

int selected_obj_id = -1;
bool f_multiple_obj = false;

struct ObjTransformInfo
{
	Eigen::Vector3f center;
	float x_angle;
	float y_angle;
	float x_trans;
	float y_trans;

	ObjTransformInfo()
		:x_angle(0),y_angle(0)
		,x_trans(0),y_trans(0)
	{
	}
};


int num_objs = 1;
vector<ObjTransformInfo> vec_objTransformInfo;
vector<PolyVector> vec_polyVector;

Eigen::Vector3f center;

bool _hiddenLine = false;
bool _flatShading = false;

unsigned char Buttons[3] = {0};

//PolyVector teapot;
//Eigen::Vector3f center;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
			break;
		case 'z':
			//change selected obj
			if(f_multiple_obj)
			{
				selected_obj_id = (selected_obj_id+1) % (num_objs+1);
			}
			break;
		case 's':
			_flatShading = !_flatShading;
			if(_flatShading){
				glShadeModel(GL_FLAT);
			} else {
				glShadeModel(GL_SMOOTH);
			}
			glutPostRedisplay();
			break;
		case 'w':
			_hiddenLine = !_hiddenLine;
			glutPostRedisplay();
			break;
		case '-':
		case '_':
			{
				zoom = zoom + .1;
				//Eigen::Vector3f vector_zoom((center - pos_camera)*zoom);
				//pos_camera += vector_zoom;
				//cout<<pos_camera<<endl;
				
				break;
			}
		case '=':
		case '+':
			{
				zoom = zoom - .1;
				//Eigen::Vector3f vector_zoom((center - pos_camera)*zoom);
				//pos_camera += vector_zoom;
				//cout<<pos_camera<<endl;
				
				break;
			}
		default:
			break;
	}
}

Eigen::Vector3f getCenter(PolyVector& shape)
{
	Eigen::Vector3f vertex;
	Eigen::Vector3f min;
	Eigen::Vector3f max;
	int poly_size = shape.getPolySize();
	for (int i = 0; i < poly_size; i += 1){		
		for(int j = 0; j < shape.getVertexSize(i); j+= 1){
			vertex = shape.getVertex(i, j).p;
			if(i == 0 && j == 0){
				min = vertex;
				max = vertex;
			} else {
				if (vertex[0] < min[0])
				{
					min[0] = vertex[0];
				}
				if (vertex[1] < min[1])
				{
					min[1] = vertex[1];
				}
				if (vertex[2] < min[2])
				{
					min[2] = vertex[2];
				}
				if (vertex[0] > max[0])
				{
					max[0] = vertex[0];
				}
				if (vertex[1] > max[1])
				{
					max[1] = vertex[1];
				}
				if (vertex[2] > max[2])
				{
					max[2] = vertex[2];
				}
			}
		}
	}
	return (min + max)/2;
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	//glShadeModel(GL_FLAT);
	
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);



	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColord[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightColors[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos[] = {1000, 1000, 1000, 1.0f};
	//Diffuse (non-shiny) light component
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColord);
	//Specular (shiny) light component
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColors);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
}

void handleResize(int w, int h) {
	window_h = h;
	window_w = w;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	//center = getCenter(teapot);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0 * zoom, (float)window_w / (float)window_h, 1.0, 200.0);
	glutPostRedisplay();
	

	//gluLookAt(pos_camera.x(),pos_camera.y(),pos_camera.z()
	//	,center.x(),center.y(),center.z(),0,-1,0);
	gluLookAt(pos_camera.x(),pos_camera.y(),pos_camera.z()
		,0,0,0,0,-1,0);
	
	//light used to be here


	



	

	for ( int i = 0; i<num_objs; i++)
	{

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//glTranslatef(0, 0, zoom);
		glTranslatef(vec_objTransformInfo.at(i).x_trans, vec_objTransformInfo.at(i).y_trans, 0);
		glRotatef(vec_objTransformInfo.at(i).x_angle, 1, 0, 0);
		glRotatef(vec_objTransformInfo.at(i).y_angle, 0, 1, 0);
		glTranslatef(-vec_objTransformInfo.at(i).center.x()
			,-vec_objTransformInfo.at(i).center.y()
			,-vec_objTransformInfo.at(i).center.z());



		//Determine the specularity, emissivity, and shininess parameters, based on
		//variables that can be toggled with keystrokes

		float specularity = 1;
		float emissivity = 0;
		float shininess = 12;

		GLfloat materialColor[4];
		if(!f_multiple_obj)
		{
			materialColor[0] = 0.2f;
			materialColor[1] = 0.2f;
			materialColor[2] = 1.0f;
			materialColor[3] = 1.0f;
		}
		else if(selected_obj_id != i)
		{
			//nothing is selected
			int color_type = i%3;
			if(color_type == 1)
			{
				//red
				materialColor[0] = 1.0f;
				materialColor[1] = 0.2f;
				materialColor[2] = 0.2f;
				materialColor[3] = 1.0f;
			}
			else if(color_type ==2)
			{
				//yellow
				materialColor[0] = 1.0f;
				materialColor[1] = 1.0f;
				materialColor[2] = 0.2f;
				materialColor[3] = 1.0f;
			}
			else if(color_type ==0)
			{
				//blue
				materialColor[0] = 0.2f;
				materialColor[1] = 0.2f;
				materialColor[2] = 1.0f;
				materialColor[3] = 1.0f;
			}
			
		}
		else if(selected_obj_id == i)
		{
			materialColor[0] = 1.0f;
			materialColor[1] = 1.0f;
			materialColor[2] = 1.0f;
			materialColor[3] = 1.0f;

			specularity = 0.0f;
			emissivity = 1.0f;
		}
		
		//The specular (shiny) component of the material
		GLfloat materialSpecular[] = {specularity, specularity, specularity, 1.0f};
		//The color emitted by the material
		GLfloat materialEmission[] = {emissivity, emissivity, emissivity, 1.0f};

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
		glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter

		

		Eigen::Vector3f point;
		Eigen::Vector3f normal;

		int poly_size = vec_polyVector.at(i).getPolySize();
		for (int k = 0; k < poly_size; k += 1){
			//cout << "Polygon " << i << ":" << endl;
			if(_hiddenLine){
				glBegin(GL_LINE_LOOP);
			} else {
				glBegin(GL_POLYGON);
			}

			for(int j = 0; j < vec_polyVector.at(i).getVertexSize(k); j+= 1){
				point = vec_polyVector.at(i).getVertex(k, j).p;
				normal = vec_polyVector.at(i).getVertex(k, j).n;
				//glNormal3f(-normal[0], -normal[1], -normal[2]);
				glNormal3f(normal[0], normal[1], normal[2]);
				glVertex3f(point[0], point[1], point[2]);
			}
			glEnd();
		}

		glPopMatrix();


	}

	glutSwapBuffers();
}

//Called every 25 milliseconds
void myFrameMove() {
  //nothing here for now
//#ifdef _WIN32
//  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
//#endif
//  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


void SpecialKeys(int key, int x, int y)
{
	int state;
	state=glutGetModifiers();
	if (state != GLUT_ACTIVE_SHIFT){
		if(selected_obj_id < num_objs)
		{
			if (key == GLUT_KEY_LEFT)
			{
				vec_objTransformInfo.at(selected_obj_id).y_angle += 1;
				if (vec_objTransformInfo.at(selected_obj_id).y_angle > 360) {
					vec_objTransformInfo.at(selected_obj_id).y_angle -= 360;
				}
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_RIGHT)
			{
				vec_objTransformInfo.at(selected_obj_id).y_angle -= 1;
				if (vec_objTransformInfo.at(selected_obj_id).y_angle < 0) {
					vec_objTransformInfo.at(selected_obj_id).y_angle += 360;
				}
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_UP)
			{
				vec_objTransformInfo.at(selected_obj_id).x_angle += 1;
				if (vec_objTransformInfo.at(selected_obj_id).x_angle > 360) {
					vec_objTransformInfo.at(selected_obj_id).x_angle -= 360;
				}
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_DOWN)
			{
				vec_objTransformInfo.at(selected_obj_id).x_angle -= 1;
				if (vec_objTransformInfo.at(selected_obj_id).x_angle < 0) {
					vec_objTransformInfo.at(selected_obj_id).x_angle += 360;
				}
				glutPostRedisplay();
			}
		}
	} else {
		if(selected_obj_id < num_objs)
		{
			const float trans_step = 0.1f;
			if (key == GLUT_KEY_UP)
			{
				vec_objTransformInfo.at(selected_obj_id).y_trans -= trans_step;
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_DOWN)
			{
				vec_objTransformInfo.at(selected_obj_id).y_trans += trans_step;
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_LEFT)
			{
				vec_objTransformInfo.at(selected_obj_id).x_trans -= trans_step;
				glutPostRedisplay();
			}
			if (key == GLUT_KEY_RIGHT)
			{
				vec_objTransformInfo.at(selected_obj_id).x_trans += trans_step;
				glutPostRedisplay();
			}
		}
	}
}

void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[0] && Buttons[1] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
	{
		if(selected_obj_id < num_objs)
		{
			if( Buttons[0] )
			{
				vec_objTransformInfo.at(selected_obj_id).x_angle += (float) 0.5f * diffy;
				vec_objTransformInfo.at(selected_obj_id).y_angle += (float) 0.5f * diffx;		
			}
			else
				if( Buttons[1] )
				{
					vec_objTransformInfo.at(selected_obj_id).x_trans += (float) 0.05f * diffx;
					vec_objTransformInfo.at(selected_obj_id).y_trans -= (float) 0.05f * diffy;
				}
		}
	}
	glutPostRedisplay();
}

void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}



int main(int argc, char** argv) {

	if(strcmp(argv[1],"-m") == 0)
	{
		//	0	 1	2			3		4		5
		//./ASS3 -m scene.scene 0.1(step) [-a  [0.01] ]

		//multiple object
		f_multiple_obj = true;
		//read a scene file
		bool f_adaptive = false;

		float step = atof(argv[3]);

		ifstream in;
		in.open(argv[2]);


		float error = 0.01;
		if(argc > 4)
		{
			
			if(argc>5)
			{
				error = atof(argv[5]);
			}

			if(strcmp(argv[4], "-a") == 0)
			{
				f_adaptive = true;
				
			} 
		}

		string line;

		int last_id = -1;
		while(in>>line)
		{
			//in>>line;
			if(line == "")
			{
				break;
			}

			vec_polyVector.push_back(PolyVector());
			vec_objTransformInfo.push_back(ObjTransformInfo());
			last_id++;

			BezReader bezReader;
			bezReader.ReadBezFile(line.c_str());
			if(f_adaptive)
			{
				bezReader.BuildPolyVector_Adaptive(vec_polyVector.at(last_id),error,step);
			}
			else
			{
				bezReader.BuildPolyVector_Uniform(vec_polyVector.at(last_id),step);
			}
			
			vec_objTransformInfo.at(last_id).center = getCenter(vec_polyVector.at(last_id));

		}


		in.close();


		vector<ObjTransformInfo>::iterator iter;
		for(iter = vec_objTransformInfo.begin();iter!= vec_objTransformInfo.end();++iter)
		{
			center += iter->center ;
		}
		num_objs = vec_objTransformInfo.size();
		center /= num_objs;

		
		selected_obj_id = num_objs;
	}
	else
	{
		//ugly implementation...
		f_multiple_obj = false;

		vec_polyVector.push_back(PolyVector());
		vec_objTransformInfo.push_back(ObjTransformInfo());


		BezReader bezReader;
		bezReader.ReadBezFile(argv[1]);

		if(argc > 3)
		{
			float error = 0.01;
			if(argc>4)
			{
				error = atof(argv[4]);
			}

			if(strcmp(argv[3], "-a") == 0)
			{
				bezReader.BuildPolyVector_Adaptive(vec_polyVector.at(0),error,atof(argv[2]));
			} 
		}
		else 
		{
			bezReader.BuildPolyVector_Uniform(vec_polyVector.at(0),atof(argv[2]));
		}

		num_objs = 1;
		center = getCenter(vec_polyVector.at(0));
		vec_objTransformInfo.at(0).center = center;

		selected_obj_id = 0;
	}
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_w, window_h);
	
	glutCreateWindow("ASS3");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutIdleFunc(myFrameMove);



	glutMainLoop();
	return 0;
}









