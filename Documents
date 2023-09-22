#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char option;
    char input_str[20];
    char num1_str[20];
    char num2_str[20];
    int num1, num2, result;

    int exit_option = 5; // Número de opción para salir del menú
    int sub_exit_option = 5; // Número de opción para salir del submenú de operaciones aritméticas

    do {
        // Mostrar el menú principal
        write(1, "Seleccione una opción:\n", sizeof("Seleccione una opción:\n"));
        write(1, "1. Tabla de multiplicar\n", sizeof("1. Tabla de multiplicar\n"));
        write(1, "2. Operaciones aritméticas\n", sizeof("2. Operaciones aritméticas\n"));
        write(1, "3. Serie Fibonacci\n", sizeof("3. Serie Fibonacci\n"));
        write(1, "4. Área de un trapecio\n", sizeof("4. Área de un trapecio\n"));
        write(1, "5. Salir\n", sizeof("5. Salir\n"));

        // Leer la opción del usuario
        ssize_t option_bytes_read = read(0, &option, sizeof(option));
        if (option_bytes_read < 0) {
            perror("Error al leer la opción");
            return 1;
        }

        // Limpiar el búfer de entrada
        char c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Convertir la opción a número
        int selected_option = option - '0';

        switch (selected_option) {
            case 1:
                // Tabla de multiplicar
                write(1, "Ingrese un número: ", sizeof("Ingrese un número: "));
                ssize_t num_bytes_read1 = read(0, input_str, sizeof(input_str) - 1);
                input_str[num_bytes_read1 - 1] = '\0';
                sscanf(input_str, "%d", &num1);

                write(1, "Tabla de multiplicar del número ingresado:\n", sizeof("Tabla de multiplicar del número ingresado:\n"));
                char result_str1[50];
                for (int i = 1; i <= 10; i++) {
                    snprintf(result_str1, sizeof(result_str1), "%d x %d = %d\n", num1, i, num1 * i);
                    write(1, result_str1, strlen(result_str1));
                }
                break;

            case 2:
                do {
                    // Submenú de operaciones aritméticas
                    write(1, "Seleccione una operación:\n", sizeof("Seleccione una operación:\n"));
                    write(1, "1. Suma\n", sizeof("1. Suma\n"));
                    write(1, "2. Resta\n", sizeof("2. Resta\n"));
                    write(1, "3. Multiplicación\n", sizeof("3. Multiplicación\n"));
                    write(1, "4. División\n", sizeof("4. División\n"));
                    write(1, "5. Volver al menú principal\n", sizeof("5. Volver al menú principal\n"));

                    // Leer la opción del usuario
                    ssize_t sub_option_bytes_read = read(0, &option, sizeof(option));
                    if (sub_option_bytes_read < 0) {
                        perror("Error al leer la opción");
                        return 1;
                    }

                    // Limpiar el búfer de entrada
                    while ((c = getchar()) != '\n' && c != EOF);

                    int sub_selected_option = option - '0';

                    // Leer números para la operación
                    write(1, "Ingrese el primer número: ", sizeof("Ingrese el primer número: "));
                    ssize_t num1_bytes_read2 = read(0, num1_str, sizeof(num1_str) - 1);
                    num1_str[num1_bytes_read2 - 1] = '\0';
                    sscanf(num1_str, "%d", &num1);

                    write(1, "Ingrese el segundo número: ", sizeof("Ingrese el segundo número: "));
                    ssize_t num2_bytes_read2 = read(0, num2_str, sizeof(num2_str) - 1);
                    num2_str[num2_bytes_read2 - 1] = '\0';
                    sscanf(num2_str, "%d", &num2);

                    switch (sub_selected_option) {
                        case 1:
                            result = num1 + num2;
                            write(1, "Resultado de la suma: ", sizeof("Resultado de la suma: "));
                            break;
                        case 2:
                            result = num1 - num2;
                            write(1, "Resultado de la resta: ", sizeof("Resultado de la resta: "));
                            break;
                        case 3:
                            result = num1 * num2;
                            write(1, "Resultado de la multiplicación: ", sizeof("Resultado de la multiplicación: "));
                            break;
                        case 4:
                            if (num2 == 0) {
                                write(1, "Error: división por cero\n", sizeof("Error: división por cero\n"));
                                continue; // Volver al submenú de operaciones
                            }
                            result = num1 / num2;
                            write(1, "Resultado de la división: ", sizeof("Resultado de la división: "));
                            break;
                        default:
                            write(1, "Opción de operación no válida\n", sizeof("Opción de operación no válida\n"));
                            continue; // Volver al submenú de operaciones
                    }

                    char sub_result_str[20];
                    snprintf(sub_result_str, sizeof(sub_result_str), "%d\n", result);
                    write(1, sub_result_str, strlen(sub_result_str));
                } while (sub_exit_option != 5);
                break;

            case 3:
                // Serie Fibonacci
                write(1, "Ingrese la cantidad de números de la serie Fibonacci a mostrar: ", sizeof("Ingrese la cantidad de números de la serie Fibonacci a mostrar: "));
                ssize_t num_bytes_read3 = read(0, input_str, sizeof(input_str) - 1);
                input_str[num_bytes_read3 - 1] = '\0';
                sscanf(input_str, "%d", &num1);

                int a = 0, b = 1, c;
                write(1, "Serie Fibonacci:\n", sizeof("Serie Fibonacci:\n"));
                char result_str2[50];
                for (int i = 1; i <= num1; i++) {
                    snprintf(result_str2, sizeof(result_str2), "%d ", a);
                    write(1, result_str2, strlen(result_str2));
                    c = a + b;
                    a = b;
                    b = c;
                }
                write(1, "\n", sizeof("\n"));
                break;

            case 4:
                // Área de un trapecio
                write(1, "Ingrese la base mayor del trapecio: ", sizeof("Ingrese la base mayor del trapecio: "));
                ssize_t num1_bytes_read4 = read(0, num1_str, sizeof(num1_str) - 1);
                num1_str[num1_bytes_read4 - 1] = '\0';
                sscanf(num1_str, "%d", &num1);

                write(1, "Ingrese la base menor del trapecio: ", sizeof("Ingrese la base menor del trapecio: "));
                ssize_t num2_bytes_read4 = read(0, num2_str, sizeof(num2_str) - 1);
                num2_str[num2_bytes_read4 - 1] = '\0';
                sscanf(num2_str, "%d", &num2);

                write(1, "Ingrese la altura del trapecio: ", sizeof("Ingrese la altura del trapecio: "));
                ssize_t input_bytes_read4 = read(0, input_str, sizeof(input_str) - 1);
                input_str[input_bytes_read4 - 1] = '\0';
                sscanf(input_str, "%d", &result);

                if (num2 == 0) {
                    write(1, "Error: división por cero\n", sizeof("Error: división por cero\n"));
                    continue; // Volver al menú principal
                }

                double area = (num1 + num2) * result / 2.0;
                write(1, "Área del trapecio: ", sizeof("Área del trapecio: "));
                char result_str3[50];
                snprintf(result_str3, sizeof(result_str3), "%.2lf\n", area);
                write(1, result_str3, strlen(result_str3));
                break;

            case 5:
                break; // Salir del programa

            default:
                write(1, "Opción no válida\n", sizeof("Opción no válida\n"));
        }
    } while (option - '0' != exit_option);

    return 0;
}
