#pragma once

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <stdio.h>
#include <string.h>

#define KEY_LENGTH 512
#define OUTPUT_LENGTH 128


class qot_receiver
{
private:
	/* data */
public:

	unsigned char receiver_OTkey[KEY_LENGTH];
	unsigned char receiver_OTauxkey[KEY_LENGTH];
	unsigned int indexlist[2][KEY_LENGTH/2];

	qot_receiver();
	void receiver_okd(); //call OKD service and read the output key from text file
	void receiver_indexlist(); //define a pair of index lists based on the oblivious keys
	void receiver_output(unsigned long long int * , unsigned char *); //set receiver output

};









/**

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define KEY_LENGTH 512
#define OUTPUT_LENGTH 128

struct qot_receiver
{
		unsigned char receiver_OTkey[KEY_LENGTH];
		unsigned char receiver_OTauxkey[KEY_LENGTH];
		unsigned int indexlist[2][KEY_LENGTH/2];
};

typedef struct qot_receiver OKDOT_RECEIVER;

void receiver_okd(OKDOT_RECEIVER *); //call OKD service and read the output key from text file
void receiver_indexlist(OKDOT_RECEIVER *); //define a pair of index lists based on the oblivious keys
void receiver_output(OKDOT_RECEIVER *, unsigned long long int * , unsigned char *); //set receiver output

#ifdef __cplusplus
}
#endif

#endif //ifndef OT_RECEIVER_H

**/
