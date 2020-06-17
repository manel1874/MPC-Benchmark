#define LEN 2

typedef struct {
	int el[LEN];
} Array;


int mpc_main(Array INPUT_A, Array INPUT_B)
{
	_Bool z;
	z = 1;

	for(int i=0; i<LEN; i++){
		// Compare each element in the array
		if(INPUT_A.el[i] != INPUT_B.el[i]){
			z = 0;
		}	
	}

	//int product = 0;

	//for( int i=0; i<LEN; i++){
	//	product += INPUT_A.el[i] * INPUT_B.el[i];	
	//}

	return z;
	
}

