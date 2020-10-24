#include "qot_receiver.hpp"
#include <stdlib.h>
//#include <process.h>

qot_receiver::qot_receiver()
{
}

void qot_receiver::receiver_okd()
{

	/*opening key files and storing the keys in the receiver structure*/


	// TODO::::: tentar ler bem os sinais. testar e integrar com o BaseOT do mascot!

	/**
	std::ifstream inFile;
	inFile.open("ObliviousKeys.sgn", std::ios::in | std::ios::binary);
	std::string line;
	getline(inFile, line);
	printf("%s\n", line.c_str());
	getline(inFile, line);
	printf("%s\n", line.c_str());
	getline(inFile, line);
	printf("%s\n", line.c_str());
	getline(inFile, line);
	printf("%s\n", line.c_str());
	getline(inFile, line);
	**/
	


	/**
	while (line.compare("// ### HEADER TERMINATOR ###\r"))
	{
		if (std::size_t found = line.find("Signal type:")!= std::string::npos)
		{
		} else if (std::size_t found = line.find("Symbol Period (s):") != std::string::npos)
		{
			line.replace(line.find("Symbol Period (s):"), 18, "");
			std::stringstream aux{ line };
			aux >> symbolPeriod;
		} else if (std::size_t found = line.find("Sampling Period (s): ") != std::string::npos)
		{
			line.replace(line.find("Sampling Period (s):"), 20, "");
			std::stringstream aux{ line };
			aux >> samplingPeriod;
		}
		getline(inFile, line);
	}

	**/

	FILE *receiverfile;
	int i = 0;

	if ((receiverfile = fopen("ObliviousKeys.sgn","r")))
	{
		/**
		for(int j=0; j<16; j++)
		{
			char* to_print;
			fscanf(receiverfile, "%s", to_print);
			printf("%s\n", to_print);
		}**/

		while (i<KEY_LENGTH)
		{
			if (fscanf(receiverfile, "%c", &this->receiver_OTkey[i]))
				i++;
			else
				printf ("QOT ERROR: failed to read oblivious keys.\n");
		}
	}	
	else
		printf ("QOT ERROR: failed to open oblivious key file: receiver's key file.\n");

	fclose (receiverfile);
	i=0;

	if ((receiverfile = fopen("key_aux.txt","r")))
	{
		while (i<KEY_LENGTH)
		{
			if (fscanf(receiverfile,"%c", &this->receiver_OTauxkey[i]))
				i++;
			else
				printf ("QOT ERROR: failed to read oblivious keys.\n");
		}
	}
	else
		printf ("QOT ERROR: failed to open oblivious key file: receiver's auxkey file.\n");

	fclose (receiverfile);

}





void qot_receiver::receiver_indexlist()
{
	int j = 0;
	int k = 0;


	/*generate I0 and I1 index lists from the receiver aux key*/
	for (int i = 0; i<KEY_LENGTH; i++)
	{	
		
		if (this->receiver_OTauxkey[i] == '0')
		{
			this->indexlist[0][j] = i;
			j++;
		}
		else if (this->receiver_OTauxkey[i] == '1')
		{
			this->indexlist[1][k] = i;
			k++;
		}
		else
			printf ("OT ERROR: invalid key character found.\n");
	}

}





void qot_receiver::receiver_output(unsigned long long int * vb, unsigned char * output)
{
	unsigned long int input32[KEY_LENGTH/(2*32)] = {0}; 


	/*converts the binary hash inputs into 32bit ints*/
	for (int i=0; i<32; i++)
	{
		for (int j=0; j<KEY_LENGTH/(2*32); j++)
		{	
			input32[j] <<= 1;
			input32[j] += this->receiver_OTkey[this->indexlist[0][i+j*32]] - '0';
		}
	}


	/*hashes pairs of ints from the input32 array into another 32bit value, which is stored in the output array*/
	for (int i=0; i<OUTPUT_LENGTH/32; i++)
	{
		output[i] = (unsigned long int) ((vb[0+3*i]*input32[0+2*i] + vb[1+3*i]*input32[1+2*i] + vb[2+3*i]) >> 32);
	}



}



