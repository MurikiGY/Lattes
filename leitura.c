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
classe_t *leQualificativos(char *filename, int *tam){
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


//Calcula a quantidade de artigos e eventos
void calcArtigoEvento(FILE *stream, int *num_evento, int *num_artigo){
  int evento = 0;
  int artigo = 0;
  char *s = malloc( sizeof(char) * STRSIZE );

  while ( fscanf(stream, "%s", s) != EOF )

    if ( strstr(s, "<TRABALHO-EM-EVENTOS") )
      evento++;
    else 
      if ( strstr(s, "<ARTIGO-PUBLICADO") )
        artigo++;

  free(s);

  *num_evento = evento;
  *num_artigo = artigo;

  rewind(stream);
}


//Le nome do pesquisador
void leNome(FILE *stream, curriculo_t *V_pesq){
  char *s = malloc( sizeof(char) * STRSIZE );

  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-COMPLETO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);
  V_pesq->pesquisador = malloc(sizeof(char) * (strlen(s)+1));
  strncpy(V_pesq->pesquisador, s, strlen(s)+1);

  free(s);
}


//Le dados de um evento
void leEvento(FILE *stream, producao_t *prod){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->producao = malloc( sizeof(char) * (strlen(s)+1) );
  strncpy(prod->producao, s, strlen(s)+1);

  //Acha o ano do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->ano = atoi(s);

  //Acha o nome do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-DO-EVENTO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->titulo = malloc( sizeof(char) * (strlen(s)+1) );
  strncpy(prod->titulo, s, strlen(s)+1);

  free(s);
}


//Le dados de um artigo
void leArtigo(FILE *stream, producao_t *prod){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->producao = malloc( sizeof(char) * (strlen(s)+1) );
  strncpy(prod->producao, s, strlen(s)+1);

  //Acha o ano do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->ano = atoi(s);

  //Acha o periodico do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-PERIODICO-OU-REVISTA=") )
    fscanf(stream, "%s", s);
  pegaDados(stream, s);

  prod->titulo = malloc( sizeof(char) * (strlen(s)+1) );
  strncpy(prod->titulo, s, strlen(s)+1);

  free(s);
}


//Desaloca vetor de classes
void destroiClasse(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    free(vetor[i].nome);
    free(vetor[i].tipo);
  }

  free(vetor);
}


//Desaloca vetor de curriculos
void destroiCurriculos(curriculo_t *vetor, int tam){

  for (int i=0; i<tam ;i++){

    //Desaloca vetor de eventos
    for (int j=0; j<vetor[i].tam_eventos ;j++){
      free(vetor[i].V_eventos[j].producao);
      free(vetor[i].V_eventos[j].titulo);
      free(vetor[i].V_eventos[j].qualis);
    }
    free(vetor[i].V_eventos);

    //Desaloca vetor de periodicos
    for (int k=0; k<vetor[i].tam_artigos ;k++)
    {
      free(vetor[i].V_artigos[k].producao);
      free(vetor[i].V_artigos[k].titulo);
      free(vetor[i].V_artigos[k].qualis);
    }
    free(vetor[i].V_artigos);

    //Desaloca nome
    free(vetor[i].pesquisador);
  }

  free(vetor);
}


void imprimeClasse(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    printf("%s\n", vetor[i].nome);
    printf("%s\n", vetor[i].tipo);
  }
}

void imprimeCurriculo(curriculo_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    printf("Imprimindo pesquisador: %s\n", vetor[i].pesquisador);

    printf("\n");

    printf("Eventos:\n");
    for (int j=0; j<vetor[i].tam_eventos ;j++){
      printf("Producao: %s\n", vetor[i].V_eventos[j].producao);
      printf("Evento: %s\n", vetor[i].V_eventos[j].titulo);
      printf("Qualis: %s\n", vetor[i].V_eventos[j].qualis);
      printf("Ano: %d\n", vetor[i].V_eventos[j].ano);
    }

    printf("\n");

    printf("Artigos:\n");
    for (int j=0; j<vetor[i].tam_artigos ;j++){
      printf("producao: %s\n", vetor[i].V_artigos[j].producao);
      printf("Periodico: %s\n", vetor[i].V_artigos[j].titulo);
      printf("Qualis: %s\n", vetor[i].V_artigos[j].qualis);
      printf("Ano: %d\n", vetor[i].V_artigos[j].ano);
    }

  }

}

