#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "leitura.h"
#include "formata.h"


// Delay de execucao
void delay(int number_of_seconds){
  //Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds);
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
  char  *str = malloc( sizeof(char) * STRSIZE );
  while ( fgets(str, STRSIZE, filestream) )
    count++;

  *tam = count;

  //Malloc do tamanho do vetor
  vetor = malloc(count * sizeof(classe_t));
  if ( !vetor ){
    fprintf(stderr, "Falha na alocacao do vetor de classes\n");
    return NULL;
  }

  rewind(filestream);

  int i=0;
  while ( fgets(str, STRSIZE, filestream) ){
    str[strcspn(str, "\n")] = '\0'; 
    char *final = strrchr(str, ' ');
    final[0] = '\0';
    (final)++;        //str eh o conteudo e final eh o tipo
    vetor[i].nome = malloc( sizeof(char) * (strlen(str)+1));
    strncpy(vetor[i].nome, str, strlen(str)+1);
    vetor[i].tipo = malloc( sizeof(char) * (strlen(final)+1));
    strncpy(vetor[i].tipo, final, strlen(final)+1);
    i++;
  }

  free(str);

  fclose(filestream);

  return vetor;
}

//Le nome do pesquisador
void leNome(FILE *stream){
  char *s = malloc( sizeof(char) * STRSIZE );

  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-COMPLETO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("Nome do pesquisador: %s\n", s );

  free(s);
}


//Le dados de um evento
void leEvento(FILE *stream){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  //Acha o ano do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  //Acha o nome do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-DO-EVENTO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  free(s);
  printf("\n");
}


//Le dados de um artigo
void leArtigo(FILE *stream){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  //Acha o ano do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  //Acha o periodico do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-PERIODICO-OU-REVISTA=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  printf("%s\n", s );

  free(s);
  printf("\n");
}


//Desaloca vetor de classes
void destroiClasse(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    free(vetor[i].nome);
    free(vetor[i].tipo);
  }

  free(vetor);
}


void imprimeVetor(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    printf("%s\n", vetor[i].nome);
    printf("%s\n", vetor[i].tipo);
  }
}
