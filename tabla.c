#include <stdio.h>

int i, mult;

// int tab
//tab = atoi(argv[1]); //atoi es para convertir el string en int

int main(argc, char*argv[])
{
	if(argc==0)
	{
		printf("Ingrese algun valor en los parametros \n");
	}
	else
	{
		for(i=1; i<= argc-1; i++)
		{
			mult = i *argc;
			printf();
		}
		
	}
	
}
