#ifndef _LIBLISTAANO_
#define _LIBLISTAANO_


struct nodo_ano {
  int         ano;      //Ano do nodo
  int         *conf;    //Vetor de conferencias do ano
  int         *per;     //vetor de periodicos do ano
  struct      nodo_ano *prox;
};
typedef struct nodo_ano nodo_ano_t;

struct ano {
  int         tam;
  nodo_ano_t  *head;
  nodo_ano_t  *tail;
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
