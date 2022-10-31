#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <locale.h>

#include "utils.h"

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

  //Iteração sobre o diretorio
  while ( entry = readdir(dirstream) )
    if ( entry->d_type == DT_REG ){

      char path[strlen(dir) + strlen(entry->d_name) + 2];
      snprintf(path, strlen(dir) + strlen(entry->d_name) + 1,
               "%s\%s", dir, entry->d_name);

      printf("Lendo arquivo %s\n", entry->d_name);

      //Leitura dos arquivos
      filestream = fopen(path, "r");
      if (filestream){

        //Le dados do arquivo


        char strng[STRSIZE];
        while ( fscanf(filestream, "%s", &strng) != EOF ){

          //Chegou em um artigo, salva o titulo
          if ( strstr(strng, "TITULO-DO-ARTIGO=") ){
            char *aux2;

            //Pega o resto do conteudo do titulo
            completaDado(filestream, strng);
            aux2 = ISO8859ToUTF8(strng);
            printf("%s\n", aux2 );
            free(aux2);

            //Pegar o ano do artigo
            fscanf(filestream, "%s", &strng);
            while ( !strstr(strng, "ANO-DO-ARTIGO=") )
              fscanf(filestream, "%s", &strng);
            printf("%s\n", strng);
  
            //Acha o titulo do periodico do artigo
            fscanf(filestream, "%s", &strng);
            while ( !strstr(strng, "TITULO-DO-PERIODICO-OU-REVISTA=") )
              fscanf(filestream, "%s", &strng);
 
            //Monta string de periodico
            completaDado(filestream, strng);
            aux2 = ISO8859ToUTF8(strng);
            printf("%s\n", aux2 );
            free(aux2);

            //Pegar o nome dos autores
            fscanf(filestream, "%s", &strng);
            while ( !strstr(strng, "/></") ){

              //Busca tag de nome do autor
              if ( strstr(strng, "NOME-COMPLETO-DO-AUTOR=") ){

                //Monta o nome do autor
                completaDado(filestream, strng);
                aux2 = ISO8859ToUTF8(strng);
                printf("%s\n", aux2 );
                free(aux2);

              }

              fscanf(filestream, "%s", &strng);
            }

            printf("\n");
          }

        }


        //Le dados do arquivo

      } else
        fprintf(stderr, "Erro em abrir o arquivo %s", entry->d_name);

      fclose(filestream);
    }

}


int main (int argc, char **argv){
  DIR       *dirstream;               //Variavel de stream do diretorio
  char      *locale;                  //Configurar em UTF-8
  char      periodicos[FILENAME];     //Nome do arquivo de periodicos
  char      conferencias[FILENAME];   //Nome do arquivo de conferencias
  classe_t  *V_periodicos;            //Vetor de classes periodicos
  classe_t  *V_conferencias;          //Vetor de classes conferencias
  int       tam_periodicos;           //Tamanho do vetor de periodicos
  int       tam_conferencias;         //Tamanho do vetor de conferencias


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

  printf("Numero de curriculos: %d\n", nfiles(dirstream));


  //Sumariza curriculos
  ledados(dirstream, argv[2]);

  //Desaloca vetor de strings
  destroiVetor(V_periodicos, tam_periodicos);
  destroiVetor(V_conferencias, tam_conferencias);

  //Fechamento da stream
  closedir(dirstream);

  return 0;
}
