#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(){
	int int_var = INT_MAX;
	float float_var = FLT_MAX;
	double double_var = DBL_MAX;
	printf("Int var max value is %d, size of var: %lu\n", int_var, sizeof(int_var));
	printf("Float var max value is %f, size of var: %lu\n", float_var, sizeof(float_var));
	printf("Double var max value is %fl, size of var: %lu\n", double_var, sizeof(double_var));
	return 0;
}
