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
#include <limits.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/
#pragma pack (1) // change the packing to 1 byte so no extra padding is added even on a 1 byte field
struct header{
	//header* buddy;//8 bytes
	header* next;//8 bytes
	int size;//4 bytes
	bool free;//1 byte
	
};


header* memory;	//Entire memory allocated
int memspace;	//amount of space available
int basic;		//basic block size global variable
const int header_size = sizeof(header);//UNCHANGEABLE SIZE OF HEADER CONST
vector<header>free_list;
/* free list indexing:
[0] bbs
[1] bbs * 2^1
[2] bbs * 2^2
[3] bbs * 2^3
...
[n] total length = bbs * 2^n

so if total length is 128 and our bbs is 8, then our last index is n = 4 
*/
/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

unsigned int bitFlip(unsigned int input)// input is the address, need to find the size of the address block
{
	//check for power
	unsigned int check = 1;
	int pwr;
	/* 
	while (check != input)
	{
		check *= 2;
		pwr++;
	}
	pwr =-2;	//s-1th bit.  I subtract 2 here for the bit switch later.
	 */
	//input would not work, you need to take the log2(size of memory block that input points to)
	pwr = log2(input);//figure out the power of the input
	pwr -=2;//s-1th bit. This bit will be flipped
    
	//unsigned int output = input;
	
	bitset<8>output(input);//bitset<8> might not be enough bits
	//cout<<output<<endl;
	output ^= 1 << pwr;	//flips the pwrth bit (http://www.cplusplus.com/forum/beginner/34307/)
	//cout<<output<<endl;
	
    return (unsigned int) (output.to_ulong());
}

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)//i think this is wrong
/* This function initializes the memory allocator and makes a portion of
'_length' bytes available. The allocator uses a '_basic_block_size' as
its minimal unit of allocation. The function returns the amount of
memory made available to the allocator. If an error occurred, it returns 0. */
{
	int block = _basic_block_size;			//memory allocated
	int freesize = 1;						//free list vector size
	
	//tanzir said that we should only allocate M amount of memory, and place the header inside of it
	//i.e. we don't allocate M + sizeof(header)
	
	while (block < _length /* + sizeof(free_list) */)//keep looping until block is the appropriate size
	{
		block *= 2;
		++freesize;
	}
	
	if (block > UINT_MAX/* 4294967295 */)	{
		return 0;
	}
	else	{
		header mem;
		mem.size = block;
		mem.next = NULL;
		
		//use first() and last() to access free_list elements
		cout<<"block size: "<<block<<endl;
		cout<<"freesize: "<<freesize<<endl;
		
		//free_list.push_back(mem);
		memory = (header*)malloc(block);
		//free_list.resize(freesize + 1);
		
		free_list.resize(freesize);
		free_list[freesize-1] = mem;//put the allocated block into the appropriate free list
		
		basic = _basic_block_size;	//setting global basic block size
		memspace = block;	//setting global total memory
		return block;
	}
}

extern Addr my_malloc(unsigned int _length) {// _length is memory requested by the user
/*	This preliminary implementation simply hands the call over the 
    the C standard library! 
    Of course this needs to be replaced by your implementation.
	Allocate _length number of bytes of free memory and returns the 
	address of the allocated portion. Returns 0 when out of memory. */
	
	int tempsize = memspace;
	int power = free_list.size();
	
	
	//if _length = 12 and sizeof = 13, then we know 4 < log2(26) < 5, so the correct value would be 5, therefore 5 = ceil(log2(_length+sizeof(header)))
	tempsize = ceil(log2(_length + header_size)); 
	if (tempsize < basic)//case where we need less than the bbs; we just give them the bbs 
		tempsize = log2(basic);//
		
	//our freelist starts index[0] at the bbs
	//so then if we want constant time accessing, we must convert it to the appropriate index
	int temp_val = pow(2,tempsize);//if tempsize = 5, then temp_val is 32
	temp_val = temp_val / basic;// 32 / 8 = 4
	temp_val = log2(temp_val);//contains the appropriate index in freelist
	bool is_filled = true;
	/* 
	while(is_filled){
	
		if(free_list[temp_val].next == NULL){//there are no available blocks, start SPLITIN'
			//for()
			
		}
		else{//WE FOUND IT!!!111!
			
			
			is_filled=false;
			
		}
	}
	 */
	cout<<"_length: "<<_length<<endl;
	cout<<"tempsize: "<<tempsize<<endl;
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
	/* int a = 126;
	bitset<8> x(a);
	cout << x << endl;
	
	x ^=1 << 0;	//flips 1st bit
	cout << x << endl; */
	
	
	cout<<"=============================="<<endl
		<<"bitFlip: "<<128<<"\n"<<endl;
	cout<<bitFlip(128);
	cout<<"header size:"<<endl;
	header mainheader;
	cout<<"size of header pointer: "<<sizeof(mainheader.next)<<endl
		<<"size of int: "<<sizeof(mainheader.size)<<endl
		<<"size of bool: "<<sizeof(mainheader.free)<<endl;
	cout<<endl<<"size of header: "<<sizeof(mainheader)<<endl;
	
	init_allocator(8,64);
	my_malloc(12);
	
	return 0;
}
