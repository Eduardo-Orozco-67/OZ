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

void empty_stdin() {
    int s;
    do {
        s = getchar();
    } while (s != '\n' && s != EOF);
}

int main() {
    int fd_parent_to_child[2]; // Tubería del padre al hijo
    int fd_child_to_parent[2]; // Tubería del hijo al padre

    // Crear las tuberías
    if (pipe(fd_parent_to_child) == -1 || pipe(fd_child_to_parent) == -1) {
        perror("Error al crear las tuberías");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork(); // Crear un proceso hijo

    if (child_pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Proceso hijo

        close(fd_parent_to_child[1]); // Cerrar el descriptor de escritura del padre al hijo
        close(fd_child_to_parent[0]); // Cerrar el descriptor de lectura del hijo al padre

        int filas;
        read(fd_parent_to_child[0], &filas, sizeof(filas)); // Recibir el número de medicamentos

        struct Medicamento *medicamentos = (struct Medicamento *)malloc(filas * sizeof(struct Medicamento));

        if (medicamentos == NULL) {
            perror("Error al asignar memoria");
            exit(EXIT_FAILURE);
        }

        // Leer los datos de los medicamentos desde el padre
        for (int i = 0; i < filas; i++) {
            read(fd_parent_to_child[0], medicamentos[i].nombre, sizeof(medicamentos[i].nombre));
            read(fd_parent_to_child[0], &medicamentos[i].precio, sizeof(medicamentos[i].precio));
            read(fd_parent_to_child[0], &medicamentos[i].descuento, sizeof(medicamentos[i].descuento));
            medicamentos[i].precio *= (1 - medicamentos[i].descuento / 100); // Aplicar descuento
        }

            // Ordenar los medicamentos por precio de menor a mayor
        for (int i = 0; i < filas - 1; i++) {
            for (int j = i + 1; j < filas; j++) {
                if (medicamentos[i].precio > medicamentos[j].precio) { // Cambio de < a >
                    // Intercambiar medicamentos
                    struct Medicamento temp = medicamentos[i];
                    medicamentos[i] = medicamentos[j];
                    medicamentos[j] = temp;
                }
            }
        }


        // Enviar los medicamentos ordenados al padre
        for (int i = 0; i < filas; i++) {
            write(fd_child_to_parent[1], medicamentos[i].nombre, sizeof(medicamentos[i].nombre));
            write(fd_child_to_parent[1], &medicamentos[i].precio, sizeof(medicamentos[i].precio));
            write(fd_child_to_parent[1], &medicamentos[i].descuento, sizeof(medicamentos[i].descuento));
        }

        // Liberar memoria
        free(medicamentos);

        close(fd_parent_to_child[0]); // Cerrar el descriptor de lectura del padre al hijo
        close(fd_child_to_parent[1]); // Cerrar el descriptor de escritura del hijo al padre

        exit(EXIT_SUCCESS);

    } else {
        // Proceso padre
        close(fd_parent_to_child[0]); // Cerrar el descriptor de lectura del padre al hijo
        close(fd_child_to_parent[1]); // Cerrar el descriptor de escritura del hijo al padre

        int filas;

        // Solicitar al usuario el número de medicamentos
        printf("Ingrese el número de medicamentos: ");
        scanf("%i", &filas);
        write(fd_parent_to_child[1], &filas, sizeof(filas)); // Enviar el número de medicamentos al hijo

        struct Medicamento medicamentos[filas];

        // Leer los datos de los medicamentos desde el usuario
        for (int i = 0; i < filas; i++) {
            empty_stdin();
            printf("Ingrese el nombre del medicamento %d:  ", i + 1);
            fgets(medicamentos[i].nombre, sizeof(medicamentos[i].nombre), stdin);

            printf("Ingrese el precio del medicamento %s : ", medicamentos[i].nombre);
            scanf("%f", &medicamentos[i].precio);

            printf("Ingrese el descuento para %s (en porcentaje): ", medicamentos[i].nombre);
            scanf("%f", &medicamentos[i].descuento);
        }

        // Enviar los datos de los medicamentos al hijo
        for (int i = 0; i < filas; i++) {
            write(fd_parent_to_child[1], medicamentos[i].nombre, sizeof(medicamentos[i].nombre));
            write(fd_parent_to_child[1], &medicamentos[i].precio, sizeof(medicamentos[i].precio));
            write(fd_parent_to_child[1], &medicamentos[i].descuento, sizeof(medicamentos[i].descuento));
        }

        // Leer los medicamentos ordenados desde el hijo
        printf("\nMedicamentos ordenados de menor a mayor precio con descuento:\n");
        for (int i = 0; i < filas; i++) {
            read(fd_child_to_parent[0], medicamentos[i].nombre, sizeof(medicamentos[i].nombre));
            read(fd_child_to_parent[0], &medicamentos[i].precio, sizeof(medicamentos[i].precio));
            read(fd_child_to_parent[0], &medicamentos[i].descuento, sizeof(medicamentos[i].descuento));
            printf("Nombre: %s, Precio: %.2f, Descuento: %.2f%%\n", medicamentos[i].nombre, medicamentos[i].precio, medicamentos[i].descuento);
        }

        // Esperar al hijo
        wait(NULL);

        close(fd_parent_to_child[1]); // Cerrar el descriptor de escritura del padre al hijo
        close(fd_child_to_parent[0]); // Cerrar el descriptor de lectura del hijo al padre
    }

    return 0;
}
