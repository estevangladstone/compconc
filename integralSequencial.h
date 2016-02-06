#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
#include "timer.h"

/* Protótipos de funções da aplicação */
double integra(double l, double r);

 /* Variáveis globais */
double erro_maximo = 0.01;  // Erro máximo
double (*fun)(double);  	// Ponteiro da funcao que queremos usar, se eh funA, funB ...

/* Função principal */
void sequencial(int argc, char *argv[]) {
	double l , r; 							// intervalos de integração;
	double resultado; 						// saída e resultado da integração
    double tempo_inicio , tempo_termino , tempo_concorrente_inicio , tempo_concorrente_termino ;
	char escolha_da_funcao; 				// char entre as funções disponíveis

    GET_TIME(tempo_inicio);

  	/* Valida e recebe os valores de entrada */
	if(argc < 5) {
		printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo>\n", argv[0]);
		printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
		exit(EXIT_FAILURE);
	}

	if(strlen(argv[1]) > 1){
		printf("ERRO: a <funcao> deve estar dentre as funcoes disponiveis.\n");
    	printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo>\n", argv[0]);
		printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
		exit(EXIT_FAILURE);
	}

	escolha_da_funcao = argv[1][0];		// pega o primeiro caracter de argv[1]
	l = atof(argv[2]); 					// início do intervalo de integração
	r = atof(argv[3]); 					// fim do intervalo de integração
	erro_maximo = atof(argv[4]); 		// erro máximo

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

    GET_TIME(tempo_concorrente_inicio);

	resultado = integra(l, r);

    GET_TIME(tempo_concorrente_termino);
    GET_TIME(tempo_termino);
	
	printf("Tempo total: %lf\n" , tempo_termino - tempo_inicio);
	printf("Tempo da parte não otimizada: %lf\n" , tempo_concorrente_termino - tempo_concorrente_inicio);
	printf("Resultado da Integração = %lf\n", resultado);

 	return;
}

/* Função que realiza o cálculo da integral recursivamente */
double integra(double l, double r){
	double m; 				// ponto médio
	double A1, A2 , A ;  	// A1 e A2 sao as areas dos intervalos (l,m) e (m,r). A é o retangulao, fun(l,r)
	double resultado;       // resultado da integração no intervalo dado
	double diferenca;    	// diferença entre áreas

 	m = (r + l)/2; // ponto médio do intervalo

	A = calcula_retangulo(fun , l,  r);
    // printf("A = %lf\n" , A);

	A1 = calcula_retangulo(fun , l, m);
	// printf("A1 = %lf\n", A1);

	A2 = calcula_retangulo(fun , m, r);
	// printf("A2 = %lf\n", A2);

	diferenca = fabsf( (A1 + A2) - A); // Obtém o valor absoluto da subtração
	// printf("diferenca = %lf\n", dif);

 	if ( diferenca <= erro_maximo ) {
        // printf("dif deu menor ou igual ao erro, que eh igual a %lf\n" , erro );
		resultado = A; //No pdf diz q nesse caso "(...)considera-se a área do maior retangulo como uma área aceitável(...)"
		// printf("resultado = %lf\n", resultado);
 	} else{
	    // printf("dif deu maior que o erro permitido...q eh de %lf\n" , erro );
		resultado = integra(l, m) + integra(m, r);
 	}

 	return resultado;
}
