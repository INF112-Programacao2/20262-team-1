# ==============================================================================
# MAKEFILE GENÉRICO: SRC, INCLUDE E TESTS
# ==============================================================================

# Compilador e opções de compilação
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -MMD -MP

# Diretórios do projeto
SRC_DIR   := src
INC_DIR   := include
TEST_DIR  := tests
BUILD_DIR := build
BIN_DIR   := bin

# Inclusão de diretórios de cabeçalhos
INCLUDES := -I$(INC_DIR) -I$(SRC_DIR)

# Nome dos executáveis finais
TARGET      := $(BIN_DIR)/programa
TEST_TARGET := $(BIN_DIR)/test_runner

# Função recursiva para encontrar todos os ficheiros em subpastas
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# ------------------------------------------------------------------------------
# MAPEAMENTO AUTOMÁTICO DE FICHEIROS
# ------------------------------------------------------------------------------
# Procura todos os ficheiros .cpp na pasta src/ e subpastas
ALL_SRC    := $(call rwildcard,$(SRC_DIR)/,*.cpp)

# Separa o main.cpp dos restantes ficheiros para os testes usarem apenas a lógica das classes
SRC_MAIN   := $(SRC_DIR)/main.cpp
SRC_COMMON := $(filter-out $(SRC_MAIN),$(ALL_SRC))

# Procura todos os ficheiros de teste na pasta tests/
TEST_SRCS  := $(call rwildcard,$(TEST_DIR)/,*.cpp)

# Converte caminhos .cpp para objetos .o dentro de build/
OBJS_MAIN   := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(ALL_SRC))
OBJS_COMMON := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC_COMMON))
OBJS_TESTS  := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Carrega os ficheiros de dependência (.d)
DEPS := $(call rwildcard,$(BUILD_DIR)/,*.d)

# ------------------------------------------------------------------------------
# REGRAS DE COMPILAÇÃO
# ------------------------------------------------------------------------------
.PHONY: all run test clean

# Regra padrão: gera o executável principal
all: $(TARGET)

# Compila o programa principal
$(TARGET): $(OBJS_MAIN) | $(BIN_DIR)
	$(CXX) $(OBJS_MAIN) -o $@

# Compila o executável de testes (Classes de src/ + Ficheiros de teste)
$(TEST_TARGET): $(OBJS_COMMON) $(OBJS_TESTS) | $(BIN_DIR)
	$(CXX) $(OBJS_COMMON) $(OBJS_TESTS) -o $@

# Regra genérica para compilar qualquer .cpp para .o mantendo a estrutura de pastas
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Cria a pasta de binários
$(BIN_DIR):
	mkdir -p $@

# Inclui automaticamente dependências de cabeçalhos (.hpp)
-include $(DEPS)

# Compila e executa o programa principal
run: all
	./$(TARGET)

# Compila e executa os testes de unidade
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Limpa todas as pastas de compilação
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


# Executa apenas um teste específico pelo nome (sem a extensão .cpp)
test-%: $(TEST_DIR)/%.cpp $(OBJS_COMMON) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS_COMMON) $< -o $(BIN_DIR)/$*
	./$(BIN_DIR)/$*