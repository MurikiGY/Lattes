#ifndef _PESQUISADOR_
#define _PESQUISADOR_

#define STRSIZE 1024    //Tam do buffer de leitura

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "libstring.h"
#include "liblistaautor.h"

struct producao {
  char  *producao;      //Titulo da producao
  char  *titulo;        //periodico ou evento(conferencia)
  char  *qualis;        //A1, A2, ... B4 ou C ou NC
  int   ano;            //Ano da producao
};
typedef struct producao producao_t;

struct curriculo {
  char *pesquisador;      //Nome do pesquisador
  producao_t *V_eventos;  //Vetor de conferencias
  producao_t *V_artigos;  //Vetor de artigos
  int tam_eventos;
  int tam_artigos;
  lista_autor_t *coautores;
};
typedef struct curriculo curriculo_t;

//Busca dados de todos os pesqisadores pelo diretorio especificado
void leDados (DIR *dirstream, char *dir, curriculo_t *V_pesq, int tam_pesq);

//Calcula a quantidade de artigos e eventos
void calcArtigoEvento(FILE *stream, int *num_evento, int *num_artigo);

//Le nome do pesquisador
void leNome(FILE *stream, curriculo_t *V_pesq);

//Le dados de um evento
void leEvento(FILE *stream, producao_t *prod, lista_autor_t *autor);

//Le dados de um artigo
void leArtigo(FILE *stream, producao_t *prod, lista_autor_t *autor);

//Desaloca vetor de curriculos
void destroiCurriculos(curriculo_t *vetor, int tam);

//Imprime vetor de curriculos
void imprimeCurriculo(curriculo_t *vetor, int tam);

#endif
