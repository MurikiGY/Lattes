#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libano.h"


//Cria lista de anos
ano_t *criaListaAno(){
  ano_t *a;

  a = malloc( sizeof(ano_t) );
  if ( !a )
    return NULL;

  a->tam = 0;
  a->head = NULL;

  return a;
}

//Desaloca lista de anos
ano_t *destroiListaAno(ano_t *a){


}


int criaNodo(nodo_t *pointer, int ano, char *qualis, int option){

  //Aloca estrutura
  pointer = malloc( sizeof(nodo_t) );
  pointer->ano = ano;
  pointer->conf = malloc ( sizeof(int) * 9 );
  pointer->per  = malloc ( sizeof(int) * 9 );
  
  //Zera vetores
  memset(pointer->conf, 0, sizeof(int)*9 );
  memset(pointer->per, 0, sizeof(int)*9 );

  if ( option == 0 ){
    //Conferencias
    if ( !strcmp(qualis, "A1") )
      (pointer->conf[0])++;
    else if ( !strcmp(qualis, "A2") )
      (pointer->conf[1])++;
    else if ( !strcmp(qualis, "A3") )
      (pointer->conf[2])++;
    else if ( !strcmp(qualis, "A4") )
      (pointer->conf[3])++;
    else if ( !strcmp(qualis, "B1") )
      (pointer->conf[4])++;
    else if ( !strcmp(qualis, "B2") )
      (pointer->conf[5])++;
    else if ( !strcmp(qualis, "B3") )
      (pointer->conf[6])++;
    else if ( !strcmp(qualis, "B4") )
      (pointer->conf[7])++;
    else if ( !strcmp(qualis, "C") )
      (pointer->conf[8])++;
  } else {
    //Artigos
    if ( !strcmp(qualis, "A1") )
      (pointer->per[0])++;
    else if ( !strcmp(qualis, "A2") )
      (pointer->per[1])++;
    else if ( !strcmp(qualis, "A3") )
      (pointer->per[2])++;
    else if ( !strcmp(qualis, "A4") )
      (pointer->per[3])++;
    else if ( !strcmp(qualis, "B1") )
      (pointer->per[4])++;
    else if ( !strcmp(qualis, "B2") )
      (pointer->per[5])++;
    else if ( !strcmp(qualis, "B3") )
      (pointer->per[6])++;
    else if ( !strcmp(qualis, "B4") )
      (pointer->per[7])++;
    else if ( !strcmp(qualis, "C") )
      (pointer->per[8])++;
  }

  return 0;
}


//Insere um artigo ou conferencia de maneira ordenada na lista
int insereOrdenadoListaAno(ano_t *a, int ano, char *qualis, int option){

  if ( !a->head )
    return criaNodo(a->head, ano, qualis, option);

  nodo_t *aux = a->head;

  while ( aux->prox != NULL ){
    if ( aux->ano == ano ){
      return 0;
    }
    aux = aux->prox;
  }

  return criaNodo(aux->prox, ano, qualis, option);
}


