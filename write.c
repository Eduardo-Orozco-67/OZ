#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    char message[256];

    mkfifo("chat", 0666);
    mkfifo("chat2", 0666);

    int fifo_fd = open("chat", O_WRONLY); //abre el archivo con write
    int fd2 = open("chat2", O_RDONLY); //abre el archivo con read para leer al chat2

    printf("Chat iniciado (escriba 'adios' para salir):\n");

    while (1) {
        printf("Sender: ");
        fgets(message, sizeof(message), stdin);

        write(fifo_fd, message, sizeof(message));

        if (strcmp(message, "adios\n") == 0) {
            break; // Salir del bucle si el usuario escribe 'adios'
        }

        read(fd2, message, sizeof(message));

        printf("Mensaje recibido: %s", message);
    }

    close(fifo_fd);
    close(fd2);
    unlink("chat"); // Eliminar el FIFO cuando se cierra
    unlink("chat2"); // Eliminar el FIFO cuando se cierra

    return EXIT_SUCCESS;
}
