#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>

#include "utils.h"

struct artigo {
  char      *nomeArtigo;
  int       ano;
};
typedef struct artigo artigo_t;

struct curriculum {
  char      *name;
  artigo_t  *producoes;
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

  while ( entry = readdir(dirstream) )
    if ( entry->d_type == DT_REG ){

      char path[strlen(dir) + strlen(entry->d_name) + 2];
      snprintf(path, strlen(dir) + strlen(entry->d_name) + 1,
          "%s\%s", dir, entry->d_name);

      filestream = fopen(path, "r");
      if (filestream){

        char string[200];
        fscanf(filestream, "%s", &string);
        printf("String %s lida do arquivo %s\n", string, entry->d_name);

      } else
        fprintf(stderr, "Erro em abrir o arquivo %s", entry->d_name);

      fclose(filestream);
    }

}


int main (int argc, char **argv){
  DIR     *dirstream;      //Variavel de stream do diretorio

  //Teste de parametros
  int option;
  while ( (option = getopt(argc, argv, "d:")) != -1){
    switch (option){
      case 'd':
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

  printf("Quantidade de arquivos encontrados: %d\n", nfiles(dirstream));

  ledados(dirstream, argv[2]);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
