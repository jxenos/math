#include <stdio.h>
#include <errno.h>

//https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html#Argument-Syntax

int main(int argc, char** argv){
	float x[] = {1,100,200,300,450,600,800,1000,1300,1600};
	float y[] = {550,509.5,468,437,398,361.5,331,297,265.5,236};

	float input = 0;
	float output;
	int i;
	float length = sizeof(x)/sizeof(*x);

	// printf("%f\n%f\n%f\n%f\n", length, x[0], x[(int) length-1], input);

	if(input < x[0] || input > x[(int) length-1]){
		printf("Error: Outside bounds\n");
		return 1;
	}

	for(i=0; i<length; i++){
		if(input == x[i]){
			output = y[i];
			break;
		}
		if(input < x[i]){
			output = (y[i]-y[i-1])*(input-x[i-1])/(x[i]-x[i-1])+y[i-1];
			break;
		}
	}

	printf("%f\n", output);

	return 0;
}
