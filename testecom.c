#include <stdio.h>

void main(){
    int i;
    double m = 0;

    for (i = 1; i < 1000000; i++) {
        double f = 1;
        m += f/i;
    }

    printf("%.20lf\n" , m);

    m = 0;

    for (i = 999999; i > 0; i--) {
        double f = 1;
        m += f/i;
    }

    printf("%.20lf\n" , m);

    return;
}
