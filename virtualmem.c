#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h> 



typedef struct pageNode
{
	int page;
	struct pageNode *next;
}pageNode;


//Default Parameters
int usage_history=0;
int available_frames=5;
int replacement_policy=0;//0:FIFO	1:LFU	2:LRU-STACK	  3:LRU-CLOCK	4:LRU-REF8
char *myinputfile=NULL;
int refStrLen=0;
//


int main(int argc, char* argv[])
{
	initiaParameters(argv);
	//printf("\n%d %d %d %s\n",usage_history,available_frames,replacement_policy,myinputfile);

	if(usage_history==1)
		{
			usage(argv[0]);
		}

	if(myinputfile==NULL)
	printf("\nRead from STDIN");
	
	//File Reading
		FILE *fp = fopen(myinputfile, "r");
		int valid=1;
		pageNode *start=NULL;
		pageNode *last=NULL;
		refStrLen=0;
		while (!feof(fp))
		{
			char buffer[80]; 
			int n = 0;
			int ch;
			for (ch= fgetc(fp); ch != EOF && ch!=' '; ch = fgetc(fp)) 
			{
				buffer[n++] = ch;
			}
			buffer[n] = '\0';
			
			valid=isnumeric(buffer);
			if(valid==1)
			{
				if(atoi(buffer)<0)
				valid=0;
			}

			if(valid==0)
				break;
			else if(valid==1)
			{
				refStrLen++;
				if(start!=NULL)
				{
					pageNode *newNode;
					newNode=(pageNode*)malloc(sizeof(pageNode));
					newNode->page=atoi(buffer);
					newNode->next=NULL;
					last->next=newNode;
					last=newNode;
				}
				else{
					//printf("start=NULL");
					start=(pageNode*)malloc(sizeof(pageNode));
					start->page=atoi(buffer);
					start->next=NULL;
					last=start;
				}
			}	
			
			//printf("%s\n", buffer);
		}
		
		if(valid==1)
			printf("Valid Input");
		else
		{
			printf("Invalid Input");
			exit(0);
		}

		pageNode *temp=NULL;
		temp=start;
		int referenceStrng[refStrLen];
		int i;
		//printf("\n%d %d",refStrLen,start->page);
		for(i=0;i<refStrLen && temp!=NULL ;i++)
		{
			referenceStrng[i]=temp->page;
			temp=temp->next	;
			//printf("%d ",referenceStrng[i]);
		}	
		fclose(fp);
	//File Reading Ends

		time_t s1,s2;
		int pgr_cnt_optimal;
		long time_optimal;
		double penalty;
		struct timeval tv;
		
			switch(replacement_policy)
			{
				case 0:
					
					
					s1 = time (NULL);
					int pgr_cnt_fifo = implementFIFO(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					long time_fifo=s2-s1;
					
					s1 = time (NULL);
					pgr_cnt_optimal = implementOptimalAlgo(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					time_optimal=s2-s1;
					
					printf("\n# of page replacements with FIFO: %d",pgr_cnt_fifo);
					printf("\n# of page replacements with Optimal: %d",pgr_cnt_optimal);
					
					penalty = ((double)abs(pgr_cnt_fifo-pgr_cnt_optimal))/pgr_cnt_optimal;
					
					printf("\npercentage page replacement penalty using FIFO:%f",(penalty*100));
					
					printf("\nTotal time to run FIFO algorithm: %ld sec",time_fifo);
					printf("\nTotal time to run Optimal algorithm: %ld sec",time_optimal);
					
					if(time_fifo < time_optimal)
					printf("\nFIFO is %f faster than Optimal Algorithm",(((double)(time_optimal-time_fifo)/time_optimal)*100));
					else
					printf("\nFIFO is %f slower than Optimal Algorithm",(((double)(time_fifo-time_optimal)/time_optimal)*100));
					
				break;
				
				case 1:
				
					
					s1 = time (NULL);
					int pgr_cnt_lfu = implementLFU(available_frames,referenceStrng,refStrLen);					
					s2 = time (NULL);
					long time_lfu=s2-s1;
					
					s1 = time (NULL);
					pgr_cnt_optimal = implementOptimalAlgo(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					time_optimal=s2-s1;

					printf("\n# of page replacements with LFU: %d",pgr_cnt_lfu);
					printf("\n# of page replacements with Optimal: %d",pgr_cnt_optimal);
					penalty=(double)abs(pgr_cnt_lfu-pgr_cnt_optimal)/pgr_cnt_optimal;
					
					printf("\npercentage page replacement penalty using LFU:%f",(penalty*100));
					
					printf("\nTotal time to run LFU algorithm: %ld sec",time_lfu);
					printf("\nTotal time to run Optimal algorithm: %ld sec",time_optimal);
					
					if(time_lfu < time_optimal)
					printf("\nLFU is %f faster than Optimal Algorithm",(((double)(time_optimal-time_lfu)/time_optimal)*100));
					else
					printf("\nLFU is %f slower than Optimal Algorithm",(((double)(time_lfu-time_optimal)/time_optimal)*100));
				break;
				
				case 2:
					
					s1 = time (NULL);
					int pgr_cnt_lrustack = implementLRUStack(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					long time_lrustack=s2-s1;
					
					s1 = time (NULL);
					pgr_cnt_optimal = implementOptimalAlgo(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					time_optimal=s2-s1;

					printf("\n# of page replacements with LRU-STACK: %d",pgr_cnt_lrustack);
					printf("\n# of page replacements with Optimal: %d",pgr_cnt_optimal);
					penalty=(double)abs(pgr_cnt_lrustack-pgr_cnt_optimal)/pgr_cnt_optimal;
					
					printf("\npercentage page replacement penalty using LRU-STACK:%f",(penalty*100));
					
					printf("\nTotal time to run LRU-STACK algorithm: %ld sec",time_lrustack);
					printf("\nTotal time to run Optimal algorithm: %ld sec",time_optimal);
					
					if(time_lrustack < time_optimal)
					printf("\nLRU-STACK is %f faster than Optimal Algorithm",(((double)(time_optimal-time_lrustack)/time_optimal)*100));
					else
					printf("\nLRU-STACK is %f slower than Optimal Algorithm",(((double)(time_lrustack-time_optimal)/time_optimal)*100));
					
					
				break;
				
				case 3:
					
					s1 = time (NULL);
					int pgr_cnt_lruclock = implementLruClock(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					long time_lruclock=s2-s1;
					
					s1 = time (NULL);
					pgr_cnt_optimal = implementOptimalAlgo(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					time_optimal=s2-s1;

					printf("\n# of page replacements with LRU-CLOCK: %d",pgr_cnt_lruclock);
					printf("\n# of page replacements with Optimal: %d",pgr_cnt_optimal);
					penalty=(double)abs(pgr_cnt_lruclock-pgr_cnt_optimal)/pgr_cnt_optimal;
					
					printf("\npercentage page replacement penalty using LRU-CLOCK:%f",(penalty*100));
					
					printf("\nTotal time to run LRU-CLOCK algorithm: %ld sec",time_lruclock);
					printf("\nTotal time to run Optimal algorithm: %ld sec",time_optimal);
					
					if(time_lruclock < time_optimal)
					printf("\nLRU-CLOCK is %f faster than Optimal Algorithm",(((double)(time_optimal-time_lruclock)/time_optimal)*100));
					else
					printf("\nLRU-CLOCK is %f slower than Optimal Algorithm",(((double)(time_lruclock-time_optimal)/time_optimal)*100));
				break;
				
				case 4:
					
					s1 = time (NULL);
					int pgr_cnt_refbit = implementLruRef8Bit(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					long time_refbit=s2-s1;
					
					s1 = time (NULL);
					pgr_cnt_optimal = implementOptimalAlgo(available_frames,referenceStrng,refStrLen);
					s2 = time (NULL);
					time_optimal=s2-s1;

					printf("\n# of page replacements with LRU-REF8: %d",pgr_cnt_refbit);
					printf("\n# of page replacements with Optimal: %d",pgr_cnt_optimal);
					penalty=(double)abs(pgr_cnt_refbit-pgr_cnt_optimal)/pgr_cnt_optimal;
					
					printf("\npercentage page replacement penalty using LRU-REF8:%f",(penalty*100));
					
					printf("\nTotal time to run LRU-REF8 algorithm: %ld sec",time_refbit);
					printf("\nTotal time to run Optimal algorithm: %ld sec",time_optimal);
					
					if(time_refbit < time_optimal)
					printf("\nLRU-REF8 is %f faster than Optimal Algorithm",(((double)(time_optimal-time_refbit)/time_optimal)*100));
					else
					printf("\nLRU-REF8 is %f slower than Optimal Algorithm",(((double)(time_refbit-time_optimal)/time_optimal)*100));
					
				break;
			}

	return 0;
}

void usage(char *argv)
{
		fprintf(stderr,"\n\nUsage Summary\n");
		fprintf(stderr, "usage: %s [−h] [-f available-frames] [−r replacement-policy] [−i input_file]\n",argv);
        fprintf(stderr, "OPTIONS:\n");
        fprintf(stderr, "−h: Print a usage summary with all options and exit.\n");
        fprintf(stderr,"-f available-frames: Set the number of available frames. By default it should be 5.\n");
        fprintf(stderr,"−r replacement-policy : Set the page replacement policy. It can be either\n");
        fprintf(stderr,"\t\tFIFO (First-in-first-out)\n");	
        fprintf(stderr,"\t\tLFU (Least-frequently-used)\n");
        fprintf(stderr,"\t\tLRU-STACK (Least-recently-used stack implementation)\n");
        fprintf(stderr,"\t\tLRU-CLOCK ((Least-recently-used clock implementation second chance alg.).\n");
        fprintf(stderr,"\t\tLRU-REF8 (Least-recently-used Reference-bit Implementation, using 8 reference bits)\n");
        fprintf(stderr,"\t\tThe default will be FIFO.\n");
        fprintf(stderr,"−i input file: Read the page reference sequence from a specified file. If not given,\n");
		fprintf(stderr,"\tthe sequence should be read from STDIN (ended with ENTER).\n");	
        exit(0);
}
void initiaParameters(char *argv[])
{
	int i=0;
	for(i=0;argv[i]!=NULL;i++)
	{
		if(strcmp(argv[i],"-h")==0)
		{
			//printf("\n%s",argv[i]);
			usage_history = 1;
		}
		else if(strcmp(argv[i],"-f")==0)
		{
			//printf("\n %s",argv[i]);
			i=i+1;
			available_frames=atoi(argv[i]);
		}
		else if(strcmp(argv[i],"-r")==0)
		{
			//printf("\n %s",argv[i]);
			i=i+1;
			if(strcmp(argv[i],"FIFO")==0)
				replacement_policy = 0;
			else if(strcmp(argv[i],"LFU")==0)
				replacement_policy = 1;
			else if(strcmp(argv[i],"LRU-STACK")==0)
				replacement_policy = 2;
			else if(strcmp(argv[i],"LRU-CLOCK")==0)
				replacement_policy = 3;
			else if(strcmp(argv[i],"LRU-REF8")==0)
				replacement_policy = 4;			
		}
		else if(strcmp(argv[i],"-i")==0)
		{
			//printf("\n %s",argv[i]);
			i=i+1;
			if(argv[i]!=NULL)
			myinputfile=argv[i];
		}
	}
}

int* fileRead(char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	int valid=1;
	pageNode *start=NULL;
	pageNode *last=NULL;
	refStrLen=0;
	while (!feof(fp))
	{
		char buffer[80]; 
		int n = 0;
		int ch;
		for (ch= fgetc(fp); ch != EOF && ch!=' '; ch = fgetc(fp)) 
		{
			buffer[n++] = ch;
		}
		buffer[n] = '\0';
		
		valid=isnumeric(buffer);
		if(valid==1)
		{
			if(atoi(buffer)<0)
			valid=0;
		}

		if(valid==0)
			break;
		else if(valid==1)
		{
			refStrLen++;
			if(start!=NULL)
			{
				pageNode *newNode;
				newNode=(pageNode*)malloc(sizeof(pageNode));
				newNode->page=atoi(buffer);
				newNode->next=NULL;
				last->next=newNode;
				last=newNode;
			}
			else{
				//printf("start=NULL");
				start=(pageNode*)malloc(sizeof(pageNode));
				start->page=atoi(buffer);
				start->next=NULL;
				last=start;
			}
		}	
		
		//printf("%s\n", buffer);
	}
	
	if(valid==1)
		printf("Valid Input");
	else
	{
		printf("Invalid Input");
		exit(0);
	}

	pageNode *temp=NULL;
	temp=start;
	int referenceStrng[refStrLen];
	int i;
	//printf("\n%d %d",refStrLen,start->page);
	for(i=0;i<refStrLen && temp!=NULL ;i++)
	{
		referenceStrng[i]=temp->page;
		temp=temp->next;
		//printf("%d ",referenceStrng[i]);
	}
	
	fclose(fp);
	return referenceStrng;
}


int isnumeric(char *str)
{
  if(isdigit(*str))
  {
	return 1;
  }
  else
  {
	return 0;
  }
}

void implementLruRef8Bit(int frameSize, int referenceStrng[], int len)
		{
			int *uniqueElements;//DS
			uniqueElements=(int*)malloc(sizeof(len));
			int i,j,found=0;
			int count_unique=0;
			
			//Find the unique count and array in ref string
			for(i=0;i<len;i++)
			{
				found=0;
				for(j=0;j<count_unique;j++)
				{
					if(referenceStrng[i]==uniqueElements[j])
					{
						found=1;
						break;
					}
				}
				if(found==0)
				{
					uniqueElements[j]=referenceStrng[i];
					count_unique++;
				}
			}
			
			//Initialize 2D Matrix
			int refBit[count_unique][9];//DS
			
			for(i=0;i<count_unique;i++)
			{
				refBit[i][0]=uniqueElements[i];
				//printf("%d ",refBit[i][0]);
				for(j=1;j<9;j++)
				{
					refBit[i][j]=0;
					//printf("%d ",refBit[i][j]);
				}
				//printf("\n");
			}
			
			int *frameArr;//Frame Array
			frameArr=(int*)malloc(frameSize*sizeof(int));
			int pageHit=0, pageReplacement=0, pageFault=0;
			int currentPosi=0,itr;
			int row;
			int k;
			int bit[8];
			
			for(i=0;i<frameSize;i++)
				frameArr[i]=-1;
			
			for(i=0;i<len;i++)
			{
				for(itr=0;itr<frameSize;itr++)
				{
					if(frameArr[itr]==-1)
					{
						pageFault++;
						frameArr[itr]=referenceStrng[i];
						shiftRight(referenceStrng[i],refBit,count_unique);
						break;
					}
					else if(frameArr[itr]==referenceStrng[i])
					{
						pageHit++;
						shiftRight(referenceStrng[i],refBit,count_unique);
						break;
					}
					currentPosi=(currentPosi+1)%frameSize;
				}
				
				if(itr==frameSize)
				{
				  pageReplacement++;
				  int min=9999;
				  int min_index_frame=-1;
				  int min_page=-1;
				  for(j=0;j<frameSize;j++)
				  {
					  for(row=0;row < count_unique;row++)
					  {
						  if(refBit[row][0]==frameArr[j])
						  {
							  for(k=0;k<8;k++)
								bit[k]=refBit[row][k+1];
							  int value=binaryToDecimal(bit);
							  //printf("\nPage:%d value:%d\n",refBit[row][0],value);
							  if(value < min)
							  {
								  min=value;
								  min_index_frame=j;
								  min_page=frameArr[j];
							  }
							  break;
						  }
					  }
				  }
				  frameArr[min_index_frame]=referenceStrng[i];
				  shiftRight(referenceStrng[i],refBit,count_unique);
				}
				//for(k=0;k<frameSize;k++)
					//printf("%d ",frameArr[k]);
				//printf("\n");
			}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			free(frameArr);
			
			return pageReplacement;
		}
		
		void shiftRight(int page, int refBit[][9],int count_unique)
		{
			int i,j;
			int bit[8];
			for(i=0;i<count_unique;i++)
			{
				for(j=1;j<8;j++)
				bit[j]=refBit[i][j];
				
				for(j=1;j<8;j++)
				refBit[i][j+1]=bit[j];
				
				if(refBit[i][0]==page)
					refBit[i][1]=1;
				else
					refBit[i][1]=0;
			}
		}
		
		int binaryToDecimal(int bit[])
		{
			int result=0;
			int i;
			
			for(i=0;i<8;i++)
			{
				if(bit[i]==1)
				{
					result=result+pow(2,7-i);
				}
			}
			return result;
		}

int implementLruClock(int frameSize, int referenceStrng[], int len)
		{
			//Data Structure Required
			typedef struct frameNode{
				int page;
				int refBit;
				struct frameNode *next;
			}frameNode;
			
			int i,j;
			int pageHit=0, pageReplacement=0, pageFault=0;
			frameNode *start=NULL;
			frameNode *currentPosi=NULL;
			int cLen=0;//Length of circular link list
			for(i=0;i<len;i++)
			{
				if(start!=NULL)
				{
					frameNode *temp=start;
					frameNode *prev=NULL;
					int pageFound=0;
					
					if(cLen < frameSize)
					{
						while(temp!=NULL)
						{
							if(temp->page==referenceStrng[i])
							{
								temp->refBit=1;
								pageHit++;
								pageFound=1;
								break;		
							}
							prev=temp;
							temp=temp->next;
						}
						
						if(temp==NULL && pageFound==0)
						{
							frameNode *newNode;
							newNode=(frameNode*)malloc(sizeof(frameNode));
							newNode->page=referenceStrng[i];
							newNode->refBit=0;
							newNode->next=NULL;
							prev->next=newNode;
							currentPosi=newNode;
							cLen++;
							pageFault++;
							//printf("\nCreated newnode for %d",newNode->page);
						}
						if(cLen==frameSize)
						{
							currentPosi->next=start;//That makes circular linked list
							//printf("\ncLen==frameSize");
						}
					}
					else if(cLen==frameSize)
					{
						//First traverse and search for page
						//Keep a list of pages with refBit=1
						//If it is hit, make refBit=1 and flush list of pages
						//Else if is miss then replace the page with refbit=0 by making all refbit 1=0 on the way
						temp=currentPosi;
						pageFound=0;
						for(j=0;j<frameSize;j++)
						{
							if(temp->page==referenceStrng[i])
							{
								temp->refBit=1;
								pageHit++;
								pageFound=1;
								//printf("\nPage hit for %d",temp->page);
								break;
							}
							else{
								temp=temp->next;
							}
						}
						if(pageFound==1)
						{
							currentPosi=temp;
						}
						else//Find page for replacement by making refbit 1 = 0;
						{
							pageFault++;
							pageReplacement++;
							int pageReplaced=0;
							temp=currentPosi->next;
							for(j=0;j<frameSize;j++)
							{
								if(temp->refBit==1)
								{
									temp->refBit=0;
								}
								else
								{
									//printf("\nPage Replaced:%d for %d in ref strng",temp->page,referenceStrng[i]);
									temp->page=referenceStrng[i];
									currentPosi=temp;
									pageReplaced=1;
									break;
								}
								temp=temp->next;
							}
							if(pageReplaced==0)//If refbit of all pages was 1 then replace next page
							{
								if(temp->refBit==0)
								{
									temp->page=referenceStrng[i];
									currentPosi=temp;
								}
							}	
						}
					}
				}
				else
				{
					start=(frameNode*)malloc(sizeof(frameNode));
					start->page=referenceStrng[i];
					start->refBit=0;
					start->next=NULL;
					currentPosi=start;
					pageFault++;
					cLen++;
					//printf("\nCreated newnode:start:%d",start->page);
				}
			}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			return pageReplacement;
		}


int implementOptimalAlgo(int frameSize, int referenceStrng[], int len)
		{	
			//Data Structure Required	
			int *frameArr;
			frameArr=(int*)malloc(frameSize*sizeof(int));
			
			int currentPosi=0;
			int pageHit=0, pageReplacement=0, pageFault=0;
			
			int i,itr,j,k;
			for(i=0;i<frameSize;i++)
			{
				frameArr[i]=-1;
			}
			
			for(i=0;i<len;i++)
			{
				for(itr=0;itr<frameSize;itr++)
				{
				   if(frameArr[itr]==-1)
				   {
					 frameArr[itr]=referenceStrng[i];
					 currentPosi=(currentPosi+1)%frameSize;
					 pageFault++;
					 break;
				   }
				   else if(frameArr[itr]==referenceStrng[i])
				   {
					 pageHit++;
					 currentPosi=(currentPosi+1)%frameSize;
					 break;
				   }
				   currentPosi=(currentPosi+1)%frameSize;
			    }
			    
			    if(itr==frameSize)
			    {
					int victimPageIndex=-1;
					int maxIndex=-1;
					pageFault++;
					pageReplacement++;
					for(j=0;j<frameSize;j++)
					{
						for(k=i+1;k<len;k++)
						{
							if(referenceStrng[k]==frameArr[j])
							{
								if(k>maxIndex)
								{
									maxIndex=k;
									victimPageIndex=j;
								}
								break;
							}
						}
						if(k==len)//Page in frameArray is not present in ref string
						{
							victimPageIndex=j;
							break;
						}
					}
					if(victimPageIndex!=-1)
					frameArr[victimPageIndex]=referenceStrng[i];
				}
					//for(j=0;j<frameSize;j++)
						//printf(" %d ",frameArr[j]);
					//printf("\n");
			}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			free(frameArr);
		return pageReplacement;
		}



int implementLRUStack(int frameSize, int referenceStrng[], int len)
		{
			//Data Structure Used
			typedef struct frameNode{
				int page;
				struct frameNode *next;
			}frameNode;
			
			frameNode *start=NULL;
			int lenLinkList=0;
			int pageHit=0, pageFault=0, pageReplacement=0;
			int i=0;
			for(i=0;i<len;i++)
			{
				if(start!=NULL)
				{
					frameNode *temp=start;
					frameNode *prev=NULL;
					lenLinkList=0;
					while(temp!=NULL && temp->page!=referenceStrng[i])
					{
						prev=temp;
						temp=temp->next;
						lenLinkList++;
					}
					if(temp!=NULL && temp->page == referenceStrng[i])//Page Hit Case
					{
						pageHit++;
						prev->next=temp->next;
						free(temp);//
						frameNode *newNode;
						newNode=(frameNode*)malloc(sizeof(frameNode));
						newNode->page=referenceStrng[i];
						newNode->next=start;
						start=newNode;
					}
					else if(temp==NULL)//Page Miss and Page Replacement
					{
						frameNode *newNode;
						newNode=(frameNode*)malloc(sizeof(frameNode));
						newNode->page=referenceStrng[i];
						newNode->next=start;
						start=newNode;
						if(lenLinkList == frameSize){
							//printf("\nPrev:%d\n",prev->page);
							//prev=NULL;//Remove the Least Recently Used Node
							frameNode *traverse=start;
							frameNode *prev_traverse=NULL;
							while(traverse->next!=NULL)
							{
								prev_traverse=traverse;
								traverse=traverse->next;
							}
							prev_traverse->next=NULL;
							free(traverse);
							pageReplacement++;
						}
						pageFault++;
					}
			}
			else{
					start=(frameNode*)malloc(sizeof(frameNode));
					start->page=referenceStrng[i];
					start->next=NULL;
			}
			//frameNode *temp=start;
			//while(temp!=NULL){
				//printf("%d ",temp->page);
				//temp=temp->next;
			//}
			//printf("\n");
		}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			if(start!=NULL)
				free(start);
				
				return pageReplacement;
		}
		
		
	int implementFIFO(int frameSize, int referenceStrng[],int len)
		{	
			//Data Structure used
			int i,j;
			int *frameArr;// Frame Array pointer
			frameArr=(int *)malloc(frameSize*sizeof(int));
			
			//int len=sizeof(referenceStrng)/sizeof(int);
			int fifoPointer=0;//Keeps the track of page that came in first
			int pageHit=0,pageFault=0,pageReplacement=0;//Page Replacement Algorithms Parameters
			int currentPointer=0;//Current Pointer for frame array
			int itr=0;
			
			for(i=0;i<frameSize;i++)//Initialize the frame Array with -1
			{
				frameArr[i]=-1;
			}
			
			
			for(i=0;i<len;i++)
			{
				for(itr=0;itr<frameSize;itr++)
				{
					if(frameArr[itr]==-1)//This is used for initial run when all frames are available
					{
						frameArr[itr]=referenceStrng[i];
						pageFault++;
						currentPointer=(currentPointer+1)%frameSize;//D
						break;
					}
					else if(frameArr[itr]==referenceStrng[i])//Page Hit Case
					{
						pageHit++;
						currentPointer=(currentPointer+1)%frameSize;//D
						break;
					}
					currentPointer=(currentPointer+1)%frameSize;//Move the pointer in either case
				}
				
				if(itr==frameSize)//If the page referenced is not found in frame array
				{
					frameArr[fifoPointer]=referenceStrng[i];
					pageFault++;
					pageReplacement++;
					fifoPointer=(fifoPointer+1)%frameSize;
				}
				
				//for(j=0;j<frameSize;j++)
					//printf(" %d ",frameArr[j]);
				//printf("\n");
			}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			free(frameArr);
			return pageReplacement;
		}
		
		
		int implementLFU(int frameSize,int referenceStrng[],int len)
		{
			//Data Structure Required
			
			int *frameArr;
			frameArr=(int *)malloc(frameSize*sizeof(int));
			
			int *indexInRefStr;
			indexInRefStr=(int *)malloc(frameSize*sizeof(int));
			
			int i,j,k;
			int itr;
			int currentPosi=0;
			int pageFault=0,pageHit=0,pageReplacement=0;
			
			typedef struct pageNode{
				struct pageNode *next;
				int count;
				int page;
			}pageNode;
		
			pageNode *start=NULL;
			
			
			for(i=0;i<frameSize;i++)//Initialize Frame and Index Array to -1
			{
				frameArr[i]=-1;
				indexInRefStr[i]=-1;
			}
			//printf("\nInitialized Frame Array\n");
		
			for(i=0;i<len;i++)//For each page
			{
				//printf("\nReferenceStrng[i]: %d",referenceStrng[i]);
				for(itr=0;itr<frameSize;itr++)//Search in the frame array
				{
					if(frameArr[itr]==-1)//Frame Array Vacant
					{
						frameArr[itr]=referenceStrng[i];
						indexInRefStr[itr]=i;
						pageFault++;
						if(start!=NULL)//Not an empty linked list
						{
							pageNode *temp=start;
							while(temp->next!=NULL)temp=temp->next;
							pageNode *newNode=(pageNode*)malloc(sizeof(pageNode));
							newNode->page=referenceStrng[i];
							newNode->count=1;
							newNode->next=NULL;
							temp->next=newNode;
						}
						else //Empty Linked List
						{
							start=(pageNode*)malloc(sizeof(pageNode));
							start->next=NULL;
							start->count=1;
							start->page=referenceStrng[i];
						}
						currentPosi=(currentPosi+1)%frameSize;
						break;
					}//End if(frameArr[currentPosi]==-1)
					else if(frameArr[itr]==referenceStrng[i])
					{
						pageNode *temp=start;
						while(temp!=NULL && temp->page!=referenceStrng[i])
						temp=temp->next;
						if(temp!=NULL)
						{
							temp->count++;
						}
						pageHit++;
						currentPosi=(currentPosi+1)%frameSize;
						break;
					}//End else if(frameArr[currentPosi]==referenceStrng[i])
					currentPosi=(currentPosi+1)%frameSize;
				}//End For: Frame Array
				
				if(itr==frameSize)//Page not found. Now Find a Victim
				{
					int sameMinFlag=0;
					pageFault++;
					pageReplacement++;
					
					pageNode *temp=start;
					int min=9999;
					int min_page=-1;
					int found=0;
					while(temp!=NULL)//Finding a page with minimum count i.e Least Frequently Used
					{
						found=0;
						for(k=0;k<frameSize;k++)
						{
							if(frameArr[k]==temp->page)
							{
								found=1;
								break;
							}
						}
						if(found==1)
						{
							if(min > temp->count)
							{
								min=temp->count;
								min_page=temp->page;//Check whether this page is present in FrameArr
							}
							else if(min==temp->count)
								sameMinFlag=1;
						}	
						temp=temp->next;
					}//end While
					
					if(sameMinFlag==0)//Only 1 page with minimum count
					{
						//Search in frameArr with min_page and replace
						int min_index=-1;
						for(k=0;k<frameSize;k++)
						{
							if(frameArr[k]==min_page)
							{
								min_index=k;
								break;
							}
						}
						if(min_index!=-1)
						{
							frameArr[min_index]=referenceStrng[i];
							indexInRefStr[min_index]=i;
						}
						pageNode *temp=start;
						pageNode *prev;
						while(temp!=NULL && temp->page!=referenceStrng[i])
						{
							prev=temp;
							temp=temp->next;
						}
						if(temp!=NULL)
						{
							temp->count++;
						}
						else if(temp==NULL && prev!=NULL){
							pageNode *newNode=(pageNode*)malloc(sizeof(pageNode));
							newNode->count=1;
							newNode->next=NULL;
							newNode->page=referenceStrng[i];
							prev->next=newNode;
						}
					}//end if(sameMinFlag==0)
					else if(sameMinFlag==1)//Tie Breaker(i.e More than 1 page with same min): USE FIFO
					{
						int min_index=9999;
						int min_index_frame=-1;
						pageNode *temp=start;
						int consider=0;
						for(j=0;j<frameSize;j++)//Find the page which came first
						{
							consider=0;
							temp=start;
							while(temp!=NULL)
							{
								if(temp->page==frameArr[j] && min==temp->count)
								{
									consider=1;
									break;
								}
								temp=temp->next;
							}
							
							if(consider==1 && min_index > indexInRefStr[j])
							{
								min_index = indexInRefStr[j];
								min_index_frame=j;
							}
						}
						
								frameArr[min_index_frame]=referenceStrng[i];
								indexInRefStr[min_index_frame]=i;
								temp=start;
								pageNode *prev;
								while(temp!=NULL && temp->page!=referenceStrng[i])
								{
									prev=temp;
									temp=temp->next;
								}
								if(temp!=NULL)
								{
									temp->count++;
								}
								else if(temp==NULL && prev!=NULL)
								{
									pageNode *newNode=(pageNode*)malloc(sizeof(pageNode));
									newNode->count=1;
									newNode->next=NULL;
									newNode->page=referenceStrng[i];
									prev->next=newNode;
								}
					
					}
				}
				//for(j=0;j<frameSize;j++)
					//printf(" %d ",frameArr[j]);	
				//printf("\n");
			}
			//printf("\nPage Fault=%d, Page Hit=%d, Page Replacement=%d",pageFault,pageHit,pageReplacement);
			
			free(frameArr);
			free(indexInRefStr);
			if(start!=NULL)
			free(start);
			return pageReplacement;
		}//End Implement LFU
