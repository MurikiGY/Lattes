#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liblistaano.h"
#include "qualis.h"


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


nodo_ano_t *criaNodo(int ano, char *qualis, int option){
  nodo_ano_t *nodo;

  //Aloca estrutura
  nodo = malloc( sizeof(nodo_ano_t) );
  if ( !nodo ){
    fprintf(stderr, "Erro de criacao de nodo ano\n");
    return NULL;}
  nodo->ano = ano;
  nodo->conf = malloc ( sizeof(int) * 10 );
  nodo->per  = malloc ( sizeof(int) * 10 );
  nodo->prox = NULL;
  
  //Zera vetores
  memset(nodo->conf, 0, sizeof(int)*10 );
  memset(nodo->per, 0, sizeof(int)*10 );

  if ( option == 0 ){
    //Conferencias
    (nodo->conf[ estrato(qualis) ])++;
  } else {
    //Artigos
    (nodo->per[ estrato(qualis) ])++;
  }

  return nodo;
}


//Insere uma conferencia(0) ou artigo(1) de maneira ordenada na lista
int insereOrdenadoListaAno(ano_t *a, int ano, char *qualis, int option){
  nodo_ano_t *nodo;

  //Insere primeiro nodo na lista
  if ( a->tam == 0 ){
    nodo = criaNodo(ano, qualis, option);
    a->head = nodo;
    a->tail = nodo;
    (a->tam)++;
    return 0;
  } //lista vazia

  //Insere antes do primeiro nodo da lista
  if ( ano < a->head->ano ){
    nodo = criaNodo(ano, qualis, option);
    nodo->prox = a->head;
    a->head = nodo;
    (a->tam)++;
    return 0;

  } else {

    //Percorre lista
    nodo_ano_t *aux1 = a->head;
    nodo_ano_t *aux2 = a->head;
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
      nodo = criaNodo(ano, qualis, option);

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
        printf("| A%d: %-3d      | A%d: %-3d    |\n", j+1, aux->conf[j], j+1, aux->per[j]);
      for (int j=4; j<8 ;j++)
        printf("| B%d: %-3d      | B%d: %-3d    |\n", j-3, aux->conf[j], j-3, aux->per[j]);
      printf("| C : %-3d      | C : %-3d    |\n", aux->conf[8], aux->per[8]);
      printf("+---------------------------+\n");
      printf("\n");
      aux = aux->prox;
    }
  }
}
