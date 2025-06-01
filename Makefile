# 1. Ferramentas e flags comuns 
CC          := gcc
WARN_FLAGS  := -Wall -Wextra -pedantic
OPT_FLAGS   := -O1
DBG_FLAGS   := -g -fno-omit-frame-pointer
SAN_FLAGS   := -fsanitize=address       
COV_FLAGS   := --coverage                  # Ativa a cobertura gcov

PKGCONF     := pkg-config
CUNIT_CFLAGS:= $(shell $(PKGCONF) --cflags cunit 2>/dev/null)
CUNIT_LIBS  := $(shell $(PKGCONF) --libs cunit 2>/dev/null)

CFLAGS_BASE := $(WARN_FLAGS) $(OPT_FLAGS) $(DBG_FLAGS) $(SAN_FLAGS)
CFLAGS      := $(CFLAGS_BASE) $(CUNIT_CFLAGS) $(COV_FLAGS) -I./include

LIBS        := $(CUNIT_LIBS) -lgcov -lcunit

# Diretórios
SRC_DIR     := src
TEST_DIR    := tests

# Ficheiros fonte e objetos
SRC_C       := $(wildcard $(SRC_DIR)/*.c)
OBJ         := $(SRC_C:.c=.o)

TEST_C      := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ    := $(TEST_C:.c=.o)
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c,%,$(TEST_C))  

EXEC        := jogo

.PHONY: all clean run testar

# alvo padrão compila o jogo
all: $(EXEC)

# regra para compilar .o a partir de .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# build do executável principal
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# regra para cada teste (ex: test-ficheiros)
$(TEST_BINS): %: $(TEST_DIR)/%.o $(filter-out $(SRC_DIR)/main.o,$(OBJ))
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Executa todos os testes e gera relatório de cobertura gcov
testar: $(TEST_BINS)
	@echo "--------------  EXECUTANDO TESTES  --------------"
	@rm -f *.gcda *.gcno *.gcov
	@for t in $(TEST_BINS); do echo ">>> Executando $$t"; ./$$t; done
	@echo "--------------  RELATÓRIO GCOV  --------------"
	@gcov -b -c $(SRC_DIR)/*.c $(TEST_DIR)/*.c | grep -E "Lines executed|File"
	@echo "-----------------------------------------------"

# executa o jogo
run: $(EXEC)
	./$(EXEC)

# limpa objetos, executáveis, e arquivos de cobertura
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_BINS) *.gcda *.gcno *.gcov
