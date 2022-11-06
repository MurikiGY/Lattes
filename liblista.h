#ifndef _LIBLISTA_
#define _LIBLISTA_

struct nodo {
  char    *titulo;    //String da producao
  int     cntr;       //Quantidade encontrada
  struct nodo *prox;
};
typedef struct nodo nodo_t;

struct lista {
  int     tam;      //Tamanho da lista
  nodo_t  *head;    //Ponteiro do inicio da lista
  nodo_t  *tail;    //Ponteiro do fim da lista
};
typedef struct lista lista_t;

//Retorna ponteiro de criação da lista
lista_t *criaLista();

//Desaloca uma lista criada
lista_t *listaDestroi(lista_t *l);

//Insere elemento na lista
int queue(lista_t *l, char *t);

//Remove elemento da lista
//Retorna titulo e qualis por parametro
int dequeue(lista_t *l, char *t);

//Busca string dentro da lista
int buscaListaIncrementa(lista_t *l, char *t);

void listaImprime(lista_t *l);

#endif
