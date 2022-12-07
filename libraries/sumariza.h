#ifndef _SUMARIZA_
#define _SUMARIZA_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qualis.h"
#include "pesquisador.h"
#include "liblistaqualis.h"
#include "liblistaano.h"

#define RED  "\x1B[31m"
#define BLU  "\x1B[34m"
#define DFT  "\x1B[0m"


void calculaGlobal(curriculo_t *V_pesq, int tam_pesq, int opcao);

void calculaPesquisador(curriculo_t *V_pesq, int tam_pesq);

void calculaAno(curriculo_t *V_pesq, int tam_pesq);

void calculaEstratoCNC(curriculo_t *V_pesq, int tam_pesq, int option);

void sumarizaDados (curriculo_t *V_pesq1, int tam_pesq1, curriculo_t *V_pesq2, int tam_pesq2, int dir_count);

#endif
