/*
 * buffer.c
 *
 *  Created on: 23 Jan 2022
 *      Author: Tunjow
 */

#include "buffer.h"
/*
 * initailise the FIFO BUFFER.
 */
buffer_init(fifo_t *f, int size)
{
    f->head = 0;
    f->tail = 0;
    f->size = size;   //sizeof(f->buf ); //
   // f->buf = buf;
    f->element = 0;
    for (int i = 0 ; i<f->size ; i++)
    		{
    		    f->buf[i] = 0;     //clean the buffer
    		}
}

/*
 * READ N BYTES OUT OF THE BUFFER
 */
buffer_read_bytes(fifo_t *f,void  * buf, int nbytes)
{
   int i;
   char* p;
   p = buf;
 //  int temp = f->element;

   for(i=0;i<nbytes;i++)
   {
	   if(f->element > 0) //(f->tail != f->head){//check if data is available
	   {
		   *p++ = f->buf[f->tail];  // read a byte from buffer
		   f->element--;
		   f->tail++;				//increment the tail
		   if(f->tail == f->size)	// check for wrap around
		   {
			   f->tail = 0;
		   }
	   }
	      else
	   {
		   return i;    // number of bytes  read
	   }
   }
 return nbytes;
}

/*
 * READ 1 BYTE FROM BUFFER
 */
int buffer_read(fifo_t *f)
{
   int i;
	   if(f->element >= 15) //(f->tail != f->head)       //check if data is available
	    {
		   i = f->buf[f->tail];    // read a byte from buffer
		   f->element--;
		   f->tail++;					//increment the tail
		   if(f->tail == f->size)    // check for wrap around
			   {
				   f->tail = 0;
			   }
	   }
	      else
	   {
		   return -1;               // number of bytes  read
	   }
   return i;
}


/*
 * CHECK THE BUFFER IF EMPTY
 */
int isbuffer_empty(fifo_t *f)
{
  int res = ( ( f->tail == f->head )? 1:0  );
  return res;
}

/*
 * writes BYTES to buffer
 */
 buffer_write_bytes(fifo_t *f,const void  * buf, int nbytes)
 {
	   int i;
	   const char* p;
	   p = buf;

	   for(i=0;i<nbytes;i++){  //check to see if space is in the buffer...
		   if( (f->head + 1 == f->tail)||(  (f->head+1 == f->size)&&(f->tail == 0)   ) )
			   return i;                          //no more room
		   else{
		   		   f->buf[f->head] = *p++;
		   		   f->head++;                     //incr the write ptr.
		   		   f->element++;                  //incr the num elements in buffer.
		   		   if(f->head == f->size){        // wrap the head around.
		   			   f->head = 0;
		   		   }
		   	   }
	   }
	   return nbytes;
 }

 /*
  * writes BYTES to buffer
  */
 int buffer_write(fifo_t *f, uint8_t byte)
  {

	   if( (f->head + 1 == f->tail)||(  (f->head+1 == f->size)&&(f->tail == 0) ) ) //check to see if space is in the buffer...
		   return -1;                          //no more room
	   else{
			   f->buf[f->head] = byte;        //write byte to buffer
			   f->head++;                     //incr the write ptr.
			   f->element++;                  //incr the num elements in buffer.
			   if(f->head == f->size){        // wrap the head around.
				   f->head = 0;
			   }
		   }
 	   return 1;   //success
  }

/*
 * CHECK BUFFER FULL
 */
 int isbuffer_full(fifo_t *f)
 {
	int res = ( ( (f->head + 1 == f->tail)||(  (f->head+1 == f->size)&&(f->tail == 0)   ) )? 1:0 );
	return res;
 }
