#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "leitura.h"
#include "formata.h"

//Le o conteudo de uma tag
void pegaDados(FILE *stream, char *s){
  char *aux = malloc( sizeof(char) * STRSIZE );

  //Pega string completa com tag e dado
  while (s[strlen(s)-1] != '"' ){
    fscanf(stream, "%s", aux);
    strcat(s, " ");
    strcat(s, aux);  //Concatena titulo completo
  }
  free(aux);

  //Remove a tag
  s[strlen(s)-1] = '\0'; //Remove aspas do final

  //Aux aponta para o inicio
  aux = strchr(s, '"');
  (aux)++;

  int i;
  for (i=0; aux[i]!='\0' ;i++)
    s[i] = aux[i];
  s[i] = '\0';
}
