#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // Criando o segmento de memoria compartilhada com o tamanho de 10 chars
    int segment_id = shmget(IPC_PRIVATE, 10 * sizeof(char), S_IRUSR | S_IWUSR);
    // Criando o ponteiro que vai apontar para o endereço de memória compartilhada
    char *sharedMemory;
    pid_t pid = fork(); // Funcao usada para criar um novo processo
    if (pid < 0)
    { // Erro na criacao do processo
        printf("Sou o processo %d e nao consegui criar um novo processo.\n", getpid());
        return 1;
    }
    else if (pid == 0)
    { // Processo filho
        // Anexando o segmento de memoria compartilhada
        sharedMemory = (char *)shmat(segment_id, NULL, 0);
        // Escrevendo na memória compartilhada
        sprintf(sharedMemory, "Olá, processo pai, sou o seu filho e o meu ID é %d.", getpid());
        // Desanexando a memória compartilhada
        shmdt(sharedMemory);
        // Encerrando o processo filho
        exit(0);
    }
    else
    { // Processo pai
        // Fazendo o pai esperar que o filho termine de escrever
        wait(NULL);
        // Salvando o ponteiro que aponta para a memória compartilhada
        sharedMemory = (char *)shmat(segment_id, NULL, 0);
        // Imprimindo mensagem deixada pelo processo filho
        printf("%s\n", sharedMemory);
        // Removendo o segmento de memória compartilhada
        shmctl(segment_id, IPC_RMID, NULL);
    }
    return 0;
}