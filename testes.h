/* Cálculo de áreas de funções teste para a aplicação */

/* f(x) = 1 + x */
float funA(float a, float b){
	float x = (b - a)/2;
	return (b - a) * (1 + x);
}

/* f(x) = (1 - x^2)^(1/2) , -1 < x < 1 */
//
// Ver a parte da condição |x| < 1 !!!!!
//
float funB(float a, float b){
	float x = (b - a)/2;
	return (b - a) * sqrt(1 - (x * x));
}

/* f(x) = (1 + x^4)^(1/2) */
float funC(float a, float b){
	float x = (b - a)/2;
	return (b - a) * sqrt(1 + pow(x, 4));
}

/* f(x) = sen(x^2) */
float funD(float a, float b){
	float x = (b - a)/2;
	return (b - a) * sin(x * x);
}

/* f(x) = cos(e^(-x)) * (0.005 * x^3 + 1) */
float funE(float a, float b){
	float x = (b - a)/2;
	return (b - a) * (cos(exp(-x)) * ((0.005 * pow(x, 3)) + 1));
}