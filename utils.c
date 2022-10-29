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
int leStringsArquivo(char *filename, char **vetor, int *tam){
  FILE  *filestream;
  int   count = 0;

  printf("Abrindo arquivo: %s\n", filename);

  filestream = fopen(filename, "r");
  if (!filestream){
    fprintf(stderr, "Falha na abertura da stream %s\n", filename);
    return 1;
  }

  char  s[STRSIZE];
  //Conta quantas linhas tem o arquivo
  while ( fgets(s, STRSIZE, filestream) )
    count++;

  *tam = count;
  printf("Foram encontrados %d linhas em %s\n", count, filename);

  //Malloc do tamanho do vetor
  vetor = malloc(count * sizeof(char*));
  if ( !vetor ){
    fprintf(stderr, "Falha na alocacao do vetor de strings\n");
    return 2;
  }

  rewind(filestream);

  int i = 0;
  while ( fgets(s, STRSIZE, filestream) ){
    vetor[i] = malloc( sizeof(char) * ( strlen(s) + 1 ) );
    strncpy(vetor[i], s, strlen(s) + 1);
    i++;
  }

  fclose(filestream);

  return 0;
}

//Desaloca um vetor de strings
void destroiVetorString(char **vetor, int tam){

//  for (int i=0; i<tam ;i++)
//    free(vetor[i]);

  free(vetor);
}


void imprimeVetorStrings(char **vetor, int tam){
  
  for (int i=0; i<tam ;i++)
    printf("%s", vetor[i]);

}








