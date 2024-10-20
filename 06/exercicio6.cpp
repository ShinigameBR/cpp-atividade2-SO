#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

// Variável global que define o tamanho da soma
int n = 10;

// Função que realiza a soma de um número
void *sum(void *arg)
{
    int *args = static_cast<int *>(arg);
    args[0] += args[1];
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[n]; // Array para armazenar as threads
    int args[] = {0, 0};

    // Criar n threads para realizar as somas
    for (int i = 1; i <= n; i++)
    {
        // Struct para passar o valor a somar e sumResult
        args[1] = i;

        // Cria a thread para somar o valor
        int status = pthread_create(&threads[i], NULL, sum, static_cast<void *>(args));
        if (status != 0)
        {
            printf("Erro ao criar a thread. Código de erro: %d\n", status);
            return 1;
        }
        pthread_join(threads[i], NULL);
    }

    // Imprime a soma dos n primeiros números naturais
    printf("A soma dos %d primeiros números naturais é: %d\n", n, args[0]);

    return 0;
}
