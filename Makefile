# Variáveis de compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g -I./src -I/opt/homebrew/Cellar/cunit/2.1-3/include
LIBS = $(shell pkg-config --libs cunit)

# Ficheiros objeto para o executável do jogo
OBJS_JOGO = src/main.o src/tabuleiro.o src/interface.o

# Ficheiros objeto para o executável de testes
OBJS_TESTES = tests/test-tabuleiro.o src/tabuleiro.o

.PHONY: all jogo testar clean

# Target padrão: compilar o executável "jogo"
all: jogo

# Target "jogo": compila o jogo usando main.c e tabuleiro.c
jogo: $(OBJS_JOGO)
	$(CC) $(CFLAGS) $(OBJS_JOGO) -o jogo $(LIBS)

# Compilar main.c para gerar src/main.o
src/main.o: src/main.c src/tabuleiro.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

# Compilar tabuleiro.c para gerar src/tabuleiro.o
src/tabuleiro.o: src/tabuleiro.c src/tabuleiro.h
	$(CC) $(CFLAGS) -c src/tabuleiro.c -o src/tabuleiro.o

# Target "teste": compila o executável de testes usando test-tabuleiro.c e tabuleiro.o
teste: $(OBJS_TESTES)
	$(CC) $(CFLAGS) $(OBJS_TESTES) -o teste $(LIBS)

# Target "testar": compila e executa os testes
testar: teste
	./teste

# Compilar test-tabuleiro.c para gerar tests/test-tabuleiro.o
tests/test-tabuleiro.o: tests/test-tabuleiro.c src/tabuleiro.h
	$(CC) $(CFLAGS) -c tests/test-tabuleiro.c -o tests/test-tabuleiro.o

# Target "clean": remove ficheiros objeto e executáveis
clean:
	rm -f src/*.o tests/*.o jogo teste
