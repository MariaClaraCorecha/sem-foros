#include <pthread.h>    // Biblioteca para criar e gerenciar threads
#include <semaphore.h>  // Biblioteca para usar sem�foros (controle de sincroniza��o)
#include <stdio.h>      // Biblioteca padr�o para entrada e sa�da (printf)
#include <unistd.h>     // Biblioteca para fun��es como sleep (espera)


// Declara dois sem�foros que ser�o usados para sincronizar os sem�foros de tr�nsito
sem_t semaforo1;  // Sem�foro para controlar o primeiro sinal de tr�nsito
sem_t semaforo2;  // Sem�foro para controlar o segundo sinal de tr�nsito

// Fun��o que ser� executada pelas threads, respons�vel por simular o comportamento de um sem�foro
void* semaforo_func(void* arg) {
    int semaforo_num = *((int*)arg);  // Converte o argumento recebido para um n�mero inteiro, que ser� o n�mero do sem�foro

    // Loop infinito para manter o ciclo dos sem�foros funcionando continuamente
    while (1) {
        // Se o sem�foro for o n�mero 1, controla o funcionamento do primeiro sem�foro
        if (semaforo_num == 1) {
            sem_wait(&semaforo1);  // Aguarda at� o sem�foro 1 estar "verde" (liberado)

            // Exibe que o Sem�foro 1 est� verde e o Sem�foro 2 est� vermelho
            printf("\rSemaforo 1: VERDE     | Semaforo 2: VERMELHO");
            fflush(stdout);  // Garante que o texto seja imediatamente exibido no terminal
            sleep(3);  // Simula o tempo em que o sem�foro 1 permanece verde (3 segundos)

            // Agora o Sem�foro 1 est� amarelo e o Sem�foro 2 continua vermelho
            printf("\rSemaforo 1: AMARELO   | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a sa�da imediatamente
            sleep(1);  // Simula o tempo do amarelo (1 segundo)

            // O Sem�foro 1 fica vermelho, e o Sem�foro 2 ainda est� vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a sa�da para exibir os dois sem�foros vermelhos
            sem_post(&semaforo2);  // Libera o sem�foro 2, permitindo que ele mude para verde
            sleep(3);  // Sem�foro 1 fica vermelho por mais 3 segundos
        } else {
            // Caso contr�rio, controla o funcionamento do Sem�foro 2
            sem_wait(&semaforo2);  // Aguarda at� o sem�foro 2 estar "verde" (liberado)

            // Exibe que o Sem�foro 2 est� verde e o Sem�foro 1 est� vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERDE    ");
            fflush(stdout);  // Atualiza a sa�da para refletir o estado atual
            sleep(3);  // Sem�foro 2 fica verde por 3 segundos

            // Agora o Sem�foro 2 est� amarelo e o Sem�foro 1 continua vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: AMARELO ");
            fflush(stdout);  // Atualiza a sa�da
            sleep(1);  // Sem�foro 2 fica amarelo por 1 segundo

            // Ambos os sem�foros est�o vermelhos novamente
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a sa�da com ambos vermelhos
            sem_post(&semaforo1);  // Libera o sem�foro 1, permitindo que ele mude para verde
            sleep(3);  // Sem�foro 2 permanece vermelho por mais 3 segundos
        }
    }

    return NULL;  // A fun��o retorna NULL, mas esse retorno nunca ser� alcan�ado por causa do loop infinito
}

int main() {
    pthread_t thread1, thread2;  // Declara duas threads para controlar cada sem�foro
    int semaforo_num1 = 1;  // N�mero da primeira thread (sem�foro 1)
    int semaforo_num2 = 2;  // N�mero da segunda thread (sem�foro 2)

    // Inicializa os sem�foros
    // O sem�foro 1 come�a com valor 1, ou seja, ele inicia "verde"
    sem_init(&semaforo1, 0, 1);  // Inicializa sem�foro 1 com valor inicial de 1 (liberado)
    // O sem�foro 2 come�a com valor 0, ou seja, ele inicia "vermelho"
    sem_init(&semaforo2, 0, 0);  // Inicializa sem�foro 2 com valor inicial de 0 (bloqueado)

    // Cria as duas threads que ir�o executar a fun��o dos sem�foros
    pthread_create(&thread1, NULL, semaforo_func, &semaforo_num1);  // Thread para o sem�foro 1
    pthread_create(&thread2, NULL, semaforo_func, &semaforo_num2);  // Thread para o sem�foro 2

    // Aguarda a conclus�o das threads (o que n�o acontece aqui, pois o loop � infinito)
    pthread_join(thread1, NULL);  // Aguarda a thread 1 terminar (nunca termina)
    pthread_join(thread2, NULL);  // Aguarda a thread 2 terminar (nunca termina)

    // Destroi os sem�foros ao final da execu��o (nunca ser� alcan�ado devido ao loop infinito)
    sem_destroy(&semaforo1);  // Libera os recursos alocados pelo sem�foro 1
    sem_destroy(&semaforo2);  // Libera os recursos alocados pelo sem�foro 2

    return 0;  // Retorna 0 para indicar que o programa terminou sem erros
}


