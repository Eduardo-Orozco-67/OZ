#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 4086

// Función de control de señal para el proceso hijo
void sigchild_handler(int signum) {
    if (signum == SIGCHLD) {
        // El proceso hijo ha terminado
        char exit_message[] = "Hijo (PID: %d): Ha terminado.\n";
        char buffer[BUFFER_SIZE];
        snprintf(buffer, sizeof(buffer), exit_message, getpid());
        write(STDOUT_FILENO, buffer, strlen(buffer));
        exit(0);
    }
}

int main() {
    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];
    pid_t pid;

    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("Error al crear las tuberías");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        close(pipe_parent_to_child[1]); // Cerramos el extremo de escritura del pipe del padre al hijo
        close(pipe_child_to_parent[0]); // Cerramos el extremo de lectura del pipe del hijo al padre

        char message[BUFFER_SIZE];
        int continue_running = 1; // Variable de estado para continuar ejecutándose

        while (continue_running) {
            ssize_t bytes_read = read(pipe_parent_to_child[0], message, BUFFER_SIZE);

            if (bytes_read <= 0) {
                break;
            }

            char response[BUFFER_SIZE];

            // Verificar si el mensaje es 'exit' para salir del programa
            if (strcmp(message, "exit\n") == 0) {
                char exit_message[] = "Hijo (PID: %d): Saliendo del programa.\n";
                snprintf(response, sizeof(response), exit_message, getpid());
                write(pipe_child_to_parent[1], response, strlen(response));
                continue_running = 0; // Cambiar el estado para detenerse
                break;
            }

            char prompt[] = "Hijo (PID: %d) Ingresa una respuesta para papa: ";
            snprintf(response, sizeof(response), prompt, getpid());
            write(STDOUT_FILENO, response, strlen(response));

            bytes_read = read(STDIN_FILENO, response, BUFFER_SIZE);
            response[bytes_read] = '\0'; // Asegurarse de que la cadena esté terminada
            write(pipe_child_to_parent[1], response, bytes_read);
        }

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
    } else { // Proceso padre
        // Configurar el manejador de señal para manejar SIGCHLD (proceso hijo terminado)
        signal(SIGCHLD, sigchild_handler);

        close(pipe_parent_to_child[0]); // Cerramos el extremo de lectura del pipe del padre al hijo
        close(pipe_child_to_parent[1]); // Cerramos el extremo de escritura del pipe del hijo al padre

        char message[BUFFER_SIZE];
        int continue_running = 1; // Variable de estado para continuar ejecutándose

        while (continue_running) {
            char prompt[] = "Padre (PID: %d) Ingrese un mensaje para hijo: ";
            snprintf(message, sizeof(message), prompt, getpid());
            write(STDOUT_FILENO, message, strlen(message));

            ssize_t bytes_read = read(STDIN_FILENO, message, BUFFER_SIZE);
            message[bytes_read] = '\0'; // Asegurarse de que la cadena esté terminada
            write(pipe_parent_to_child[1], message, bytes_read);

            // Verificar si el mensaje es 'exit' para salir del programa
            if (strcmp(message, "exit\n") == 0) {
                char exit_message[] = "Padre (PID: %d): Saliendo del programa.\n";
                snprintf(message, sizeof(message), exit_message, getpid());
                write(STDOUT_FILENO, message, strlen(message));
                continue_running = 0; // Cambiar el estado para detenerse
                break;
            }

            char response[BUFFER_SIZE];

            bytes_read = read(pipe_child_to_parent[0], response, BUFFER_SIZE);

            if (bytes_read <= 0) {
                break;
            }

            char response_prompt[] = "Mensaje de hijo para papa (PID: %d)%s";
            snprintf(message, sizeof(message), response_prompt, getpid(), response);
            write(STDOUT_FILENO, message, strlen(message));
        }

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
    }

    return 0;
}
