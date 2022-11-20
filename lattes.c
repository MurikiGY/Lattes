#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>

#include "qualis.h"
#include "pesquisador.h"
#include "sumariza.h"
#include "libarquivos.h"
#include "libstring.h"

#define FILENAME 100    //Tam do nome dos arquivos de period. e conf.
#define DIST 3    //Parametro da distancia de edição de strings


///////////////////////////////////
/// Edit Distance
////////////////////////////////////
int distance (char* word1, char* word2){
  int len1 = strlen(word1);
  int len2 = strlen(word2);

  int matrix[len1 + 1][len2 + 1];
  int i;

  for (i = 0; i <= len1; i++)
    matrix[i][0] = i;

  for (i = 0; i <= len2; i++)
    matrix[0][i] = i;

  for (i = 1; i <= len1; i++){
    int j;
    char c1;

    c1 = word1[i-1];
    for (j = 1; j <= len2; j++){
      char c2;

      c2 = word2[j-1];
      if (c1 == c2)
        matrix[i][j] = matrix[i-1][j-1];

      else{
        int delete;
        int insert;
        int substitute;
        int minimum;

        delete = matrix[i-1][j] + 1;
        insert = matrix[i][j-1] + 1;
        substitute = matrix[i-1][j-1] + 1;
        minimum = delete;
        if (insert < minimum)
          minimum = insert;

        if (substitute < minimum)
          minimum = substitute;

        matrix[i][j] = minimum;
      }
    }
  }
  return (matrix[len1][len2]);
}


void qualifica(curriculo_t *V_pesq, int tam_pesq, classe_t *V_per, int tam_per, classe_t *V_conf, int tam_conf){

#pragma omp parallel for
  //Percorre pesquisadores
  for(int i=0; i<tam_pesq ;i++){
    printf("Qualificando %s\n", V_pesq[i].pesquisador);
    //Percorre eventos
    for(int j=0; j<V_pesq[i].tam_eventos ;j++)
      //Percorre vetor de conferencias
      for(int l=0; l<tam_conf ;l++){
        V_pesq[i].V_eventos[j].qualis = NULL;
        if ( distance(V_pesq[i].V_eventos[j].titulo, V_conf[l].nome) < strlen(V_pesq[i].V_eventos[j].titulo)/DIST ){
          V_pesq[i].V_eventos[j].qualis = V_conf[l].tipo;
          break;
        }
      } //For de conferencias

    //Percorre artigos
    for(int k=0; k<V_pesq[i].tam_artigos ;k++)

      //Percorre vetor de artigos
      for(int l=0; l<tam_per ;l++){
        V_pesq[i].V_artigos[k].qualis = NULL;
        if ( distance(V_pesq[i].V_artigos[k].titulo, V_per[l].nome) < strlen(V_pesq[i].V_artigos[k].titulo)/DIST ){
          V_pesq[i].V_artigos[k].qualis = V_per[l].tipo;
          break;
        }
      } //For de artigos

  } //Percorre pesquisadores
}


int main (int argc, char **argv){
  DIR         *dirstream1;              //Variavel de stream do diretorio1
  DIR         *dirstream2;              //Variavel de stream do diretorio2
  char        diretorio1[FILENAME];     //Nome do primeiro diretorio
  char        diretorio2[FILENAME];     //Nome do segundo diretorio
  int         dir_count = 0;            //Contador do numero de diretorios
  char        periodicos[FILENAME];     //Nome do arquivo de periodicos
  char        conferencias[FILENAME];   //Nome do arquivo de conferencias
  classe_t    *V_per;                   //Vetor de classes periodicos
  int         tam_per;                  //Tamanho do vetor de periodicos
  classe_t    *V_conf;                  //Vetor de classes conferencias
  int         tam_conf;                 //Tamanho do vetor de conferencias
  curriculo_t *V_pesq1;                  //Vetor de pesquisadores
  int         tam_pesq1;                 //Tamanho do vetor de pesquisadores
  curriculo_t *V_pesq2;                  //Vetor de pesquisadores
  int         tam_pesq2;                 //Tamanho do vetor de pesquisadores

  int option;
  //Teste de parametros
  while ( (option = getopt(argc, argv, "d:p:c:")) != -1)
    switch (option){
      case 'd':
        optind--;
        for (;optind < argc && *argv[optind] != '-'; optind++){
          if ( !dir_count )
            strncpy(diretorio1, argv[optind], FILENAME);
          else
            strncpy(diretorio2, argv[optind], FILENAME);
          dir_count++;
        }
        break;
      case 'p':
        strncpy(periodicos, optarg, FILENAME);
        break;
      case 'c':
        strncpy(conferencias, optarg, FILENAME);
        break;
      default:
        fprintf(stderr, "Passagem incorreta de parametros\n");
        exit(1);
    }

  //Abertura da stream do primeiro diretorio
  dirstream1 = opendir(diretorio1);
  if (!dirstream1){
    perror("Não foi possivel acessar o diretório\n");
    exit(2);
  }

  if ( dir_count == 2 ){
    //Abertura da stream do segundo diretorio
    dirstream2 = opendir(diretorio2);
    if (!dirstream2){
      perror("Não foi possivel acessar o diretório\n");
      exit(2);
    }
  }

  //Inicializa vetor de periodicos
  V_per = leQualitativos(periodicos, &tam_per);
  if (!V_per){
    fprintf(stderr, "Erro na leitura dos periodicos\n");
    exit(3);
  }

  //Inicializa vetor de conferencias
  V_conf = leQualitativos(conferencias, &tam_conf);
  if (!V_conf){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiClasse(V_per, tam_per);
    exit(4);
  }

  //Inicializa vetor de pesquisadores
  tam_pesq1 = numberFiles(dirstream1);
  V_pesq1 = malloc( sizeof(curriculo_t) * tam_pesq1 );
  if ( !V_pesq1 ){
    fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
    destroiClasse(V_per, tam_per);
    destroiClasse(V_conf, tam_conf);
    exit(5);
  }

  if ( dir_count == 2 ){
    //Inicializa vetor de pesquisadores
    tam_pesq2 = numberFiles(dirstream2);
    V_pesq2 = malloc( sizeof(curriculo_t) * tam_pesq2 );
    if ( !V_pesq2 ){
      fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
      destroiClasse(V_per, tam_per);
      destroiClasse(V_conf, tam_conf);
      exit(5);
    }
  }

  //Sumariza curriculos
  ledados(dirstream1, diretorio1, V_pesq1, tam_pesq1);
  if ( dir_count == 2 )
    //Sumariza curriculos
    ledados(dirstream2, diretorio2, V_pesq2, tam_pesq2);

  //Atribui os qualitativos de periodicos e conferencias aos pesquisadores
  qualifica(V_pesq1, tam_pesq1, V_per, tam_per, V_conf, tam_conf);

  if ( dir_count == 2 )
  //Atribui os qualitativos de periodicos e conferencias aos pesquisadores
  qualifica(V_pesq2, tam_pesq2, V_per, tam_per, V_conf, tam_conf);

  //Calcula os dados coletados
  sumarizaDados(V_pesq1, tam_pesq1);
  if ( dir_count == 2 )
    //Calcula os dados coletados
    sumarizaDados(V_pesq2, tam_pesq2);

  //Desaloca vetores
  destroiCurriculos(V_pesq1, tam_pesq1);
  if ( dir_count == 2 )
    destroiCurriculos(V_pesq2, tam_pesq2);
  destroiClasse(V_per, tam_per);
  destroiClasse(V_conf, tam_conf);

  //Fechamento da stream
  closedir(dirstream1);
  closedir(dirstream2);
  return 0;
}
