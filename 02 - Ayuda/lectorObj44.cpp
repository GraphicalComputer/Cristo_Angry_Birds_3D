#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#define MAX 50

typedef struct
{
 int ID;
 float x;
 float y;
 float z;
}Vertex;
typedef struct
{
 int face[MAX];
}Face;
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
   struct nodoObj *sigObj;
} *L_Obj;
char nomArchivo[45];
L_Vertex PLV;
   L_Face PLF;
   L_Obj PLO;
/*  void IniL_Vertex;
  void IniL_Face(void);
   void iniListaOBJ(void);
   void N_Archivo(void);
   void R_Archivo(void);*/
void init(void);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void Dib_f_display(L_Obj lo);
void IniL_Vertex(L_Vertex *lv);
int Ins_V_O(L_Obj lo, char *nom, Vertex v);
void IniFace(Face *c);
void R_Archivo(char nombreArchivo[], L_Obj *lo, L_Face *lc, L_Vertex *lv);
int Ins_Obj(L_Obj *lo, char *nom);
int Crea_NodoObj(L_Obj *lo, char *nom);
int Crea_NodoVertex(L_Vertex *lv, Vertex v);
int Ins_Face(L_Face *lc, Face c);
int Ins_F_O(L_Obj lo, char *nom, Face c);
void Muestra_memory(L_Obj lo);
void IniL_Face(L_Face *lc);
void iniListaOBJ(L_Obj *lo);
void N_Archivo(char nombreArchivo[]);
int Ins_Vertex(L_Vertex *lv, Vertex v);
int Crea_NodoFace(L_Face *lc, Face c);
void inici();

static GLfloat spin = 0.0;

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Lector Obj");
   init ();
   inici();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
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
void N_Archivo(char nombreArchivo[])
{
    printf("Archivo OBJ: ");
    gets(nombreArchivo);
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
   IniL_Face(&PLF);
   iniListaOBJ(&PLO);
   N_Archivo(nomArchivo);
   R_Archivo(nomArchivo,&PLO,&PLF,&PLV);
   Muestra_memory(PLO);
}
void R_Archivo(char nombreArchivo[], L_Obj *lo, L_Face *lc, L_Vertex *lv)
{
    FILE *archivo;
    char nomObj[25];
    char aux;
    char extObj[4];
    int res,r,re;
    Vertex vaux;
    Face caux;
    float faux, fauy, fauz;
    int indver=0;
    int i,iaux;

    strcpy(extObj,".obj");
    strcat(nombreArchivo,extObj);
    archivo=fopen(nombreArchivo,"r");

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
void Muestra_memory(L_Obj lo)
{
    L_Vertex lva;
    L_Face lca;
    int i;

    while(lo)
    {
        printf("%s\n",lo->nombre);
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lva)
        {
            printf("%f ",lva->ver.x);
            printf("%f ",lva->ver.y);
            printf("%f\n",lva->ver.z);
            lva=lva->sigVer;
        }
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1)
                    printf("%d ",lca->face.face[i]);
                else
                {
                    printf("\n");
                    i=MAX;
                }
            }
            lca=lca->sigFace;
        }
        printf("\n");
        lo=lo->sigObj;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
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
   glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(45, 1.0, 1.0, 1.0);
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
   Dib_f_display(PLO);
   //glRotatef(45,1.0,1.0, 1.0);
   //system("pause");
}

void Dib_f_display(L_Obj lo)
{
    L_Vertex lva;
    L_Face lca;
    float vx,vy,vz;
    int i,vc;
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    while(lo)
    {
        lva=lo->cabVer;
        lca=lo->cabFace;
        while(lca)
        {
            for(i=0;i<MAX;i++)
            {
                if(lca->face.face [i]!=-1){
                    //lca->face.face[i]
                    glBegin(GL_LINE_STRIP);
                    while(lva)
                    {
                        //printf("\n%d ",lva->ver.ID);
                        //printf("%d ",lca->face.face[i]);
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
        }

        lo=lo->sigObj;
    }
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}
