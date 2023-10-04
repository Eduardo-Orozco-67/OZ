#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    char mensaje[256];
    mkfifo("chat2", 0666);
    int fd1 = open("chat", O_RDONLY); //abre el archivo con read para leer al chat1
    int fd2 = open("chat2", O_WRONLY); //abre el archivo con write

    printf("Chat iniciado (escriba 'adios' para salir):\n");

    while (1) {
        read(fd1, mensaje, sizeof(mensaje));
        printf("Mensaje recibido: %s", mensaje);

        // Comprueba si el mensaje recibido es "adios"
        if (strcmp(mensaje, "adios\n") == 0) {
            printf("Receiver: Adios, el chat ha finalizado.\n");
            break; // Salir del bucle si se recibe "adios"
        }

        printf("Receiver: ");
        fgets(mensaje, sizeof(mensaje), stdin);
        
        write(fd2, mensaje, sizeof(mensaje));
    }

    close(fd1);
    close(fd2);
    unlink("chat"); // Eliminar el FIFO cuando se cierra
    unlink("chat2"); // Eliminar el FIFO cuando se cierra

    return EXIT_SUCCESS;
}
