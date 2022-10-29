#ifndef _UTILS_
#define _UTILS_

#define STRSIZE 1000
#define FILENAME 100

//Delay de execucao
void delay(int number_of_seconds);

//Completa a leitura dos dados de uma tag
void completaDado(FILE *stream, char *s);

//Le strings em um arquivo e as aloca em um vetor
int leStringsArquivo(char *filename, char **vetor, int *tam);

//Desaloca um vetor de strings
void destroiVetorString(char **vetor, int tam);

void imprimeVetorStrings(char **vetor, int tam);

#endif

