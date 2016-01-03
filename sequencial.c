#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
 
/* Protótipos de funções da aplicação */
float integra(float l, float r);
 
 /* Variáveis globais */
float   erro = 0.01;    // Erro máximo
float (*fun)(float);            // Ponteiro da funcao que queremos usar, se eh funA, funB ...
 
float integra(float l, float r){
	float m;
	float A1, A2 , A ;  // A1 e A2 sao as areas dos intervalos (l,m) e (m,r) . A eh o retangulao, fun(l,r);
	float resp;         // resultado da integração no intervalo dado
	float dif;          // diferença entre áreas

	// As linhas de comentario abaixo foram digitadas pela gata do meu irmao
	// kl,,,,,,,,,,,,,,,,,oi
	// ´[fcdvghhhhhhhhhhhhhhhhhhb]
	// iu88888888oki
 
 	m = (r + l)/2; // ponto médio do intervalo
 
	A1 = calculaRet (fun ,  l,  m);
	printf("A1 = %f\n", A1);

	A2 = calculaRet (fun ,  m,  r);
	printf("A2 = %f\n", A2);

	A = calculaRet  (fun ,   l,  r);
    printf("A = %f\n" , A);
 
	dif = fabsf( (A1 + A2) - A);

	printf("dif = %f\n", dif);

 	if(dif <= erro){
        printf("dif deu menor ou igual ao erro, que eh igual a %f\n" , erro );
		resp = A; //No pdf diz q nesse caso "(...)considera-se a área do maior retangulo como uma área aceitável(...)"
		printf("resp = %f\n", resp);
 	}
 	else{
	    printf("dif deu maior que o erro permitido...q eh de %f\n" , erro );
		resp = integra(l, m) + integra(m, r);
 	}
 
 	return resp;
}
 
int main(int argc, char *argv[]) {
	float l, r; // intervalos de integração;
	char escolha_da_funcao; //Eh pra ser 'a' ou 'b' ou 'c'...
 	float resp; // saída e resultado da integração
 
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
 
	resp = integra(l, r);
 
	printf("Resultado da Integração = %f\n", resp);
 
 	return 0;
}
