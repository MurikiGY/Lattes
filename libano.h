#ifndef _LIBANO_
#define _LIBANO_

struct nodo {
  int   ano;      //Ano do nodo
  int   *conf;    //Vetor de conferencias do ano
  int   *per;     //vetor de periodicos do ano
  struct nodo *prox;
};
typedef struct nodo nodo_t;

struct ano {
  int     tam;
  nodo_t  *head;
};
typedef struct ano ano_t;

//Cria lista de anos
ano_t *criaListaAno();

//Desaloca lista de anos
ano_t *destroiListaAno(ano_t *a);

//Insere um artigo ou conferencia de maneira ordenada na lista
int insereOrdenadoListaAno(ano_t *a, int ano, char *qualis, int option);

//Imprime lista
void imprimeListaAno(ano_t *a);


#endif
