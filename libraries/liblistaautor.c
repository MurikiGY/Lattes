#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liblistaautor.h"

lista_autor_t *criaListaAutor(){
  lista_autor_t *lista;

  lista = malloc( sizeof(lista_autor_t) );
  lista->tam = 0;
  lista->head = NULL;

  return lista;
}


lista_autor_t *destroiListaAutor(lista_autor_t *l){
  nodo_autor_t *aux = l->head;

  while ( aux ){
    l->head = aux->prox;
    free(aux->nome);
    free(aux);
    aux = l->head;
  }
  free(l);
  return NULL;
}


nodo_autor_t *criaNodoAutor(char *s){
  nodo_autor_t *nodo;

  nodo = malloc ( sizeof(nodo_autor_t) );
  nodo->nome = malloc( sizeof(char) * (strlen(s)+1) );
  strcpy(nodo->nome, s);
  nodo->prox = NULL;

  return nodo;
}


void insereListaAutor(lista_autor_t *l, char *s){
  nodo_autor_t *aux;

  if ( l->tam == 0 ){
    l->head = criaNodoAutor(s); //Lista vazia
    (l->tam)++;
    return;
  } else {
    //Percorre lista
    aux = l->head;
    while ( aux->prox ){
      if ( !strcmp(aux->nome, s) )
        return;
      aux = aux->prox;
    }
  }
  //Testa final da lista
  if ( !strcmp(aux->nome, s) )
    return;
  //Insere no final da lista
  aux->prox = criaNodoAutor(s);
  (l->tam)++;
}


void imprimeListaAutor(lista_autor_t *l){
  nodo_autor_t *aux = l->head;

  while ( aux ){
    printf("- %s\n", aux->nome);
    aux = aux->prox;
  }
}
