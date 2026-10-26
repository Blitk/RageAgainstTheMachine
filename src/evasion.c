#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "evasion.h"

// Algoritmo de Hashing leve para ocultar nomes de APIs (Evita detecção estática por strings)
uint32_t calculate_hash(const char *str) {
    uint32_t hash = 0;
    while (*str) {
        hash = (hash >> 13) | (hash << 19); // Rotação de bits (ROR13)
        hash += (unsigned char)(*str);
        str++;
    }
    return hash;
}

// Auditoria preliminar de Hooks (Exemplo conceitual para verificar se o primeiro byte é um JMP)
int audit_user_hooks(void *function_ptr) {
    if (!function_ptr) return -1;

    unsigned char *byte_ptr = (unsigned char *)function_ptr;

    // Em muitas implementações de EDR (como CrowdStrike/Defender), o início de uma função 
    // monitorada é substituído por uma instrução JMP (0xE9) ou MOV R10, RCX / MOV RAX, syscall.
    if (*byte_ptr == 0xE9 || *byte_ptr == 0xFF) {
        printf("[!] ALERTA: Hook detectado no ponteiro de função %p (Instrução de desvio encontrada)\n", function_ptr);
        return 1; // Hooked
    }

    printf("[+] Função limpa de hooks em user-mode: %p\n", function_ptr);
    return 0; // Clean
}

// Exemplo conceitual de preparação para Direct Syscall
int execute_direct_syscall(void) {
    // Em ambientes Windows/Linux, aqui preparamos os registradores 
    // e executamos a instrução 'syscall' ou 'sysenter' de forma crua em assembly inline (__asm__)
    // para ignorar completamente as DLLs hookadas do sistema.
    
    printf("[*] Preparando estrutura para Direct Syscall...\n");
    
    #if defined(__x86_64__) && defined(__linux__)
        // Exemplo de syscall nativa em assembly inline para Linux (ex: sys_getpid)
        long result;
        __asm__ __volatile__ (
            "movq $39, %%rax\n\t" // Número da syscall getpid no Linux x64
            "syscall\n\t"
            "movq %%rax, %0\n\t"
            : "=r" (result)
            :
            : "%rax", "rcx", "r11", "memory"
        );
        printf("[+] Syscall executada com sucesso. PID obtido via Kernel direto: %ld\n", result);
        return 0;
    #else
        printf("[*] Arquitetura ou sistema não configurado para o exemplo de assembly inline direto.\n");
        return -1;
    #endif
}
