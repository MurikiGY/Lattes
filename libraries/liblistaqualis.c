#include "liblistaqualis.h"


//Retorna ponteiro de criação da lista
lista_t *criaLista(){
  lista_t *l;

  l = malloc( sizeof(lista_t) );
  if ( !l )
    return NULL;

  l->tam = 0;
  l->head = NULL;
  l->tail = NULL;
  return l;
}


////Desaloca uma lista criada
lista_t *listaDestroi(lista_t *l){
  nodo_t *aux;

  while (l->head != NULL){
    aux = l->head->prox;
    free(l->head->titulo);
    free(l->head);
    l->head = aux;
  }

  l->head = NULL;
  l->tail = NULL;
  l->tam = 0;
  free(l);
  return NULL;
}


////Insere elemento na lista
int queue(lista_t *l, char *t){
  nodo_t *nodo;

  nodo = malloc( sizeof(nodo_t) );
  if ( !nodo )
    return 1;

  nodo->titulo = malloc( sizeof(char) * (strlen(t)+1) );
  strcpy(nodo->titulo, t);
  nodo->prox =  NULL;
  nodo->cntr = 1;

  if ( l->tam == 0 )
    l->head = nodo;
  else
    l->tail->prox = nodo;

  l->tail = nodo;
  (l->tam)++;
  return 0;
}


//Busca string dentro da lista e incrementa cntr se encontrar
int buscaListaIncrementa(lista_t *l, char *t){
  nodo_t *aux;

  aux = l->head;
  while ( aux != NULL ){
    if ( !strcmp(aux->titulo, t) ){
      (aux->cntr)++;
      return 0;
    }
    aux = aux->prox;
  }

  return queue(l, t);
}


void listaImprime(lista_t *l){
  nodo_t *aux;

  if ( l->tam != 0 ){
    aux = l->head;
    while ( aux != NULL){
      printf("%s: ", aux->titulo);
      printf("%d\n", aux->cntr);
      aux = aux->prox;
    }
  }
}

