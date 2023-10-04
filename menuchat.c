#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    int fd_parent_to_child[2]; // Tubería del padre al hijo
    int fd_child_to_parent[2]; // Tubería del hijo al padre

    // Crear las tuberías
    if (pipe(fd_parent_to_child) == -1 || pipe(fd_child_to_parent) == -1) {
        perror("Error al crear las tuberías");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork(); // Crear un proceso hijo

    if (child_pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Proceso hijo

        close(fd_parent_to_child[1]); // Cerrar el descriptor de escritura del padre al hijo
        close(fd_child_to_parent[0]); // Cerrar el descriptor de lectura del hijo al padre

        while (1) {
            // Leer la opción del padre
            char opcion;
            read(fd_parent_to_child[0], &opcion, sizeof(opcion));

            switch (opcion) {
                case '1':
                    // Realizar cálculo del factorial y enviar el resultado al padre
                    write(1, "\nProceso hijo:\n", sizeof("\nProceso hijo:\n"));
                    int numero;
                    read(fd_parent_to_child[0], &numero, sizeof(numero));
                    int resultado_factorial = 1;
                    for (int i = 1; i <= numero; i++) {
                        resultado_factorial *= i;
                    }
                    write(fd_child_to_parent[1], &resultado_factorial, sizeof(resultado_factorial));
                    break;

                case '2':
                    // Realizar cálculo de la serie Fibonacci y enviar el resultado al padre
                    write(1, "\nProceso hijo:\n", sizeof("\nProceso hijo:\n"));
                    int n;
                    read(fd_parent_to_child[0], &n, sizeof(n));

                    int primero = 0, segundo = 1, resultado_fibonacci;

                    printf("Serie Fibonacci de %d términos:\n", n);

                    for (int i = 0; i <= n; i++) {
                        if (i <= 1) {
                            resultado_fibonacci = i;
                        } else {
                            resultado_fibonacci = primero + segundo;
                            primero = segundo;
                            segundo = resultado_fibonacci;
                        }

                        write(fd_child_to_parent[1], &resultado_fibonacci, sizeof(resultado_fibonacci));
                    }
                    break;

                case '3':
                    // Realizar cálculo de la tabla de multiplicar y enviar el resultado al padre
                    write(1, "\nProceso hijo:\n", sizeof("\nProceso hijo:\n"));
                    int multiplicando;
                    read(fd_parent_to_child[0], &multiplicando, sizeof(multiplicando));
                    int tabla_de_multiplicar[11];
                    for (int i = 1; i <= 10; i++) {
                        tabla_de_multiplicar[i] = multiplicando * i;
                    }
                    write(fd_child_to_parent[1], tabla_de_multiplicar, sizeof(tabla_de_multiplicar));
                    break;

                case '4':
                    // Realizar cálculo del área del trapecio y enviar el resultado al padre
                    write(1, "\nProceso hijo:\n", sizeof("\nProceso hijo:\n"));
                    double base_mayor, base_menor, altura;
                    read(fd_parent_to_child[0], &base_mayor, sizeof(base_mayor));
                    read(fd_parent_to_child[0], &base_menor, sizeof(base_menor));
                    read(fd_parent_to_child[0], &altura, sizeof(altura));
                    double area_trapecio = 0.5 * (base_mayor + base_menor) * altura;
                    write(fd_child_to_parent[1], &area_trapecio, sizeof(area_trapecio));
                    break;

                case '5':
                    // Salir del proceso hijo si se selecciona la opción 5
                    return 0;

                default:
                    printf("Opción no válida en el proceso hijo.\n");
            }
        }

        close(fd_parent_to_child[0]); // Cerrar el descriptor de lectura del padre al hijo
        close(fd_child_to_parent[1]); // Cerrar el descriptor de escritura del hijo al padre
        exit(EXIT_SUCCESS);

    } else {
        // Proceso padre
        close(fd_parent_to_child[0]); // Cerrar el descriptor de lectura del padre al hijo
        close(fd_child_to_parent[1]); // Cerrar el descriptor de escritura del hijo al padre

        char opcion;
        int numero, n, multiplicando;
        double base_mayor, base_menor, altura;
        int resultado_factorial = -1; // Inicializar con un valor distinto de 0
        int resultado_fibonacci = -1; // Inicializar con un valor distinto de 0
        int tabla_de_multiplicar[11];
        double resultado_area = -1.0; // Inicializar con un valor distinto de 0.0

        do {
            // Menú principal
            write(1, "\n Proceso padre, Seleccione una opción:\n", sizeof("\n Proceso padre, Seleccione una opción:\n"));
            write(1, "1. Factorial de un numero\n", sizeof("1. Factorial de un numeron"));
            write(1, "2. Fibonacci\n", sizeof("2. Fibonacci\n"));
            write(1, "3. Tabla de multiplicar\n", sizeof("3. Tabla de multiplicar\n"));
            write(1, "4. Área de un trapecio\n", sizeof("4. Área de un trapecio\n"));
            write(1, "5. Salir\n", sizeof("5. Salir\n"));
            read(0, &opcion, sizeof(opcion));

            if (opcion < '1' || opcion > '5') {
                printf("Opción no válida. Por favor, seleccione una opción válida.\n");
            } else {
                write(fd_parent_to_child[1], &opcion, sizeof(opcion)); // Enviar la opción al hijo

                switch (opcion) {
                    case '1': // Factorial de un número
                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese un número para calcular su factorial: \n", sizeof("Ingrese un número para calcular su factorial: \n"));
                        scanf("%d", &numero);
                        write(fd_parent_to_child[1], &numero, sizeof(numero)); // Enviar el número al hijo
                        read(fd_child_to_parent[0], &resultado_factorial, sizeof(resultado_factorial)); // Recibir el resultado del hijo
                        printf("El factorial de %d es: %d\n", numero, resultado_factorial);
                        break;

                    case '2': // Serie Fibonacci
                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese el valor de n para Fibonacci: \n", sizeof("Ingrese el valor de n para Fibonacci: \n"));
                        scanf("%d", &n);
                        write(fd_parent_to_child[1], &n, sizeof(n)); // Enviar el valor de n al hijo
                        read(fd_child_to_parent[0], &resultado_fibonacci, sizeof(resultado_fibonacci)); // Recibir el resultado del hijo
                        printf("El término %d de Fibonacci es: %d\n", n, resultado_fibonacci);

                        int termino_fibonacci;

                        for (int i = 1; i <= n; i++) {
                            read(fd_child_to_parent[0], &termino_fibonacci, sizeof(termino_fibonacci));
                            printf("%d ", termino_fibonacci);
                        }

                        printf("\n");
                        break;

                    case '3': // Tabla de multiplicar
                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese un número para generar su tabla de multiplicar: \n", sizeof("Ingrese un número para generar su tabla de multiplicar: \n"));
                        scanf("%d", &multiplicando);
                        write(fd_parent_to_child[1], &multiplicando, sizeof(multiplicando)); // Enviar el número al hijo
                        read(fd_child_to_parent[0], tabla_de_multiplicar, sizeof(tabla_de_multiplicar)); // Recibir la tabla de multiplicar del hijo
                        printf("Tabla de multiplicar del %d:\n", multiplicando);
                        for (int i = 0; i <= 10; i++) {
                            printf("%d x %d = %d\n", multiplicando, i, tabla_de_multiplicar[i]);
                        }
                        break;

                    case '4': // Área de un trapecio
                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese la longitud de la base mayor: \n", sizeof("Ingrese la longitud de la base mayor: \n"));
                        scanf("%lf", &base_mayor);

                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese la longitud de la base menor: \n", sizeof("Ingrese la longitud de la base menor: \n"));
                        scanf("%lf", &base_menor);

                        write(1, "\nProceso Padre:\n", sizeof("\nProceso Padre:\n"));
                        write(1, "Ingrese la altura: \n", sizeof("Ingrese la altura: \n"));
                        scanf("%lf", &altura);

                        write(fd_parent_to_child[1], &base_mayor, sizeof(base_mayor)); // Enviar la longitud de la base mayor al hijo
                        write(fd_parent_to_child[1], &base_menor, sizeof(base_menor)); // Enviar la longitud de la base menor al hijo
                        write(fd_parent_to_child[1], &altura, sizeof(altura)); // Enviar la altura al hijo
                        read(fd_child_to_parent[0], &resultado_area, sizeof(resultado_area)); // Recibir el resultado del hijo
                        printf("El área del trapecio es: %.2lf\n", resultado_area);
                        break;

                    case '5':
                        // Salir del programa si se selecciona la opción 5
                        return 0;

                    default:
                        printf("Opción no válida en el proceso padre.\n");
                }
            }

        } while (opcion != '5');

        close(fd_parent_to_child[1]); // Cerrar el descriptor de escritura del padre al hijo
        close(fd_child_to_parent[0]); // Cerrar el descriptor de lectura del hijo al padre
    }

    return 0;
}
