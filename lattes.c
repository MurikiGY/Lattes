#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "qualis.h"
#include "leitura.h"
#include "formata.h"
#include "sumariza.h"

#define DIST 3    //Parametro da distancia de edição de strings

void strtolow (char *s){
  for (int i=0; i<strlen(s) ;i++)
    s[i] = tolower(s[i]);
}

//Retorna o numero de arquivos dentro de um diretorio
int numberFiles (DIR *dirstream){
  struct dirent   *entry;     //Estrutura do dirent
  int             tam = 0;    //Contador do numero de arquivos

  //Loop de contagem de arquivos
  while ( entry = readdir(dirstream) )
    if (entry->d_type == DT_REG)
      tam++;

  //Retorna o ponteiro para o inicio do diretorio
  rewinddir(dirstream);

  return tam;
}


void ledados (DIR *dirstream, char *dir, curriculo_t *V_pesq, int tam_pesq){
  struct dirent   *entry;         //Estrutura do dirent dara o dirstream
  FILE            *filestream;    //Stream de acesso a arquivo
  int i=0;  //Contador do vetor do pesquisador
  int j=0;  //Contador do vetor de eventos para cada pesquisador
  int k=0;  //Contador do vetor de artigos para cada pesquisador


  //Iteração sobre o diretorio
  while ( entry = readdir(dirstream) )
    if ( entry->d_type == DT_REG ){

      char path[strlen(dir) + strlen(entry->d_name) + 2];
      snprintf(path, strlen(dir) + strlen(entry->d_name) + 1,
          "%s\%s", dir, entry->d_name);

      //Leitura dos arquivos
      filestream = fopen(path, "r");
      if (filestream){

        //Le dados do arquivo

        int cnt_evento, cnt_artigo;
        calcArtigoEvento(filestream, &cnt_evento, &cnt_artigo);
        V_pesq[i].tam_eventos = cnt_evento;
        V_pesq[i].tam_artigos = cnt_artigo;
        V_pesq[i].V_eventos = malloc( sizeof(producao_t) * cnt_evento );
        V_pesq[i].V_artigos = malloc( sizeof(producao_t) * cnt_artigo );

        //Busca o nome do pesquisador
        leNome(filestream, &V_pesq[i]);
        printf("Buscando dados do pesquisador %s\n", V_pesq[i].pesquisador);

        char *strng = malloc( sizeof(char) * STRSIZE );
        while ( fscanf(filestream, "%s", strng) != EOF ){

          //Le evento
          if ( strstr(strng, "<TRABALHO-EM-EVENTOS") )
            leEvento(filestream, &V_pesq[i].V_eventos[j++]);

          //Le artigo
          if ( strstr(strng, "<ARTIGO-PUBLICADO") )
            leArtigo(filestream, &V_pesq[i].V_artigos[k++]);

        }
        free(strng);
        i++;
        j = 0;
        k = 0;

        //Le dados do arquivo

        fclose(filestream);

      } else
        fprintf(stderr, "Erro em abrir o arquivo %s", entry->d_name);

    } //if ( entry->d_type == DT_REG )

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

    //Percorre eventos
    for(int j=0; j<V_pesq[i].tam_eventos ;j++)
      //Percorre vetor de conferencias
      for(int l=0; l<tam_conf ;l++){
        printf("\rAnalisando conferencias de %s", V_pesq[i].pesquisador);
        fflush(stdout);
        V_pesq[i].V_eventos[j].qualis = NULL;
        if ( distance(V_pesq[i].V_eventos[j].titulo, V_conf[l].nome) < strlen(V_pesq[i].V_eventos[j].titulo)/DIST ){
          V_pesq[i].V_eventos[j].qualis = V_conf[l].tipo;
          break;
        }
      } //For de conferencias
    printf("\n");

    //Percorre artigos
    for(int k=0; k<V_pesq[i].tam_artigos ;k++)

      //Percorre vetor de artigos
      for(int l=0; l<tam_per ;l++){
        printf("\rAnalisando artigos de %s", V_pesq[i].pesquisador);
        fflush(stdout);
        V_pesq[i].V_artigos[k].qualis = NULL;
        if ( distance(V_pesq[i].V_artigos[k].titulo, V_per[l].nome) < strlen(V_pesq[i].V_artigos[k].titulo)/DIST ){
          V_pesq[i].V_artigos[k].qualis = V_per[l].tipo;
          break;
        }
      } //For de artigos
    printf("\n");

  } //Percorre pesquisadores

}


int main (int argc, char **argv){
  DIR         *dirstream;               //Variavel de stream do diretorio
  char        periodicos[FILENAME];     //Nome do arquivo de periodicos
  char        conferencias[FILENAME];   //Nome do arquivo de conferencias
  classe_t    *V_per;                   //Vetor de classes periodicos
  classe_t    *V_conf;                  //Vetor de classes conferencias
  int         tam_per;                  //Tamanho do vetor de periodicos
  int         tam_conf;                 //Tamanho do vetor de conferencias
  curriculo_t *V_pesq;                  //Vetor de pesquisadores
  int         tam_pesq;                 //Tamanho do vetor de pesquisadores
  int option;

  //Teste de parametros
  while ( (option = getopt(argc, argv, "d:p:c:")) != -1)
    switch (option){
      case 'd':
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

  //Abertura da stream do diretorio
  dirstream = opendir(argv[2]);
  if (!dirstream){
    perror("Não foi possivel acessar o diretório\n");
    exit(2);
  }

  //Inicializa vetor de periodicos
  V_per = leQualitativos(periodicos, &tam_per);
  if (!V_per){
    fprintf(stderr, "Erro na leitura dos periodicos\n");
    exit(3);
  }
  for (int i=0; i<tam_per ;i++)
    strtolow(V_per[i].nome);

  //Inicializa vetor de conferencias
  V_conf = leQualitativos(conferencias, &tam_conf);
  if (!V_conf){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiClasse(V_per, tam_per);
    exit(4);
  }
  for (int i=0; i<tam_conf ;i++)
    strtolow(V_conf[i].nome);

  //Inicializa vetor de pesquisadores
  tam_pesq = numberFiles(dirstream);
  V_pesq = malloc( sizeof(curriculo_t) * tam_pesq );
  if ( !V_pesq ){
    fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
    destroiClasse(V_per, tam_per);
    destroiClasse(V_conf, tam_conf);
    exit(5);
  }

  //Sumariza curriculos
  ledados(dirstream, argv[2], V_pesq, tam_pesq);
  for (int i=0; i<tam_pesq ;i++){
    for (int j=0; j<V_pesq[i].tam_eventos ;j++)
      strtolow(V_pesq[i].V_eventos[j].titulo);
    for (int j=0; j<V_pesq[i].tam_artigos ;j++)
      strtolow(V_pesq[i].V_artigos[j].titulo);
  }

  //Atribui os qualitativos de periodicos e conferencias aos pesquisadores
  qualifica(V_pesq, tam_pesq, V_per, tam_per, V_conf, tam_conf);

  //Calcula os dados coletados
  sumarizaDados(V_pesq, tam_pesq);

  //Desaloca vetores
  destroiCurriculos(V_pesq, tam_pesq);
  destroiClasse(V_per, tam_per);
  destroiClasse(V_conf, tam_conf);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
