## 1. Variáveis de compilação
CC          = gcc
BASEFLAGS   = -Wall -Wextra -pedantic -O1 -fsanitize=address \
              -fno-omit-frame-pointer -g
COVFLAGS    = --coverage                  # -fprofile-arcs -ftest-coverage
INCFLAGS    = -I./src -I/opt/homebrew/Cellar/cunit/2.1-3/include
CFLAGS      = $(BASEFLAGS) $(COVFLAGS) $(INCFLAGS)

LIBS        = $(shell pkg-config --libs cunit) -lgcov

## 2. Lista de objectos
SRC_OBJS    = src/tabuleiro.o src/interface.o src/ficheiros.o src/stack.o
OBJS_JOGO   = src/main.o $(SRC_OBJS)

# objectos de cada teste
OBJS_TEST_TABULEIRO = tests/test-tabuleiro.o  src/tabuleiro.o
OBJS_TEST_STACK     = tests/test-stack.o      src/stack.o
OBJS_TEST_FICHEIROS = tests/test-ficheiros.o  src/ficheiros.o
OBJS_TEST_INTERFACE = tests/test-interface.o  $(SRC_OBJS) 

## 3. Regras genéricas
.PHONY: jogo testar clean

# compilar qualquer .c → .o (src ou tests)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## 4. Executável principal
jogo: $(OBJS_JOGO)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

## 5. Testes individuais (executáveis)
teste_tabuleiro: $(OBJS_TEST_TABULEIRO)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

teste_stack: $(OBJS_TEST_STACK)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

teste_ficheiros: $(OBJS_TEST_FICHEIROS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

teste_interface: $(OBJS_TEST_INTERFACE)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

## 6. Target “testar” – compila tudo, corre testes e imprime cobertura
testar: teste_tabuleiro teste_stack teste_ficheiros teste_interface
	@echo "--------------  TESTES  --------------"
	@./teste_tabuleiro
	@./teste_stack
	@./teste_ficheiros
	@./teste_interface
	@echo "-----------  GCOV  --------------"
	@gcov -b -c src/*.c | grep -E "Lines executed|File"
	@echo "--------------------------------------"

## 7. Limpeza
clean:
	rm -f src/*.o tests/*.o *.gcda *.gcno *.gcov \
	      jogo teste_tabuleiro teste_stack teste_ficheiros teste_interface
