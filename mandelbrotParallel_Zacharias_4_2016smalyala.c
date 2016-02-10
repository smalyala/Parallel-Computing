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

double xMin ;
double xMax;
double yMin ;
double yMax;
double iter = 100.0;
int arr[width+1][height+1];
double y, x, Zx, Zy, xt, yt;
int xIt, yIt, t;
int        rank   ;
int        size   ;
MPI_Status status ;
int        tag = 0;
int ret[height+1], jmpi, assign, colWorker;

void displayfunc()
{
   //
   // other variables
   //
   //
   // boilerplate
   //
   int col = 0;
   //
   // manager has rank = 0
   //
   int tempCol[size], currCol;

   for (assign = 1; assign < size; assign++)
   {
      MPI_Send( &col , 1 , MPI_INT , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &xMin , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &xMax , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &yMin , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &yMax , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &iter , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      tempCol[assign] = col;
      col += 1;
   }
   while (col <= width)
   {
      MPI_Recv( ret , height+1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
      assign = status.MPI_SOURCE;
      currCol = tempCol[assign];
      for (yIt = 0; yIt <= height; yIt++)
      {
         arr[currCol][yIt] = ret[yIt];
      }
      MPI_Send( &col , 1 , MPI_INT , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &xMin , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &xMax , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &yMin , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &yMax , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &iter , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
      tempCol[assign] = col;
      col += 1;
   }
   //col = -1;
   for (jmpi = 1; jmpi < size; jmpi++)
   {
      MPI_Recv( ret , height+1 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
      assign = status.MPI_SOURCE;
      currCol = tempCol[assign];
      for (yIt = 0; yIt <= height; yIt++)
      {
         arr[currCol][yIt] = ret[yIt];
      }
      //MPI_Send( &col , 1 , MPI_INT , assign , tag , MPI_COMM_WORLD ) ;
   }

   glClear(GL_COLOR_BUFFER_BIT);

   for( xIt = 0; xIt <= width ; xIt++ )
   {
      for( yIt = 0; yIt <= height ; yIt++ )
      { 
         t = arr[xIt][yIt];
         if (t == iter)
         {
            glColor3f( 0.0 , 0.0 , 0.0 );
         }
         else
         {
            glColor3f(2.0*t/iter, 3.0*t/iter, 8.0*t/iter);
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
         xMax = xt + .25 * xr;
         yMax = yt + .25 * yr;
         xMin = xt - .25 * xr;
         yMin = yt - .25 * yr;
         //iter *= 2;
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

   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

   if (rank == 0)
   {
      xMin = -2.0;
      xMax = 2.0;
      yMin = -2.0;
      yMax = 2.0;
      iter = 100.0;
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
   }
   else
   {
      while (1) 
      {
         int arrTemp[height+1];
         MPI_Recv( &colWorker , 1 , MPI_INT , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &xMin , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &xMax , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &yMin , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &yMax , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &iter , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         if (colWorker < 0) break;
         for( yIt = 0; yIt <= height ; yIt++ )
         { 
            x = 1.0 * colWorker/width * (xMax - xMin) + xMin;
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
            arrTemp[yIt] = t;
            
         }

         MPI_Send( arrTemp, height+1 , MPI_INT , 0 , tag , MPI_COMM_WORLD ) ;
      }
   }
   //
   //printf("%s\n", "here");

   MPI_Finalize();
   
   return 0;
}
// 
// end of file
// 