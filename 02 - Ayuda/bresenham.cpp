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
void Bresenham(int x0,int y0,int xFin,int yFin)
{
    int dx = fabs(xFin - x0), dy = fabs(yFin - y0);
    int p = 2 * dy - dx;
    int dUPper = 2 * dy, dLOwer = 2 * (dy - dx);
    float x, y;
    if (x0 > xFin) {
        x = xFin;
        y = yFin;
        xFin = x0;
    }
    else {
    x = x0;
    y = y0;
    }
    putPixel(x, y);
    while (x < xFin){
        x+=0.001;
    if (p <0)
        p += dUPper;
    else {
        y+=0.001;
        p += dLOwer;
        }
        putPixel(x, y);
    }
}
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(spin, 0.0, 0.0, 1.0);
   glColor3f(1.0, 1.0, 1.0);
   Bresenham(0.0,0.0,10.0,0.0);
   Bresenham(0.0,0.0,10.0,5.0);
   Bresenham(0.0,0.0,-5.0,-2.5);
   Bresenham(0.0,0.0,-1.0,-10.5);
   Bresenham(0.0,0.0,-1.0,-1.5);
   Bresenham(0.0,0.0,-4.0,-10.5);
   Bresenham(0.0,5.0,4.0,10.5);
   Bresenham(0.0,8.0,4.0,-10.5);
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
   glutCreateWindow ("Bresenham");
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
