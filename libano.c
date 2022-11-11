#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formata.h"
#include "libano.h"


//Cria lista de anos
ano_t *criaListaAno(){
  ano_t *a;

  a = malloc( sizeof(ano_t) );
  if ( !a )
    return NULL;

  a->tam = 0;
  a->head = NULL;
  a->tail = NULL;

  return a;
}

//Desaloca lista de anos
ano_t *destroiListaAno(ano_t *a){
  nodo_ano_t *aux = a->head;

  while ( aux != NULL){
    free(aux->conf);
    free(aux->per);
    aux = aux->prox;
    free(a->head);
    a->head = aux;
  }

  free(a);

  return NULL;
}


int criaNodo(nodo_ano_t **pointer, int ano, char *qualis, int option){
  nodo_ano_t *aux;

  //Aloca estrutura
  aux = malloc( sizeof(nodo_ano_t) );
  aux->ano = ano;
  aux->conf = malloc ( sizeof(int) * 10 );
  aux->per  = malloc ( sizeof(int) * 10 );
  
  //Zera vetores
  memset(aux->conf, 0, sizeof(int)*10 );
  memset(aux->per, 0, sizeof(int)*10 );

  if ( option == 0 ){

    //Conferencias
    (aux->conf[ estrato(qualis) ])++;

  } else {

    //Artigos
    (aux->per[ estrato(qualis) ])++;

  }

  *pointer = aux;

  return 0;
}


//Insere uma conferencia(0) ou artigo(1) de maneira ordenada na lista
int insereOrdenadoListaAno(ano_t *a, int ano, char *qualis, int option){
  nodo_ano_t *nodo;

  nodo_ano_t *aux1, aux2;

  if ( a->tam == 0 ){

    //Configura o nodo
    nodo = malloc( sizeof(nodo_ano_t) );
    nodo->ano = ano;
    nodo->conf = malloc ( sizeof(int) * 10 );
    nodo->per  = malloc ( sizeof(int) * 10 );
    nodo->prox = NULL;
    memset(nodo->conf, 0, sizeof(int) * 10 );
    memset(nodo->per , 0, sizeof(int) * 10 );
    if ( option == 0 )
      (nodo->conf[ estrato(qualis) ])++;
    else
      (nodo->per[ estrato(qualis) ])++;

    a->head = nodo;
    a->tail = nodo;
    (a->tam)++;
    return 0;
  } //lista vazia

  //Insere inicio
  if ( ano < a->head->ano ){

    //Configura o nodo
    nodo = malloc( sizeof(nodo_ano_t) );
    nodo->ano = ano;
    nodo->conf = malloc ( sizeof(int) * 10 );
    nodo->per  = malloc ( sizeof(int) * 10 );
    nodo->prox = NULL;
    memset(nodo->conf, 0, sizeof(int) * 10 );
    memset(nodo->per , 0, sizeof(int) * 10 );
    if ( option == 0 )
      (nodo->conf[ estrato(qualis) ])++;
    else
      (nodo->per[ estrato(qualis) ])++;

    nodo->prox = a->head;
    a->head = nodo;
    (a->tam)++;
    return 0;

  } else {

    nodo_ano_t *aux1 = a->head;
    nodo_ano_t *aux2;
    
    //Ajusta ponteiros
    while ( aux1->ano < ano && aux1->prox != NULL){
      aux2 = aux1;
      aux1 = aux1->prox;
    }

    if ( aux1->ano == ano ){

      //Incrementa nodo
      if ( option == 0 )
        (aux1->conf[ estrato(qualis) ])++;
      else
        (aux1->per[ estrato(qualis) ])++;
      return 0;

    } else {

      //Configura o nodo
      nodo = malloc( sizeof(nodo_ano_t) );
      nodo->ano = ano;
      nodo->conf = malloc ( sizeof(int) * 10 );
      nodo->per  = malloc ( sizeof(int) * 10 );
      nodo->prox = NULL;
      memset(nodo->conf, 0, sizeof(int) * 10 );
      memset(nodo->per , 0, sizeof(int) * 10 );
      if ( option == 0 )
        (nodo->conf[ estrato(qualis) ])++;
      else
        (nodo->per[ estrato(qualis) ])++;

      if ( ano > aux1->ano ){

        //Insere no final
        aux1->prox = nodo;
        a->tail = nodo;
      } else {
        //Insere entre aux1 e aux2
        nodo->prox = aux1;
        aux2->prox = nodo;

      }
      (a->tam)++;

    } //Insere dados

  } //Insere inicio

  return 0;
}


//Imprime lista
void imprimeListaAno(ano_t *a){
  nodo_ano_t *aux;

  if ( a->tam > 0 ){
    aux = a->head;
    while ( aux != NULL ){
      printf("Ano: %d\n", aux->ano);
      printf("+---------------------------+\n");
      printf("| Conferencias | Periodicos |\n");
      printf("+---------------------------+\n");
      for (int j=0; j<4 ;j++)
        printf("| A%d: %-03d      | A%d: %-03d    |\n", j+1, aux->conf[j], j+1, aux->per[j]);
      for (int j=4; j<8 ;j++)
        printf("| B%d: %-03d      | B%d: %-03d    |\n", j-3, aux->conf[j], j-3, aux->per[j]);
      printf("| C : %-03d      | C : %-03d    |\n", aux->conf[8], aux->per[8]);
      printf("+---------------------------+\n");
      printf("\n");
      aux = aux->prox;
    }
  }
}
