#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double func(double x);
double trapezoidal_integration(double a, double b, int n);

int main(){
	double a = 1.0;
	double b = exp(4.0);
	int n = 1024;

	clock_t start_time = clock();	
	double solution = trapezoidal_integration(a,b,n);
	clock_t end_time = clock();

	printf("The approximate area under the curve ln(x) on [1,e^4] using the trapezoidal rule is %f\n", solution);
	double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
  	printf("Total time taken: %f seconds\n", total_time);	
	return 0;
}


double func(double x){
	return log(x);
}

double trapezoidal_integration(double a, double b, int n){
	double h = (b-a)/n;
	double sum = 0.5 * (func(a) + func(b));
	for (int step = 1; step < n; step++){
		double x = a + step * h;
		sum = sum + func(x);
}
	return h*sum;

}
