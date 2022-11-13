#ifndef _SUMARIZA_
#define _SUMARIZA_

#include "leitura.h"
#include "formata.h"
#include "liblista.h"
#include "libano.h"


void calculaGlobal(curriculo_t *V_pesq, int tam_pesq, int opcao);

void calculaPesquisador(curriculo_t *V_pesq, int tam_pesq);

void calculaAno(curriculo_t *V_pesq, int tam_pesq);

void calculaEstratoCNC(curriculo_t *V_pesq, int tam_pesq, int option);

void sumarizaDados (curriculo_t *V_pesq, int tam_pesq);

#endif
