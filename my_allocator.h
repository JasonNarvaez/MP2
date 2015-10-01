/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>
#include <limits>

using namespace std;

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr; 

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)
/* This function initializes the memory allocator and makes ’_length’ bytes available.
	The allocator uses a ’_basic_block_size’ as its minimal unit of allocation. 
	The function returns the amount of memory made available to the allocator. 
	If an error occurred, it returns 0. 
*/ 
{
	int *block = (int*)malloc(_length);
	return sizeof(block);
	/*int block = 1;
	while( _length > block)
		{block *= 2;}
	
	if(block > 4294967295)
		return 0;
	else
		return block;*/
	//initialize freelist array and block of memory
}

int release_allocator()
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails.
*/ 
{
	
}

Addr my_malloc(unsigned int _length)
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 
{
	int *block = (int*)malloc(_length);
	return block; 
	
	//allocate block with header pointer (if requests 128 bytes and bbs is 128, allocate 256)
}
   
int my_free(Addr _a)
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 
{
	if(free(_a))
		return 0;
}
   
#endif 
