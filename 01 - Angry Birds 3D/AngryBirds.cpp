/***************************************************
  Nombre Del Alumno: Juan Manuel Gómez Silva
                     Cristo Guillermo Álvarez Silva
  Fecha: 24 de Mayo del 2018
  Programa: Angry Birds 3D [Proyecto]
  Versón: 1.0
***************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

#define MAX 50

typedef struct
{
    int ID;
    float x;
    float y;
    float z;
}Vertex;

typedef struct {
   float A;
   float B;
   float C;
}TNormal;

typedef struct
{
    int face[MAX];
    TNormal normal;
}Face;



typedef struct
{
    /*
    float R=0.0;
    float G=1.0;
    float B=1.0;*/
    float R;
    float G;
    float B;
}Color;


typedef struct nodoVer
{
    Vertex ver;
    struct nodoVer *sigVer;
} *L_Vertex;

typedef struct nodoFace
{
    Face  face;
    struct nodoFace *sigFace;
} *L_Face;

typedef struct nodoObj
{
    char nombre[45];
    L_Vertex cabVer;
    L_Face cabFace;
    Color colr;
    struct nodoObj *sigObj;
} *L_Obj;
//Calc_Anima(lvaCUB2,-5.0,-8-0,1.0,-2.0,5.0,1.0,2.0,5.0,1.0,5.0,-8.0,1.0);
float Tx=-5.0,Ty=-8.0,Tz=1.0,Tstart=0;
float Tx2=5.0,Ty2=5.0,Tz2=1.0;
float Tx3=-2.0,Ty3=5.0,Tz3=1.0;
float Tx4=-5.0,Ty4=-8.0,Tz4=1.0;
float Rx=0.0,Ry=0.0,Rz=0.0;
char nomArchivo[45];
L_Vertex PLV;
L_Face PLF;
L_Vertex PLV2;
L_Face PLF2;
L_Vertex PLV3;
L_Face PLF3;
L_Vertex PLV4;
L_Face PLF4;
L_Obj PLO;
L_Obj PLO2;
L_Obj PLO3;
L_Obj PLO4;
L_Vertex lvaCUB;
L_Vertex lvaCUB2;
L_Vertex lvaCUB3;
L_Vertex lvaCUB4;
float t = 0;

Vertex PRP;
Vertex light;
float Ka = 0.5;//Ambient coeficient
float Kd = 0.5;//Difuse coefficient
float Ia = 1.0;//Ambient Intensity
float Il = 1.0;//Light Intensity

void init(void);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void Dib_f_display(L_Obj lo);
void IniL_Vertex(L_Vertex *lv);
void timer(int insu);
int Ins_V_O(L_Obj lo, char *nom, Vertex v);
void IniFace(Face *c);
void R_Archivo( L_Obj *lo, L_Face *lc, L_Vertex *lv, int numOBJ);
int Ins_Obj(L_Obj *lo, char *nom);
int Crea_NodoObj(L_Obj *lo, char *nom);
int Crea_NodoVertex(L_Vertex *lv, Vertex v);
int Ins_Face(L_Face *lc, Face c);
int Ins_F_O(L_Obj lo, char *nom, Face c);
void Muestra_memory(L_Obj lo);
void IniL_Face(L_Face *lc);
void iniListaOBJ(L_Obj *lo);
int Ins_Vertex(L_Vertex *lv, Vertex v);
int Crea_NodoFace(L_Face *lc, Face c);
void Calc_Anima(L_Vertex lv,float Px1,float Py1,float Pz1, float Px2,float Py2,float Pz2,float Px3,float Py3,float Pz3,float Px4,float Py4,float Pz4);
void inici();
void translate(L_Obj lo,float dx,float dy,float dz);
void scale(L_Obj lo,float sx,float sy,float sz);
void rotateX(L_Obj lo,float angle);
void rotateY(L_Obj lo,float angle);
void rotateZ(L_Obj lo,float angle);
void keyboardFunc(unsigned char Key, int x, int y);
void generateNormal(L_Obj lo);
void unitvector(L_Obj lo,L_Face lca);
int faceVisible(L_Obj lo,L_Face lca);
float illumination(L_Obj lo,L_Face lca);

static GLfloat spin = 0.0;

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (1000, 600);
   glutInitWindowPosition (500, 500);
   glutCreateWindow ("Lector with Animacion");
   init();
   inici();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboardFunc);
   glutMainLoop();
   return 0;
}
void IniL_Vertex(L_Vertex *lv)
{
    *lv=NULL;
}
void IniL_Face(L_Face *lc)
{
    *lc=NULL;
}
void iniListaOBJ(L_Obj *lo)
{
   *lo=NULL;
}
void IniFace(Face *c)
{
    int i;

    for(i=0;i<MAX;i++)
    {
        c->face[i]=-1;
    }
}

int Crea_NodoObj(L_Obj *lo, char *nom)
{
    int res=0;


    *lo=(L_Obj)malloc(sizeof(struct nodoObj));
    if(*lo)
    {
        strcpy((*lo)->nombre,nom);
        (*lo)->sigObj=NULL;
        (*lo)->cabVer=NULL;
        (*lo)->cabFace=NULL;
        res=1;
    }


    return(res);
}
int Crea_NodoVertex(L_Vertex *lv, Vertex v)
{
    int res=0;


    *lv = (L_Vertex)malloc(sizeof(struct nodoVer));
    if(*lv)
    {
        (*lv)->ver=v;
        (*lv)->sigVer = NULL;
        res=1;
    }


    return(res);
}
int Crea_NodoFace(L_Face *lc, Face c)
{
    int res=0;


    *lc=(L_Face)malloc(sizeof(struct nodoFace));
    if(*lc)
    {
        (*lc)->face=c;
        (*lc)->sigFace=NULL;
        res=1;
    }


    return(res);
}
int Ins_Obj(L_Obj *lo, char *nom)
{
    int res;


    if(!*lo)
        res=Crea_NodoObj(lo,nom);
    else
        if(!strcmp((*lo)->nombre,nom))
            res=0;
        else
            res=Ins_Obj(&(*lo)->sigObj, nom);


    return (res);
}
int Ins_Vertex(L_Vertex *lv, Vertex v)
{
    int res;


    if(!*lv)
        res=Crea_NodoVertex(lv,v);
    else
        res=Ins_Vertex(&(*lv)->sigVer,v);


    return(res);
}
int Ins_Face(L_Face *lc, Face c)
{
    int res;


    if(!*lc)
        res=Crea_NodoFace(lc,c);
    else
        res=Ins_Face(&(*lc)->sigFace,c);


    return(res);
}
int Ins_V_O(L_Obj lo, char *nom, Vertex v)
{
    int res;


    if(!lo)
        res=0;
    else
        if(!strcmp(lo->nombre,nom))
            res=Ins_Vertex(&lo->cabVer,v);
        else
            res=Ins_V_O(lo->sigObj,nom,v);


    return(res);
}

int Ins_F_O(L_Obj lo, char *nom, Face c)
{
    int res;


    if(!lo)
        res=0;
    else
        if(!strcmp(lo->nombre,nom))
            res=Ins_Face(&lo->cabFace,c);
        else
            res=Ins_F_O(lo->sigObj,nom,c);


    return(res);
}

void inici(){
   IniL_Vertex(&PLV);
   IniL_Vertex(&PLV2);
   IniL_Vertex(&PLV3);
   IniL_Vertex(&PLV4);
   IniL_Face(&PLF);
   IniL_Face(&PLF2);
   IniL_Face(&PLF3);
   IniL_Face(&PLF4);
   iniListaOBJ(&PLO);
   iniListaOBJ(&PLO2);
   iniListaOBJ(&PLO3);
   iniListaOBJ(&PLO4);
   R_Archivo(&PLO,&PLF,&PLV,0);
   R_Archivo(&PLO2,&PLF2,&PLV2,1);
   R_Archivo(&PLO3,&PLF3,&PLV3,2);
   R_Archivo(&PLO4,&PLF4,&PLV4,3);
   //Muestra_memory(PLO);
   //Muestra_memory(PLO2);
}

void R_Archivo(L_Obj *lo, L_Face *lc, L_Vertex *lv, int numOBJ)
{
    FILE *archivo;
    char nomObj[25];
    char nomArc[40];
    char *nombres[]={"p1.obj","AngryBird.obj","p2.obj","p3.obj"};
    char aux;
    char extObj[4];
    int res,r,re;
    Vertex vaux;
    Face caux;
    float faux, fauy, fauz;
    int indver=0;
    int i,iaux;

    //strcpy(nomArc, /*nombres[numOBJ]*/ "AngryBird.obj");
    strcpy(nomArc, nombres[numOBJ]);

    //strcpy(extObj,".obj");
    //strcat(nomArc,extObj);

    archivo=fopen(nomArc,"r");

    if(!archivo)
    {
        printf("No se encuentra el archivo disponible\n");
    }
    else
    {
        while(fread(&aux,sizeof(char),1,archivo)>0)
        {
            if(aux=='o')
            {
                fscanf(archivo,"%c",&aux);
                if(aux==' ')
                {
                    fscanf(archivo,"%s",nomObj);
                    res=Ins_Obj(lo,nomObj);
                }
            }
            if(aux=='v')
            {
                fscanf(archivo,"%c",&aux);
                if(aux==' ')
                {
                        fscanf(archivo,"%f",&faux);
                        vaux.x=faux;
                    if(aux==' ')
                    {
                        fscanf(archivo,"%f",&fauy);
                        vaux.y=fauy;
                    }
                    if(aux==' ')
                    {
                        fscanf(archivo,"%f",&fauz);
                        vaux.z=fauz;
                    }
                    indver++;
                    vaux.ID=indver-1;
                }
                r=Ins_V_O(*lo,nomObj,vaux);
           }
           if(aux=='f')
           {
               i=0;
               IniFace(&caux);
               while(fscanf(archivo,"%d",&iaux)>0)
               {
                   caux.face[i]=iaux-1;
                   i++;
               }
               if((*lo)->sigObj!=0)
               caux.face[i]=-1;
               else
               caux.face[i]=-1;
               re=Ins_F_O(*lo,nomObj,caux);
           }
        }
    }
fclose(archivo);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init(void)
{
    ////////cielo
   //glClearColor(0.1, 0.39, 0.88, 1.0);
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glColor3f(1.0, 1.0, 1.0);
   //////////
   glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
   /////////////////
   glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 1, 40);
   ///////////////////////
   glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);
	//glRotatef(50, 1, 0, 0);
	//glRotatef(70, 0, 1, 0);
   //glClearColor (0.0, 0.0, 0.0, 0.0);
   //glShadeModel (GL_FLAT);
   /////////////iluminacion
   PRP.x = 0.0;
   PRP.y = 0.0;
   PRP.z = 10.0;
   // Direction of light
   light.x = 0.0;
   light.y = 1.0;
   light.z = 0.0;
}
void spinDisplay(void)
{
   spin = spin + 2.0;
   if (spin > 360.0){
      spin = spin - 360.0;

   }
   /*Calc_Anima(lvaCUB2,Tx,Ty,Tz,Tx2,Ty2,Tz2,Tx3,Ty3,Tz3,Tx4,Ty4,Tz4);
      Calc_Anima(lvaCUB3,2.5,2.5,2.5,3.0,3.0,3.0,0.0,0.0,1.1,4.0,-8.1,8.0);
      //Calc_Anima(lvaCUB,3.0,0.0,2.0,3.5,0.5,3.5,0.0,0.0,1.1,4.0,-8.1,8.0);
      //Calc_Anima(lvaCUB4,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.1,4.0,-8.1,8.0);
      if(Rx<=90){
        rotateX (PLO,Rx);
   rotateY (PLO,Rx++);
   rotateZ (PLO,Rx++);
   rotateX (PLO4,Rx++);
   rotateY (PLO4,Rx++);
   rotateZ (PLO4,Rx++);
      }*/
   glutPostRedisplay();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-5.0, 5.0, -5.0, 5.0, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //glRotatef(25.0, 1.0, 1.0, 1.0);
   PLO3->colr.R=0.0;
   PLO3->colr.G=1.0;
   PLO3->colr.B=1.0;
   PLO->colr.R=1.0;
   PLO->colr.G=1.0;
   PLO->colr.B=1.0;
   PLO2->colr.R=0.9882;
   PLO2->colr.G=0.1882;
   PLO2->colr.B=0.0156;
   PLO4->colr.R=0.0;
   PLO4->colr.G=1.0;
   PLO4->colr.B=1.0;
   //rotateX (PLO4,90);
   //rotateY (PLO4,90);
   rotateZ (PLO4,90);
  /* rotateX (PLO4,Rx);
   rotateY (PLO4,Rx);
   rotateZ (PLO4,Rx);*/
   //scale(PLO4,0.8, 0.8, 0.8);
   //translate (PLO4,-2.0, 0.0, -3.0);
   generateNormal(PLO);
   generateNormal(PLO2);
   generateNormal(PLO3);
   generateNormal(PLO4);
}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(spinDisplay);
         break;
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}

void display(void)
{
    float illum;
    lvaCUB = PLO->cabVer;
    lvaCUB2 = PLO2->cabVer;
    lvaCUB3 = PLO3->cabVer;
    lvaCUB4 = PLO4->cabVer;
    //glRotatef(spin,0.0,0.0,1.0);
    timer(0);
    //spinDisplay();

    glClear(GL_COLOR_BUFFER_BIT);

	// Draw a white grid "floor" for the tetrahedron to sit on.
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
	for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
		glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
	glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
	}
	glEnd();
	glPushMatrix();
	//glRotatef(spin, 0.0, 1.0, 0.0);
	glPopMatrix();

    /*if(lvaCUB){
        if(t<=2.0){

            //-6.0,8.5,1.0,1.0,-8.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0
            //-8.0,-8.1,-4.0,-4.1,1.0,4.0,4.0,1.0,1.0,8.0,8.0,1.0

               /* Calc_Anima(lvaCUB4,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.1,4.0,-8.1,8.0);
                Calc_Anima(lvaCUB3,2.5,2.5,2.5,3.0,3.0,3.0,0.0,0.0,1.1,4.0,-8.1,8.0);
                Calc_Anima(lvaCUB,3.0,0.0,2.0,3.5,0.5,3.5,0.0,0.0,1.1,4.0,-8.1,8.0);
                rotateZ (PLO3,Rz+.01);
            Dib_f_display(PLO);*/
            //rotateZ (PLO3,30.0);
            //rotateX (PLO3,30.0);
            //rotateY (PLO3,30.0);
            //Dib_f_display(PLO2);
            //Dib_f_display(PLO3);
            //Dib_f_display(PLO3);
            /*Dib_f_display(PLO3);
            Dib_f_display(PLO4);
            //rotateZ (PLO4,Rz);
            //scale(PLO4,Rx,Ry,Rz);*/

       /* }
        else
            t=0;
    }*/

    /*if(lvaCUB2){
        if(t<=2.0){

            //glRotatef(spin, 0.0, 1.0, 0.0);
            //-6.0,8.5,1.0,1.0,-8.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0
            //-8.0,-8.1,-4.0,-4.1,1.0,4.0,4.0,1.0,1.0,8.0,8.0,1.0
            Calc_Anima(lvaCUB2,Tx,Ty,Tz,Tx2,Ty2,Tz2,Tx3,Ty3,Tz3,Tx4,Ty4,Tz4);
            if(Tstart>=1){
                Calc_Anima(lvaCUB2,Tx,Ty,Tz,Tx2,Ty2,Tz2,Tx3,Ty3,Tz3,Tx4,Ty4,Tz4);
            }
            Dib_f_display(PLO);*/
            //rotateZ (PLO,30.0);
            //rotateX (PLO,30.0);
            //rotateY (PLO3,30.0);

            Dib_f_display(PLO);
            Dib_f_display(PLO2);
            Dib_f_display(PLO3);
            Dib_f_display(PLO4);

            /*Dib_f_display(PLO3);
            Dib_f_display(PLO4);
        }
        else
            t=0;
    }*/



    glutSwapBuffers();
    glFlush();
    //timer(0);

}

void timer(int insu){
    glutTimerFunc(20,timer,2);
    Calc_Anima(lvaCUB2,Tx,Ty,Tz,Tx2,Ty2,Tz2,Tx3,Ty3,Tz3,Tx4,Ty4,Tz4);
    Calc_Anima(lvaCUB3,2.5,2.5,2.5,3.0,3.0,3.0,0.0,0.0,1.1,4.0,-8.1,8.0);
    /*for(Rx=0.0;Rx<=90;Rx++){
        rotateX (PLO,Rx);
   rotateY (PLO,Rx);
   rotateZ (PLO,Rx);
   rotateX (PLO4,Rx);
   rotateY (PLO4,Rx);
   rotateZ (PLO4,Rx);
      }*/
    glutPostRedisplay();
}

void Dib_f_display(L_Obj lo)
{
    float illum;
    L_Vertex lva;
    L_Face lca;
    float vx,vy,vz;
    int i,vc;
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    //glColor3f(1.0, 1.0, 1.0);
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            if(faceVisible(lo,lca)){
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    illum=illumination(lo,lca);
                    //illum=1.0;
                    glColor3f(illum * lo->colr.R, illum * lo->colr.G, illum * lo->colr.B);
                    glBegin(GL_POLYGON);
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            glVertex3f(lva->ver.x,lva->ver.y,lva->ver.z);
                            vx=lva->ver.x;
                            vy=lva->ver.y;
                            vz=lva->ver.z;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
            glEnd();
            }else{
                lca=lca->sigFace;
            }
        }
        lo=lo->sigObj;
    }
    glPopMatrix();
    glutSwapBuffers();
}

void Calc_Anima(L_Vertex lv,float Px1,float Py1,float Pz1, float Px2,float Py2,float Pz2,float Px3,float Py3,float Pz3,float Px4,float Py4,float Pz4) {
        float d = 1.0f/1000;
        float dx,dy,dz;
        t+=d;
        float ct = (1-t);
        float ct2 = ct * ct;
        float ct3 = ct2 * ct;
        float t2 = t*t;
        float t3 = t2*t;
        float c2 = 3*t*ct2;
        float c3 = 3*t2*ct;
        float xA = (ct3*Px1 + c2*Px2 + c3*Px3 + t3*Px4);
        float yA = (ct3*Py1 + c2*Py2 + c3*Py3 + t3*Py4);
        float zA = (ct3*Pz1 + c2*Pz2 + c3*Pz3 + t3*Pz4);
        dx=(xA-lv->ver.x);
        dy=(yA-lv->ver.y);
        dz=(zA-lv->ver.z);
        while(lv){
            lv->ver.x+=(dx);
            lv->ver.y+=(dy);
            lv->ver.z+=(dz);
            lv=lv->sigVer;
        }
}
//////////////////////////////////////////////////////////////////////////
void translate(L_Obj lo,float dx,float dy,float dz){
    L_Vertex lva;
    L_Face lca;
    int i,vc;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            lva->ver.x+=dx;
                            lva->ver.y+=dy;
                            lva->ver.z+=dz;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}
void scale(L_Obj lo,float sx,float sy,float sz){
    L_Vertex lva;
    L_Face lca;
    int i,vc;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            lva->ver.x+=sx;
                            lva->ver.y+=sy;
                            lva->ver.z+=sz;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}
void rotateX(L_Obj lo,float angle){
    L_Vertex lva;
    L_Face lca;
    int i,vc;
    float rad = (angle * 3.1416) / 100.0;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            lva->ver.y= cos(rad) * lva->ver.y - sin(rad) * lva->ver.z;
                            //printf("%f",lva->ver.y);
                            lva->ver.z= sin(rad) * lva->ver.y + cos(rad) * lva->ver.z;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}
void rotateY(L_Obj lo,float angle){
    L_Vertex lva;
    L_Face lca;
    int i,vc;
    float rad = (angle * 3.1416) / 100.0;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            lva->ver.x= cos(rad) * lva->ver.x + sin(rad) * lva->ver.z;
                            lva->ver.z= cos(rad) * lva->ver.z + sin(rad) * lva->ver.x;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}
void rotateZ(L_Obj lo,float angle){
    L_Vertex lva;
    L_Face lca;
    Vertex vec1;
    int i,vc;
    float rad = (angle * 3.1416) / 100.0;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            lva->ver.x= cos(rad) * lva->ver.x - sin(rad) * lva->ver.y;
                            lva->ver.y= sin(rad) * lva->ver.x + cos(rad) * lva->ver.y;
                            //vec1.x=cos(rad) * lva->ver.x - sin(rad) * lva->ver.y;
                            //vec1.y=sin(rad) * lva->ver.x + cos(rad) * lva->ver.y;
                            //Vertex->x=cos(rad) * lva->ver.x - sin(rad) * lva->ver.y;
                            //Vertex->y=sin(rad) * lva->ver.x + cos(rad) * lva->ver.y;
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void generateNormal(L_Obj lo){
    int i;
    L_Vertex lva;
    L_Face lca;
    Vertex vec1,vec2;
    float x1=0,x2=0,x3=0;
    float y1=0,y2=0,y3=0;
    float z1=0,z2=0,z3=0;
    float nax=0,nay=0,naz=0;
    int cont;
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    glBegin(GL_LINE_STRIP);
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {
                            if(cont==0){
                            x1=lva->ver.x;
                            y1=lva->ver.y;
                            z1=lva->ver.z;
                        }
                        if(cont==1){
                            x2=lva->ver.x;
                            y2=lva->ver.y;
                            z2=lva->ver.z;
                        }
                        if(cont==2){
                            x3=lva->ver.x;
                            y3=lva->ver.y;
                            z3=lva->ver.z;
                        }
                        cont++;
                        if(cont>=3){
                            break;
                        }
                        }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }

                        // V2 X V1
                        vec1.x=x2-x1;
                        vec1.y=y2-y1;
                        vec1.z=z2-z1;
                        // V3 - V1
                        vec2.x=x3-x1;
                        vec2.y=y3-y1;
                        vec2.z=z3-z1;
                        // N = vec1 X vec2
                        lca->face.normal.A=(vec1.y*vec2.z)-(vec1.z*vec2.y);
                        lca->face.normal.B=(vec1.z*vec2.x)-(vec1.x*vec2.z);
                        lca->face.normal.C=(vec1.x*vec2.y)-(vec1.y*vec2.x);

                        unitvector(lo,lca);
                        cont=0;

            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }
}


void unitvector(L_Obj lo,L_Face lca){
    L_Vertex lva;
    float d;
    d=sqrt((lca->face.normal.A*lca->face.normal.A)+(lca->face.normal.B*lca->face.normal.B)+(lca->face.normal.C*lca->face.normal.C));
    lca->face.normal.A=lca->face.normal.A/d;
    lca->face.normal.B=lca->face.normal.B/d;
    lca->face.normal.C=lca->face.normal.C/d;
}

///////////////////////////////////////////////////////////////////////////////////////
void keyboardFunc(unsigned char Key, int x, int y)
{
   switch(Key)
   {
       case 'I': case 'i': if(Ia < 1.0) Ia += 0.1; break;
       case 'O': case 'o': if(Ia > 0.0) Ia -= 0.1; break;
       case 'K': case 'k': if(Il < 1.0) Il += 0.1; break;
       case 'L': case 'l': if(Il > 0.0) Il -= 0.1; break;
       case 'A': case 'a': Tx -= 0.5; break;
       case 'W': case 'w': Ty += 0.5; break;
       case 'S': case 's': Ty -= 0.5; break;
       case 'D': case 'd': Tx += 0.5; break;
       ////////////////////////
       case 'T': case 't': Rx -= 0.5; break;
       case 'Y': case 'y': Rx += 0.5; break;
       case 'G': case 'g': Ry -= 0.5; break;
       case 'H': case 'h': Ry += 0.5; break;
       case 'B': case 'b': Rz -= 0.5; break;
       case 'N': case 'n': Rz += 0.5; break;
        /////////////////////////
       case 32 : Tstart +=1; break;
       case 27 : exit(1); break;
   }
   glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////////////////
int faceVisible(L_Obj lo,L_Face lca){
    float prod;
    L_Vertex lva;
    int i,vc;
    /*while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {*/
                            prod=(lca->face.normal.A*PRP.x)+(lca->face.normal.B*PRP.y)+(lca->face.normal.C*PRP.z);
                        /*}
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }*/
    if(prod > 0.0)
        return 1;
    else
        return 0;
}

float illumination(L_Obj lo,L_Face lca){
    float prod,L,Z;
    L_Vertex lva;
    int i,vc;
    L= Ia *Ka;// Ambient light
    //Z=Il *Kd;
    /*
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    while(lva)
                    {
                        if(lca->face.face [i] == lva->ver.ID)
                        {*/
                           // Point product N.L
                           //prod=lca->face.normal.A*light.x+(lca->face.normal.B*light.y);
                           prod=(lca->face.normal.A*light.x)+(lca->face.normal.B*light.y)+(lca->face.normal.C*light.z);
                           L= L+Il*Kd* (prod);// Diffuse light
                       /* }
                        lva=lva->sigVer;
                    }
                    lva=lo->cabVer;
                }
                else
                {
                    break;
                }
            }
            lca=lca->sigFace;
        }
        lo=lo->sigObj;
    }*/
    return(L);
}
void ProductVect(float x1, float y1, float z1, float x2, float y2, float z2, float *prx, float *pry, float *prz){
    *prx= y1 * z2 - z1 *y2;
    *pry= z1 * x2 - x1 *z2;
    *prz= x1 * y2 - y1 * x2;
}
void VerticeData(float x1, float y1, float z1, float x2, float y2, float z2, float *prx, float *pry, float *prz){
    *prx= x1-x2;
    *pry= y1-y2;
    *prz= z1-z2;
}/*
float ProductEsc(){

}*/
