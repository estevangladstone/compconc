#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
#include "timer.h"

//Estrutura de dado para o resultado da aplicacao
#ifndef RESULTADO_H
#define RESULTADO_H

typedef struct _Resultado{
    double valorDaIntegral;
    double tempoDaAplicacao;
    //double tempoDaAlocacaoDasThreads;
} Resultado;

#endif

/* Protótipos de funções da aplicação */
double integra(double l, double r);

 /* Variáveis globais */
double erro = 0.01;    // Erro máximo
double (*fun)(double);  // Ponteiro da funcao que queremos usar, se eh funA, funB ...

double integra(double l, double r){
	double m;
	double A1, A2 , A ;  // A1 e A2 sao as areas dos intervalos (l,m) e (m,r) . A eh o retangulao, fun(l,r);
	double resp;         // resultado da integração no intervalo dado
	double dif;          // diferença entre áreas

 	m = (r + l)/2; // ponto médio do intervalo

	A1 = calculaRet (fun ,  l,  m);
	// printf("A1 = %lf\n", A1);

	A2 = calculaRet (fun ,  m,  r);
	// printf("A2 = %lf\n", A2);

	A = calculaRet  (fun ,   l,  r);
    // printf("A = %lf\n" , A);

	dif = fabsf( (A1 + A2) - A);

	// printf("dif = %lf\n", dif);

 	if(dif <= erro){
        // printf("dif deu menor ou igual ao erro, que eh igual a %lf\n" , erro );
		resp = A; //No pdf diz q nesse caso "(...)considera-se a área do maior retangulo como uma área aceitável(...)"
		// printf("resp = %lf\n", resp);
 	}
 	else{
	    // printf("dif deu maior que o erro permitido...q eh de %lf\n" , erro );
		resp = integra(l, m) + integra(m, r);
 	}

 	return resp;
}


Resultado principal_seq( int argc, char *argv[] ) {
	double l, r;                // intervalos de integração;
	char escolha_da_funcao;     //Eh pra ser 'a' ou 'b' ou 'c'...
 	double resp;                // saída e resultado da integração
    double tempo_inicio , tempo_termino ;

  	/* Valida e recebe os valores de entrada */
	if(argc < 5) {
		printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo>\n", argv[0]);
		printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
		exit(EXIT_FAILURE);
	}

	escolha_da_funcao = argv[1][0];
	l = atof(argv[2]); // início do intervalo de integração
	r = atof(argv[3]); // fim do intervalo de integração
	erro = atof(argv[4]); // erro máximo

	// ROMEUOOO. Comenta as linhas acima e descomenta essas aqui pra rodar no seu pc
    // l = 0;
    // r = 2;
    // escolha_da_funcao = 'c';

	switch(escolha_da_funcao){
        case 'a':
            fun = funA;
            break;
        case 'b':
            fun = funB;
            break;
        case 'c':
            fun = funC;
            break;
        case 'd':
            fun = funD;
            break;
        case 'e':
            fun = funE;
            break;
        default:
        	printf("ERRO: a <funcao> deve estar dentre as funcoes disponiveis.\n");
        	printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo>\n", argv[0]);
			printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
			exit(EXIT_FAILURE);
	}

    GET_TIME(tempo_inicio);

	resp = integra(l, r);

    GET_TIME(tempo_termino);

    Resultado result;
    result.valorDaIntegral = resp;
    result.tempoDaAplicacao = tempo_termino - tempo_inicio;

 	return result;
}

#ifndef COMPARADOR_C // Nao queremos outras "mains" se estivermos usando a main do comparador
int main( int argc, char *argv[]  ){
    Resultado result = principal_seq( argc , argv );
    printf("Valor da integral: %lf\nTempo da aplicacao: %lf\n"  result.valorDaIntegral , result.tempoDaAplicacao );
    return 0;
}
#endif
