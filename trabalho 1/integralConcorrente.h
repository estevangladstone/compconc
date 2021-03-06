#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
#include "timer.h"

/* Protótipos de funções da aplicação */
void integra(double l, double m);
void *thread();

/* Estruturas de dados utilizadas */
// --FAZER FILA
typedef struct _tarefa{
	double l , r;
	struct _tarefa *prox;
} tarefa;

/* Variáveis globais */
pthread_mutex_t mutex;
pthread_cond_t cond;
tarefa* prox_tarefa;//
tarefa* ultima_tarefa;
double erro; 		// Erro máximo
double resp; 		// saída e resultado da integração
int total_threads, threads_ociosas;
double (*fun)(double);  // Ponteiro da funcao que queremos usar, se eh funA, funB ...

void integra(double l, double r){
	tarefa* nova_tarefa;
	double m;			// ponto médio do intervalo
	double A1, A2 , A;  	// A1 e A2 sao as areas dos intervalos (l,m) e (m,r) . A eh o retangulao, fun(l,r);
	double dif;          // diferença entre áreas

 	m = (r + l)/2; // ponto médio do intervalo

	A1 = calcula_retangulo(fun ,  l,  m);
	// printf("A1 = %lf\n", A1);

	A2 = calcula_retangulo(fun ,  m,  r);
	// printf("A2 = %lf\n", A2);

	A = calcula_retangulo(fun ,   l,  r);
    // printf("A = %lf\n" , A);

	dif = fabsf((A1 + A2) - A);

	// printf("dif = %lf\n", dif);

 	if(dif <= erro){
		// printf("Somei %lf ao resp!\n", A);
		resp += A;
		return ;
 	}
 	else{

		// printf("dif > %lf\n" , erro );


		pthread_mutex_lock(&mutex);
		if(threads_ociosas > 0){

			//INSERIR_TAREFA(m , r);


			// poe o ponteiro na fila para outras threads pegarem
			nova_tarefa = (tarefa*)malloc(sizeof(tarefa));
			nova_tarefa->l = m;
			nova_tarefa->r = r;
			nova_tarefa->prox = NULL;
			// if(nova_tarefa == NULL)printf("what!?\n");

			if(ultima_tarefa!=NULL){
				// printf("aqui!!!!\n");
				ultima_tarefa->prox = nova_tarefa;
				ultima_tarefa = nova_tarefa;
			}else{ //Caso em que a fila ficou vazia, nao acesse o null
				prox_tarefa = nova_tarefa;
				ultima_tarefa = nova_tarefa;
			}

			// if(prox_tarefa == NULL)printf("eita\n");

			// printf("inseri , prox tarefa tem l=%lf , r=%lf \n" , prox_tarefa->l , prox_tarefa->r );
			// printf("aqui3!!\n");

			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);


		}
		else{
			// printf("recorri\n");
			pthread_mutex_unlock(&mutex);
			integra(m,r);
		}

		integra(l, m);

 	}

}

void *thread(){
	tarefa* old_tarefa;
	double l, r;
	pthread_mutex_lock(&mutex);
	// printf("vou tentar entrar. %d \n" , threads_ociosas );
	while( prox_tarefa!=NULL || threads_ociosas < total_threads ){
		// printf("entrei\n");

		//Checa se existem tarefas
		if(prox_tarefa==NULL){
			pthread_cond_wait(&cond, &mutex);
			if(prox_tarefa==NULL){	//SE O COND DEIXOU ELE ANDAR E AINDA ASSIM prox_tarefa
									//EH NULL, eh pra dar exit;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}
		}

		threads_ociosas--;

		l = prox_tarefa->l;
		r = prox_tarefa->r;
		old_tarefa = prox_tarefa;
		prox_tarefa = prox_tarefa->prox;
		if(old_tarefa == ultima_tarefa){
			ultima_tarefa = NULL;
		}
		free(old_tarefa);
		old_tarefa = NULL;

		pthread_mutex_unlock(&mutex);
		//------------------------------------
		integra(l, r);
		//------------------------------------
		pthread_mutex_lock(&mutex);
		threads_ociosas++;
	}
	pthread_mutex_unlock(&mutex); // pro caso do lock ainda estar sendo usado aqui
	pthread_cond_broadcast(&cond); // Libera todas as threads para finalizar a aplicação
	// printf("vou dar exit aqui, namoral\n");
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  	pthread_t *threads;
	int i;
	double l, r; // intervalos de integração;
	char escolha_da_funcao; //Eh pra ser 'a' ou 'b' ou 'c'...
	double tempo_inicio , tempo_termino, tempo_concorrente_inicio , tempo_concorrente_termino ;

	GET_TIME(tempo_inicio);
	
  	/* Valida e recebe os valores de entrada */
	if(argc < 6) {
		printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo> <número de threads>\n", argv[0]);
		printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
		exit(EXIT_FAILURE);
	}

	escolha_da_funcao = argv[1][0];
	l = atof(argv[2]); // início do intervalo de integração
	r = atof(argv[3]); // fim do intervalo de integração
	erro = atof(argv[4]); // erro máximo
	total_threads = atoi(argv[5]);

	if((total_threads > 8) || (total_threads < 1)){
		printf("O número de threads deve ser de 1 a 8!\n");
		exit(EXIT_FAILURE);
	}

	// ROMEUOOO. Comenta as linhas acima e descomenta essas aqui pra rodar no seu pc
    // l = 0;
    // r = 2;
    // escolha_da_funcao = 'c';
    // erro = 0.01;
    // total_threads = 3;

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
        	printf("Use: %s <funcao> <inicio do intervalo> <fim do intervalo> <erro máximo> <número de threads>\n", argv[0]);
			printf("Funções disponíveis: \n\t a : f(x) = 1 + x\n\t b : f(x) = (1 - x^2)^(1/2) , -1 < x < 1\n\t c : f(x) = (1 + x^4)^(1/2)\n\t d : f(x) = sen(x^2)\n\t e : f(x) = cos(e^(-x)) * (0.005 * x^3 + 1)\n");
			exit(EXIT_FAILURE);
	}

	GET_TIME(tempo_concorrente_inicio);

	prox_tarefa = (tarefa*)malloc(sizeof(tarefa));
	prox_tarefa->l = l;
	prox_tarefa->r = r;
	prox_tarefa->prox = NULL;
	ultima_tarefa = prox_tarefa;

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	/* Inicializa o número de threads ociosas para uso posterior */
	threads_ociosas = total_threads;

	/* Aloca os identificadores da thread no sistema e cria as threads */
	threads = (pthread_t *)malloc(total_threads * sizeof(pthread_t));
	for(i = 0; i < total_threads; i++) {
		if(pthread_create(&threads[i], NULL, thread, NULL)) {
			// printf("ERRO: pthread_create()\n");
			exit(EXIT_FAILURE);
		}
	}

	/* Espera todas as threads completarem */
	for(i = 0; i < total_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Desaloca variáveis de lock e de condição */
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	GET_TIME(tempo_concorrente_termino);
	GET_TIME(tempo_termino);


	printf("Tempo total: %lf\n" , tempo_termino - tempo_inicio );
	printf("Tempo da parte otimizada: %lf\n" , tempo_concorrente_termino - tempo_concorrente_inicio );
	printf("Resultado da Integração = %lf\n", resp);

	pthread_exit(NULL);
}
