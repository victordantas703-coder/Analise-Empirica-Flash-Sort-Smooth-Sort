# 1. Variáveis de configuração
CXX      := g++
TARGET   := analise_empirica

# 2. Busca automática por todos os arquivos .cpp na pasta src
SRC_DIR  := src
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)

# 3. Regra principal (default)
.PHONY: all clean

all: $(TARGET)

# 4. Compilação direta de .cpp para o executável
$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET)

# 5. Limpeza do executável
clean:
	rm -f $(TARGET)
