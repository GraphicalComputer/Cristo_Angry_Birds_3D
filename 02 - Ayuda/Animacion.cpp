#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
int rad=10;

static GLfloat spin = 0.0;

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void putPixel(float x, float y){

	glBegin(GL_POINTS);
    glVertex3f (x,y,0.0);
	glEnd();

}

void Bezier(float Px1,float Py1,float Pz1, float Px2,float Py2,float Pz2,float Px3,float Py3,float Pz3,float Px4,float Py4,float Pz4,float n) {
	float d = 0.10f/n;
    glBegin(GL_LINE_STRIP);
    for(float t = 0; t <= 1; t+=d)
    {
        float ct = (1-t);
        float ct2 = ct * ct;
        float ct3 = ct2 * ct;
        float t2 = t*t;
        float t3 = t2*t;
        float c2 = 3*t*ct2;
        float c3 = 3*t2*ct;
        float x = ct3*Px1 + c2*Px2 + c3*Px3 + t3*Px4;
        float y = ct3*Py1 + c2*Py2 + c3*Py3 + t3*Py4;
        float z = ct3*Pz1 + c2*Pz2 + c3*Pz3 + t3*Pz4;
		glColor3f(100, 100, 100);
        glVertex3f (x, y, z);
	}
	glEnd();
}
void circulo(int x, int y, int radio){
  int angulo=0;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x,y);

  for (angulo=0;angulo<=360; angulo+=6){ glVertex2f(x + sin(angulo) * radio, y + cos(angulo) * radio);}
  glEnd();
}
void Dibuja(float p1x,float p1y,float p1z, float p2x,float p2y,float p2z,float p3x,float p3y,float p3z,float p4x,float p4y,float p4z,float n) {
    float d = 0.10f/n;
    //glBegin(GL_LINE_STRIP);
    for(float t = 0; t <= 1; t+=d)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float ct = (1-t);
        float ct2 = ct * ct;
        float ct3 = ct2 * ct;
        float t2 = t*t;
        float t3 = t2*t;
        float c2 = 3*t*ct2;
        float c3 = 3*t2*ct;
        float x = ct3*p1x + c2*p2x + c3*p3x + t3*p4x;
        float y = ct3*p1y + c2*p2y + c3*p3y + t3*p4y;
        float z = ct3*p1z + c2*p2z + c3*p3z + t3*p4z;
        glColor3f(100, 100, 100);
        circulo(x,y,1);
        glFlush();
        glutSwapBuffers();
  }
  glEnd();
  glFlush();
  //glutSwapBuffers();

}

void display(void)
{

   //glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(spin, 0.0, 0.0, 1.0);
   glColor3f(1.0, 1.0, 1.0);
   Bezier(0.0,0.0,0.0,1.0,8.0,0.0,6.0,-5.0,0.0,2.5,5.0,0.0,8.0);
   Dibuja(0.0,0.0,0.0,1.0,8.0,0.0,6.0,-5.0,0.0,2.5,5.0,0.0,8.0);
   glPopMatrix();
   glutSwapBuffers();
}




void spinDisplay(void)
{
   spin = spin + 2.0;
   if (spin > 360.0)
      spin = spin - 360.0;
   glutPostRedisplay();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (500, 500);
   glutCreateWindow ("algoritmos");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
