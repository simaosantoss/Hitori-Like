# Variáveis de compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g -I./src -I/opt/homebrew/Cellar/cunit/2.1-3/include
LIBS = $(shell pkg-config --libs cunit)

# Ficheiros objeto para o executável do jogo
OBJS_JOGO = src/main.o src/tabuleiro.o src/interface.o src/ficheiros.o src/stack.o

# Ficheiros objeto para os testes:
OBJS_TEST_TABULEIRO = tests/test-tabuleiro.o src/tabuleiro.o
OBJS_TEST_STACK = tests/test-stack.o src/stack.o
OBJS_TEST_FICHEIROS = tests/test-ficheiros.o src/ficheiros.o
OBJS_TEST_INTERFACE = tests/test-interface.o src/interface.o src/tabuleiro.o src/ficheiros.o src/stack.o

.PHONY: all jogo teste_tabuleiro teste_stack teste_ficheiros teste_interface testar clean

# Target padrão: compilar o executável "jogo"
all: jogo

# Executável principal do jogo
jogo: $(OBJS_JOGO)
	$(CC) $(CFLAGS) $(OBJS_JOGO) -o jogo $(LIBS)

# Compila main.c
src/main.o: src/main.c src/tabuleiro.h src/interface.h src/stack.h src/ficheiros.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Compila tabuleiro.c
src/tabuleiro.o: src/tabuleiro.c src/tabuleiro.h
	$(CC) $(CFLAGS) -c src/tabuleiro.c -o src/tabuleiro.o

# Compila interface.c
src/interface.o: src/interface.c src/interface.h src/tabuleiro.h src/stack.h src/ficheiros.h
	$(CC) $(CFLAGS) -c src/interface.c -o src/interface.o

# Compila ficheiros.c
src/ficheiros.o: src/ficheiros.c src/ficheiros.h
	$(CC) $(CFLAGS) -c src/ficheiros.c -o src/ficheiros.o

# Compila stack.c
src/stack.o: src/stack.c src/stack.h
	$(CC) $(CFLAGS) -c src/stack.c -o src/stack.o

# Compilar teste para o módulo tabuleiro
teste_tabuleiro: $(OBJS_TEST_TABULEIRO)
	$(CC) $(CFLAGS) $(OBJS_TEST_TABULEIRO) -o teste_tabuleiro $(LIBS)

tests/test-tabuleiro.o: tests/test-tabuleiro.c src/tabuleiro.h
	$(CC) $(CFLAGS) -c tests/test-tabuleiro.c -o tests/test-tabuleiro.o

# Compilar teste para o módulo stack
teste_stack: $(OBJS_TEST_STACK)
	$(CC) $(CFLAGS) $(OBJS_TEST_STACK) -o teste_stack $(LIBS)

tests/test-stack.o: tests/test-stack.c src/stack.h
	$(CC) $(CFLAGS) -c tests/test-stack.c -o tests/test-stack.o

# Compilar teste para o módulo ficheiros
teste_ficheiros: $(OBJS_TEST_FICHEIROS)
	$(CC) $(CFLAGS) $(OBJS_TEST_FICHEIROS) -o teste_ficheiros $(LIBS)

tests/test-ficheiros.o: tests/test-ficheiros.c src/ficheiros.h
	$(CC) $(CFLAGS) -c tests/test-ficheiros.c -o tests/test-ficheiros.o

# Compilar teste para o módulo interface
teste_interface: $(OBJS_TEST_INTERFACE)
	$(CC) $(CFLAGS) $(OBJS_TEST_INTERFACE) -o teste_interface $(LIBS)

tests/test-interface.o: tests/test-interface.c src/interface.h src/tabuleiro.h src/stack.h src/ficheiros.h
	$(CC) $(CFLAGS) -c tests/test-interface.c -o tests/test-interface.o

# Target "testar": compila e executa todos os testes
testar: teste_tabuleiro teste_stack teste_ficheiros teste_interface
	@echo "Executando teste de tabuleiro..."
	./teste_tabuleiro
	@echo "Executando teste de stack..."
	./teste_stack
	@echo "Executando teste de ficheiros..."
	./teste_ficheiros
	@echo "Executando teste de interface..."
	./teste_interface

# Target "clean": remove ficheiros objeto e executáveis
clean:
	rm -f src/*.o tests/*.o jogo teste_tabuleiro teste_stack teste_ficheiros teste_interface
