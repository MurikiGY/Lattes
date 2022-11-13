#include "qualis.h"

//Le strings em um arquivo e as retorna em um vetor
classe_t *leQualitativos(char *filename, int *tam){
  FILE      *filestream;
  classe_t  *vetor;
  int       count = 0;

  filestream = fopen(filename, "r");
  if (!filestream){
    fprintf(stderr, "Falha na abertura da stream %s\n", filename);
    return NULL;
  }

  //Conta quantas linhas tem o arquivo
  char  *str = malloc( sizeof(char) * STRBUFF );
  while ( fgets(str, STRBUFF, filestream) )
    count++;

  *tam = count;

  //Malloc do tamanho do vetor
  vetor = malloc(count * sizeof(classe_t));
  if ( !vetor ){
    fprintf(stderr, "Falha na alocacao do vetor de classes\n");
    return NULL;
  }

  rewind(filestream);

  int i=0;
  while ( fgets(str, STRBUFF, filestream) ){
    str[strcspn(str, "\n")] = '\0';
    char *final = strrchr(str, ' ');
    final[0] = '\0';
    (final)++;        //str eh o conteudo e final eh o tipo
    vetor[i].nome = malloc( sizeof(char) * (strlen(str)+1));
    strncpy(vetor[i].nome, str, strlen(str)+1);
    vetor[i].tipo = malloc( sizeof(char) * (strlen(final)+1));
    strncpy(vetor[i].tipo, final, strlen(final)+1);
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
