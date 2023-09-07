#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


int main()

{

   //Variable para el ciclo de creacion de procesos HIJOS
   int contador;

    int filas=0, columnas=0, f=0, c=0;
    int i, x, y=0, z=0, s, m, n;


      int lt;
      

   //for para ciclar la creacion de HIJOS
   for(contador=1;contador<=1;contador++)
   {
      //creacion de procesos
      if(fork()==0)
      { 
         if(contador==1)
         {             
            puts("HOLAAAAA SOY HIJO 1");
            printf("PID del HIJO %d proceso = %d Pid padre = %d\n\n", getpid(),contador,getppid());
         	exit(2);
         }
         
      }
      
    	else
		{
			wait(NULL);
			continue;	
   	}	
      
   }
    printf("\nSoy el Padre con PID de: %i Pid de mi padre = %i\n", getpid(), getppid());
  
 return 0;
}
