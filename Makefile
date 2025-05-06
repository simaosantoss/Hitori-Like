#  1. Ferramentas e flags comuns 
CC          := gcc
WARN_FLAGS  := -Wall -Wextra -pedantic
OPT_FLAGS   := -O1
DBG_FLAGS   := -g -fno-omit-frame-pointer
SAN_FLAGS   := -fsanitize=address       
CFLAGS_BASE := $(WARN_FLAGS) $(OPT_FLAGS) $(DBG_FLAGS) $(SAN_FLAGS)

#  Flags de cobertura (gcov) 
COV_FLAGS   := --coverage                  # Ativa a cobertura de código
CFLAGS      := $(CFLAGS_BASE) $(CUNIT_CFLAGS) $(COV_FLAGS) -I./src

LIBS        := $(CUNIT_LIBS) -lcunit -lgcov  # Inclui gcov para a cobertura

#  2. Configuração da biblioteca CUnit 
PKGCONF     := pkg-config
CUNIT_CFLAGS:= $(shell $(PKGCONF) --cflags cunit 2>/dev/null)
CUNIT_LIBS  := $(shell $(PKGCONF) --libs cunit 2>/dev/null)

#  3. Directórios e listas automáticas 
SRC_DIR     := src
TEST_DIR    := tests

SRC_C       := $(wildcard $(SRC_DIR)/*.c)
OBJ         := $(SRC_C:.c=.o)

TEST_C      := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ    := $(TEST_C:.c=.o)
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c,%,$(TEST_C))  

EXEC        := jogo

#  4. Regras genéricas 
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
	@rm -f *.gcda *.gcno *.gcov  # Limpar arquivos de cobertura antigos
	@for t in $(TEST_BINS); do ./$$t; done
	@echo "-----------  GCOV  --------------"
	@gcov -b -c $(SRC_DIR)/*.c $(TEST_DIR)/*.c | grep -E "Lines executed|File"
	@echo "--------------------------------------"

# utilidades
run: $(EXEC)
	./$(EXEC)

# limpa os ficheiros gerados
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(EXEC) $(TEST_BINS) *.gcda *.gcno *.gcov

