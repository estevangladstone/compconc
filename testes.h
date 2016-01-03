/* Cálculo de áreas de funções teste para a aplicação */

/* Calcula a area do retangulo, dependendo da funcao que for passada como parametro. */
float calculaRet(float (*f)(float), float a, float b){
    float m = (a+b)/2;
    return ( (b-a) * f(m) );
}

/* f(x) = 1 + x */
float funA(float x){
	return (1 + x);
}

/* f(x) = (1 - x^2)^(1/2) , -1 < x < 1 */
float funB(float x){
    if( x < -1 || x > 1 ){
        printf("OPA, RAIZ DE NUM NEGATIVO, PARSA????????????\n" );
        exit(EXIT_FAILURE);
    }
	return sqrt(1 - (x * x));
}

/* f(x) = (1 + x^4)^(1/2) */
float funC(float x){
	return sqrt(1 + pow(x, 4));
}

/* f(x) = sen(x^2) */
float funD(float x){
	return sin(x * x);
}

/* f(x) = cos(e^(-x)) * (0.005 * x^3 + 1) */
float funE(float x){
	return ( cos(exp(-x)) * ( ( 0.005 * pow(x, 3) ) + 1 ) );
}