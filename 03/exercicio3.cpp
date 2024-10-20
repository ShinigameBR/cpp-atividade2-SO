#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

// Variável global que define o tamanho da sequência de Fibonacci
int n = 10;

// Função que realiza a soma de dois números
void sum(int *sharedMemory, int valor) {
    *sharedMemory += valor;

}

int main() {
    /// Criação de um segmento de memória compartilhada
    int segment_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);

    // Criando o ponteiro que vai apontar para o segmento de memória compartilhada
    int *sharedMemory = (int *)shmat(segment_id, NULL, 0);

    // Inicializa a memória compartilhada com 0 (primeiro natural)
    *sharedMemory = 0;

    // Criar n processos para realizar as somas
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork(); // Cria um processo filho para cada iteração

        if (pid < 0) {
            perror("Erro ao criar processo filho");
            exit(1);
        } else if (pid == 0) { 
            // Processo filho realiza a soma e desanexa a memória
            sum(sharedMemory, i);
            shmdt(sharedMemory);
            exit(0);
        } else {
            // Processo pai espera o filho terminar
            wait(NULL);
        }
    }

    // Processo pai imprime a soma da memória compartilhada
    printf("A soma dos %d primeiros números naturais é: %d\n", n, *sharedMemory);

    // Removendo o segmento de memória compartilhada
    shmdt(sharedMemory);
    shmctl(segment_id, IPC_RMID, NULL);

    return 0;
}