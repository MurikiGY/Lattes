#ifndef _QUALIS_
#define _QUALIS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstring.h"
#include "libarquivos.h"

#define STRBUFF 1024

struct classes {
  char  *nome;          //Nome da classe
  char  *tipo;          //A1, A2, ... B4 ou C
};
typedef struct classes classe_t;

//Retorna um inteiro de acordo com a string de qualidade
int estrato(char *qualis);

//Le strings em um arquivo e as retorna num vetor de classe
classe_t *leQualitativos(char *filename, int *tam);

//Desaloca vetor de classes
void destroiClasse(classe_t *vetor, int tam);

//Imprime vetor de classe
void imprimeClasse(classe_t *vetor, int tam);

#endif
