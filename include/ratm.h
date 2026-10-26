#ifndef RATM_H
#define RATM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Informações do Framework
#define RATM_VERSION "1.0.0"
#define RATM_NAME    "RageAgainstTheMachine"

// Códigos de Cores ANSI para Output do Terminal (Red Team Theme)
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_RESET   "\033[0m"

// Macros de Log Padronizadas
#define LOG_INFO(fmt, ...)  printf(COLOR_BLUE "[*] " fmt COLOR_RESET "\n", ##__VA_ARGS__)
#define LOG_SUCCESS(fmt, ... ) printf(COLOR_GREEN "[+] " fmt COLOR_RESET "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  printf(COLOR_YELLOW "[!] " fmt COLOR_RESET "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf(COLOR_RED "[-] " fmt COLOR_RESET "\n", ##__VA_ARGS__)

// Inclusão dos Módulos do Framework
#include "recon.h"
#include "auth.h"
#include "evasion.h"
#include "tunnel.h"

#endif // RATM_H
