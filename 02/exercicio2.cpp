#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

// Variável global que define o tamanho da sequência de Fibonacci
int n = 10;

// Função para gerar a sequência de Fibonacci
void fibonacci(int *sharedMemory, int n)
{
    int t1 = 1, t2 = 1, nextTerm;

    for (int i = 0; i < n; ++i)
    {
        sharedMemory[i] = t1;
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
}

int main()
{
    // Criação de um segmento de memória compartilhada
    int segment_id = shmget(IPC_PRIVATE, n * sizeof(int), S_IRUSR | S_IWUSR);

    // Criando o ponteiro que vai apontar para o segmento de memória compartilhada
    int *sharedMemory = (int *)shmat(segment_id, NULL, 0);

    pid_t pid = fork(); // Funcao usada para criar um novo processo

    if (pid < 0)
    {
        perror("Erro ao criar processo filho");
        exit(1);
    }
    else if (pid == 0)
    {
        // Processo filho gera a sequência de Fibonacci e a armazena na memória compartilhada
        fibonacci(sharedMemory, n);
        shmdt(sharedMemory);
        exit(0);
    }
    else
    {
        // Processo pai espera o processo filho terminar
        wait(NULL);

        // Processo pai imprime a sequência de Fibonacci da memória compartilhada
        printf("Sequência de Fibonacci até %d termos:\n", n);
        for (int i = 0; i < n; i++)
        {
            printf("%d ", sharedMemory[i]);
        }
        printf("\n");

        // Removendo o segmento de memória compartilhada
        shmdt(sharedMemory);
        shmctl(segment_id, IPC_RMID, NULL);
    }

    return 0;
}
