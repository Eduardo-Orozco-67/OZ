#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3550   /* El puerto del servidor */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <dirección_ip_del_servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Crear el socket del cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        perror("Dirección IP no válida");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error al conectar al servidor");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor %s en el puerto %d\n", argv[1], PORT);

    // Enviar datos al servidor
    char message[1024];
    while (1) {
        printf("Escribe un mensaje (o 'q' para salir): ");
        fgets(message, sizeof(message), stdin);

        if (strcmp(message, "q\n") == 0) {
            break;
        }

        // Enviar el mensaje al servidor
        send(client_socket, message, strlen(message), 0);

        // Recibir respuesta del servidor (opcional)
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error al recibir datos del servidor");
            break;
        } else if (bytes_received == 0) {
            printf("Servidor desconectado.\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Servidor dice: %s", buffer);
        }
    }

    // Cerrar el socket del cliente
    close(client_socket);

    return 0;
}