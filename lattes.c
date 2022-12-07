#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>

#include "libraries/qualis.h"
#include "libraries/pesquisador.h"
#include "libraries/sumariza.h"
#include "libraries/libarquivos.h"
#include "libraries/libstring.h"

#define FILENAME  100   //Tam do nome dos arquivos de period. e conf.
#define DIST      4     //Parametro da distancia de edição de strings


int iniciaComparativos(char *per, char *conf, classe_t **V_per, int *tam_per, classe_t **V_conf, int *tam_conf){
  
  //Inicializa vetor de periodicos
  *V_per = leQualitativos(per, tam_per);
  if (!V_per){
    fprintf(stderr, "Erro na leitura dos periodicos\n");
    return 1;
  }

  //Inicializa vetor de conferencias
  *V_conf = leQualitativos(conf, tam_conf);
  if (!V_conf){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiClasse(*V_per, *tam_per);
    return 2;
  }

  return 0;
}

int iniciaDiretorios(char *dir1, char *dir2, DIR **dStream1, DIR **dStream2,
curriculo_t **V_pesq1, int *tam1, curriculo_t **V_pesq2, int *tam2, int dir_count){

  //Abertura da stream do primeiro diretorio
  *dStream1 = opendir(dir1);
  if ( !dir1 ){
    perror("Não foi possivel acessar o diretório\n");
    return 1;
  }
  //Inicializa vetor de pesquisadores
  *tam1 = numberFiles(*dStream1);
  *V_pesq1 = malloc( sizeof(curriculo_t) * (*tam1) );
  if ( !V_pesq1 ){
    fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
    return 2;
  }

  //Abertura da stream do segundo diretorio
  if ( dir_count == 2 ){
    //Abertura da stream do segundo diretorio
    *dStream2 = opendir(dir2);
    if (!dir2){
      perror("Não foi possivel acessar o diretório\n");
      return 3;}
    //Inicializa vetor de pesquisadores
    *tam2 = numberFiles(*dStream2);
    *V_pesq2 = malloc( sizeof(curriculo_t) * (*tam2) );
    if ( !V_pesq2 ){
      fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
      return 4;
    }
  }

  return 0;
}

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
  DIR         *dirstream1, *dirstream2;     //Variavel de stream do diretorio1
  char        dir1[FILENAME];               //Nome do primeiro diretorio
  char        dir2[FILENAME];               //Nome do segundo diretorio
  int         dir_count = 0;                //Contador do numero de diretorios
  char        periodicos[FILENAME];         //Nome do arquivo de periodicos
  char        conferencias[FILENAME];       //Nome do arquivo de conferencias
  classe_t    *V_per, *V_conf;              //Vetor de classes de periodicos e conferencias
  int         tam_per, tam_conf;            //Tamanho do vetor de periodicos e conferencias
  curriculo_t *V_pesq1, *V_pesq2;           //Vetores de pesquisadores
  int         tam_pesq1, tam_pesq2;         //Tamanho dos vetores de pesquisadores
  int         log;

  int option;
  //Teste de par
  while ( (option = getopt(argc, argv, "d:p:c:")) != -1)
    switch (option){
      case 'd':
        optind--;
        for (;optind < argc && *argv[optind] != '-'; optind++){
          if ( !dir_count )
            strcpy(dir1, argv[optind]);
          else
            strcpy(dir2, argv[optind]);
          dir_count++;
        }
        break;
      case 'p':
        strcpy(periodicos, optarg);
        break;
      case 'c':
        strcpy(conferencias, optarg);
        break;
      default:
        fprintf(stderr, "Passagem incorreta de parametros\n");
        exit(1);
    }

  //Inicia periodicos e conferencias
  log = iniciaComparativos(periodicos, conferencias, &V_per, &tam_per, &V_conf, &tam_conf);
  if ( log )
    return 2;

  //Inicia acesso aos diretorios
  log = iniciaDiretorios(dir1, dir2, &dirstream1, &dirstream2, &V_pesq1, &tam_pesq1, &V_pesq2, &tam_pesq2, dir_count);
  if ( log ){
    destroiClasse(V_per, tam_per);
    destroiClasse(V_conf, tam_conf);
    return 3;
  }
  
  leDados(dirstream1, dir1, V_pesq1, tam_pesq1);                    //Busca dados dos curriculos
  qualifica(V_pesq1, tam_pesq1, V_per, tam_per, V_conf, tam_conf);  //Atribui qualitativos
  if ( dir_count == 2 ){
    leDados(dirstream2, dir2, V_pesq2, tam_pesq2);
    qualifica(V_pesq2, tam_pesq2, V_per, tam_per, V_conf, tam_conf);
  }

  sumarizaDados(V_pesq1, tam_pesq1, V_pesq2, tam_pesq2, dir_count);  //Calcula os dados coletados

  //Libera estruturas
  destroiCurriculos(V_pesq1, tam_pesq1);
  closedir(dirstream1);
  if ( dir_count == 2 ){
    destroiCurriculos(V_pesq2, tam_pesq2);
    closedir(dirstream2);}

  //Desaloca periodicos e conferencias
  destroiClasse(V_per, tam_per);
  destroiClasse(V_conf, tam_conf);
  return 0;
}
