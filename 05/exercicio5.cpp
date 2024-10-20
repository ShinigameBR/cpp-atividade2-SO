#include <pthread.h>
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
void *fibonacci(void *arg)
{
    int *fibSequence = (int *)arg; // Obtendo o ponteiro para o fibSequence
    int t1 = 1, t2 = 1, nextTerm;

    for (int i = 0; i < n; ++i)
    {
        fibSequence[i] = t1;
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }

    pthread_exit(NULL);
}

int main()
{
    int fibSequence[n];
    pthread_t thread;

    // Criando uma thread e passando o array fibSequence como argumento
    int status = pthread_create(&thread, NULL, fibonacci, (void *)fibSequence);

    // Se o status não for 0, houve um erro na criação da thread
    if (status != 0)
    {
        printf("Erro na criação da thread. Código de erro: %d\n", status);
        return 1;
    }

    // Faz a thread principal esperar pela thread filha
    pthread_join(thread, NULL);

    // Thread principal imprime a sequência de Fibonacci
    printf("Sequência de Fibonacci até %d termos:\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", fibSequence[i]);
    }
    printf("\n");

    return 0;
}
