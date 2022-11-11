#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "leitura.h"
#include "formata.h"
#include "liblista.h"
#include "libano.h"

#define DIST 3    //Parametro da distancia de edição de strings

void strtolow (char *s){
  for (int i=0; i<strlen(s) ;i++)
    s[i] = tolower(s[i]);
}

//Retorna o numero de arquivos dentro de um diretorio
int nfiles (DIR *dirstream){
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
  //printf("Distancia Inside %d\n", matrix[len1][len2] ) ;
  //exit(0);
  return (matrix[len1][len2]);
}


void qualifica(curriculo_t *V_pesq, int tam_pesq, classe_t *V_per, int tam_per, classe_t *V_conf, int tam_conf){

  //Percorre pesquisadores
  for(int i=0; i<tam_pesq ;i++){

    //Percorre eventos
    for(int j=0; j<V_pesq[i].tam_eventos ;j++)
      //Percorre vetor de conferencias
      for(int l=0; l<tam_conf ;l++){
        //printf("\rAnalisando conferencias de %s", V_pesq[i].pesquisador);
        //fflush(stdout);
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
        //printf("\rAnalisando artigos de %s", V_pesq[i].pesquisador);
        //fflush(stdout);
        V_pesq[i].V_artigos[k].qualis = NULL;
        if ( distance(V_pesq[i].V_artigos[k].titulo, V_per[l].nome) < strlen(V_pesq[i].V_artigos[k].titulo)/DIST ){
          V_pesq[i].V_artigos[k].qualis = V_per[l].tipo;
          break;
        }
      } //For de artigos

  } //Percorre pesquisadores

}


static int comparaTitulo(const void *p1, const void *p2){
  return strcmp( ((producao_t *)p1)->titulo, ((producao_t *)p2)->titulo );
}


static int comparaClasse(const void *p1, const void *p2){
  return strcmp( ((classe_t *)p1)->nome, ((classe_t *)p2)->nome );
}


void ordenaCurriculos(curriculo_t *vetor, int tam){
  for(int i=0; i<tam ;i++){
    qsort(vetor[i].V_eventos, vetor[i].tam_eventos, sizeof(producao_t), comparaTitulo);
    qsort(vetor[i].V_artigos, vetor[i].tam_artigos, sizeof(producao_t), comparaTitulo);
  }
}


void calculaGlobal(curriculo_t *V_pesq, int tam_pesq, int opcao){
  lista_t **V_lista;   //Vetor de listas de estratos

  V_lista = malloc( sizeof(lista_t *)*10 );

  //Inicializa listas
  for (int i=0; i<10 ;i++)
    V_lista[i] = criaLista();

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){
    if ( opcao == 1 ){
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( buscaListaIncrementa(V_lista[ estrato(V_pesq[i].V_eventos[j].qualis) ],
              V_pesq[i].V_eventos[j].titulo) )
          printf("Erro em buscaListaIncrementa\n");
    } else {
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( buscaListaIncrementa(V_lista[ estrato(V_pesq[i].V_artigos[j].qualis) ],
              V_pesq[i].V_artigos[j].titulo) )
          printf("Erro em buscaListaIncrementa\n");
    } //If opcao
  } //For pesquisadores

  //Impressao de dados
  for (int i=0; i<4 ;i++){
    printf("A%d:\n", i+1);
    listaImprime(V_lista[i]);
    printf("\n");}
  for (int i=4; i<8 ;i++){
    printf("B%d:\n", i-3);
    listaImprime(V_lista[i]);
    printf("\n");}
  printf("C:\n");
  listaImprime(V_lista[8]);
  printf("\n");

  //Destroi listas
  for (int i=0; i<10 ;i++)
    V_lista[i] = listaDestroi(V_lista[i]);

  free(V_lista);

}


void calculaPesquisador(curriculo_t *V_pesq, int tam_pesq){
  int per[10] = {};
  int conf[10] = {};
  int count1 = 0;
  int count2 = 0;

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){

    //percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++){
      (conf[ estrato(V_pesq[i].V_eventos[j].qualis) ])++;
      if (V_pesq[i].V_eventos[j].qualis != NULL)
        count1++;
    }

    //percorre periodicos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++){
      (per[ estrato(V_pesq[i].V_artigos[j].qualis) ])++;
      if (V_pesq[i].V_artigos[j].qualis != NULL)
        count2++;
    }
    
    //Impressão
    printf("Pesquisador: %s\n", V_pesq[i].pesquisador);
    printf("+---------------------------+\n");
    printf("| Conferencias | Periodicos |\n");
    printf("+---------------------------+\n");
    for (int j=0; j<4 ;j++)
    printf("| A%d: %-03d      | A%d: %-03d    |\n", j+1, conf[j], j+1, per[j]);
    for (int j=4; j<8 ;j++)
    printf("| B%d: %-03d      | B%d: %-03d    |\n", j-3, conf[j], j-3, per[j]);
    printf("| C : %-03d      | C : %-03d    |\n", conf[8], per[8]);
    printf("+---------------------------+\n");

    printf("Total de conferências: %d\n", count1);
    printf("Total de periodicos: %d\n", count2);
    count1 = 0;
    count2 = 0;

    //Zera vetores
    memset(conf, 0, 10*sizeof(int));
    memset(per, 0, 10*sizeof(int));
    printf("\n");

  } //For de pesquisadores

}


void calculaEstratoCNC(curriculo_t *V_pesq, int tam_pesq, int option){
  lista_t *conf_lista;
  lista_t *per_lista;

  conf_lista = criaLista();
  per_lista = criaLista();

  for (int i=0; i<tam_pesq ;i++){

    if ( option == 0 ){
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( estrato(V_pesq[i].V_eventos[j].qualis) == 8 )
          if ( buscaListaIncrementa(conf_lista, V_pesq[i].V_eventos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( estrato(V_pesq[i].V_artigos[j].qualis) == 8 )
          if ( buscaListaIncrementa(per_lista, V_pesq[i].V_artigos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
    } else {
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( estrato(V_pesq[i].V_eventos[j].qualis) == 9 )
          if ( buscaListaIncrementa(conf_lista, V_pesq[i].V_eventos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( estrato(V_pesq[i].V_artigos[j].qualis) == 9 )
          if ( buscaListaIncrementa(per_lista, V_pesq[i].V_artigos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
    }
  }

  if ( option == 0){
    printf("+----Imprimindo conferencias classificadas em C-----+\n");
    listaImprime(conf_lista);
    printf("Numero de conferencias em C: %d\n", conf_lista->tam);
    printf("\n");
    printf("+-----Imprimindo periodicos classificados em C------+\n");
    listaImprime(per_lista);
    printf("Numero de periodicos em C: %d\n", per_lista->tam);
    printf("\n");
  } else {
    printf("+-----Imprimindo conferencias não encontrados-------+\n");
    listaImprime(conf_lista);
    printf("Numero de conferencias nao encontradas: %d\n", conf_lista->tam);
    printf("\n");
    printf("+-------Imprimindo periodicos não encontrados-------+\n");
    listaImprime(per_lista);
    printf("Numero de periodicos nao encontrados: %d\n", per_lista->tam);
    printf("\n");
  }

  listaDestroi(conf_lista);
  listaDestroi(per_lista);
}


void calculaAno(curriculo_t *V_pesq, int tam_pesq){
  ano_t *anos;

  anos = criaListaAno();

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){

    //Percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++)
      if ( V_pesq[i].V_eventos[j].qualis != NULL )
        if ( insereOrdenadoListaAno(anos, V_pesq[i].V_eventos[j].ano,
              V_pesq[i].V_eventos[j].qualis, 0) )
          printf("Erro em insereOrdenadoListaAno\n");

    //Percorre artigos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++)
      if ( V_pesq[i].V_artigos[j].qualis != NULL )
        if ( insereOrdenadoListaAno(anos, V_pesq[i].V_artigos[j].ano,
              V_pesq[i].V_artigos[j].qualis, 1) )
          printf("Erro em insereOrdenadoListaAno\n");

  } //For de pesquisadores

  imprimeListaAno(anos);
  anos = destroiListaAno(anos);

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
  V_per = leQualificativos(periodicos, &tam_per);
  if (!V_per){
    fprintf(stderr, "Erro na leitura dos periodicos\n");
    exit(3);
  }
  for (int i=0; i<tam_per ;i++)
    strtolow(V_per[i].nome);

  //Inicializa vetor de conferencias
  V_conf = leQualificativos(conferencias, &tam_conf);
  if (!V_conf){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiClasse(V_per, tam_per);
    exit(4);
  }
  for (int i=0; i<tam_conf ;i++)
    strtolow(V_conf[i].nome);

  //Inicializa vetor de pesquisadores
  tam_pesq = nfiles(dirstream);
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

  //Ordena periodicos e eventos
//  ordenaCurriculos(V_pesq, tam_pesq);

  //Atribui os qualitativos de periodicos e conferencias aos pesquisadores
  qualifica(V_pesq, tam_pesq, V_per, tam_per, V_conf, tam_conf);

  printf("\n");
  printf("+--------------------------------------+\n");
  printf("|    Imprimindo Periodicos Globais     |\n");
  printf("+--------------------------------------+\n");
  calculaGlobal(V_pesq, tam_pesq, 0);
  printf("+--------------------------------------+\n");
  printf("|   Imprimindo Conferencias Globais    |\n");
  printf("+--------------------------------------+\n");
  calculaGlobal(V_pesq, tam_pesq, 1);
  printf("+--------------------------------------+\n");
  printf("|            Pesquisadores             |\n");
  printf("+--------------------------------------+\n");
  calculaPesquisador(V_pesq, tam_pesq);
  printf("+--------------------------------------+\n");
  printf("|                Anual                 |\n");
  printf("+--------------------------------------+\n");
  calculaAno(V_pesq, tam_pesq);
  printf("+--------------------------------------+\n");
  printf("|      Periodicos e Conferencias C     |\n");
  printf("+--------------------------------------+\n");
  calculaEstratoCNC(V_pesq, tam_pesq, 0);
  printf("+--------------------------------------+\n");
  printf("|     Periodicos e Conferencias NC     |\n");
  printf("+--------------------------------------+\n");
  calculaEstratoCNC(V_pesq, tam_pesq, 1);

  //Desaloca vetores
  destroiCurriculos(V_pesq, tam_pesq);
  destroiClasse(V_per, tam_per);
  destroiClasse(V_conf, tam_conf);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
