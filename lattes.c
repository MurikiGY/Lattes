#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>

#include "utils.h"

//Retorna o numero de caracteres dentro de um arquivo
//int nchar (char *s){
//    FILE *filestream;
//    long int numchar = 0;
//
//    filestream = fopen(s, "r");
//    if (filestream)
//        while (getc(filestream) != EOF)
//            numchar++;
//
//    fclose(filestream);
//    return numchar;
//}


//Retorna o numero de arquivos dentro da stream de um diretorio passado como parametro
int nfiles (DIR *dirstream){
    struct dirent   *entry;         //Estrutura do dirent
    int             tam     = 0;    //Contador do numero de arquivos

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
    FILE *filestream;               //Stream de acesso a arquivo

    while ( entry = readdir(dir) )

        if ( entry->d_type == DT_REG ){

            //Concatenar caminho do arquivo dir/file.xml

            if ( filestream = fopen() ){

            } else
                fprintf(stderr, "Erro em abrir o arquivo: %s\n", entry->d_name);

        }
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

    //Abertura da stream do diretorio
    dirstream = opendir(argv[2]);
    if (!dirstream){
        perror("Não foi possivel acessar o diretório\n");
        exit(2);
    }

    printf("Quantidade de arquivos encontrados: %d\n", nfiles(dirstream));

    //Ate aqui funciona


    ledados(dirstream, dir);


    //Fechamento da stream
    closedir(dirstream);

    return 0;
}
