#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes
#include "timer.h"

#define LOCAL_BUFFER_SIZE 15

/* Estruturas de dados utilizadas */
typedef struct _tarefa{
	double l , r;
	struct _tarefa *prox;
} tarefa;

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
void integra_conc(double l, double r, tarefa *lista_tarefas, int idt, int *espaco_buffer);
void *thread();
void insere_tarefa(tarefa* nt);


/* Variáveis globais */
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_cond_t begin;
int threads_iniciadas = 0;
tarefa* prox_tarefa_global;
tarefa* ultima_tarefa;
double erro; 		// Erro máximo
int total_threads, threads_ociosas;
double (*fun)(double);  // Ponteiro da funcao que queremos usar, se eh funA, funB ...
double *valores_areas; // valores da integral calculados por cada thread

void integra_conc(double l, double r, tarefa *lista_tarefas, int idt, int *espaco_buffer){
	tarefa* nova_tarefa;
	double m;			// ponto médio do intervalo
	double A1, A2 , A;  	// A1 e A2 sao as areas dos intervalos (l,m) e (m,r) . A eh o retangulao, fun(l,r);
	double dif;          // diferença entre áreas
	int i;

 	m = (r + l)/2; // ponto médio do intervalo

	A1 = calculaRet(fun ,  l,  m);
	// printf("A1 = %lf\n", A1);

	A2 = calculaRet(fun ,  m,  r);
	// printf("A2 = %lf\n", A2);

	A = calculaRet(fun ,   l,  r);
    // printf("A = %lf\n" , A);

	dif = fabsf((A1 + A2) - A);

	// printf("dif = %lf\n", dif);

 	if(dif <= erro){
		// printf("Somei %lf ao resp!\n", A);
		valores_areas[idt] += A;
		return ;
 	}
 	else{
	    if(*espaco_buffer == 0){
			for( i = 0 ; i < (LOCAL_BUFFER_SIZE-1)/2 ; i++ )
			{
				nova_tarefa = (tarefa*)malloc(sizeof(tarefa));
	    		nova_tarefa->l = m;
	    		nova_tarefa->r = r;
	    		nova_tarefa->prox = NULL;
				pthread_mutex_lock(&mutex);
	    		insere_tarefa(nova_tarefa);
				pthread_mutex_unlock(&mutex);
				*espaco_buffer--;
			}
		}
		else{
		    integra_conc(m, r, lista_tarefas, idt, &(*espaco_buffer));
		}
	    //Faco a conta da esquerda agora
	    integra_conc(l, m, lista_tarefas, idt, &(*espaco_buffer));
 	}

}

void *thread(void *arg){
	tarefa *old_global_tarefa, // Usado na hora de apagar prox tarefa
		   *lista_tarefas, // lista interna de tarefas a serem feitas
		   *prox_tarefa, // proxima tarefa com prioridade dentro da lista interna da thread
		   *nova_tarefa; // tarefa para ser colocada no buffer global
	double l, r;
	int i, count = 0; // contadores
	int	*idt = (void *) arg; // identificador da thread
	int	espaco_buffer;

	espaco_buffer = LOCAL_BUFFER_SIZE;

	lista_tarefas = (tarefa*)malloc(LOCAL_BUFFER_SIZE * sizeof(tarefa));

	pthread_mutex_lock(&mutex);
	threads_iniciadas++;	
	while( prox_tarefa_global!=NULL || threads_ociosas < total_threads ){
		//Checa se existem tarefas
		while(prox_tarefa_global==NULL){
			pthread_cond_wait(&cond, &mutex);
			if(prox_tarefa_global==NULL){	//SE O COND DEIXOU ELE ANDAR E AINDA ASSIM prox_tarefa_global
									//EH NULL, eh pra dar exit;
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}
		}
        //Tira uma tarefa do buffer
		l = prox_tarefa_global->l;
		r = prox_tarefa_global->r;
		old_global_tarefa = prox_tarefa_global;
		prox_tarefa_global = prox_tarefa_global->prox;
		if(old_global_tarefa == ultima_tarefa){
			ultima_tarefa = NULL;
		}
		free(old_global_tarefa);
		threads_ociosas--;
		pthread_mutex_unlock(&mutex);
		old_global_tarefa = NULL;
		
		while(prox_tarefa != NULL){
			//------------------------------------
			l = prox_tarefa->l;
			r = prox_tarefa->r;
			count = (count++) % LOCAL_BUFFER_SIZE;
			*prox_tarefa = lista_tarefas[count];
			integra_conc(l, r, lista_tarefas, *idt, &espaco_buffer);
			//------------------------------------
			if(espaco_buffer == 0){
				for( i = 0 ; i < (LOCAL_BUFFER_SIZE-1)/2 ; i++ )
				{
					nova_tarefa = (tarefa*)malloc(sizeof(tarefa));
		    		nova_tarefa->l = lista_tarefas[LOCAL_BUFFER_SIZE - i].l;
		    		nova_tarefa->r = lista_tarefas[LOCAL_BUFFER_SIZE - i].r;
		    		nova_tarefa->prox = NULL;
					pthread_mutex_lock(&mutex);
		    		insere_tarefa(nova_tarefa);
					pthread_mutex_unlock(&mutex);
					espaco_buffer--;
				}
			}
		}
		pthread_mutex_lock(&mutex);
		threads_ociosas++;
	}
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond); // Libera todas as threads para finalizar a aplicação
	// printf("vou dar exit aqui, namoral\n");
	pthread_exit(NULL);
}


Resultado principal_conc(int argc, char *argv[]) {
  	pthread_t *threads;
	int i;
	double l, r;                            //  Intervalos de integração;
	char escolha_da_funcao;                 //  Eh pra ser 'a' ou 'b' ou 'c'...
	double tempo_inicio , tempo_termino ;   //  
	double resp; 		// saída e resultado da integração

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

	GET_TIME(tempo_inicio);

	prox_tarefa_global = (tarefa*)malloc(sizeof(tarefa));
	prox_tarefa_global->l = l;
	prox_tarefa_global->r = r;
	prox_tarefa_global->prox = NULL;
	ultima_tarefa = prox_tarefa_global;

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);
	pthread_cond_init (&begin, NULL);
	
	/* Inicializa o vetor para os valores de areas */
	valores_areas = (double *)malloc(total_threads * sizeof(double));

	/* Inicializa o número de threads ociosas para uso posterior */
	threads_ociosas = total_threads;

	/* Aloca os identificadores da thread no sistema e cria as threads */
	threads = (pthread_t *)malloc(total_threads * sizeof(pthread_t));
	for(i = 0; i < total_threads; i++) {
		int *t = (int *)malloc(sizeof(int));
		if(pthread_create(&threads[i], NULL, thread, (void *)t)) {
			// printf("ERRO: pthread_create()\n");
			exit(EXIT_FAILURE);
		}
	}
	
    // Espera as threads completarem
	for(i = 0; i < total_threads; i++) {
		pthread_join(threads[i], NULL);
		/* Realiza as somas das áreas calculadas */
		resp += valores_areas[i];
		printf("valor = %lf", valores_areas[i]);
	}

	/* Desaloca variáveis de lock e de condição */
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	pthread_cond_destroy(&begin);

	GET_TIME(tempo_termino);

    Resultado result;
    result.valorDaIntegral = resp;
    result.tempoDaAplicacao = tempo_termino - tempo_inicio;

 	return result;

}

void insere_tarefa(tarefa* nova_tarefa){
    if(ultima_tarefa!=NULL){
		ultima_tarefa->prox = nova_tarefa;
		ultima_tarefa = nova_tarefa;
	}else{ //Caso em que a fila ficou vazia, nao acesse o null
		prox_tarefa_global = nova_tarefa;
		ultima_tarefa = nova_tarefa;
	}
}

#ifndef COMPARADOR_C // Nao queremos outras "mains" se estivermos usando a main do comparador
int main(int argc, char *argv[]) {
    Resultado result = principal_conc( argc , argv );
    printf("Valor da integral: %lf\nTempo da aplicacao: %lf\n" , result.valorDaIntegral , result.tempoDaAplicacao );
    return 0;
}
#endif
