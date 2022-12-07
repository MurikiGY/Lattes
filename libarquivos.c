#include "libarquivos.h"

//Retorna o numero de arquivos dentro de um diretorio
int numberFiles (DIR *dirstream){
  struct dirent   *entry;     //Estrutura do dirent
  int             tam = 0;    //Contador do numero de arquivos

  //Loop de contagem de arquivos
  entry = readdir(dirstream);
  while ( entry ){
    if (entry->d_type == DT_REG)
      tam++;
    entry = readdir(dirstream);
  }


  //Retorna o ponteiro para o inicio do diretorio
  rewinddir(dirstream);

  return tam;
}

//Calcula o numero de linhas de um arquivo
int numberLines (FILE *stream, int buffer){
  char *s = malloc( sizeof(char) * buffer );
  int count = 0;

  if ( !s )
    return -1;

  while ( fgets(s, buffer, stream) )
    count++;

  free(s);
  rewind(stream);
  return count;
}

