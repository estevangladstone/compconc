#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "testes.h" // Arquivo externo com implementações de funções para testes

/* Protótipos de funções da aplicação */
float integra(node *ptr);
float soma_retangulos(node *ptr);
void *thread();

/* Estruturas de dados utilizadas */
typedef struct _node {
	float l, r;
	float val; // valor da área do retangulo
	struct _node *esq, *dir;
} node; // NOS DA ARVORE

// FAZER FILA

/* Variáveis globais */
pthread_mutex_t mutex;
pthread_cond_t cond;
float erro; // Erro máximo
int total_threads, threads_ociosas;

/* Soma as areas dos retangulos que estão nas folhas da árvore */
float soma_retangulos(node *ptr){
	if(ptr->esq != NULL || ptr->dir != NULL){
		return soma_retangulos(&ptr->esq) + soma_retangulos(&ptr->dir);
	}
	else{
		return ptr->val;
	}
}

void integra(node *ptr){
	float m;			// ponto médio do intervalo
	float A1, A2 , A;  	// A1 e A2 sao as areas dos intervalos (l,m) e (m,r) . A eh o retangulao, fun(l,r);
	float resp;         // resultado da integração no intervalo dado
	float dif;          // diferença entre áreas
 
 	m = (r + l)/2; // ponto médio do intervalo
 
	A1 = calculaRet(fun ,  l,  m);
	printf("A1 = %f\n", A1);

	A2 = calculaRet(fun ,  m,  r);
	printf("A2 = %f\n", A2);

	A = calculaRet(fun ,   l,  r);
    printf("A = %f\n" , A);
 
	dif = fabsf((A1 + A2) - A);

	printf("dif = %f\n", dif);

 	if(dif <= erro){
        printf("dif deu menor ou igual ao erro, que eh igual a %f\n" , erro );
		printf("resp = %f\n", A);

 		ptr->esq = NULL;
 		ptr->dir = NULL;
 		ptr->val = A;
 	}
 	else{
	    printf("dif deu maior que o erro permitido...q eh de %f\n" , erro );
		
		/* Prepara os próximos nós da árvore com seus valores */
		ptr->esq = (node*)malloc(sizeof(node));
		ptr->esq->l = l;
		ptr->esq->r = m;

		ptr->dir = (node*)malloc(sizeof(node));
		ptr->dir->l = m;
		ptr->dir->r = r;

		pthread_mutex_lock(&mutex);
		if(threads_ociosas > 0){
			// poe o ponteiro na fila para outras threads pegarem
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
		}
		else{
			pthread_mutex_unlock(&mutex);
			integra(ptr->dir);
		}

		integra(ptr->esq);
 	}

 	return;
}

void *thread(){
	pthread_mutex_lock(&mutex);
	while(threads_ociosas != total_threads && fila != NULL){ //sdfsfdsf sdfsd gsd gsdgsdgds errado
		while(fila == NULL){
			if(threads_ociosas == total_threads && fila == NULL){
				pthread_exit(NULL);
			}
			pthread_cond_wait(&cond, &mutex);
		}
		threads_ociosas--;

		// pegar o ponteiro da fila e por em ptr

		pthread_mutex_unlock(&mutex);
		integra(ptr);
		pthread_mutex_lock(&mutex);
		threads_ociosas++;
	}
	pthread_mutex_unlock(&mutex); // pro caso do lock ainda estar sendo usado aqui
	pthread_cond_broadcast(&cond); // Libera todas as threads para finalizar a aplicação
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  	pthread_t *threads;
	float l, r; // intervalos de integração;
	char escolha_da_funcao; //Eh pra ser 'a' ou 'b' ou 'c'...
 	float resp; // saída e resultado da integração
 
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

	/* Cria a raiz da árvore */
	raiz = (node*)malloc(sizeof(node));
	raiz->l = l;
	raiz->r = r;

	/* Põe a raiz na fila para cálculo */
	// -- fazer função para inserir na fila

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	/* Inicializa o número de threads ociosas para uso posterior */
	threads_ociosas = total_threads;

	/* Aloca os identificadores da thread no sistema e cria as threads */
	threads = (pthread_t *)malloc(total_threads * sizeof(pthread_t));
	for(i = 0; i < total_threads; i++) {
//
// CHECAR ISSO AQUI !!!!! O thread É UM VETOR DE PONTEIROS!!??
//
		if(pthread_create(thread[i], NULL, thread, NULL)) {
			printf("ERRO: pthread_create()\n");
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

	/* Realiza a soma das áreas dos retângulos */
	resp = soma_retangulos(raiz);
 
	printf("Resultado da Integração = %f\n", resp);
	
	pthread_exit(NULL);
}
