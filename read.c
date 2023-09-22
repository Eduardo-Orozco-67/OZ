#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *fifo_name = "myfifo"; // Nombre del tubo con nombre
    int fd;
    char buffer[1024];

    // Abrir el tubo con nombre para lectura
    fd = open(fifo_name, O_RDONLY);

    if (fd == -1) {
        perror("Error al abrir el tubo con nombre para lectura");
        exit(EXIT_FAILURE);
    }

    // Leer datos del tubo con nombre
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

    if (bytes_read == -1) {
        perror("Error al leer del tubo con nombre");
        exit(EXIT_FAILURE);
    }

    printf("Mensaje recibido del programa escritor: %s\n", buffer);

    close(fd);

    return 0;
}
