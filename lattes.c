#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "leitura.h"
#include "formata.h"

struct artigo {
  char      *nomeArtigo;
  int       ano;
};
typedef struct artigo artigo_t;

struct curriculum {
  char      *name;        //Nome da pessoa do curriculo
  artigo_t  *producoes;   //Lista ligada de produções realizadas
};
typedef struct curriculum curriculum_t;


//Retorna o numero de arquivos dentro da stream de um diretorio passado como parametro
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

void ledados (DIR *dirstream, char *dir){
  struct dirent   *entry;         //Estrutura do dirent dara o dirstream
  FILE            *filestream;    //Stream de acesso a arquivo

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

        char *strng = malloc( sizeof(char) * STRSIZE );
        
        //Busca o nome do pesquisador
        fscanf(filestream, "%s", strng);
        while ( !strstr(strng, "NOME-COMPLETO=") )
          fscanf(filestream, "%s", strng);
        pegaDados(filestream, strng);
        printf("Nome do pesquisador: %s\n", strng );

        while ( fscanf(filestream, "%s", strng) != EOF ){

          //Le evento
          if ( strstr(strng, "<TRABALHO-EM-EVENTOS") )
            leEvento(filestream);

          //Le artigo
          if ( strstr(strng, "<ARTIGO-PUBLICADO") )
            leArtigo(filestream);

        }
        free(strng);

        //Le dados do arquivo

        fclose(filestream);

      } else
        fprintf(stderr, "Erro em abrir o arquivo %s", entry->d_name);

    }
}


int main (int argc, char **argv){
  DIR         *dirstream;               //Variavel de stream do diretorio
  char        *locale;                  //Configurar em UTF-8
  char        periodicos[FILENAME];     //Nome do arquivo de periodicos
  char        conferencias[FILENAME];   //Nome do arquivo de conferencias
  classe_t    *V_periodicos;            //Vetor de classes periodicos
  classe_t    *V_conferencias;          //Vetor de classes conferencias
  int         tam_periodicos;           //Tamanho do vetor de periodicos
  int         tam_conferencias;         //Tamanho do vetor de conferencias
//  curriculo_t *V_pesquisador;             //Vetor de pesquisadores
//  int         tam_pesquisador;          //Tamanho do vetor de pesquisadores


  locale = setlocale(LC_ALL, "");

  //Teste de parametros
  int option;
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
  V_periodicos = leStringsArquivo(periodicos, &tam_periodicos);
  if (!V_periodicos){
    fprintf(stderr, "Erro na leitura dos periodicos\n");
    exit(3);
  }

  //Inicializa vetor de conferencias
  V_conferencias = leStringsArquivo(conferencias, &tam_conferencias);
  if (!V_conferencias){
    fprintf(stderr, "Erro na leitura das conferencias\n");
    destroiVetor(V_periodicos, tam_periodicos);
    exit(4);
  }

//  tam_pesquisador = nfiles(dirstream);
//  V_pesquisador = malloc( sizeof(curriculo_t) * tam_pesquisador );
//  if ( !V_pesquisador ){
//    fprintf(stderr, "Falha de alocacao no vetor de pesquisadores\n");
//    destroiVetor(V_periodicos, tam_periodicos);
//    destroiVetor(V_conferencias, tam_conferencias);
//    exit(5);
//  }

  //Sumariza curriculos
  ledados(dirstream, argv[2]);



  //Desaloca vetor de strings
  destroiVetor(V_periodicos, tam_periodicos);
  destroiVetor(V_conferencias, tam_conferencias);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
