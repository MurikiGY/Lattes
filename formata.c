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


//Retorna um inteiro de acordo com a string de qualidade
int estrato(char *qualis){

  if ( !strcmp(qualis, "A1") )
    return 0;
  else if ( !strcmp(qualis, "A2") )
    return 1;
  else if ( !strcmp(qualis, "A3") )
    return 2;
  else if ( !strcmp(qualis, "A4") )
    return 3;
  else if ( !strcmp(qualis, "B1") )
    return 4;
  else if ( !strcmp(qualis, "B2") )
    return 5;
  else if ( !strcmp(qualis, "B3") )
    return 6;
  else if ( !strcmp(qualis, "B4") )
    return 7;
  else if ( !strcmp(qualis, "C") )
    return 8;

  return 9;
}

