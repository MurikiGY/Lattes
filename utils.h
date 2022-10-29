#ifndef _UTILS_
#define _UTILS_

#define STRSIZE 1000
#define FILENAME 100

struct classes {
  char *nome;
  char *tipo;
};
typedef struct classes classe_t;

//Delay de execucao
void delay(int number_of_seconds);

//Completa a leitura dos dados de uma tag
void completaDado(FILE *stream, char *s);

//Le strings em um arquivo e as retorna num vetor de classe
classe_t *leStringsArquivo(char *filename, int *tam);

//Desaloca vetor de classes
void destroiVetor(classe_t *vetor, int tam);

void imprimeVetor(classe_t *vetor, int tam);

#endif

