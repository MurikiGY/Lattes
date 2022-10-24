#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "utils.h"

#define STRSIZE 1000

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


char* ISO8859ToUTF8(unsigned char *str)
{
  char* utf8 = (char*) malloc(1 + (2 * strlen(str)));

  int len = 0;

  char *c = utf8;
  for (; *str; ++str) {
    if (!(*str & 0x80)) {
      *c++ = *str;
      len++;
    } else {
      *c++ = (char) (0xc2 | ((unsigned char)(*str) >> 6));

      *c++ = (char) (0xbf & *str);
      len += 2;
    }
  }
  *c++ = '\0';


  return utf8;
}


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

        //Le dados do arquivo

        char strng[STRSIZE];
        while ( fscanf(filestream, "%s", &strng) != EOF ){

          //Pegar o titulo do artigo

          //Pegar o ano do artigo

          //Acha o titulo do periodico do artigo
          if ( strstr(strng, "TITULO-DO-PERIODICO-OU-REVISTA=") ){

            //Pega o resto do conteudo do titulo
            while ( strng[strlen(strng)-1] != '"' ){

              char aux[STRSIZE];
              fscanf(filestream, "%s", &aux);
              strcat(strng, " ");
              strcat(strng, aux);  //Justa string

            }

            char *aux2 = ISO8859ToUTF8(strng);
            printf("%s\n", aux2 );
            free(aux2);

          }

          //Pegar o nome dos autores





        }


        //Le dados do arquivo

      } else
        fprintf(stderr, "Erro em abrir o arquivo %s", entry->d_name);

      fclose(filestream);
    }

}


int main (int argc, char **argv){
  DIR     *dirstream;      //Variavel de stream do diretorio
  char    *locale;

  locale = setlocale(LC_ALL, "");

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
