#include "qualis.h"


//Retorna um inteiro de acordo com a string de qualidade
int estrato(char *qualis){

  if ( !qualis )
    return 9;
  else if ( !strcmp(qualis, "A1") )
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
  else return -1;
}


//Le strings em um arquivo e as retorna em um vetor
classe_t *leQualitativos(char *filename, int *tam){
  FILE      *filestream;
  classe_t  *vetor;

  filestream = fopen(filename, "r");
  if (!filestream){
    fprintf(stderr, "Falha na abertura da stream %s\n", filename);
    return NULL;
  }

  //Malloc do tamanho do vetor
  *tam = numberLines (filestream, STRBUFF);
  vetor = malloc( sizeof(classe_t) * (*tam) );
  if ( !vetor ){
    fprintf(stderr, "Falha na alocacao do vetor de classes\n");
    return NULL;
  }

  char *str = malloc( sizeof(char) * STRBUFF);
  int i=0;
  while ( fgets(str, STRBUFF, filestream) ){
    str[strcspn(str, "\n")] = '\0';
    char *final = strrchr(str, ' ');
    final[0] = '\0';
    (final)++;        //str eh o conteudo e final eh o tipo
    vetor[i].nome = malloc( sizeof(char) * (strlen(str)+1));
    strcpy(vetor[i].nome, str);
    strtolow(vetor[i].nome);
    vetor[i].tipo = malloc( sizeof(char) * (strlen(final)+1));
    strcpy(vetor[i].tipo, final);
    i++;
  }

  free(str);
  fclose(filestream);
  return vetor;
}


//Desaloca vetor de classes
void destroiClasse(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    free(vetor[i].nome);
    free(vetor[i].tipo);
  }
  free(vetor);
}


void imprimeClasse(classe_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    printf("%s\n", vetor[i].nome);
    printf("%s\n", vetor[i].tipo);
  }
}
