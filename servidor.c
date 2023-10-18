#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3550   /* El puerto que será abierto */
#define BACKLOG 2   /* El número de conexiones permitidas */

int main() {
    // Crear el socket del servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket a la dirección
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error al enlazar el socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Error al escuchar conexiones entrantes");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor esperando conexiones en el puerto %d...\n", PORT);

    // Aceptar una conexión entrante
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Error al aceptar la conexión entrante");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado. Esperando datos...\n");

    // Manejar la comunicación con el cliente
    char buffer[1024];
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error al recibir datos");
            break;
        } else if (bytes_received == 0) {
            printf("Cliente desconectado.\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Cliente dice: %s", buffer);

            // Permite al servidor responder al cliente.
            printf("Escribe una respuesta al cliente: ");
            fgets(buffer, sizeof(buffer), stdin);
            send(client_socket, buffer, strlen(buffer), 0);
        }
    }

    // Cerrar los sockets
    close(client_socket);
    close(server_socket);

    return 0;
}