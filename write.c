#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *fifo_name = "myfifo"; // Nombre del tubo con nombre
    int fd;

    // Crear el tubo con nombre (si no existe)
    mkfifo(fifo_name, 0666);

    // Abrir el tubo con nombre para escritura
    fd = open(fifo_name, O_WRONLY);

    if (fd == -1) {
        perror("Error al abrir el tubo con nombre para escritura");
        exit(EXIT_FAILURE);
    }

    char message[] = "Hola, programa lector!";

    // Escribir el mensaje en el tubo con nombre
    write(fd, message, strlen(message) + 1);
    close(fd);

    printf("Mensaje enviado al programa lector: %s\n", message);

    return 0;
}
