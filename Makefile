# ───────────────────────────────────────────────
# 1. Ferramentas e flags comuns
CC          := gcc
WARN_FLAGS  := -Wall -Wextra -pedantic
OPT_FLAGS   := -O1
DBG_FLAGS   := -g -fno-omit-frame-pointer
SAN_FLAGS   := -fsanitize=address
CFLAGS_BASE := $(WARN_FLAGS) $(OPT_FLAGS) $(DBG_FLAGS) $(SAN_FLAGS)

# 2. Biblioteca CUnit (apenas necessária nos testes)
PKGCONF        := pkg-config
CUNIT_CFLAGS   := $(shell $(PKGCONF) --cflags cunit 2>/dev/null)
CUNIT_LIBS     := $(shell $(PKGCONF) --libs   cunit 2>/dev/null)

# Flags finais de compilação
CFLAGS      := $(CFLAGS_BASE) $(CUNIT_CFLAGS) -I./src
LIBS_TEST   := $(CUNIT_LIBS) -lcunit          # só para os binários de teste

# ───────────────────────────────────────────────
# 3. Directórios e listas automáticas
SRC_DIR   := src
TEST_DIR  := tests

SRC_C     := $(wildcard $(SRC_DIR)/*.c)
OBJ       := $(SRC_C:.c=.o)

TEST_C    := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ  := $(TEST_C:.c=.o)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,%,$(TEST_C))  # ex.: tests/test-stack.c → test-stack

EXEC := jogo

# ───────────────────────────────────────────────
# 4. Regras genéricas
.PHONY: all clean run testar

all: $(EXEC)

# Compilação de *.c → *.o (src/ ou tests/)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executável principal (não precisa de -lcunit)
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Cada testeX.o + objectos da src (excepto main.o) → binário de testeX
$(TEST_BINS): %: $(TEST_DIR)/%.o $(filter-out $(SRC_DIR)/main.o,$(OBJ))
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS_TEST)

# Corre todos os testes
testar: $(TEST_BINS)
	@echo "--------------  TESTES  --------------"
	@for t in $(TEST_BINS); do ./$$t; done
	@echo "--------------------------------------"

# Atalho para correr o jogo
run: $(EXEC)
	./$(EXEC)

# Limpeza
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_BINS)
