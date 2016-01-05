#ifndef COMPARADOR_C
#define COMPARADOR_C

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
#include "timer.h"
#include "sequencial.c"
#include "concorrente.c"

int main (int argc , char* argv[] ){
    Resultado result_seq , result_conc;
    result_seq = principal_seq(  argc , argv );
    result_conc = principal_conc( argc , argv );
    
    printf("\nValor da integral na aplicacao...\nSequencial: %lf\nConcorrente: %lf\n\n" , result_seq.valorDaIntegral , result_conc.valorDaIntegral);
    printf("Tempo da aplicacao...\nSequencial: %lf\nConcorrente: %lf\n" , result_seq.tempoDaAplicacao , result_conc.tempoDaAplicacao );
    if(result_conc.tempoDaAplicacao > result_seq.tempoDaAplicacao){
        printf("A aplicacao sequencial foi %.1f vezes mais rapida\n\n" , (float)result_conc.tempoDaAplicacao / result_seq.tempoDaAplicacao );
    }else{
        printf("A aplicacao concorrente foi %.1f vezes mais rapida\n\n" , (float)result_seq.tempoDaAplicacao / result_conc.tempoDaAplicacao );
    }

}

#endif //COMPARADOR_C
