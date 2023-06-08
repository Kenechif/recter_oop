/*
 * buffer.h
 *
 *  Created on: 23 Jan 2022
 *      Author: Tunjow
 */

#ifndef INC_BUFFER_H_
#define INC_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define buffer_length  30  //  was formally 300 , a multiple of 1complete packet

typedef struct
{
	//char* buf;
	int head;
	int tail;
	int size;
	int element;
	uint8_t buf[buffer_length];
}  fifo_t;


int isbuffer_empty(fifo_t *f);
int isbuffer_full(fifo_t *f);


void buffer_init(fifo_t *f, int size);

void buffer_read_bytes(fifo_t *f,void  * buf, int nbytes);
int buffer_read(fifo_t *f);

void buffer_write_bytes(fifo_t *f,const void  * buf, int nbytes);
int buffer_write(fifo_t *f,uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* INC_BUFFER_H_ */

