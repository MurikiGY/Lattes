#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>

#include "utils.h"

//Retorna o numero de arquivos dentro da stream de um diretorio passado como parametro
int nfiles (DIR *dirstream){
    struct dirent   *entry;     //Estrutura do dirent
    int             tam = 0;    //Contador do numero de arquivos

    //Loop de leitura de arquivos
    while ( entry = readdir(dirstream) )
        if (entry->d_type == DT_REG){
            printf("\rLendo Arquivos %s", entry->d_name);
            fflush(stdout);
            delay(3);
            tam++;
        }
    printf("\n");

    //Retorna o ponteiro para o inicio do diretorio
    rewinddir(dirstream);

    return tam;
}


int main (int argc, char **argv){
    DIR     *dirstream;                 //Variavel de stream do diretorio

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

    //Abertura do diretorio
    dirstream = opendir(argv[2]);
    if (!dirstream){
        perror("Não foi possivel acessar o diretório\n");
        exit(2);
    }

    printf("Foram encontrados %d arquivos lattes\n", nfiles(dirstream));


    return 0;
}
