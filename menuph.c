#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipe_parent_child[2]; // Tubería padre al hijo
    int pipe_child_parent[2]; // Tubería hijo al padre
    pid_t pid;
    
    // Crear las tuberías
    if (pipe(pipe_parent_child) == -1 || pipe(pipe_child_parent) == -1) {
        perror("Error al crear las tuberías");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        close(pipe_parent_child[1]); // Cerrar el extremo de escritura de padre a hijo
        close(pipe_child_parent[0]); // Cerrar el extremo de lectura de hijo a padre

        char message[1024];

        while (1) {
            // Leer el mensaje del padre desde la tubería (menú)
            ssize_t bytes_read = read(pipe_parent_child[0], message, sizeof(message));
            if (bytes_read <= 0) {
                perror("Error al leer desde la tubería padre-hijo");
                break;
            }
            printf("Hijo: Menú recibido del padre:\n%s\n", message);

            int option;
            sscanf(message, "%d", &option);

            char response[1024];
            memset(response, 0, sizeof(response));

            switch (option) {
                case 1:
                    // Calcular factorial
                    int num;
                    sscanf(message, "%*d %d", &num);
                    int factorial = 1;
                    for (int i = 1; i <= num; i++) {
                        factorial *= i;
                    }
                    sprintf(response, "El factorial de %d es %d\n", num, factorial);
                    break;
                case 2:
                    // Generar serie Fibonacci
                    int num_terms;
                    sscanf(message, "%*d %d", &num_terms);
                    long long a = 0, b = 1;
                    for (int i = 0; i < num_terms; i++) {
                        sprintf(response + strlen(response), "%lld ", a);
                        long long temp = a + b;
                        a = b;
                        b = temp;
                    }
                    strcat(response, "\n");
                    break;
                case 3:
                    // Mostrar tabla de multiplicar
                    int multiplier;
                    sscanf(message, "%*d %d", &multiplier);
                    for (int i = 1; i <= 10; i++) {
                        sprintf(response + strlen(response), "%d x %d = %d\n", multiplier, i, multiplier * i);
                    }
                    break;
                case 4:
                    // Calcular área del rectángulo
                    float length, width;
                    sscanf(message, "%*d %f %f", &length, &width);
                    float area = length * width;
                    sprintf(response, "El área del rectángulo es %.2f\n", area);
                    break;
                case 5:
                    // Salir del proceso hijo
                    exit(EXIT_SUCCESS);
                    break;
                default:
                    strcpy(response, "Opción inválida\n");
                    break;
            }

            // Enviar la respuesta al padre
            write(pipe_child_parent[1], response, strlen(response) + 1);
        }
    } else { // Proceso padre
        close(pipe_parent_child[0]); // Cerrar el extremo de lectura de padre a hijo
        close(pipe_child_parent[1]); // Cerrar el extremo de escritura de hijo a padre

        char message[1024];

        do {
            // Mostrar el menú al usuario
            printf("+-----------------------------+\n");
            printf("Padre: Elija una opción:\n");
            printf("1: Calcular el factorial de un número\n");
            printf("2: Generar serie Fibonacci\n");
            printf("3: Mostrar tabla de multiplicar\n");
            printf("4: Calcular el área de un rectángulo\n");
            printf("5: Salir\n");
            printf("+-----------------------------+\n");

            // Leer la opción del usuario
            int option;
            printf("Padre: Opción: ");
            scanf("%d", &option);

            char input_message[1024];
            char response[1024];

            switch (option) {
                case 1:
                    // Solicitar número para calcular factorial
                    printf("Padre: Ingrese un número para calcular su factorial: ");
                    scanf("%d", &option);
                    sprintf(input_message, "%d", option);
                    break;
                case 2:
                    // Solicitar cantidad de términos para la serie Fibonacci
                    printf("Padre: Ingrese la cantidad de términos para la serie Fibonacci: ");
                    scanf("%d", &option);
                    sprintf(input_message, "%d", option);
                    break;
                case 3:
                    // Solicitar número para mostrar tabla de multiplicar
                    printf("Padre: Ingrese un número para mostrar su tabla de multiplicar: ");
                    scanf("%d", &option);
                    sprintf(input_message, "%d", option);
                    break;
                case 4:
                    // Solicitar dimensiones para calcular área del rectángulo
                    float length, width;
                    printf("Padre: Ingrese la longitud del rectángulo: ");
                    scanf("%f", &length);
                    printf("Padre: Ingrese el ancho del rectángulo: ");
                    scanf("%f", &width);
                    sprintf(input_message, "%.2f %.2f", length, width);
                    break;
                case 5:
                    // Salir del programa
                    sprintf(input_message, "5");
                    break;
                default:
                    printf("Padre: Opción inválida\n");
                    continue;
            }

            // Enviar la opción al hijo
            write(pipe_parent_child[1], input_message, strlen(input_message) + 1);

            // Leer y mostrar la respuesta del hijo
            ssize_t bytes_read = read(pipe_child_parent[0], response, sizeof(response));
            if (bytes_read <= 0) {
                perror("Error al leer desde la tubería hijo-padre");
                break;
            }
            printf("Padre: Respuesta del hijo:\n%s\n", response);

        } while (1);

        // Esperar a que el hijo termine
        wait(NULL);
    }

    return 0;
}
