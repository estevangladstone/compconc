/* Cálculo de áreas de funções teste para a aplicação */

/* Calcula a area do retangulo, dependendo da funcao que for passada como parametro. */
double calculaRet(double (*f)(double), double a, double b){
    double m = (a+b)/2;
    return ( (b-a) * f(m) );
}

/* f(x) = 1 + x */
double funA(double x){
	return (1 + x);
}

/* f(x) = (1 - x^2)^(1/2) , -1 < x < 1 */
double funB(double x){
    if( x < -1 || x > 1 ){
        printf("OPA, RAIZ DE NUM NEGATIVO, PARSA????????????\n" );
        exit(EXIT_FAILURE);
    }
	return sqrt(1 - (x * x));
}

/* f(x) = (1 + x^4)^(1/2) */
double funC(double x){
	return sqrt(1 + pow(x, 4));
}

/* f(x) = sen(x^2) */
double funD(double x){
	return sin(x * x);
}

/* f(x) = cos(e^(-x)) * (0.005 * x^3 + 1) */
double funE(double x){
	return ( cos(exp(-x)) * ( ( 0.005 * pow(x, 3) ) + 1 ) );
}
