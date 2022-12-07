#include "sumariza.h"

void calculaGlobal(curriculo_t *V_pesq, int tam_pesq, int option){
  lista_t   **V_lista;   //Vetor de listas de estratos

  V_lista = malloc( sizeof(lista_t *) * 10 );

  //Inicializa listas
  for (int i=0; i<10 ;i++)
    V_lista[i] = criaLista();

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){
    if ( option == 1 ){
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( buscaListaIncrementa(V_lista[ estrato(V_pesq[i].V_eventos[j].qualis) ], V_pesq[i].V_eventos[j].titulo) )
          printf("Erro em buscaListaIncrementa\n");
    } else {
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( buscaListaIncrementa(V_lista[ estrato(V_pesq[i].V_artigos[j].qualis) ], V_pesq[i].V_artigos[j].titulo) )
          printf("Erro em buscaListaIncrementa\n");
    } //If option
  } //For pesquisadores

  //Impressao de dados
  for (int i=0; i<4 ;i++){
    printf("Estrato A%d:\n", i+1);
    listaImprime(V_lista[i]);
    printf("\n");}
  for (int i=4; i<8 ;i++){
    printf("Estrato B%d:\n", i-3);
    listaImprime(V_lista[i]);
    printf("\n");}
  printf("Estrato C:\n");
  listaImprime(V_lista[8]);
  printf("\n");

  //Destroi listas
  for (int i=0; i<10 ;i++)
    V_lista[i] = listaDestroi(V_lista[i]);

  free(V_lista);
}


void calculaPesquisador(curriculo_t *V_pesq, int tam_pesq){
  int per[10] = {};
  int conf[10] = {};
  int count1 = 0;
  int count2 = 0;

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){

    //percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++){
      (conf[ estrato(V_pesq[i].V_eventos[j].qualis) ])++;
        count1++;
      //if (V_pesq[i].V_eventos[j].qualis != NULL)
    }
    //percorre periodicos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++){
      (per[ estrato(V_pesq[i].V_artigos[j].qualis) ])++;
        count2++;
      //if (V_pesq[i].V_artigos[j].qualis != NULL)
    }
    
    //Impressão
    printf("Pesquisador: %s\n", V_pesq[i].pesquisador);
    printf("+---------------------------+\n");
    printf("| Conferencias | Periodicos |\n");
    printf("+---------------------------+\n");
    for (int j=0; j<4 ;j++)
      printf("| A%d: %-3d      | A%d: %-3d    |\n", j+1, conf[j], j+1, per[j]);
    for (int j=4; j<8 ;j++)
      printf("| B%d: %-3d      | B%d: %-3d    |\n", j-3, conf[j], j-3, per[j]);
    printf("| C : %-3d      | C : %-3d    |\n", conf[8], per[8]);
    printf("| NC: %-3d      | NC: %-3d    |\n", conf[9], per[9]);
    printf("+---------------------------+\n");

    printf("Total de conferências: %d\n", count1);
    printf("Total de periodicos: %d\n", count2);
    count1 = 0;
    count2 = 0;

    //Zera vetores
    memset(conf, 0, 10*sizeof(int));
    memset(per, 0, 10*sizeof(int));
    printf("\n");

  } //For de pesquisadores
}


void calculaAno(curriculo_t *V_pesq, int tam_pesq){
  ano_t *anos;

  anos = criaListaAno();

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){

    //Percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++)
      if ( V_pesq[i].V_eventos[j].qualis != NULL )
        if ( insereOrdenadoListaAno(anos, V_pesq[i].V_eventos[j].ano,
              V_pesq[i].V_eventos[j].qualis, 0) )
          printf("Erro em insereOrdenadoListaAno\n");

    //Percorre artigos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++)
      if ( V_pesq[i].V_artigos[j].qualis != NULL )
        if ( insereOrdenadoListaAno(anos, V_pesq[i].V_artigos[j].ano,
              V_pesq[i].V_artigos[j].qualis, 1) )
          printf("Erro em insereOrdenadoListaAno\n");

  } //For de pesquisadores

  imprimeListaAno(anos);
  anos = destroiListaAno(anos);
}


void calculaEstratoCNC(curriculo_t *V_pesq, int tam_pesq, int option){
  lista_t *conf_lista;
  lista_t *per_lista;

  conf_lista = criaLista();
  per_lista = criaLista();

  for (int i=0; i<tam_pesq ;i++){

    if ( option == 0 ){
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( estrato(V_pesq[i].V_eventos[j].qualis) == 8 )
          if ( buscaListaIncrementa(conf_lista, V_pesq[i].V_eventos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( estrato(V_pesq[i].V_artigos[j].qualis) == 8 )
          if ( buscaListaIncrementa(per_lista, V_pesq[i].V_artigos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
    } else {
      //Percorre eventos
      for (int j=0; j<V_pesq[i].tam_eventos ;j++)
        if ( estrato(V_pesq[i].V_eventos[j].qualis) == 9 )
          if ( buscaListaIncrementa(conf_lista, V_pesq[i].V_eventos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
      //Percorre artigos
      for (int j=0; j<V_pesq[i].tam_artigos ;j++)
        if ( estrato(V_pesq[i].V_artigos[j].qualis) == 9 )
          if ( buscaListaIncrementa(per_lista, V_pesq[i].V_artigos[j].titulo) )
            printf("Erro na funcao buscaListaIncrementa\n");
    }
  }

  if ( option == 0){
    printf("+---- Conferencias Classificadas em C-----+\n");
    listaImprime(conf_lista);
    printf("Numero de conferencias em C: %d\n", conf_lista->tam);
    printf("\n");
    printf("+----- Periodicos Classificados em C------+\n");
    listaImprime(per_lista);
    printf("Numero de periodicos em C: %d\n", per_lista->tam);
    printf("\n");
  } else {
    printf("+----- Conferencias não encontradas-------+\n");
    listaImprime(conf_lista);
    printf("Numero de conferencias não encontradas: %d\n", conf_lista->tam);
    printf("\n");
    printf("+------- Periodicos não encontrados-------+\n");
    listaImprime(per_lista);
    printf("Numero de periodicos não encontrados: %d\n", per_lista->tam);
    printf("\n");
  }

  listaDestroi(conf_lista);
  listaDestroi(per_lista);
}

void calculaCoautorias (curriculo_t *V_pesq, int tam_pesq){
  for (int i=0; i<tam_pesq ;i++){
    printf("Coautores de %s:\n", V_pesq[i].pesquisador);
    imprimeListaAutor(V_pesq[i].coautores);
    printf("\n");
  }
}


void plotGrafico (curriculo_t *V_pesq, int tam){
  int conf = 0;
  int per = 0;

  for (int i=0; i<tam ;i++){
    conf = conf + V_pesq[i].tam_eventos;
    per = per + V_pesq[i].tam_artigos;
  }

  printf("%sConferencias  |", BLU);
  for (int i=0; i<conf/3 ;i++)
    printf("#");
  printf(" %d\n", conf);
  printf("%sPeriodicos    |", RED);
  for (int i=0; i<per/3 ;i++)
    printf("#");
  printf(" %d\n", per);
  printf("%s", DFT);
}


//https://stackoverflow.com/questions/327576/how-do-you-plot-bar-charts-in-gnuplot
void plotData(curriculo_t *V_pesq, int tam_pesq, int option){
  int conf[10] = {};
  int per[10] = {};

  char *plotCommands[] = {
    "set title \"Periodicos/Conferencias\"",
    "red = \"#FF0000\"; blue = \"#0000FF\";",
    "set style fill solid",
    "set style data histogram",
    "set boxwidth 0.4",
    "set xtics rotate",
    "set xrange [-1:9]",
    "set grid ytics",
    "plot \"data.dat\" using 2:xtic(1) title \"periodicos\" linecolor rgb red, \"data.dat\" using 3 title \"conferencias\" linecolor rgb blue"
  };

  //Percorre pesquisadores
  for (int i=0; i<tam_pesq ;i++){
    //percorre conferencias
    for (int j=0; j<V_pesq[i].tam_eventos ;j++)
      (conf[ estrato(V_pesq[i].V_eventos[j].qualis) ])++;
    //percorre periodicos
    for (int j=0; j<V_pesq[i].tam_artigos ;j++)
      (per[ estrato(V_pesq[i].V_artigos[j].qualis) ])++;
  }

  FILE *temp = fopen("data.dat", "w");
  FILE *gnuplot = popen("gnuplot -persistent", "w");

  //Escreve os dados
  for (int i=0; i<4 ;i++)
    fprintf(temp, "A%d %d %d\n", i+1, per[i], conf[i]);
  for (int i=4; i<8 ;i++)
    fprintf(temp, "B%d %d %d\n", i-3, per[i], conf[i]);

  //Monta grafico
  for (int i=0; i<9 ;i++)
    fprintf(gnuplot, "%s \n", plotCommands[i]);

  fclose(temp);
  pclose(gnuplot);
}


void sumarizaDados (curriculo_t *V_pesq1, int tam_pesq1, curriculo_t *V_pesq2, int tam_pesq2, int dir_count){

  printf("\n");
  if ( dir_count < 2 ){
    printf("+--------------------------------------+\n");
    printf("|    Imprimindo Periodicos Globais     |\n");
    printf("+--------------------------------------+\n");
    calculaGlobal(V_pesq1, tam_pesq1, 0);
    printf("+--------------------------------------+\n");
    printf("|   Imprimindo Conferencias Globais    |\n");
    printf("+--------------------------------------+\n");
    calculaGlobal(V_pesq1, tam_pesq1, 1);
    printf("+--------------------------------------+\n");
    printf("|            Pesquisadores             |\n");
    printf("+--------------------------------------+\n");
    calculaPesquisador(V_pesq1, tam_pesq1);
    printf("+--------------------------------------+\n");
    printf("|                Anual                 |\n");
    printf("+--------------------------------------+\n");
    calculaAno(V_pesq1, tam_pesq1);
    printf("+--------------------------------------+\n");
    printf("|      Periodicos e Conferencias C     |\n");
    printf("+--------------------------------------+\n");
    calculaEstratoCNC(V_pesq1, tam_pesq1, 0);
    printf("+--------------------------------------+\n");
    printf("|     Periodicos e Conferencias NC     |\n");
    printf("+--------------------------------------+\n");
    calculaEstratoCNC(V_pesq1, tam_pesq1, 1);
    printf("+--------------------------------------+\n");
    printf("|         Lista de coautorias          |\n");
    printf("+--------------------------------------+\n");
    calculaCoautorias(V_pesq1, tam_pesq1);
    plotData(V_pesq1, tam_pesq1, 1);
  } else {
    printf("+--------------------------------------+\n");
    printf("|    Imprimindo Periodicos Globais     |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaGlobal(V_pesq1, tam_pesq1, 0);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaGlobal(V_pesq2, tam_pesq2, 0);
    printf("+--------------------------------------+\n");
    printf("|   Imprimindo Conferencias Globais    |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaGlobal(V_pesq1, tam_pesq1, 1);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaGlobal(V_pesq2, tam_pesq2, 1);
    printf("+--------------------------------------+\n");
    printf("|            Pesquisadores             |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaPesquisador(V_pesq1, tam_pesq1);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaPesquisador(V_pesq2, tam_pesq2);
    printf("+--------------------------------------+\n");
    printf("|                Anual                 |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaAno(V_pesq1, tam_pesq1);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaAno(V_pesq2, tam_pesq2);
    printf("+--------------------------------------+\n");
    printf("|      Periodicos e Conferencias C     |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaEstratoCNC(V_pesq1, tam_pesq1, 0);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaEstratoCNC(V_pesq2, tam_pesq2, 0);
    printf("+--------------------------------------+\n");
    printf("|     Periodicos e Conferencias NC     |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaEstratoCNC(V_pesq1, tam_pesq1, 1);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaEstratoCNC(V_pesq2, tam_pesq2, 1);
    printf("+--------------------------------------+\n");
    printf("|         Lista de coautorias          |\n");
    printf("+--------------------------------------+\n");
    printf("\n<== Imprimindo dados do primeiro diretório ==>\n");
    calculaCoautorias(V_pesq1, tam_pesq1);
    printf("\n<=== Imprimindo dados do segundo diretório ===>\n");
    calculaCoautorias(V_pesq2, tam_pesq2);
    plotData(V_pesq1, tam_pesq1, 1);
    plotData(V_pesq2, tam_pesq2, 2);
  }
}

