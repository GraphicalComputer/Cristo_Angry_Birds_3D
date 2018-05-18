#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

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
void lineaDDA(float xStar, float yStar, float xEnd, float yEnd)
{
  int dx=xEnd-xStar, dy=yEnd-yStar, steps;
  double Xincrement, Yincrement, xL=xStar, yL=yStar,k;
  if(fabs(dx)>fabs(dy))
  {
    steps=fabs(dx);
  }
  else
  {
    steps=fabs(dy);
  }
  Xincrement=float(dx)/float(steps);
  Yincrement=float(dy)/float(steps);
  putPixel(round(xL),round(yL));
  for(k=0;k<=steps;k+=0.001)
  {
    xL+=(Xincrement/500);
    yL+=(Yincrement/500);
    putPixel((xL),(yL));
  }
}
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(spin, 0.0, 0.0, 1.0);
   glColor3f(1.0, 1.0, 1.0);
   lineaDDA(0.0,0.0,10.0,10.0);
   lineaDDA(0.0,0.0,15.0,5.0);
   lineaDDA(0.0,0.0,5.0,2.5);
   lineaDDA(0.0,0.0,85.0,62.5);
   lineaDDA(0.0,0.0,-50.0,50.0);
   lineaDDA(0.0,0.0,-20.0,30.0);
   lineaDDA(0.0,0.0,-10.0,20.0);
   lineaDDA(0.0,0.0,-2.0,10.0);
   lineaDDA(0.0,0.0,-1.0,10.0);
   lineaDDA(0.0,0.0,-0.5,10.0);
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
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (500, 500);
   glutCreateWindow ("DDA");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
