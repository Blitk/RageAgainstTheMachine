#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include "auth.h"

// Função auxiliar para definir socket como não-bloqueante (evita travamentos)
int set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

// Worker executado por cada thread de forma assíncrona
void *thread_auth_worker(void *arg) {
    auth_target_t *target = (auth_target_t *)arg;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        free(target);
        pthread_exit(NULL);
    }

    set_nonblocking(sock);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(target->port);
    server_addr.sin_addr.s_addr = inet_addr(target->target_ip);

    // Tentativa de conexão rápida com timeout simulado
    int res = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res < 0 && errno == EINPROGRESS) {
        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = 2; // Timeout rigoroso de 2 segundos para manter a velocidade
        tv.tv_usec = 0;

        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
            int valopt;
            socklen_t lon = sizeof(valopt);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
            if (valopt == 0) {
                printf("[+] [AUTH] Conexao bem-sucedida com %s:%d | Testando: %s:%s\n", 
                       target->target_ip, target->port, target->username, target->password);
                
                // Aqui entraria a lógica específica do protocolo (ex: handshake SMB/Negotiate)
            }
        }
    }

    close(sock);
    free(target);
    pthread_exit(NULL);
}

// Lançador principal do motor adaptativo
int adaptive_brute_launch(const char *target_ip, int port, const char *username, const char *password, int concurrency) {
    pthread_t thread;
    auth_target_t *target = malloc(sizeof(auth_target_t));
    if (!target) return -1;

    strncpy(target->target_ip, target_ip, sizeof(target->target_ip) - 1);
    target->port = port;
    strncpy(target->username, username, sizeof(target->username) - 1);
    strncpy(target->password, password, sizeof(target->password) - 1);

    // Dispara a thread de forma isolada
    if (pthread_create(&thread, NULL, thread_auth_worker, (void *)target) != 0) {
        perror("[-] Erro ao criar thread de autenticação");
        free(target);
        return -1;
    }

    // Desacopla a thread para liberar recursos automaticamente ao terminar
    pthread_detach(thread);
    
    // Pequeno Jitter (pausa estocástica) para controle de taxa
    usleep((rand() % 50000) + 10000); // Pausa entre 10ms e 60ms

    return 0;
}
