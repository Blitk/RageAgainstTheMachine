#ifndef AUTH_H
#define AUTH_H

#include <pthread.h>

// Estrutura para passar parâmetros para as threads de autenticação de forma segura
typedef struct {
    char target_ip[64];
    int port;
    char username[128];
    char password[128];
    int service_type; // Ex: 1 = SMB, 2 = SSH, 3 = WinRM
} auth_target_t;

// Protótipos das funções do Adaptive Auth Engine
void *thread_auth_worker(void *arg);
int adaptive_brute_launch(const char *target_list_file, const char *user_file, const char *pass_file, int concurrency);

#endif // AUTH_H
