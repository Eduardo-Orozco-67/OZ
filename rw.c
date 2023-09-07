#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char user_name[20];
    char number_str[20];

    // Solicitar el nombre de usuario
    write(1, "Ingrese su nombre de usuario: ", 31);
    ssize_t user_bytes_read = read(0, user_name, sizeof(user_name) - 1);
    user_name[user_bytes_read - 1] = '\0'; 

    write(1, "Ingrese un número: ", sizeof("Ingrese un número: "));
    ssize_t number_bytes_read = read(0, number_str, sizeof(number_str) - 1);
    number_str[number_bytes_read - 1] = '\0';

    // Imprimir el nombre de usuario y el número
    write(1, "Nombre de usuario: ", 20);
    write(1, user_name, strlen(user_name));
    write(1, "\n", sizeof("\n"));

    write(1, "Número ingresado: ", sizeof("Número ingresado: "));
    write(1, number_str, strlen(number_str));
    write(1, "\n", sizeof("\n"));

    // Convertir número de cadena a entero
    int number;
    sscanf(number_str, "%d", &number);

    // Imprimir la tabla de multiplicar
    write(1, "Tabla de multiplicar del número ingresado:  \n\n", 46);

    char result_str[50];
    for (int i = 1; i <= 10; i++) {
        snprintf(result_str, sizeof(result_str), "%d x %d = %d\n", number, i, number * i);
        write(1, result_str, strlen(result_str));
    }

    return 0;
}
