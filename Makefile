# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -Iinclude
LDFLAGS = 

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Alvo Final
TARGET = $(BIN_DIR)/ratm

# Arquivos Fonte e Objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regra Principal
all: directories $(TARGET)

# Criação de diretórios necessários caso não existam
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Linkagem do binário final
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "[+] Compilação concluída com sucesso! Binário gerado em: $(TARGET)"

# Compilação dos arquivos fonte para objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos de compilação
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "[-] Limpeza realizada com sucesso."

# Regra para recompilar do zero
rebuild: clean all

.PHONY: all directories clean rebuild
