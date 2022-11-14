#ifndef _LIBARQUIVOS_
#define _LIBARQUIVOS_

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

//Retorna o numero de arquivos dentro de um diretorio
int numberFiles (DIR *dirstream);

//Calcula o numero de linhas de um arquivo
int numberLines(FILE *stream, int buffer);

#endif
