#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes

/* Protótipos de funções da aplicação */
float integra(float valor, float l, float r);

/* Variáveis globais */
float erro; // Erro máximo

float integra(float valor, float l, float r){
	float m;
	float f1, f2;
	float resp; // resultado da integração no intervalo dado
	float dif; // diferença entre áreas

	m = (r + l)/2; // ponto médio do intervalo

	f1 = funA(l, m);
	printf("oi 1: %f\n", f1);

	f2 = funA(m, r);
	printf("oi 2: %f\n", f2);
	
	dif = fabsf((f1 + f2) - valor);
	printf("dif: %f\n", dif);

	if(dif <= erro){
		resp = valor;
		printf("LOLOLOLOLOL = %f\n", resp);
	}
	else{
		printf("m-l = %f, r-m = %f\n", m-l, r-m);
		resp = integra(f1, l, m) + integra(f2, m, r);
	}

	return resp;
}

int main(int argc, char *argv[]) {
	float l, r; // intervalos de integração;
	float resp; // saída e resultado da integração
	float valor; // valor da primeira aproximação por retangulo da integral da função

	// valida e recebe os valores de entrada
	if(argc < 4) {
		printf("Use: %s <inicio do intervalo de integração> <fim do intervalo de integração> <erro máximo>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	l = atof(argv[1]); // início do intervalo de integração
	r = atof(argv[2]); // fim do intervalo de integração
	erro = atof(argv[3]); // erro máximo

	// if(argc < 5) {
	// 	printf("Use: %s <inicio do intervalo de integração> <fim do intervalo de integração> <erro máximo> <número de threads>\n", argv[0]);
	// 	exit(EXIT_FAILURE);
	// }
	// l = atof(argv[1]); // início do intervalo de integração
	// r = atof(argv[2]); // fim do intervalo de integração
	// erro = atof(argv[3]); // erro máximo
	// nThreads = atoi(argv[4]); // número de threads a serem utilizadas

	// if((nThreads > 8) || (nThreads < 1)){
	// 	printf("O número de threads deve ser de 1 a 8!\n");
	// 	exit(EXIT_FAILURE);
	// }

	valor = funA(l, r); // O valor passado é a base do retângulo
	printf("oi: %f\n", valor);

	resp = integra(valor, l, r);

	printf("%f\n", resp);

	return 0;
}