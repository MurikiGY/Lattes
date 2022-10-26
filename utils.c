#include <stdio.h>
#include <time.h>
#include <string.h>

#include "utils.h"

// Delay de execucao
void delay(int number_of_seconds){
    //Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

//Completa a leitura dos dados de uma tag
void completaDado(FILE *stream, char *s){

  while (s[strlen(s)-1] != '"' ){

    char aux[STRSIZE];
    fscanf(stream, "%s", &aux);
    strcat(s, " ");
    strcat(s, aux);  //Concatena titulo completo

  }
}

