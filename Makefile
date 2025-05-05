############################################################
#        Makefile simples e automático (com CUnit)         #
############################################################

# ——— 1. Ferramentas e flags comuns ————————————————
CC          := gcc
WARN_FLAGS  := -Wall -Wextra -pedantic
OPT_FLAGS   := -O1
DBG_FLAGS   := -g -fno-omit-frame-pointer
SAN_FLAGS   := -fsanitize=address        # comenta se não quiseres AddressSanitizer
CFLAGS_BASE := $(WARN_FLAGS) $(OPT_FLAGS) $(DBG_FLAGS) $(SAN_FLAGS)

# ——— 2. Configuração da biblioteca CUnit —————————
PKGCONF     := pkg-config
CUNIT_CFLAGS:= $(shell $(PKGCONF) --cflags cunit 2>/dev/null)
CUNIT_LIBS  := $(shell $(PKGCONF) --libs cunit 2>/dev/null)

CFLAGS      := $(CFLAGS_BASE) $(CUNIT_CFLAGS) -I./src
LIBS        := $(CUNIT_LIBS) -lcunit  # Adiciona a flag -lcunit

# ——— 3. Directórios e listas automáticas ————————————
SRC_DIR     := src
TEST_DIR    := tests

SRC_C       := $(wildcard $(SRC_DIR)/*.c)
OBJ         := $(SRC_C:.c=.o)

TEST_C      := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ    := $(TEST_C:.c=.o)
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c,%,$(TEST_C))   # nomes dos executáveis

EXEC        := jogo

# ——— 4. Regras genéricas ————————————————————————————
.PHONY: all clean run testar

all: $(EXEC)

# %.o a partir de %.c (src ou tests)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# executável principal
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# regra automática: cada testeX.o -> bin testeX
# (usa todos os objectos de src exceto main.o)
$(TEST_BINS): %: $(TEST_DIR)/%.o $(filter-out $(SRC_DIR)/main.o,$(OBJ))
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# alvo para correr todos os testes
testar: $(TEST_BINS)
	@echo "--------------  TESTES  --------------"
	@for t in $(TEST_BINS); do ./$$t; done
	@echo "--------------------------------------"

# utilidades
run: $(EXEC)
	./$(EXEC)

# limpa os ficheiros gerados
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_BINS)

############################################################
# Fim                                                      #
############################################################
