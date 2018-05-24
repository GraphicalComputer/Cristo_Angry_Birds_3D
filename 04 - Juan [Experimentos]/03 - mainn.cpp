/*              
											 UASLP
										FACULTY OF ENGINEERERING
										 COMPUTER ENGINEERERING

										    COMPUTER GRAPHICS
									PROFESSOR DR. JOSE IGNACIO NUÑEZ VARELA

										      Project v2.0000548

									      ERICK EDUARDO GALINDO CHAVEZ
											248570
									
*/

#include <iostream>
#include <list>
#include <vector>
#include <cstdio>
#include <cstring>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>


using namespace std;

//ILLUMINATION PROPERTIES
float kd = 0.5; //DIFUSE COEFFICIENT
float ka = 0.5; //AMBIENT COEFFICIENT
float Ia = 1.0; //AMBIENT INTENSITY
float Il = 1.0; //LIGHT INTENSITY

typedef struct{
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
} Point;


void translate(float x, float y, float z);
void crossProduct(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float *prx, float *pry, float *prz);
float dotProduct(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z);
void substractVertex(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float *prx, float *pry, float *prz);
void unitaryVector (float *x1, float *y1, float *z1);
bool GenerateNormal(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float *pnx, float *pny, float *pnz);
float calculateIllumination(float Nx, float Ny, float Nz);
void calculateNormal(void);
void Matrix(void);
void DrawBezier();
void GenerateBezier(Point P1, Point P2);


//GLOBAL LIST WHICH CONTAINS THE POINTS OF THE BEZIER CURVE
list<Point> Points;
list<Point> BezierPoints;
Point P1, P2;


/*engineererick*/

//CLASS VERTEX -> EACH OBJECT REPRESENTS A SINGLE VERTEX
class Vertex{
    public:
        float x, y, z;
        int n;

        /*CONSTRUCTOR*/
        /*INITIALIZES THE VARIABLES*/
        Vertex(){
            x = 0;
            y = 0;
            z = 0;
            n = 0;
        }

        /*OVERWRITES THE FIRST VARIABLES*/
        Vertex(float x1, float y1, float z1,int num){
            x = x1;
            y = y1;
            z = z1;
            n = num;
        }

        /*ALL 'get' FUNCTIONS GETS THE VALUES FOR EACH VARIABLE*/
        int getNumber() {
            return n;
        }

        float getX() {
            return x;
        }

        float getY() {
            return y;
        }

        float getZ() {
            return z;
        }

        //PRINT THE VALUES
        void printV() {
            cout<<"Vertex "<<n<<" : x = "<<x<<" y = "<<y<<" z = "<<z<<"\n";
        }
};

//GLOBAL VERTEX VARIABLES WHICH CONTAIN THE PRP AND THE LIGHT
Vertex PRP;
Vertex L;

class Normal{
public:
    float A, B, C;
    
    Normal(){
        A = B = C = 0;
    }
    
    float getA(){
        return A;
    }
    
    float getB(){
        return B;
    }
    
    float getC(){
        return C;
    }
};

/*engineererick*/

class Face{
    public:

        float x;
        float y;
        float z;
        bool vis;

        list <Normal> normals;
        list <Vertex> vertices;

        Face(){
            n = 0;
        }

        Face(list <Vertex> vert, int num){
            vertices = vert;
            n = num;
            x = 0;
            y = 0;
            z = 0;
            vis = false;
        }

        void printFc() {
            cout<<"\n"<<"Face "<<n<<" : \n";
            /*THE ITERATOR IS USED BY AN ALGORITHM TO TRAVERS THE ITEMS STORED IN A LIST*/
            for(list<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it)
                it->printV();
            cout<<"\n";
        }

    private:
        int n;
};

/*engineererick*/

class Object{
    public:
        list<Face> faces;

        Object(string nm){
            name = nm;
        }

        Object(list<Face> fcs, string nm){
            name = nm;
            faces = fcs;
        }

        void addFace(Face fc){
            faces.push_back(fc); /*push_back -> ADD NUMBERS*/
        }

        string name;

    private:

        void printO() {
            int i=0;
            for(std::list<Face>::iterator it1 = faces.begin(); it1!=faces.end(); it1++){
                for(std::list<Vertex>::iterator it2 = it1->vertices.begin(); it2 != it1->vertices.end(); it2++)
                    if(i<it2->getNumber()){
                        it2->printV();
                        i++;
                    }
            }
            for(std::list<Face>::iterator it = faces.begin(); it != faces.end(); it++)
                it->printFc();
        }
};


list <Object> OBJ;

/*engineererick*/

class Reader{
    public:
        Reader(){
            Fi = 0;
            Vi = 0;
            File = NULL;
        }

        bool openFile(const char* filename){
            File = fopen(filename, "r");

            if(File)
                return true;
            else
                return false;
        }

        bool closeFile(){
            if(fclose(File) == 0)
                return true;
            else
                return false;
        }

        void printFile(){
            cout<<"The file is being printed ... "<<File<<endl;
            while (!feof(File)){
                cout<<getc(File);
            }
        }

        //GET THE FILE INFORMATION
         /*engineererick*/
        Object* readObject(){
            std::string ObjName;
            char i;
            i = getc(File);

            while (i != '\n'){
                i = getc(File);
                ObjName += i;
            }

            Object *tempObj = new Object(ObjName);

            return tempObj;
        }

        //METHOD TO READ THE FILE
        list<Object> readFile(){
            list<Object> objs;
            Object* newObjs;
            char i;

            while (!feof(File)){
                i = getc(File);
                if(i == '#')
                    while(i != '\n')
                        i = getc(File);
                if(i == 'o'){
                    newObjs = readObject();

                    do{
                        i = getc(File);
                        if(i == 'v')
                            readVertex();
                        if(i == 'f'){
                            Face tempf = createFace();
                            newObjs->addFace(tempf);
                        }
                    }while (!feof(File) && i != 'o');

                    ungetc(i, File); //CONVERT TO CHAR AND PUSH IT
                    objs.push_back(*newObjs);
                }
            }
            return objs;
        }
	
//URL HELP: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    private:
        int Fi;
        int Vi;
        FILE* File;
        list<Vertex> fileVertex;

        void readVertex(){
            float x, y, z;
            fscanf(File, "%f", &x); 
	    fscanf(File, "%f", &y); 
	    fscanf(File, "%f", &z);
            Vi++; //GO THROUGH THE LINES	
            Vertex* TempVer = new Vertex(x, y, z, Vi);
            fileVertex.push_back(*TempVer);
        }

        Face createFace(){
            list<Vertex> Vert;
            int v;
            char i;
            do{
                i = getc(File); //RETURNS THE CHARACTER CURRENTLY POINTED BY THE INTERNAL FILE POSOTION
                if(i == ' ')
                {
                    fscanf(File, "%d", &v);
                    for (list<Vertex>::iterator it = fileVertex.begin(); it != fileVertex.end(); ++it)
                        if(v == it->getNumber())
                            Vert.push_back(*it);
                }
            }while(i != '\n' && !feof(File));

            Fi++;
            Face* faceAux = new Face(Vert, Fi);

            return *faceAux;
        }
};

//READ THE OBJ FILE
list<Object> ReadObject(){
    list<Object> obj;
    Reader *reader = new Reader();

    if(reader->openFile("moto3.obj")){ //THE USER GIVES THE NAME OF THE OBJ FILE

        obj = reader->readFile(); //obj GETS THE VALUE OF THE FUNCTION 'READFILE'
        reader->closeFile();
    }
    return obj;
}

std::list<Point>::iterator itBezier0 = BezierPoints.begin(), itBezier1 = BezierPoints.begin();

/*engineererick*/

void display(void){
    float I = 0;
    
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    
    Matrix(); //SURFACE
    
    DrawBezier();
    
    translate(((itBezier1->X-itBezier0->X)), ((itBezier1->Y-itBezier0->Y)), ((itBezier1->Z-itBezier0->Z)));
   
    for(std::list<Object>::iterator it = OBJ.begin(); it!=OBJ.end(); ++it){
         for(std::list<Face>:: iterator it1=it->faces.begin(); it1!=it->faces.end(); ++it1){
              I = calculateIllumination(it1->x,it1->y,it1->z);
              glPushMatrix();
              if(it1->vis){
                  glColor3f(I, I/2, 0);
                  glBegin(GL_POLYGON);
                  for(std::list<Vertex>:: iterator it2=it1->vertices.begin(); it2!=it1->vertices.end(); ++it2)
                        glVertex3f(it2->getX(), it2->getY(), it2->getZ());
              }
              glEnd();
              glPopMatrix();   
         }
    }
    
    translate(-((itBezier1->X-itBezier0->X)), -((itBezier1->Y-itBezier0->Y)), -((itBezier1->Z-itBezier0->Z)));
    
    glutSwapBuffers();
    glFlush ();
}

/*engineererick*/

//CREATES/SIMULATES A SURFACE
void Matrix(){
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        for (GLfloat i = -400; i <= 400; i += 10)
        {
            glVertex3f(i, -50, 400); glVertex3f(i, -50, -400);
            glVertex3f(400, -50, i); glVertex3f(-400, -50, i);
        }
    glEnd();
    
}


void init (void){
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-15, 15, -10, 10, 20 , 1500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,-40, -550);
    glRotatef(45, 1, 0, 0);

    //PRP POSITION
    PRP.x = 0; PRP.y = 10; PRP.z = 20;
    
    //LIGHT POSITION
    L.x = 0; L.y = -10; L.z = 10;
    
    //BEZIER POINTS
    P1.X = -300; P1.Y = 0; P1.Z = -300;
    P2.X = -30; P2.Y = 0; P2.Z = 0;
    
    GenerateBezier(P1, P2);
}

/*engineererick*/


//CALCULATE THE NORMAL OF EACH VERTEX
bool GenerateNormal(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float *pnx, float *pny , float *pnz){
    float r1x, r2x, r1y, r2y, r1z, r2z, anx, any, anz, value = 0;
 
    // V2 - V1
    substractVertex(p2x, p2y, p2z, p1x, p1y, p1z, &r1x, &r1y, &r1z);
    // V3 - V1
    substractVertex(p3x, p3y, p3z, p1x, p1y, p1z, &r2x, &r2y, &r2z);
    // R1 X R2
    crossProduct(r1x, r1y, r1z, r2x, r2y, r2z, &anx, &any, &anz);
    
    value = dotProduct(anx, any, anz, PRP.x, PRP.y, PRP.z);
    
    *pnx = anx;
    *pny = any;
    *pnz = anz;
    
    if(value <= 0)
        return false;
    else
        return true;
}


//DO A DOT PRODUCT WITH 2 GIVEN VECTORS AND GET AN SCALAR
float dotProduct(float p1x, float p1y, float p1z, float p2x, float p2y,float p2z){
    return(p1x * p2x + p1y * p2y + p1z * p2z);
}

//DO A CROSS PRODUCT WITH 2 GIVEN VECTORS AND GET A NEW ONE
void crossProduct(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float *prx, float *pry, float *prz){
    *prx = p1y * p2z - p1z * p2y;
    *pry = p1z * p2x - p1x * p2z;
    *prz = p1x * p2y - p1y * p2x;
}

//SUBTRACT TO VECTORS AND GET A NEW ONE
void substractVertex(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float *prx, float *pry, float *prz){
    *prx = p1x - p2x;
    *pry = p1y - p2y;
    *prz = p1z - p2z;
}

//CONVERT THE GIVEN VECTOR TO AN UNITARY VECTOR
void unitaryVector(float *x1, float *y1, float *z1){
    float magnitude;
    float x = *x1, y = *y1, z = *z1;
    
    magnitude = sqrt (x * x + y * y + z * z);
    *x1 = x / magnitude;
    *y1 = y / magnitude;
    *z1 = z / magnitude;
}

//CALCULATE THE ILLUMINATION OF EACH FACE
float calculateIllumination (float x, float y, float z){
    float illum, Li;
    float lx = L.x;
    float ly = L.y;
    float lz = L.z;
    
    unitaryVector (&x, &y, &z);
    unitaryVector (&lx, &ly, &lz);
    
    Li = dotProduct(x, y, z, lx, ly, lz);
    illum = (Ia * ka) + (Il * kd * Li);
    
    return illum;
}

/*engineererick*/

//GENERATE BEZIER CURVE
void GenerateBezier(Point P1, Point P2){
    BezierPoints = list<Point>();
    Point P3, P4, tmp;
    GLfloat t;
    GLfloat x = 0, y = 0, z = 0;
    GLfloat p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    
    P3.X = -50;
    P3.Y = 0;
    P3.Z = -30;
    
    P4.X = 300;
    P4.Y = 0;
    P4.Z = 200;
    
    for (t = 0; t <= 1; t += (float).01){
        p1 = (1 - t) * (1 - t) * (1 - t);
        p2 = 3 * t * (1 - t) * (1 - t);
        p3 = 3 * t * t * (1 - t);
        p4 = t * t * t;
        
        x = (p1 * P1.X) + (p2 * P2.X) + (p3 * P3.X) + (p4 * P4.X);
        y = p1 * P1.Y + p2 * P2.Y + p3 * P3.Y + p4 * P4.Y;
        z = p1 * P1.Z + p2 * P2.Z + p3 * P3.Z + p4 * P4.Z;
        
        tmp.X = x;
        tmp.Y = y;
        tmp.Z = z;
        
        BezierPoints.push_back(tmp);
    }
}

//DRAW BEZIER CURVE
void DrawBezier(){
    list<Point> BezierAux;
    BezierAux = BezierPoints;
    
    glPointSize(3);
    
    glBegin(GL_POINTS);
    list<Point>::const_iterator it, it1;
    it = BezierPoints.begin();
    it1 = BezierAux.begin();
    it1++;
    glColor3f(0.0, 1.0, 0.0); //GREEN
    while (it1 != BezierAux.end()){
        glVertex3f(it->X, it->Y, it->Z);
        glVertex3f(it1->X, it1->Y, it1->Z);
        it1++;
        it++;
        }
    glEnd();
    
    glFlush();
    
}

/*engineererick*/

//LOOP THROUGH THE OBJECT TO CALCULATE ALL THEIR NORMALS AND GET VISIBILITY
void calculateNormal(void){
    float x1 = 0, x2 = 0, x3 = 0;
    float y1 = 0, y2 = 0, y3 = 0;
    float z1 = 0, z2 = 0, z3 = 0;
    float nax = 0, nay = 0, naz = 0, count = 0;

    for(std::list<Object>::iterator it = OBJ.begin(); it!=OBJ.end(); ++it){
        for(list<Face>::iterator it2 = it->faces.begin(); it2 != it->faces.end(); ++it2){//FACES LIST
              for(list<Vertex>::iterator it3 = it2->vertices.begin(); it3 != it2->vertices.end(); ++it3){
                    if(count == 0) {
                        x1 = it3->x;
                        y1 = it3->y;
                        z1 = it3->z;
                    }
                    if(count == 1) {
                        x2 = it3->x;
                        y2 = it3->y;
                        z2 = it3->z;
                    }
                    if(count == 2) {
                        x3 = it3->x;
                        y3 = it3->y;
                        z3 = it3->z;
                    }
                    count++;
                    if(count >= 3)
                        break;
              }
                it2->vis = GenerateNormal(x1,y1,z1,x2,y2,z2,x3,y3,z3,&nax,&nay,&naz);
                it2->x = nax;
                it2->y = nay;
                it2->z = naz;
                count = 0;
        }
    }
}


//TRANSLATE THE OBJ TO THE GIVEN COORDINATES
void translate(float x, float y, float z){
    for(std::list<Object>::iterator it = OBJ.begin(); it!=OBJ.end(); ++it){
        for(std::list<Face>:: iterator it1 = it->faces.begin(); it1 != it->faces.end(); ++it1){   //FACES LIST
            for(std::list<Vertex>:: iterator it2 = it1->vertices.begin(); it2 != it1->vertices.end(); ++it2){   //VERTEX LIST
                it2->x += x;
                it2->y += y;
                it2->z += z;
             }
        }   
    }
}




//GO THROUGH THE POINTS EVERY AMOUNT OF TIME
void timer(int value){
        ++itBezier1;

        if(itBezier1 != BezierPoints.end()){
            itBezier1++;
            itBezier0++;
        }
    
        glutPostRedisplay();
        glutTimerFunc(60, timer, 1);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (1280, 720);
    glutInitWindowPosition (0, 0);
    glutCreateWindow (" OBJ PROGRAM  |  by: engineererick ");
    init ();
    OBJ = ReadObject();
    glutDisplayFunc(display);
    glutIdleFunc(calculateNormal);
    glutTimerFunc(60, timer, 1); //REGISTERS THE TIMER CALLBACK FUNCTION TO BE TRIGGERED IN AT LEAST # MILLISECONDS

    glutMainLoop();
    return 0;
    
}
