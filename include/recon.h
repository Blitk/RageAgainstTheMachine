#ifndef RECON_H
#define RECON_H

#include <stddef.h>

// Estrutura para o pseudo-cabeçalho TCP (necessário para calcular o Checksum corretamente)
struct pseudo_header {
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
};

// Protótipos das funções do Ghost Discovery
unsigned short csum(unsigned short *ptr, int nbytes);
int get_local_ip(char *ip_buffer, size_t max_len);
int send_ghost_syn(const char *src_ip, const char *dst_ip, int dst_port);
void start_ghost_listener(int target_port);

#endif // RECON_H
