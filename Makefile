# Definir as variáveis
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g

# Ficheiro executável final
EXEC = jogo

# Target principal, que compila tudo (por enquanto, só a Etapa 1)
all: etapa1.o
	$(CC) $(CFLAGS) -o $(EXEC) etapa1.o

# Target para compilar o código da Etapa 1
etapa1.o: Etapa1/etapa1.c
	$(CC) $(CFLAGS) -c Etapa1/etapa1.c -o etapa1.o

# Target para limpar os ficheiros object e o executável
clean:
	rm -f etapa1.o $(EXEC)

# Target para correr os testes
test:
	./$(EXEC) # Adiciona os comandos necessários para correr os testes
