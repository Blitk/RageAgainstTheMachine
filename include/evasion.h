#ifndef EVASION_H
#define EVASION_H

#include <stdint.h>

// Estrutura para representar uma API resolvida via Hash
typedef struct {
    uint32_t hash;
    void *address;
} resolved_api_t;

// Protótipos das funções do Hook & Defense Auditor
uint32_t calculate_hash(const char *str);
void *resolve_api_by_hash(const char *module_name, uint32_t target_hash);
int audit_user_hooks(void *function_ptr);
int execute_direct_syscall(void);

#endif // EVASION_H
