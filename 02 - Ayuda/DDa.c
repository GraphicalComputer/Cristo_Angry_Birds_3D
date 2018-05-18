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
void lineaDDA(float x0, float y0, float xEnd, float yEnd)
{
	int dx=xEnd-x0, dy=yEnd-y0, steps;
	float xInc, yInc, x=x0, y=y0, k;

	if(fabs(dx)>fabs(dy))
	{
		steps=fabs(dx);
	}
	else
	{
		steps=fabs(dy);
	}

	xInc=(float)(dx)/(float)(steps);
	yInc=(float)(dy)/(float)(steps);
    glPointSize(10.0);
	putPixel(round(x),round(y));

	for(k=0;k<steps;k+=0.001)
	{
		x+=(xInc);
		y+=(yInc);
		putPixel(round(x),round(y));
	}
}

void display(void)
{

   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(spin, 0.0, 0.0, 1.0);
   glColor3f(1.0, 1.0, 1.0);
   lineaDDA(0.0,0.0,- 50.0,50.0);
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

/*
 *  Request double buffer display mode.
 */
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
