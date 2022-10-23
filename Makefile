CC = gcc
CCFLAGS = -Wall -g -std=c99     #Flags de compilação
LDFLAGS =                       #Blibliotecas a ligar
SOURCES = $(wildcard *.c)       #Arquivos .c
OBJECTS = $(SOURCES: .c=.o)     #Arquivos .o
	TARGET	= lattes                #Executavel

#Regra default (primeira regra)
all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

#Remove arquivos temporarios
clean:
	rm -f *~ *.o

#Remove o que não for o codigo fonte original
purge:
	rm $(TARGET)
