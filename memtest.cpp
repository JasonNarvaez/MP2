#include "ackerman.h"
#include "my_allocator.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char ** argv) {

	unsigned long long b, M = 0;
	int opt;
	//char input[256];
	while((opt = getopt(argc, argv, "b::s::")) != -1){
		
		switch (opt){
			case 'b':{
				if(argv[2]) b = atoi(argv[2]);
				else b=32;
				//printf("b: %s \n",argv[2]);
				break;
			}
			case 's':{
				if(argv[4]) M = atoi(argv[4]);
				else M = 128*1024*1024;
				//printf("M: %s \n",argv[4]);
				break;
			}
			default:{
				fprintf(stderr, "Usage: %s [-b blocksize] [-s] memsize\n",argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		
	}
	if (b == 0 || M == 0 ){
		b = 32;
		M = 128*1024*1024;
	}
	printf("using values:\nb: %d \nM: %d \n",b,M);
	
	// init_allocator(basic block size, memory length)
	init_allocator(b,M);
	
	//init_allocator(32,128*1024*1024);
	
  
	
	ackerman_main();
	
	//	my_allocator DEMO
  /* 
	init_allocator(32,256*32);
	PrintList();
	cout<<endl;
	PrintList2();
	cout<<endl;
	int* block1 = (int*) my_malloc(64);//allocates 128 block
	int* block2 = (int*) my_malloc(12);//allocates 32 block
	int* block3 = (int*) my_malloc(12);//allocates 32 block
	int* block4 = (int*) my_malloc(32);//allocates 64 block
	cout<<"PRINTING AFTER ALLOCATING BLOCKS"<<endl;
	PrintList();
	cout<<endl;
	PrintList2();
	
	my_free(block1);
	my_free(block2);
	my_free(block3);
	my_free(block4);
	cout<<endl<<"PRINTING AFTER FREEING BLOCKS"<<endl;
	PrintList();
	cout<<endl;
	PrintList2();
	 */
	//atexit(release_allocator());
  release_allocator();
}
