#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "leitura.h"
#include "formata.h"
#include "liblista.h"

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
int distance (char* word1, int len1, char* word2, int len2)
{

  int matrix[len1 + 1][len2 + 1];
  int i;

  for (i = 0; i <= len1; i++)
    matrix[i][0] = i;

  for (i = 0; i <= len2; i++)
    matrix[0][i] = i;

  for (i = 1; i <= len1; i++)
  {
    int j;
    char c1;

    c1 = word1[i-1];
    for (j = 1; j <= len2; j++)
    {
      char c2;

      c2 = word2[j-1];
      if (c1 == c2)
        matrix[i][j] = matrix[i-1][j-1];

      else
      {
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
    for(int j=0; j<V_pesq[i].tam_eventos ;j++){

      //Percorre vetor de conferencias
      for(int l=0; l<tam_conf ;l++){
        //printf("\rBuscando por conferencias");

        V_pesq[i].V_eventos[j].qualis = NULL;
        if ( distance(V_pesq[i].V_eventos[j].titulo, strlen(V_pesq[i].V_eventos[j].titulo), V_conf[l].nome, strlen(V_conf[l].nome)) < 10/*strlen(V_pesq[i].V_eventos[j].titulo)*/ ){
          V_pesq[i].V_eventos[j].qualis = malloc( sizeof(char) * (strlen(V_conf[l].tipo)+2) );
          //strcpy(V_pesq[i].V_eventos[j].qualis, V_conf[l].tipo);
          strncpy(V_pesq[i].V_eventos[j].qualis, V_conf[l].tipo, strlen(V_conf[l].tipo)+1 );
          break;
        }

      } //For de conferencias

      if ( !V_pesq[i].V_eventos[j].qualis ){
        V_pesq[i].V_eventos[j].qualis = malloc( sizeof(char) * (strlen("NC")+1) );
        //strcpy(V_pesq[i].V_eventos[j].qualis, "NC" );
        strncpy(V_pesq[i].V_eventos[j].qualis, "NC", strlen("NC")+1 );
      }

    } //Percorre eventos

    printf("\n");

    //Percorre artigos
    for(int k=0; k<V_pesq[i].tam_artigos ;k++){

      //Percorre vetor de artigos
      for(int l=0; l<tam_per ;l++){
        //printf("\rBuscando por Artigos");

        V_pesq[i].V_artigos[k].qualis = NULL;
        if ( distance(V_pesq[i].V_artigos[k].titulo, strlen(V_pesq[i].V_artigos[k].titulo), V_per[l].nome, strlen(V_per[l].nome)) < 10/*strlen(V_pesq[i].V_artigos[k].titulo)*/ ){
          V_pesq[i].V_artigos[k].qualis = malloc( sizeof(char) * (strlen(V_per[l].tipo)+2) );
          //strcpy(V_pesq[i].V_artigos[k].qualis, V_per[l].tipo);
          strncpy(V_pesq[i].V_artigos[k].qualis, V_per[l].tipo, strlen(V_per[l].tipo)+1 );
          break;
        }

      } //For de artigos

      if ( !V_pesq[i].V_artigos[k].qualis ){
        V_pesq[i].V_artigos[k].qualis = malloc( sizeof(char) * (strlen("NC")+1) );
        //strcpy(V_pesq[i].V_artigos[k].qualis, "NC" );
        strncpy(V_pesq[i].V_artigos[k].qualis, "NC", strlen("NC")+1 );
      }

    } //Percorre artigos

    printf("\n");

  } //Percorre pesquisadores

}


static int comparaAno(const void *p1, const void *p2){
  return ((producao_t *)p1)->ano - ((producao_t *)p2)->ano;
}


void ordenaDados(curriculo_t *vetor, int tam){
  for(int i=0; i<tam ;i++){
    qsort(vetor[i].V_eventos, vetor[i].tam_eventos, sizeof(producao_t), comparaAno);
    qsort(vetor[i].V_artigos, vetor[i].tam_artigos, sizeof(producao_t), comparaAno);
  }
}


int menu (){
  int numero;
  int bol = 0;

  printf("+-----------------------------------------------------------+\n");
  printf("| Opções de entrada:                                        |\n");
  printf("| 1 - Periodicos globais                                    |\n");
  printf("| 2 - Conferencias globais                                  |\n");
  printf("| 3 - Pesquisadores                                         |\n");
  printf("| 4 - Anual                                                 |\n");
  printf("| 5 - Classificações em C                                   |\n");
  printf("| 6 - Não classificados                                     |\n");
  printf("| 7 - Sair                                                  |\n");
  printf("+-----------------------------------------------------------+\n");
  printf("Entrada: ");

  while ( !bol ){
    scanf("%d", &numero);
    if ( numero > 0 && numero < 8)
      bol = 1;
    else{
      printf("Operação inválida, tente novamente\n");
      printf("Entrada: ");
    }
  }

  printf("\e[H\e[2J"); 

  return numero;
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
      for (int j=0; j<V_pesq[i].tam_eventos ;j++){
        if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A1") ){
          if ( buscaListaIncrementa(V_lista[0], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A2") ){
          if ( buscaListaIncrementa(V_lista[1], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A3") ){
          if ( buscaListaIncrementa(V_lista[2], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A4") ){
          if ( buscaListaIncrementa(V_lista[3], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B1") ){
          if ( buscaListaIncrementa(V_lista[4], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B2") ){
          if ( buscaListaIncrementa(V_lista[5], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B3") ){
          if ( buscaListaIncrementa(V_lista[6], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B4") ){
          if ( buscaListaIncrementa(V_lista[7], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "C" ) ){
          if ( buscaListaIncrementa(V_lista[8], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "NC") ){
          if ( buscaListaIncrementa(V_lista[9], V_pesq[i].V_eventos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        }

      } //For percorre eventos
 
    } else {
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++){

        if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A1") ){
          if ( buscaListaIncrementa(V_lista[0], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A2") ){
          if ( buscaListaIncrementa(V_lista[1], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A3") ){
          if ( buscaListaIncrementa(V_lista[2], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A4") ){
          if ( buscaListaIncrementa(V_lista[3], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B1") ){
          if ( buscaListaIncrementa(V_lista[4], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B2") ){
          if ( buscaListaIncrementa(V_lista[5], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B3") ){
          if ( buscaListaIncrementa(V_lista[6], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B4") ){
          if ( buscaListaIncrementa(V_lista[7], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "C" ) ){
          if ( buscaListaIncrementa(V_lista[8], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        } else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "NC") ){
          if ( buscaListaIncrementa(V_lista[9], V_pesq[i].V_artigos[j].titulo) )
              printf("Erro em buscaListaIncrementa\n");
        }

      } //For de artigos

    } //If opcao
  } //For pesquisadores

  //Impressao de dados
  for (int i=0; i<4 ;i++){
    printf("A%d:\n", i+1);
    listaImprime(V_lista[i]);}
  for (int i=4; i<8 ;i++){
    printf("B%d:\n", i-3);
    listaImprime(V_lista[i]);}
  printf("C:\n");
  listaImprime(V_lista[8]);
  printf("NC:\n");
  listaImprime(V_lista[9]);

  //Destroi listas
  for (int i=0; i<10 ;i++)
    V_lista[i] = listaDestroi(V_lista[i]);

    free(V_lista);

}


void calculaPesquisador(curriculo_t *V_pesq, int tam_pesq){
  int per[10] = {};
  int conf[10] = {};

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){
    
    //percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++){
      if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A1") )
        conf[0]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A2") )
        conf[1]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A3") )
        conf[2]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "A4") )
        conf[3]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B1") )
        conf[4]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B2") )
        conf[5]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B3") )
        conf[6]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "B4") )
        conf[7]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "C") )
        conf[8]++;
      else if ( !strcmp(V_pesq[i].V_eventos[j].qualis, "NC") )
        conf[9]++;
    } //For de conferencias

    //percorre periodicos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++){
      if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A1") )
        per[0]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A2") )
        per[1]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A3") )
        per[2]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "A4") )
        per[3]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B1") )
        per[4]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B2") )
        per[5]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B3") )
        per[6]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "B4") )
        per[7]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "C") )
        per[8]++;
      else if ( !strcmp(V_pesq[i].V_artigos[j].qualis, "NC") )
        per[9]++;
    } //For de periodicos

    //Impressão
    printf("Pesquisador: %s\n", V_pesq[i].pesquisador);
    printf("+---------------------------+\n");
    printf("| Conferencias | Periodicos |\n");
    printf("+---------------------------+\n");
    for (int j=0; j<4 ;j++)
      printf("| A%d: %03d      | A%d: %03d    |\n", j+1, conf[j], j+1, per[j]);
    for (int j=4; j<8 ;j++)
      printf("| B%d: %03d      | B%d: %03d    |\n", j-3, conf[j], j-3, per[j]);
    printf("| C : %03d      | C : %03d    |\n", conf[8], per[8]);
    printf("| NC: %03d      | NC: %03d    |\n", conf[9], per[9]);
    printf("+---------------------------+\n");

    //Zera vetores
    memset(conf, 0, 10*sizeof(int));
    memset(per, 0, 10*sizeof(int));

    printf("\n");

  } //For de pesquisadores

}


int main (int argc, char **argv){
  DIR         *dirstream;               //Variavel de stream do diretorio
  char        *locale;                  //Configurar em UTF-8
  char        periodicos[FILENAME];     //Nome do arquivo de periodicos
  char        conferencias[FILENAME];   //Nome do arquivo de conferencias
  classe_t    *V_per;                   //Vetor de classes periodicos
  classe_t    *V_conf;                  //Vetor de classes conferencias
  int         tam_per;                  //Tamanho do vetor de periodicos
  int         tam_conf;                 //Tamanho do vetor de conferencias
  curriculo_t *V_pesq;                  //Vetor de pesquisadores
  int         tam_pesq;                 //Tamanho do vetor de pesquisadores
  int option;


  locale = setlocale(LC_ALL, "");

  //Teste de parametros
  while ( (option = getopt(argc, argv, "d:p:c:")) != -1){

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

  //Inicializa vetor de conferencias
  V_conf = leQualificativos(conferencias, &tam_conf);
  if (!V_conf){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiClasse(V_per, tam_per);
    exit(4);
  }

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

  //Ordena periodicos e eventos por ano
  ordenaDados(V_pesq, tam_pesq);

  //Atribui os qualitativos de periodicos e conferencias aos pesquisadores
  qualifica(V_pesq, tam_pesq, V_per, tam_per, V_conf, tam_conf);

  printf("\e[H\e[2J"); 
  option = menu();

  while ( option != 7 ){
    if ( option == 1 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando periodicos globais                     |\n");
      printf("+---------------------------------------------------+\n");
      calculaGlobal(V_pesq, tam_pesq, 0);
    } else if ( option == 2 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando conferencias globais                   |\n");
      printf("+---------------------------------------------------+\n");
      calculaGlobal(V_pesq, tam_pesq, 1);
    } else if ( option == 3 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando estratos dos pesquisadores             |\n");
      printf("+---------------------------------------------------+\n");
      calculaPesquisador(V_pesq, tam_pesq);
    } else if ( option == 4 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando estratos por ano                       |\n");
      printf("+---------------------------------------------------+\n");
      calculaGlobal(V_pesq, tam_pesq, 1);
    } else if ( option == 5 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando estratos de nivel C                    |\n");
      printf("+---------------------------------------------------+\n");
      calculaGlobal(V_pesq, tam_pesq, 1);
    } else if ( option == 6 ){
      printf("+---------------------------------------------------+\n");
      printf("| Calculando estratos não encontrados               |\n");
      printf("+---------------------------------------------------+\n");
      calculaGlobal(V_pesq, tam_pesq, 1);
    }
    option = menu();
  }

    
  //Desaloca vetores
  destroiCurriculos(V_pesq, tam_pesq);
  destroiClasse(V_per, tam_per);
  destroiClasse(V_conf, tam_conf);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
