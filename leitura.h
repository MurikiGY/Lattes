#ifndef _LEITURA_
#define _LEITURA_

#define STRSIZE 1024    //Tam do buffer de leitura
#define FILENAME 100    //Tam do nome dos arquivos de period. e conf.

#include <stdio.h>

struct producao {
  char  *producao;      //Titulo da producao
  char  *titulo;        //periodico ou evento(conferencia)
  char  *qualis;        //A1, A2, ... B4 ou C ou NC
  int   ano;            //Ano da producao
};
typedef struct producao producao_t;

struct curriculo {
  char *pesquisador;    //Nome do pesquisador
  producao_t *V_eventos;  //Vetor de conferencias
  producao_t *V_artigos;  //Vetor de artigos
  int tam_eventos;
  int tam_artigos;
};
typedef struct curriculo curriculo_t;

//Calcula a quantidade de artigos e eventos
void calcArtigoEvento(FILE *stream, int *num_evento, int *num_artigo);

//Le nome do pesquisador
void leNome(FILE *stream, curriculo_t *V_pesq);

//Le dados de um evento
void leEvento(FILE *stream, producao_t *prod);

//Le dados de um artigo
void leArtigo(FILE *stream, producao_t *prod);

//Desaloca vetor de curriculos
void destroiCurriculos(curriculo_t *vetor, int tam);

//Imprime vetor de curriculos
void imprimeCurriculo(curriculo_t *vetor, int tam);

#endif
