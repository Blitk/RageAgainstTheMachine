#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

struct pseudo_header {
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
};

// Função para calcular o Checksum TCP/IP
unsigned short csum(unsigned short *ptr, int nbytes) {
    register long sum = 0;
    unsigned short oddbyte;
    unsigned short answer;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *) &oddbyte) = *(unsigned char *) ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (unsigned short) ~sum;
    return answer;
}

// 1. Descoberta Automática da Interface de Rede Primária e IP Local
int get_local_ip(char *ip_buffer, size_t max_len) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;

    if (getifaddrs(&ifaddr) == -1) {
        perror("[-] Erro em getifaddrs");
        return -1;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        family = ifa->ifa_addr->sa_family;

        // Procura por IPv4 que não seja o localhost (127.0.0.1)
        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            ip_buffer, max_len, NULL, 0, NI_NUMERICHOST);
            if (s == 0 && strcmp(ip_buffer, "127.0.0.1") != 0) {
                // Interface encontrada com sucesso
                freeifaddrs(ifaddr);
                return 0;
            }
        }
    }

    freeifaddrs(ifaddr);
    return -1;
}

// 2. Motor de Envio do Ghost SYN
int send_ghost_syn(const char *src_ip, const char *dst_ip, int dst_port) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("[-] Erro ao criar Raw Socket (Execute como root)");
        return -1;
    }

    int hincl = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl)) < 0) {
        perror("[-] Erro em setsockopt(IP_HDRINCL)");
        close(sock);
        return -1;
    }

    char datagram[4096];
    memset(datagram, 0, 4096);

    struct iphdr *iph = (struct iphdr *) datagram;
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof(struct ip));
    struct sockaddr_in dest;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(dst_port);
    dest.sin_addr.s_addr = inet_addr(dst_ip);

    // Cabeçalho IP
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    iph->id = htons(rand() % 65535); // ID Randomizado para evitar padrões
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->saddr = inet_addr(src_ip);
    iph->daddr = dest.sin_addr.s_addr;
    iph->check = csum((unsigned short *) datagram, iph->tot_len);

    // Cabeçalho TCP (SYN Stealth)
    tcph->source = htons(45123);
    tcph->dest = htons(dst_port);
    tcph->seq = htonl(rand());
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->syn = 1;
    tcph->window = htons(64240);
    tcph->check = 0;

    // Pseudo-cabeçalho para checksum TCP
    struct pseudo_header psh;
    psh.source_address = inet_addr(src_ip);
    psh.dest_address = dest.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr));

    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr);
    char *pseudogram = malloc(psize);
    memcpy(pseudogram, (char *)&psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), (char *)tcph, sizeof(struct tcphdr));
    tcph->check = csum((unsigned short *) pseudogram, psize);
    free(pseudogram);

    if (sendto(sock, datagram, iph->tot_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}

// 3. Listener Assíncrono para capturar portas abertas (SYN-ACK)
void start_ghost_listener(int target_port) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("[-] Erro ao criar socket de escuta");
        return;
    }

    printf("[*] Ghost Listener ativado na porta lógica. Aguardando respostas...\n");
    char buffer[65536];

    // Loop de escuta rápida (Timeout ou controle de thread pode ser aplicado aqui)
    for (int i = 0; i < 50; i++) { // Exemplo de escuta limitada por ciclo
        ssize_t data_size = recv(sock, buffer, sizeof(buffer), 0);
        if (data_size < 0) continue;

        struct iphdr *iph = (struct iphdr *)buffer;
        struct tcphdr *tcph = (struct tcphdr *)(buffer + (iph->ihl * 4));

        // Verifica se é uma resposta SYN-ACK (SYN=1, ACK=1) destinada à nossa porta de origem
        if (tcph->syn == 1 && tcph->ack == 1 && ntohs(tcph->dest) == 45123) {
            struct sockaddr_in source;
            memset(&source, 0, sizeof(source));
            source.sin_addr.s_addr = iph->saddr;
            printf("[+] HOST VIVO / PORTA ABERTA DETECTADA: %s:%d\n", inet_ntoa(source.sin_addr), target_port);
        }
    }
    close(sock);
}
