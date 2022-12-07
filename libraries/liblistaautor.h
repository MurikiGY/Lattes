#ifndef _LIBLISTAAUTOR_
#define _LIBLISTAAUTOR_

struct nodo_autor {
  char *nome;
  struct nodo_autor *prox;
};
typedef struct nodo_autor nodo_autor_t;

struct lista_autor {
  int tam;
  nodo_autor_t *head;
};
typedef struct lista_autor lista_autor_t;


lista_autor_t *criaListaAutor();

lista_autor_t *destroiListaAutor(lista_autor_t *l);

void insereListaAutor(lista_autor_t *l, char *s);

void imprimeListaAutor(lista_autor_t *l);


#endif
