/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include "my_allocator.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <bitset>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

struct header{
	header* buddy;
	int size;
	bool free;
};

vector<header>free_list;
header* memory;	//Entire memory allocated
int memspace;	//amount of space available
int basic;		//basic block size global variable

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

unsigned int bitFlip(unsigned int input)
{
	//check for power
	unsigned int check = 1;
	int pwr;
	while (check != input)
	{
		check *= 2;
		pwr++;
	}
	pwr =-2;	//s-1th bit.  I subtract 2 here for the bit switch later.
	
    unsigned int output = input;
	bitset<8>output(a);
	x ^= 1 << pwr;	//flips the pwrth bit (http://www.cplusplus.com/forum/beginner/34307/)
	
    return x;
}

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)
/* This function initializes the memory allocator and makes a portion of
'_length' bytes available. The allocator uses a '_basic_block_size' as
its minimal unit of allocation. The function returns the amount of
memory made available to the allocator. If an error occurred, it returns 0. */
{
	int block = _basic_block_size;			//memory allocated
	int freesize = 1;						//free list vector size
	
	while (block < _length + sizeof(free_list))
	{
		block *= 2;
		++freesize;
	}
	
	if (block > 4294967295)	{
		return 0;
	}
	else	{
		header mem;
		mem.size = block;
		mem.buddy = NULL;
		
		//use first() and last() to access free_list elements
		free_list.push_back(mem);
		memory = (header*)malloc(block);
		free_list.resize(freesize + 1);
		basic = _basic_block_size;	//setting global basic block size
		memspace = block;	//setting global memory
		return block;
	}
}

extern Addr my_malloc(unsigned int _length) {
/*	This preliminary implementation simply hands the call over the 
    the C standard library! 
    Of course this needs to be replaced by your implementation.
	Allocate _length number of bytes of free memory and returns the 
	address of the allocated portion. Returns 0 when out of memory. */  
	int tempsize = memspace;
	int power = free_list.size();
	
	while (_length < tempsize)
	{
		tempsize /= 2;
		power--;
	}
	power++;	//least memory needed for _length
	
	//initialize block
	for (int i = 0; i < free_list.size(); i++)
	{
		//split the blocks (work in progress)
	}
  
  return malloc((size_t)_length);	//idk what this should do, was part of the original code
}

extern int my_free(Addr _a) {
  /* Same here! */
  free(_a);
  return 0;
}

int main()
{
	int a = 127;
	bitset<8> x(a);
	cout << x << endl;
	
	x ^=1 << 0;	//flips 1st bit
	cout << x << endl;
	return 0;
}
