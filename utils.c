#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

// Delay de execucao
void delay(int number_of_seconds){
    //Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}


//Completa a leitura dos dados de uma tag
void completaDado(FILE *stream, char *s){

  while (s[strlen(s)-1] != '"' ){

    char aux[STRSIZE];
    fscanf(stream, "%s", &aux);
    strcat(s, " ");
    strcat(s, aux);  //Concatena titulo completo

  }
}


//Le strings em um arquivo e as retorna em um vetor
classe_t *leStringsArquivo(char *filename, int *tam){
  FILE      *filestream;
  classe_t  *vetor;
  int       count = 0;

  filestream = fopen(filename, "r");
  if (!filestream){
    fprintf(stderr, "Falha na abertura da stream %s\n", filename);
    return NULL;
  }

  //Conta quantas linhas tem o arquivo
  char  s[STRSIZE];
  while ( fgets(s, STRSIZE, filestream) )
    count++;

  *tam = count;

  //Malloc do tamanho do vetor
  vetor = malloc(count * sizeof(classe_t));
  if ( !vetor ){
    fprintf(stderr, "Falha na alocacao do vetor de strings\n");
    return NULL;
  }

  rewind(filestream);

  int i = 0;
  while ( fgets(s, STRSIZE, filestream) ){
    vetor[i].nome = malloc( sizeof(char) * ( strlen(s) + 1 ) );
    vetor[i].tipo = NULL;
    strncpy(vetor[i].nome, s, strlen(s) + 1);
    printf("%s", vetor[i].nome);
    i++;
  }

  fclose(filestream);

  return vetor;
}


//Desaloca um vetor de strings
void destroiVetor(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++)
    free(vetor[i].nome);

  free(vetor);
}


void imprimeVetor(classe_t *vetor, int tam){
  
  for (int i=0; i<tam ;i++)
    printf("%s", vetor[i].nome);

}


