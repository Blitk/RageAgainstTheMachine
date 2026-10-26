#ifndef TUNNEL_H
#define TUNNEL_H

#include <stddef.h>

// Protótipos das funções do Protocol Camouflage & Tunneling
int encode_dns_tunnel(const char *data, char *output_buffer, size_t max_len);
int send_camouflaged_http_heartbeat(const char *target_ip, int port, const char *payload);

#endif // TUNNEL_H
