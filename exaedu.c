/*JOSE EDUARDO OROZCO CARDENAS B200077
SISTEMAS OPERATIVOS LIDTS 7D
27/09/2023
EXAEMEN PRACTICO
MCC. VANESSA BENAVIDES GARCIA*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

// Estructura para representar un medicamento
struct Medicamento {
    char nombre[101];
    float precio;
    float descuento;
};

int main() {
    int contador;

    int filas = 0, columnas = 0, f = 0, c = 0;
    int i, x, y = 0, z = 0, s, m, n;
    struct Medicamento *medicamentos = NULL; // Arreglo de estructuras para medicamentos
    double prom, acum = 0;

    int op;
    char cadena[100];

    int lt;

    void empty_stdin() {
        int s;
        do {
            s = getchar();
        } while (s != '\n' && s != EOF);
    }

    for (contador = 1; contador <= 2; contador++) {
        if (fork() == 0) {
            if (contador == 1) {
                puts("HOLAAAAA SOY HIJO 1");
                printf("\nPID del HIJO %d proceso = %d Pid padre = %d\n", getpid(), contador, getppid());
                printf("\n\ninserte datos del medicamento \n\n");

                printf("Ingrese el numero de medicamentos: ");
                scanf("%i", &filas);
                n = filas;

                medicamentos = (struct Medicamento *)malloc(filas * sizeof(struct Medicamento));

                if (medicamentos == NULL) {
                    printf("No se pudo asignar memoria");
                } else {
                    for (i = 0; i < filas; i++) {
                        printf("Ingrese el nombre del medicamento %d:  ", i + 1);
                        scanf("%s", medicamentos[i].nombre);
                        printf("Ingrese el precio del medicamento %s : ", medicamentos[i].nombre);
                        scanf("%f", &medicamentos[i].precio);
                        printf("Ingrese el descuento para %s (en porcentaje): ", medicamentos[i].nombre);
                        scanf("%f", &medicamentos[i].descuento);
                        medicamentos[i].precio *= (1 - medicamentos[i].descuento / 100); // Aplicar descuento
                        printf("\n");
                        printf("\n");
                    }
                    printf("\n");

                    printf("\t---------------------------------------------------------------------------------\n");
                    printf("\t|   NOMBRE  \t|\tPRECIO REAL\t|\tDESCUENTO\t|\tPRECIO DESCUENTO\t|\n");
                    printf("\t---------------------------------------------------------------------------------\n");

                    for (f = 0; f < filas; f++) {
                        printf("\t-----------------------------------------------------------------------------\n");
                        printf("\t|\t%s", medicamentos[f].nombre);
                        printf("\t|\t\t%.2f\t\t|\t\t%.2f%%\t\t|\t\t%.2f\t\t|\n", medicamentos[f].precio / (1 - medicamentos[f].descuento / 100), medicamentos[f].descuento, medicamentos[f].precio);
                        printf("\t-----------------------------------------------------------------------------\n");
                        acum += medicamentos[f].precio;
                    }
                    prom = acum / filas;
                    printf("\tpromedio de precios con descuento = %.2f\n\n\n", prom);

                    // Mostrar los medicamentos ordenados por precio con descuento
                    printf("\n\nMedicamentos ordenados de menor a mayor precio con descuento:\n");
                    for (int i = 0; i < filas; i++) {
                        printf("Nombre: %s, Precio Real: %.2f, Precio Descuento: %.2f\n", medicamentos[i].nombre, medicamentos[i].precio / (1 - medicamentos[i].descuento / 100), medicamentos[i].precio);
                    }
                }

                exit(2);
            }

            if (contador == 2) {
                empty_stdin();

                printf("\nPID del HIJO %i proceso = %i Pid padre = %i\n\n", getpid(), contador, getppid());

                printf("\tDame un numero x: ");
                scanf("%d", &op);
                printf("\n\n\tDame una Cadena de texto:  ");
                scanf("%s", cadena);
                empty_stdin();

                for (int k = 0; k < op; k++) {
                    printf("\n\t%s", cadena);
                }

                exit(2);
            }
        } else {
            wait(NULL);
            continue;
        }
    }

    printf("\n\n\nSoy el Padre con PID de: %i Pid de mi padre = %i\n", getpid(), getppid());

    // Ordenar los medicamentos por precio con descuento (de menor a mayor)
    for (int i = 0; i < filas - 1; i++) {
        for (int j = i + 1; j < filas; j++) {
            if (medicamentos[i].precio > medicamentos[j].precio) {
                // Intercambiar medicamentos
                struct Medicamento temp = medicamentos[i];
                medicamentos[i] = medicamentos[j];
                medicamentos[j] = temp;
            }
        }
    }

    // Liberar memoria
    free(medicamentos);

    return 0;
}

