#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // Criando o segmento de memoria compartilhada
    int segment_id = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);
    // Criando o ponteiro que vai apontar para o endereço de memória compartilhada
    int *sharedMemory;

    // Inicializando a memória compartilhada com o valor 0
    sharedMemory = (int *)shmat(segment_id, NULL, 0);
    *sharedMemory = 0;
    shmdt(sharedMemory);

    pid_t pid1 = fork(); // Funcao usada para criar um novo processo
    if (pid1 < 0)
    {
        printf("Erro ao criar o primeiro processo filho.\n");
        return 1;
    }
    else if (pid1 == 0)
    {
        // Processo filho 1
        sharedMemory = (int *)shmat(segment_id, NULL, 0);
        *sharedMemory += 10;
        printf("Processo filho 1 (PID: %d) adicionou 10. Valor atual: %d\n", getpid(), *sharedMemory);
        shmdt(sharedMemory);
        exit(0);
    }

    pid_t pid2 = fork(); // Funcao usada para criar um novo processo
    if (pid2 < 0)
    {
        printf("Erro ao criar o segundo processo filho.\n");
        return 1;
    }
    else if (pid2 == 0)
    {
        // Processo filho 2
        sharedMemory = (int *)shmat(segment_id, NULL, 0);
        *sharedMemory += 20;
        printf("Processo filho 2 (PID: %d) adicionou 20. Valor atual: %d\n", getpid(), *sharedMemory);
        shmdt(sharedMemory);
        exit(0);
    }
    else
    { // Processo pai
        // Fazendo o pai esperar que os dois filhos terminarem
        wait(NULL);
        wait(NULL);

        // Processo pai anexa a memória compartilhada e imprime o valor final
        sharedMemory = (int *)shmat(segment_id, NULL, 0);
        printf("Valor final na memória compartilhada: %d\n", *sharedMemory);

        // Removendo o segmento de memória compartilhada
        shmdt(sharedMemory);
        shmctl(segment_id, IPC_RMID, NULL);
    }

    return 0;
}