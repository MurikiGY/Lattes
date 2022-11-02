#ifndef _LEITURA_
#define _LEITURA_

#define STRSIZE 1024    //Tam do buffer de leitura
#define FILENAME 100    //Tam do nome dos arquivos de period. e conf.

struct classes {
  char  *nome;          //Nome da classe
  char  *tipo;          //A1, A2, ... B4 ou C
};
typedef struct classes classe_t;

struct producao {
  char  *qualificacao;  //A1, A2, ... B4 ou C ou Inex
  int   ano;            //Ano da producao
};
typedef struct producao producao_t;

struct curriculo {
  char *pesquisador;    //Nome do pesquisador
  producao_t *eventos;  //Vetor de conferencias
  producao_t *artigos;  //Vetor de artigos
};
typedef struct curriculo curriculo_t;


//Delay de execucao
void delay(int number_of_seconds);

//Le strings em um arquivo e as retorna num vetor de classe
classe_t *leStringsArquivo(char *filename, int *tam);

//Le dados de um evento
void leEvento(FILE *stream);

//Le dados de um artigo
void leArtigo(FILE *stream);

//Desaloca vetor de classes
void destroiVetor(classe_t *vetor, int tam);

void imprimeVetor(classe_t *vetor, int tam);

#endif
