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
#include <stdlib.h>
#include <string.h>
//#include <cstdint>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/
#pragma pack (1) // change the packing to 1 byte so no extra padding is added even on a 1 byte field
struct header{
	header(): next(NULL), size(8),/* offset(0), */ free(true) {}
	//header* buddy;//8 bytes
	header* next;//8 bytes
	int size;//4 bytes
	//int offset;//4bytes
	bool free;//1 byte
	
};


header* memory;	//Entire memory allocated
int memspace;	//amount of space available
int basic;		//basic block size global variable
const int header_size = sizeof(header);//UNCHANGEABLE SIZE OF HEADER CONST
vector<header*>free_list;
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
int release_allocator(){
	//freedom
	free(memory);
	
	
}

void remove_flist(header* target, int index){
	
	
	header* current = free_list[index];
	if(current == NULL) return;
	if(current == target) { free_list[index] = free_list[index]->next; return; }
	while(current != NULL){
		
		if(current->next == NULL) return;
		else if(current->next == target){
			current->next = current->next->next;
			
		}
			
		current = current->next;
	}
	

}

void insert_flist(header* target, int index){
	
	target->next = free_list[index];
	free_list[index] = target;
	

}

bool find_header(header* target, int index){
	
	
	header* current = free_list[index];
	if(current == NULL) { cout<<"target is NULL!"<<endl; return false; }
	if(current == target) return true;
	while(current != NULL){
		
		if(current->next == NULL) return false;
		else if(current->next == target){
			return true;
			
		}
			
		current = current->next;
	}
	
	return false;

}


//unsigned int bitFlip(header* input)// input is the address, need to find the size of the address block
unsigned long long bitFlip(header* input)// input is the address, need to find the size of the address block

{
	//check for power
	unsigned int check = 1;
	int pwr;
	//int block_addr;

	//store offset? yeah...
	//int offset_temp=input-memory;
	
	//input->offset=offset_temp;
	//cout<<offset_temp<<endl;
	cout<<input<<endl;
	
	//Jason's attempt
	//unsigned int _a = input->size;
	unsigned int block_size = input->size;
	unsigned long long _a = (unsigned long long) input;//change _a!!!
	unsigned int block_addr = _a - sizeof(header);
	unsigned long long offset = block_addr - (unsigned long long) memory;
	unsigned long long buddy_offset = offset ^ block_size;
	unsigned long long bud_address = (unsigned long long) memory + buddy_offset;
	
	cout<<"value of passed in address: "<<_a<<endl;
	cout<<"value of buddy address: "<<bud_address<<endl;
	
	//input->offset = offset;//update the offset in the input header, use this offset 
	
	pwr = log2(input->size);//figure out the power of the input
	pwr -=2;//s-1th bit. This bit will be flipped
	
	//input would not work, you need to take the log2(size of memory block that input points to)
	//header* temp = input;
	//header* inputptr = (header*) input;
	/*cout<<input<<endl;
	cout<<"input size: "<<input->size<<endl;
	pwr = log2(input->size);//figure out the power of the input
	pwr -=2;//s-1th bit. This bit will be flipped
    
	
	
	
	//cout<<output<<endl;
	offset_temp ^= 1 << pwr;	//flips the pwrth bit (http://www.cplusplus.com/forum/beginner/34307/)
	cout<<offset_temp<<endl;*/
	
	return bud_address;
    //return (unsigned int) (offset_temp);
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
		
		
		memory = (header*)malloc(block);
		//free_list.resize(freesize + 1);
		memory->size=block;
		memory->free=true;
		//memory->offset=0;
		memory->next=NULL;

		free_list.resize(freesize);
		free_list[freesize-1] = memory;//put the allocated block into the appropriate free list
		//cout<<"mem size: "<<memptr->size<<endl;
		// cout<<"memory size: "<<free_list[free_list.size()-1]->size<<endl;
		// cout<<"memory address: "<<free_list[free_list.size()-1]<<endl;
		// cout<<"memory address next: "<<free_list[free_list.size()-1]->next<<endl;
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
	
	
	//if _length = 12 and sizeof = 13, then we know 4 < log2(25) < 5, so the correct value would be 5, therefore 5 = ceil(log2(_length+sizeof(header)))
	tempsize = ceil(log2(_length + header_size)); 
	cout<<"length: "<<_length<<endl;
	cout<<"header size: "<<header_size<<endl;
	/*
	if (tempsize < basic)//case where we need less than the bbs; we just give them the bbs 
		tempsize = log2(basic);//
		*/
	//our freelist starts index[0] at the bbs
	//so then if we want constant time accessing, we must convert it to the appropriate index
	cout<<"tempsize"<<tempsize<<endl;

	int temp_val = pow(2,tempsize);//if tempsize = 5, then temp_val is 32
	temp_val = temp_val / basic;// 32 / 8 = 4
	
	temp_val = log2(temp_val);//contains the appropriate index in freelist
	

	

		if(free_list[temp_val] == NULL){//there are no available blocks, go up until you find a linked list that is filled
			int going_up=0;
			cout<<"temp_val: "<<temp_val<<endl;
			//cout<<"my malloc stuff"<<endl;

			while((free_list[temp_val+going_up]==NULL)&&((temp_val+going_up)<=free_list.size()-1)){//keep going up the vector until we find a block of memory or we reach the biggest memory block
				going_up++;
				//cout<<"going up value: "<<going_up<<endl;
			}//now we have found a filled linked list in vector[temp_val+going_up]
			//keep splitting until you reach your level
			//continually adding the things into free list.
			//but also moving things into proper tiers.
			
			while(going_up>0){
				//moving it out
				header* temp=free_list[temp_val+going_up];
				free_list[temp_val+going_up]=temp->next;//update the freelist
				//cout<<"temp_val + goingup: "<<temp_val+going_up<<endl;
				
				
				//adding things into free list
				//because we are adding 2 blocks into the vector.
				//call bitFlip here???
				
				//temp->next = (header*) bitFlip(temp);//get the buddy of temp
				//temp->next = temp;
				//temp = (header*) temp + (temp->size)/2;//EASILY find the address of the new buddy
				//cout<<"+++++"<<endl;
				//cout<<"temp + size/2: "<<6316048+64<<endl;
				unsigned long long split = (unsigned long long) temp + (temp->size)/2;//EASILY find the address of the new buddy
				temp->next = (header*) split;
				//temp->next =  temp + (temp->size)/2;//EASILY find the address of the new buddy
				cout<<"temp next: "<<temp->next<<endl;
				cout<<"temp: "<<temp<<endl<<endl;
				
				
				unsigned long long mem_int = (unsigned long long) memory;
				unsigned long long block1_int = (unsigned long long) temp;
				unsigned long long block2_int = (unsigned long long) temp->next;
				
				temp->size=(temp->size)/2;
				temp->next->size = temp->size;//update size of the newly split block
				cout<<"temp size: "<<temp->size<<endl;
				
				
				//cout<<"memory origin: "<<mem_int<<endl;
				//cout<<"first allocated block: "<<block1_int<<endl;
				//cout<<"second allocated block: "<<block2_int<<endl<<endl;
				
				//cout<<"adjusted second allocated block: "<<block2_int - sizeof(header)<<endl;
				//cout<<"adjusted first allocated block: "<<block1_int - sizeof(header)<<endl<<endl;
				
				//temp->next->offset = temp->offset;//update the offset value in the header of the buddy block
				
				header* placeholder = temp;//we need to switch temp and temp->next
				temp = temp->next;
				temp->next = placeholder;
				temp->next->next = NULL;//set temp->next->next to NULL or else temp->next->next will point to temp
				
				cout<<"free_list["<<temp_val+going_up-1<<"] = "<<(unsigned long long)temp<<endl;
				
				temp->next->next = free_list[temp_val+going_up-1];//attach existing blocks to the end of newly allocated blocks
				
				free_list[temp_val+going_up-1]=temp;//put the newly split block in the appropriate freelist index
				free_list[temp_val+going_up-1]->free = true;
				free_list[temp_val+going_up-1]->next->free = true;
				cout<<"temp: "<<(unsigned long long)temp<<endl;
				cout<<"temp->next: "<<(unsigned long long)temp->next<<endl;
				/*old code *********************
				//temp->next=temp+(temp->size)/2;
				//temp->next = (temp->size)/2;
				//temp->next->next=free_list[temp_val+going_up]->next;//THIS CAUSES SEGFAULT
				//free_list[temp_val+going_up]=temp;
				
				old code ***********************
				*/
				//PrintList();
				
				//now pointer pointing correctly?
				
				//do something about buddies here....
				//......
				//......
				//......
				//.....
				
				
				going_up-=1;
				
			}
			
			
		}
		else {// there is an available block!
			cout<<"temp val of else statement: "<<temp_val<<endl;
			//cout<<"free_list[tempval] size: "<<free_list[temp_val]->size<<endl;
			header* returned_memblock = free_list[temp_val];//same as the below (returned_block)
			//cout<<"temp size: "<<returned_memblock->size<<endl;
			cout<<"returned block status: "<<returned_memblock->free<<endl;
			free_list[temp_val]->free=false;//IDK????
			free_list[temp_val]=free_list[temp_val]->next;//update the freelist to point to the next free block
			unsigned long long block1_int = (unsigned long long) returned_memblock;
			cout<<"second allocated block (returned): "<<block1_int<<endl<<endl;
			cout<<"adjusted second allocated block: "<<block1_int + sizeof(header)<<endl<<endl;
			cout<<"size of header"<<sizeof(header)<<endl;
			cout<<"returned block: "<<returned_memblock<<endl;
			cout<<"returned block status: "<<returned_memblock->free<<endl;
			cout<<"returned block + header: "<<returned_memblock+sizeof(header)<<endl;
			
			return (returned_memblock+sizeof(header));//we give them the part of the memory block that does not include the header
			
		}
		//Now we know that the list has something in it add stuff.
		//and take it out of free list
		free_list[temp_val]->free=false;//we return this block
		free_list[temp_val]->next->free=true;//keep this block in the freelist
		//free_list[temp_val]=free_list[temp_val]->next;
		
		header* returned_block = free_list[temp_val];// return the rightmost memory block
		free_list[temp_val]=free_list[temp_val]->next;//only keep the leftmost block in the freelist
		//PrintList();
		
		unsigned long long mem_int = (unsigned long long) memory;
		unsigned long long block1_int = (unsigned long long) free_list[temp_val];
		unsigned long long block2_int = (unsigned long long) returned_block;
		
		cout<<"memory origin: "<<mem_int<<endl;
		cout<<"first allocated block: "<<block1_int<<endl;
		cout<<"second allocated block (returned): "<<block2_int<<endl<<endl;
		
		
		cout<<"adjusted first allocated block: "<<block1_int + sizeof(header)<<endl;
		cout<<"adjusted second allocated block: "<<block2_int + sizeof(header)<<endl<<endl;
	
	cout<<"_length: "<<_length<<endl;
	cout<<"tempsize: "<<tempsize<<endl;
	cout<<"size of header"<<sizeof(header)<<endl;
	cout<<"returned block: "<<returned_block<<endl;
	cout<<"returned block + header: "<<returned_block+sizeof(header)<<endl;
	
	return (returned_block+sizeof(header));
  
  //return malloc((size_t)_length);	//idk what this should do, was part of the original code
}

extern int my_free(Addr _a) {
  /* Same here! */
 //put back into free list
	//cout<<"_a address: "<<_a<<endl;
	
	header* temp = (header*)_a - sizeof(header);
	
	//cout<<"_a address: "<<_a<<endl;
	//cout<<"temp address: "<<temp<<endl;
	
	//cout<<"size: "<<temp->size;
	
	//standard checks empty/already free
	if(temp == NULL) {
        cout<<"not found"<<endl;
        return -1;
    }
	if(temp->free == true){
       cout<<"already free"<<endl;
        return -1;
	}else{
		
		//now put back in free list
		int temp_val=+(temp->size);
		temp_val = temp_val / basic;// 32 / 8 = 4
		temp_val = log2(temp_val);//contains the appropriate index in freelist
		
		//link list it in
		//temp->free=true;
		/*old code***********
		//temp->next=free_list[temp_val]->next;
		//free_list[temp_val]=temp;
		old code***********
		*/
		//cout<<"+++++++++++++++++++++++++++++++"<<endl;
		//cout<<temp_val<<endl;
		PrintList2();
		insert_flist(temp,temp_val);
		cout<<endl;
		PrintList2();
		//temp->next = free_list[temp_val];
		//free_list[temp_val]=temp;//put the released memory block back into the freelist
		
		
		//now check to see if it can buddy back up
		//.....
		//.....
		//.....
		//....
		//....
		
		//if(free_list[temp_val]->next == NULL){//case where the right buddy is being freed
		int going_up=0;
			
		//cout << "TEMP VAL LMAO!: " << temp_val+going_up << endl;
		while(((temp_val+going_up)<free_list.size()-1)){//keep going up the vector until we find a block of memory or we reach the biggest memory block
			going_up++;
			//cout<<"going up value: "<<going_up<<endl;
		}
			//now we have found a filled linked list in vector[temp_val+going_up]
			//keep splitting until you reach your level
			//continually adding the things into free list.
			//but also moving things into proper tiers.
		
		//while(going_up>0){
		for(int i=0;i<free_list.size()-1-temp_val;i++){
				//moving it out
			//cout<<"===BEGINNING LOOP==="<<endl;
			//Jason's attempt
			//unsigned int _a = input->size;
			
			//header* temp = (header*) _a - sizeof(header);
			
			unsigned long long block_size = temp->size;
			
			//unsigned long long _my_a = (unsigned long long) _a - sizeof(header);
			//unsigned long long block_addr = (unsigned long long) _a - sizeof(header);
			unsigned long long block_addr = (unsigned long long) temp;
			unsigned long long offset = block_addr - (unsigned long long) memory;//memory is the beginning of the entire memory allocated in init_alloc
			unsigned long long buddy_offset = offset ^ block_size;
			unsigned long long bud_address = (unsigned long long) memory + buddy_offset;
			
			unsigned long long my_temp = (unsigned long long)temp;//cast temp to my_temp for comparison
			
			cout << endl;
			cout << endl;
			
			//cout<<"passed in hex address: "<<_a<<endl;
			//cout<<"passed in address: "<<(unsigned long long)_a<<endl;
			//cout<<"address of start of block: "<<temp<<endl;
			
			header* bud_address_ptr = (header*) bud_address;
			cout<<"value of passed in address: "<<my_temp<<endl;
			
			cout<<"value of buddy address: "<<bud_address<<endl<<endl;
			//cout<<"value of buddy address: "<<bud_address+sizeof(header)<<endl;
			//cout<<"address of start of buddy block: "<<dec<<bud_address_ptr<<endl;
			cout<<"free status of passed-in block: "<<temp->free<<endl;
			cout<<"free status of buddy block: "<<bud_address_ptr->free<<endl;

			
			temp->free = true;//set the temp to free
			
			cout<<"~~~~~"<<endl;
			cout<<"(updated) free status of passed-in block: "<<temp->free<<endl;
			cout<<"free status of buddy block: "<<bud_address_ptr->free<<endl;
			//cout<<"~~~~~"<<endl;
			header* tempR;
			header* tempL;
			
			if(my_temp < bud_address){//the temp address is the left buddy
				tempL = (header*) my_temp;
				tempR = (header*) bud_address;
			}
			else{//bud_address is the left buddy
				tempL = (header*) bud_address;
				tempR = (header*) my_temp;
				
			}
			//cout<<"~~~~~"<<endl;
			cout<<"free status of left block: "<<tempL->free<<endl;
			cout<<"free status of right block: "<<tempR->free<<endl;
			cout<<"~~~~~"<<endl;
			cout<<"tempR in list["<<temp_val+i<<"]: "<<find_header(tempR,temp_val+i)<<endl;
			cout<<"tempL in list["<<temp_val+i<<"]: "<<find_header(tempL,temp_val+i)<<endl;
			PrintList2();
			cout<<"tempR in list["<<temp_val+i<<"]: "<<find_header(tempR,temp_val+i)<<endl;
			cout<<"tempL in list["<<temp_val+i<<"]: "<<find_header(tempL,temp_val+i)<<endl;
			cout<<"~~~~~"<<endl;
			if(tempL->free == true && tempR->free == true && find_header(tempR,temp_val+i)== true && find_header(tempL,temp_val+i)== true){
				tempL->size=2*(temp->size);//update size
				temp = tempL;
				//tempR->size=2*(temp->size);//update size
				cout<<"tempL size: "<<tempL->size<<endl;
				tempL->free = true;
				cout<<"(coalescing) free_list["<<temp_val+1+i<<"]"<<endl;
				cout<<(unsigned long long)free_list[temp_val+1+i]<<endl;	
				
				remove_flist(tempR,temp_val+i);
				remove_flist(tempL,temp_val+i);
				
				//insert_flist(tempR,temp_val+i+1);
				insert_flist(tempL,temp_val+i+1);
				//tempL->next = free_list[temp_val+1+i];
				//tempL->next = NULL;
			
				//free_list[temp_val+1+i]=tempL;//put the released merged memory blocks back into the freelist
				//bitFlip(free_list[temp_val+1+i]);//update the offset of the next block index (bitFlip does this automatically in the function), we are not actually flipping any bits 
				// cout<<"left block: "<<my_temp<<endl;
				// cout<<"left block: "<<tempL<<endl;
				// cout<<"right block: "<<tempR<<endl;
				
				//unsigned long long foo = (unsigned long long) free_list[temp_val+i];
				// cout<<"free_list["<<temp_val+i<<"]"<<endl;
				// cout<<"next in free list: "<<foo<<endl;
				// cout<<"next in free list: "<<free_list[temp_val+i]->next<<endl;
				//memset(tempR, 0,tempR->size);
				PrintList2();
				//bool done = false;
				//header* current = free_list[temp_val+i];
				
				
				//this might not work if we start freeing all willy-nilly
				/* 
				for(int j=0;j<2;j++){//flush out the two headers from their previous index 
					if(free_list[temp_val+i] != 0){
						cout<<"flushing: "<< (unsigned long long)free_list[temp_val+i]<<endl;
						
						free_list[temp_val+i] = free_list[temp_val+i]->next;
						
						if(free_list[temp_val+i] != 0){
							if(free_list[temp_val+i]->size != block_size){
								cout<<"flushing: "<< (unsigned long long)free_list[temp_val+i]<<endl;
								free_list[temp_val+i] = free_list[temp_val+i]->next;
							}
						}		
					}	
					
				}
				  */
				cout<<(unsigned long long)free_list[temp_val+i]<<endl;
			}
			
			else if(tempL->free == false || tempR->free == false){//case where left or right buddy is not free
				cout<<"SPECIAL CONDITION"<<endl;
				
				return 0;//don't do anything else, we already returned it to the freelist above
				
			}
			cout<<"=====END OF LOOP======"<<endl;
			going_up-=1;
				
		}
		
			
		//}
		
	}
	
  cout<<"asdfasdf"<<endl;
  return 0;
}

void PrintList(){
	
	for(int i=0;i<free_list.size();i++){
		int count = 0;

		header* current = NULL;
		cout<<"size: "<<basic * pow(2,i)<<" ["<<i<<"]: ";
		if(free_list[i] != NULL){
			//cout<<"size: "<<basic * pow(2,i)<<" ["<<i<<"]: "<<free_list[i]->size<<endl;
			
			current = free_list[i];
			while(current!=NULL){
				count++;
				//cout<<count<<endl;
				
				current = current->next;
			}
			//cout<<free_list[i]->size<<endl;
			cout<<count<<endl;
		}
		else{
			cout<<"Empty!"<<endl;
		}
	}
	
}

void PrintList2(){
	
	for(int i=0;i<free_list.size();i++){
		int count = 0;

		header* current = NULL;
		cout<<"size: "<<basic * pow(2,i)<<" ["<<i<<"]: ";
		if(free_list[i] != NULL){
			//cout<<"size: "<<basic * pow(2,i)<<" ["<<i<<"]: "<<free_list[i]->size<<endl;
			
			current = free_list[i];
			while(current!=NULL){
				//count++;
				//cout<<count<<endl;
				cout<<(unsigned long long)current<<", ";
				current = current->next;
			}
			//cout<<free_list[i]->size<<endl;
			//cout<<count<<endl;
			cout<<endl;
		}
		else{
			cout<<"Empty!"<<endl;
		}
	}
	
}


/* 
int main()
{
	
	
	
	
	
	init_allocator(8,256);
	cout<<"origin: "<<(unsigned long long) memory<<endl;
	cout<<"Print List:"<<endl;
	PrintList();
	
	//bitFlip(memory);
	// cout<<"memoryy address: "<<memory<<endl;
	// header* buddyright = (header*) bitFlip(memory);
	// cout<<"buddy right address: "<<buddyright<<endl;
	// buddyright = (header*) bitFlip(buddyright);
	// cout<<"this should be the original memory: "<<buddyright<<endl;
	// cout<<"memoryy address: "<<memory<<endl;
	
	//unsigned long long * block1 = (unsigned long long*) my_malloc(64);
	//cout<<"block1: "<<block1<<endl;
	cout<<"second my malloc==========="<<endl;
	int* block1 = (int*) my_malloc(12);
	int* block2 = (int*) my_malloc(12);
	int* block3 = (int*) my_malloc(12);
	int* block4 = (int*) my_malloc(12);
	
	//int* block5 = (int*) my_malloc(12);
	//cout<<"block2: "<<block2<<endl;
	//unsigned long long block1_int = (unsigned long long) block1;
	//unsigned long long block2_int = (unsigned long long) block2;
	//cout<<"second allocated block: "<<block2_int<<endl;
	//cout<<"first allocated block: "<<block1_int<<endl<<endl;
	
	//cout<<"adjusted second allocated block: "<<block2_int - sizeof(header)<<endl;
	//cout<<"adjusted first allocated block: "<<block1_int - sizeof(header)<<endl<<endl;
	//cout<<"size of header: "<<sizeof(header)<<endl;
	//cout<<"block1: "<<block1<<endl;
	//cout<<"block2: "<<block2<<endl;
	//int* size_header = (int*) sizeof(header);
	//block1 = block1 - sizeof(header);
	//cout<<"block1 - header: "<<block1 - 17<<endl;
	//cout<<"block2 - header: "<<block2 - sizeof(header)<<endl;
	
	cout<<"========================="<<endl;
	cout<<"print list after mallocs:"<<endl;
	PrintList();
	cout<<endl;
	PrintList2();
	
	int check = my_free(block1);
	cout<<"freed block 1 ************"<<endl;
	cout<<"========================="<<endl;
	cout<<"print list in between:"<<endl;
	PrintList2();
	
	cout<<"========================="<<endl;
	cout<<"freeing:"<<endl;
	my_free(block3);
	cout<<"freed block 3 ************"<<endl;
	PrintList2();
	
	
	//my_free(block1);
	my_free(block2);
	//if(check == 0)
	cout<<"freed block 2 ************"<<endl;
	cout<<"========================="<<endl;
	cout<<"print list after frees:"<<endl;
	PrintList2();
	
	my_free(block4);
	
	PrintList();
	cout<<endl;
	PrintList2();
	//my_malloc(3);
	//PrintList();
	
	
	
	return 0;
}
 */
