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
#include <stdlib.h>
#include <cstring>
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
float zoom = 0;
float x_angle = 0;
float y_angle = 0;
float x_trans = 0;
float y_trans = 0;
int lastx=0;
int lasty=0;

float camera_dist = 10;


bool _hiddenLine = false;
bool _flatShading = false;

unsigned char Buttons[3] = {0};

PolyVector teapot;
Eigen::Vector3f point;
Eigen::Vector3f normal;
Eigen::Vector3f center;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
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
			zoom = zoom - .5;
			glutPostRedisplay();
			break;
		case '=':
		case '+':
			zoom = zoom + .5;
			glutPostRedisplay();
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
		for(int j = 0; j < teapot.getVertexSize(i); j+= 1){
			vertex = teapot.getVertex(i, j).p;
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
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	center = getCenter(teapot);
	
	gluLookAt(center.x(), center.y(), center.z() + camera_dist,
			  center.x(), center.y(), center.z(),
			  0, 		  1, 		  0);
	
	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {0, 0, 50, 1.0f};
	//Diffuse (non-shiny) light component
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	//Specular (shiny) light component
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glTranslatef(0, 0, zoom);
	glTranslatef(x_trans, y_trans, 0);
	glRotatef(x_angle, 1, 0, 0);
	glRotatef(y_angle, 0, 1, 0);
	//glTranslatef(-center.x(), -center.y(), -center.z());
	

	
	//Determine the specularity, emissivity, and shininess parameters, based on
	//variables that can be toggled with keystrokes
	
	float specularity = 1;
	
	float emissivity = 0;
	
	float shininess = 12;

	
	//The color of the sphere
	GLfloat materialColor[] = {0.2f, 0.2f, 1.0f, 1.0f};
	//The specular (shiny) component of the material
	GLfloat materialSpecular[] = {specularity, specularity, specularity, 1.0f};
	//The color emitted by the material
	GLfloat materialEmission[] = {emissivity, emissivity, emissivity, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
	
	
	int poly_size = teapot.getPolySize();
	for (int i = 0; i < poly_size; i += 1){
		//cout << "Polygon " << i << ":" << endl;
		if(_hiddenLine){
			glBegin(GL_LINE_LOOP);
		} else {
			glBegin(GL_POLYGON);
		}
		
		for(int j = 0; j < teapot.getVertexSize(i); j+= 1){
			point = teapot.getVertex(i, j).p;
			normal = teapot.getVertex(i, j).n;
			//glNormal3f(-normal[0], -normal[1], -normal[2]);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(point[0], point[1], point[2]);
		}
		glEnd();
	}

	glPopMatrix();
	glutSwapBuffers();
}

//Called every 25 milliseconds
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


void SpecialKeys(int key, int x, int y)
{
	int state;
	state=glutGetModifiers();
	if (state != GLUT_ACTIVE_SHIFT){
		if (key == GLUT_KEY_UP)
		{
			y_angle = y_angle + 1;
			if (y_angle > 360) {
				y_angle -= 360;
			}
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_DOWN)
		{
			y_angle = y_angle - 1;
			if (y_angle < 0) {
				y_angle += 360;
			}
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_LEFT)
		{
			x_angle = x_angle + 1;
			if (x_angle > 360) {
				x_angle -= 360;
			}
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_RIGHT)
		{
			x_angle = x_angle - 1;
			if (x_angle < 0) {
				x_angle += 360;
			}
			glutPostRedisplay();
		}
	} else {
		if (key == GLUT_KEY_UP)
		{
			y_trans = y_trans + 1;
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_DOWN)
		{
			y_trans = y_trans - 1;
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_LEFT)
		{
			x_trans = x_trans - 1;
			glutPostRedisplay();
		}
		if (key == GLUT_KEY_RIGHT)
		{
			x_trans = x_trans + 1;
			glutPostRedisplay();
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
		if( Buttons[0] )
		{
			x_angle += (float) 0.5f * diffy;
			y_angle += (float) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				x_trans += (float) 0.05f * diffx;
				y_trans -= (float) 0.05f * diffy;
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
			bezReader.BuildPolyVector_Adaptive(teapot,0.01,atof(argv[2]));
		} 
	}
	else 
	{
		bezReader.BuildPolyVector_Uniform(teapot,atof(argv[2]));
	}


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	
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









