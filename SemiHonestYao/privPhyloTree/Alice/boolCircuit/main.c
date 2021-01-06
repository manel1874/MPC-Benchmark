#define LEN_SEQ 8000

typedef unsigned char uint8_t;

typedef struct {
	uint8_t el[LEN_SEQ];
} Array_Seq;



void mpc_main(Array_Seq INPUT_A, Array_Seq INPUT_B)
{

	int distance = 0;

	for(int i=0; i<LEN_SEQ; i++)
	{
		if(INPUT_A.el[i] != INPUT_B.el[i])
		{
			distance = distance + 1;
		}
	}
	
	int OUTPUT_distance;
	OUTPUT_distance = distance;
}

