# Variáveis de compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g $(shell pkg-config --cflags cunit)
LIBS = $(shell pkg-config --libs cunit)

# Ficheiros objeto para o executável do jogo
OBJS_JOGO = Etapa1/main.o Etapa1/etapa1.o

# Ficheiros objeto para o executável de testes
OBJS_TESTES = Etapa1/testes-etapa1.o Etapa1/etapa1.o

.PHONY: all jogo testar clean

# Target padrão: compilar o executável "jogo"
all: jogo

# Target "jogo": compila o jogo usando main.c e etapa1.c
jogo: $(OBJS_JOGO)
	$(CC) $(CFLAGS) $(OBJS_JOGO) -o jogo

# Compilar main.c para gerar Etapa1/main.o
Etapa1/main.o: Etapa1/main.c Etapa1/etapa1.h
	$(CC) $(CFLAGS) -c Etapa1/main.c -o Etapa1/main.o

# Compilar etapa1.c para gerar Etapa1/etapa1.o
Etapa1/etapa1.o: Etapa1/etapa1.c Etapa1/etapa1.h
	$(CC) $(CFLAGS) -c Etapa1/etapa1.c -o Etapa1/etapa1.o

# Target "teste": compila o executável de testes usando testes-etapa1.c e etapa1.c
teste: $(OBJS_TESTES)
	$(CC) $(CFLAGS) $(OBJS_TESTES) -o teste $(LIBS)

# Target "testar": compila e executa os testes
testar: teste
	./teste

# Compilar testes-etapa1.c para gerar Etapa1/testes-etapa1.o
Etapa1/testes-etapa1.o: Etapa1/testes-etapa1.c Etapa1/etapa1.h
	$(CC) $(CFLAGS) -c Etapa1/testes-etapa1.c -o Etapa1/testes-etapa1.o

# Target "clean": remove ficheiros objeto e executáveis
clean:
	rm -f Etapa1/*.o jogo teste
