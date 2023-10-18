#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define clave 4000 // De 3000 al infinito

int idsem = 0;

int crear_semaforo() {
    int val = 1;
    
    if ((idsem = semget(clave, 1, IPC_CREAT | 0777)) == -1) {
        perror("No se puede crear el semáforo");
        exit(1);
    }
    
    if (semctl(idsem, 0, SETVAL, val) == -1) {
        printf("No se puede tener el control del semáforo");
        exit(1);
    }
    
    printf("Valor idsem=%d\n", idsem);
    return idsem;
}

void remover_semaforo(int idsem) {
    if (semctl(idsem, 0, IPC_RMID) == -1) {
        perror("Error al remover el semáforo");
        exit(1);
    } else {
        printf("Semaforo eliminado\n");
    }
}

void down(int idsem) {
    struct sembuf bloqueado = {0, -1, SEM_UNDO};
    if (semop(idsem, &bloqueado, 1) == -1) {
        perror("Error al tratar de bloquear");
        remover_semaforo(idsem);
        exit(1);
    } else {
        printf("Semaforo Ocupado (Rojo - en RC) ==>\n");
    }
}

void up(int idsem) {
    struct sembuf desbloqueado = {0, 1, SEM_UNDO};
    if (semop(idsem, &desbloqueado, 1) == -1) {
        perror("Error al tratar de desbloquear");
        remover_semaforo(idsem);
        exit(1);
    } else {
        printf("Semaforo Liberado (Verde - fuera de RC) ==>\n");
    }
}

int main(void) {
    printf("Iniciando cliente\n");
    idsem = crear_semaforo();
    
    if (idsem < 0) {
        perror("El semáforo no existe\n");
        exit(0);
    }
    
    down(idsem);
    printf("En región crítica\n");
    up(idsem);
    printf("Saliendo de región crítica\n");
}
