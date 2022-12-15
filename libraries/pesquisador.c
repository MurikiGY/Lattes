#include "pesquisador.h"


void leDados (DIR *dirstream, char *dir, curriculo_t *V_pesq, int tam_pesq){
  struct dirent   *entry;         //Estrutura do dirent dara o dirstream
  FILE            *filestream;    //Stream de acesso a arquivo
  int             i=0;            //Contador do vetor do pesquisador
  int             j=0;            //Contador do vetor de eventos para cada pesquisador
  int             k=0;            //Contador do vetor de artigos para cada pesquisador

  //Iteração sobre o diretorio
  entry = readdir(dirstream);
  while ( entry ){
    if ( entry->d_type == DT_REG ){

      char path[strlen(dir) + strlen(entry->d_name) + 2];
      snprintf(path, strlen(dir) + strlen(entry->d_name) + 2, "%s/%s", dir, entry->d_name);

      //Leitura dos arquivos
      filestream = fopen(path, "r");
      if (filestream){

        //Le dados do arquivo

        //Inicia pesquisador
        int cnt_evento, cnt_artigo;
        calcArtigoEvento(filestream, &cnt_evento, &cnt_artigo);
        V_pesq[i].tam_eventos = cnt_evento;
        V_pesq[i].tam_artigos = cnt_artigo;
        V_pesq[i].V_eventos = malloc( sizeof(producao_t) * cnt_evento );
        V_pesq[i].V_artigos = malloc( sizeof(producao_t) * cnt_artigo );
        V_pesq[i].coautores = criaListaAutor();

        //Busca o nome do pesquisador
        leNome(filestream, &V_pesq[i]);
        printf("Buscando dados do pesquisador %s\n", V_pesq[i].pesquisador);

        char *strng = malloc( sizeof(char) * STRSIZE );
        while ( fscanf(filestream, "%s", strng) != EOF ){

          //Le evento
          if ( strstr(strng, "<TRABALHO-EM-EVENTOS") )
            leEvento(filestream, &V_pesq[i].V_eventos[j++], V_pesq[i].coautores);

          //Le artigo
          if ( strstr(strng, "<ARTIGO-PUBLICADO") )
            leArtigo(filestream, &V_pesq[i].V_artigos[k++], V_pesq[i].coautores);

        }
        free(strng);
        i++;
        j = 0; k = 0;

        //Le dados do arquivo

        fclose(filestream);
      } else
        fprintf(stderr, "Erro em abrir o arquivo %s\n", entry->d_name);
    } //if ( entry->d_type == DT_REG )
    entry = readdir(dirstream);
  } //while ( entry )
}


//Le o conteudo de uma tag
void completaTag(FILE *stream, char *s){
  char *aux = malloc( sizeof(char) * STRSIZE );

  //Pega string completa com tag e dado
  while (s[strlen(s)-1] != '"' ){
    fscanf(stream, "%s", aux);
    strcat(s, " ");
    strcat(s, aux);  //Concatena titulo completo
  }
  free(aux);

  //Tratamento de string
  s[strlen(s)-1] = '\0'; //Remove aspas do final
  aux = strchr(s, '"'); //Aux aponta para o inicio
  (aux)++;

  int i;
  for (i=0; aux[i]!='\0' ;i++)
    s[i] = aux[i];
  s[i] = '\0';
}


//Calcula a quantidade de artigos e eventos
void calcArtigoEvento(FILE *stream, int *num_evento, int *num_artigo){
  int evento = 0;
  int artigo = 0;
  char *s = malloc( sizeof(char) * STRSIZE );

  while ( fscanf(stream, "%s", s) != EOF )
    if ( strstr(s, "<TRABALHO-EM-EVENTOS") )
      evento++;
    else 
      if ( strstr(s, "<ARTIGO-PUBLICADO") )
        artigo++;

  free(s);
  *num_evento = evento;
  *num_artigo = artigo;
  rewind(stream);
}


//Le nome do pesquisador
void leNome(FILE *stream, curriculo_t *V_pesq){
  char *s = malloc( sizeof(char) * STRSIZE );

  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-COMPLETO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  V_pesq->pesquisador = malloc(sizeof(char) * (strlen(s)+1));
  strcpy(V_pesq->pesquisador, s);

  free(s);
}


//Le dados de um evento
void leEvento(FILE *stream, producao_t *prod, lista_autor_t *autor){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->producao = malloc( sizeof(char) * (strlen(s)+1) );
  strcpy(prod->producao, s);

  //Acha o ano do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-TRABALHO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->ano = atoi(s);

  //Acha o nome do evento
  fscanf(stream, "%s", s);
  while ( !strstr(s, "NOME-DO-EVENTO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->titulo = malloc( sizeof(char) * (strlen(s)+1) );
  strcpy(prod->titulo, s);
  strtolow(prod->titulo);

  //Busca co-autores
  fscanf(stream, "%s", s);
  while ( !strstr(s, "</TRABALHO-") ){
    if ( strstr(s, "NOME-COMPLETO-DO-AUTOR=") ){
      completaTag(stream, s);
      insereListaAutor(autor, s);
    }
    fscanf(stream, "%s", s);
  }
  fseek(stream, -strlen(s), SEEK_CUR);

  free(s);
}


//Le dados de um artigo
void leArtigo(FILE *stream, producao_t *prod, lista_autor_t *autor){
  char *s = malloc( sizeof(char) * STRSIZE );

  //Acha o titulo do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->producao = malloc( sizeof(char) * (strlen(s)+1) );
  strcpy(prod->producao, s);

  //Acha o ano do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "ANO-DO-ARTIGO=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->ano = atoi(s);

  //Acha o periodico do artigo
  fscanf(stream, "%s", s);
  while ( !strstr(s, "TITULO-DO-PERIODICO-OU-REVISTA=") )
    fscanf(stream, "%s", s);
  completaTag(stream, s);
  prod->titulo = malloc( sizeof(char) * (strlen(s)+1) );
  strcpy(prod->titulo, s);
  strtolow(prod->titulo);

  //Busca co-autores
  fscanf(stream, "%s", s);
  while ( !strstr(s, "</ARTIGO") ){
    if ( strstr(s, "NOME-COMPLETO-DO-AUTOR=") ){
      completaTag(stream, s);
      insereListaAutor(autor, s);
    }
    fscanf(stream, "%s", s);
  }
  fseek(stream, -strlen(s), SEEK_CUR);

  free(s);
}

//Desaloca vetor de curriculos
void destroiCurriculos(curriculo_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    //Desaloca vetor de eventos
    for (int j=0; j<vetor[i].tam_eventos ;j++){
      free(vetor[i].V_eventos[j].producao);
      free(vetor[i].V_eventos[j].titulo);
    }
    free(vetor[i].V_eventos);
    //Desaloca vetor de periodicos
    for (int k=0; k<vetor[i].tam_artigos ;k++){
      free(vetor[i].V_artigos[k].producao);
      free(vetor[i].V_artigos[k].titulo);
    }
    free(vetor[i].V_artigos);
    //Desaloca nome
    free(vetor[i].pesquisador);
    vetor[i].coautores = destroiListaAutor(vetor[i].coautores);
  }

  free(vetor);
}


void imprimeCurriculo(curriculo_t *vetor, int tam){

  for (int i=0; i<tam ;i++){
    printf("Imprimindo pesquisador: %s\n", vetor[i].pesquisador);
    printf("\n");
    printf("Eventos:\n");
    for (int j=0; j<vetor[i].tam_eventos ;j++){
      printf("Producao: %s\n", vetor[i].V_eventos[j].producao);
      printf("Evento: %s\n", vetor[i].V_eventos[j].titulo);
      printf("Qualis: %s\n", vetor[i].V_eventos[j].qualis);
      printf("Ano: %d\n", vetor[i].V_eventos[j].ano);
    }
    printf("\n");
    printf("Artigos:\n");
    for (int j=0; j<vetor[i].tam_artigos ;j++){
      printf("producao: %s\n", vetor[i].V_artigos[j].producao);
      printf("Periodico: %s\n", vetor[i].V_artigos[j].titulo);
      printf("Qualis: %s\n", vetor[i].V_artigos[j].qualis);
      printf("Ano: %d\n", vetor[i].V_artigos[j].ano);
    }
  }
}

