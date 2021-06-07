#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/*
	ASSIGNMENT:
	1. write a program that implements the LRU page-replacement algorithms
	2. Use the counter implementation
	3. without command line parameters, the default reference string should be [7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1]
		and should have 3 memory frames
	4. program should take the following parameters from the command line:
		a. number of frames
		b. number of pages
		c. a reference string (optional)
	5. when the number of pages is passed in, the program should randomly generate a reference string with the number of pages specified
	6. program should print the number of poge faults at the end
	7. assume page numbers range from 0-9
	8. assume that demand paging is used (meaning the frames will begin as empty
*/



//global variables with default values
int page_fault_counter = 0; //counts number of page faults
int temp; //used for holding the generated random numbers

int lru_value; //stores the time of the least recently used value
int temp_value; //stores the temporary time value being compared
int lru_index; //stores the index of the lru_value

bool hit = false; //used for checking if there is a hit or a fault


//page replacement function
void pageReplacement(int frame_amount, int page_amount, int* ref_string)
{
	//create frame arrays
	int time_of_use[frame_amount];
	int frames[frame_amount];
	int past_frames[frame_amount];
	
	//initialize them to -1
	for(int i = 0; i < frame_amount; i++)
	{
		frames[i] = -1;
		past_frames[i] = -1;
	}
	//set values of time_of_use in descending order (just for easier initial replacement)
	for(int i = frame_amount - 1; i > 0; i--)
	{
		
		time_of_use[i] = i;
	}
	
	//LRU algorithm implementation
	for(int i = 0; i < page_amount; i++)
	{
		//print the reference string
		printf("%d\t", ref_string[i]);
		
		//checks for hits/faults
		for(int j = 0; j < frame_amount; j++)
		{
			if(frames[j] == ref_string[i])
			{
				hit = true;
				lru_index = j; //stores the index of the value where the hit occurred (if there are multiple hits of the same value, it will take the higher index value)
			}
		}
		
		if(hit == true) //occurs if there is a hit
		{
			printf("HIT\t");
			
			//increments the time_of_use[] value of every index
			for(int k = 0; k < frame_amount; k++)
			{
				time_of_use[k] += 1;
			}
			
			//perform the page replacement
			frames[lru_index] = ref_string[i];
			
			//resets the time_of_use[] value of the corresponding frames[] value
			time_of_use[lru_index] = 1;
			
		}
		
		else //occurs if there is a fault
		{
			printf("FAULT\t");
			page_fault_counter++;
			lru_index = 0;
			
			//search for least recently used value
			lru_value = time_of_use[0];
			for(int k = 0; k < frame_amount; k++)
			{
				//store the lru value and it's index for later
				temp_value = time_of_use[k];
				
				if(lru_value < temp_value)
				{
					lru_value = temp_value;
					lru_index = k;
				}
			}
			
			//replace lru value in the frames[] array using the index of it's time_of_use counterpart previously found
			frames[lru_index] = ref_string[i];
			
			//increment the values of every index in the time_of_use[] array (the value at the lru index is going to be changed after, doesn't matter if it is incremented now)
			for(int k = 0; k < frame_amount; k++)
			{
				time_of_use[k] += 1;
			}
			
			//reset the time_of_use[] value that corresponds with the newly added frame[] value
			time_of_use[lru_index] = 1;
			lru_index = 0; //reset the lru index back to zero for future usage
			
		}
		
		hit = false; //resets the bool value back to default
		
		
		//print frames[] before page replacement (past_frames[])
		printf("[");
		for(int j = 0; j < frame_amount; j++)
		{
			//if-else statement to determine if tab is used
			if(j < (frame_amount - 1))
			{
				printf("%d\t", past_frames[j]);
			}
			else
			{
				printf("%d", past_frames[j]);
			}
		}
		printf("]\t");
		
		//print frames[] after page replacement (frames[])
		printf("[");
		for(int j = 0; j < frame_amount; j++)
		{
			//if-else statement to determine if tab is used
			if(j < (frame_amount - 1))
			{
				printf("%d\t", frames[j]);
			}
			else
			{
				printf("%d", frames[j]);
			}
		}
		printf("]\n");
		
		//set past_frames[] equal to frames[]
		for(int j = 0; j < frame_amount; j++)
		{
			past_frames[j] = frames[j];
		}
	}
}



//main method
int main(int argc, char* argv[]) {
	
	srand(time(0));
	printf("%d", argc);
	//initialize variables
	int num_of_frames; //number of frames
	int num_of_pages; //number of pages
	
	//takes in parameters from command line
	char* a = argv[1]; //number of frames
	char* b = argv[2]; //number of pages
	
	//sets the global variables equal to the parameters passed in on the command line, or sets them to a default value
	int reference_string[100];
	int def_reference_string[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}; //default reference string
	
	size_t n = sizeof(def_reference_string) / sizeof(def_reference_string[0]);
	int size_of_def = (int) n;
	
	
	if(argc == 1)
	{
		num_of_frames = 3;
		num_of_pages = 20;
		
		for(int i = 0; i < 20; i++)
		{
			reference_string[i] = def_reference_string[i];
		}
	}
	else if(argc == 3)
	{
		int io_frames = atoi(a);
		num_of_frames = io_frames;
		
		int io_pages = atoi(b);
		num_of_pages = io_pages;
		
		//generates random number between 0-9 and adds it to the reference string
		for(int i = 0; i < (num_of_pages - 1); i++)
		{
			temp = rand() % 9 + 1;
			reference_string[i] = temp;
			temp = 0;
		}
	}
	else
	{
		printf("no");
		exit(1);
	}
	
	
	//LRU implementation
	printf("Number of frames:\t%d \n", num_of_frames);
	printf("Number of pages:\t%d \n", num_of_pages);
	printf("_______________________________________________________________\n");
	
	pageReplacement(num_of_frames, num_of_pages, reference_string);
	
	printf("\nNumber of page faults: %d\n", page_fault_counter);
	
	
	return 0;
}
