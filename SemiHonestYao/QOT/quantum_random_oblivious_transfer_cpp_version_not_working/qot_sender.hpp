#pragma once

#include <stdio.h>

#define KEY_LENGTH 512
#define OUTPUT_LENGTH 128

class qot_sender
{
private:
	
public:

	unsigned char sender_OTkey[KEY_LENGTH];

	qot_sender();
	void sender_okd (); //call OKD service and read the output key
	void sender_output (unsigned long long int * , unsigned long long int * , unsigned int * , unsigned int * , unsigned char (*)[OUTPUT_LENGTH/32]); //sample hash functions and set sender output

	
};


/**

#ifdef __cplusplus
extern "C" {
#endif

struct qot_sender
{
	unsigned char sender_OTkey[KEY_LENGTH];
};


typedef struct qot_sender OKDOT_SENDER;

void sender_okd (OKDOT_SENDER *); //call OKD service and read the output key
void sender_output (OKDOT_SENDER * , unsigned long long int * , unsigned long long int * , unsigned int * , unsigned int * , unsigned char (*)[OUTPUT_LENGTH/32]); //sample hash functions and set sender output


#ifdef __cplusplus
}
#endif

#endif //ifndef OT_SENDER_H
**/
