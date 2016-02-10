// MPI Demo
//    mpicc mpiDemo.c
//    mpirun -np 4 a.out
// 
// Manager-Worker model for parallel processing.
// 
// time ... real ... user 
// 
// htop 

#include <stdio.h>
#include <stdlib.h>
//
#include <time.h>
#include "mpi.h"

// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
//    gcc -lGL -lGLU -lglut fireGL.c
// 
// tab character '\t' before gcc
// 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include <GL/glut.h>
// 
#define height 600
#define width  600

double xMin = -2.0;
double xMax = 2.0;
double yMin = -2.0;
double yMax = 2.0;
double iter = 100.0;

void displayfunc()
{
   double y, x, Zx, Zy, xt, yt;
   int xIt, yIt, t;
   //
   glClear(GL_COLOR_BUFFER_BIT);
   //
   for( xIt = 0; xIt <= width ; xIt++ )
   {
      for( yIt = 0; yIt <= height ; yIt++ )
      { 
         x = 1.0 * xIt/width * (xMax - xMin) + xMin;
         y = 1.0 * yIt/height * (yMax - yMin) + yMin;
         Zx = 0.0;
         Zy = 0.0;
         t = 0;

         while(Zx*Zx + Zy*Zy <= 4 && t < iter)
         {
            yt = 2*Zx*Zy + y;
            xt = Zx*Zx - Zy*Zy + x;
            Zx = xt;
            Zy = yt;
            t++;
         }

         if (t == iter)
         {
            glColor3f( 0.0 , 0.0 , 0.0 );
         }
         else
         {
            glColor3f(2.0*t/iter, 3.0*t/iter, 1.0*t/iter);
         }

         glBegin(GL_POINTS);
         glVertex2f(xIt,yIt);
         glEnd();
      }
   }
   
   glutSwapBuffers() ;
}

void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)height,(GLsizei)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*height,0.0,1.0*height); // always a square
   glMatrixMode(GL_MODELVIEW);
}

void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         yscr = height - yscr;
         double xr = (xMax - xMin);
         double yr = (yMax - yMin);
         double xt = 1.0 * xscr/width * xr + xMin;
         double yt = 1.0 * yscr/height * yr + yMin;
         xMax = xt + .2 * xr;
         yMax = yt + .2 * yr;
         xMin = xt - .2 * xr;
         yMin = yt - .2 * yr;
         
      }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         srand( time(NULL) ) ;
         iter *= 2;
      }
   }
   glutPostRedisplay();
}

int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(height,height);
   glutInitWindowPosition(100,50);
   glutCreateWindow("Mandelbrot");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   srand( 12345 ) ; // arbitrary random seed
   //
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutPostRedisplay();
   glutMouseFunc(mousefunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 