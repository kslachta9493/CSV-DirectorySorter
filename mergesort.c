#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void merge(CSV input[], int l, int m, int r, int col, int type)
{
   	int i, j, k;
   	int n1 = m - l + 1;
	int n2 =  r - m;
    
    	CSV L[n1], R[n2];
 
 
    	for (i = 0; i < n1; i++)
        	L[i] = input[l + i];
    	for (j = 0; j < n2; j++)
        	R[j] = input[m + 1+ j];
 
 
    	i = 0; 
    	j = 0; 
    	k = l; 
    	int left=0; 
	int right=0;
	float leftf = 0.0;
	float rightf = 0.0;
    
    	while (i < n1 && j < n2)
    	{
		if (type==1) // it is an int
		{  
			if (L[i].filedata[col][0] == ' ')
			{
				input[k] = L[i];
				i++;
			} else if (R[j].filedata[col][0] == ' ')
			{
				input[k] = R[j];
				j++;
			} else
			{
				left = atoi(L[i].filedata[col]);
				right = atoi(R[j].filedata[col]);
        			if ( left<= right )
       				{
            				input[k] = L[i];
            				i++;
      				} else
        			{
       	        			input[k] = R[j];
                			j++;
                		}
			}
                	k++;
		} else if (type==0)  //string
		{
			int testcaps = strcasecmp(L[i].filedata[col], R[j].filedata[col]);
			int test = strcmp(L[i].filedata[col], R[j].filedata[col]);
	 		char *rightc, *leftc;
			if (L[i].filedata[col][0] == ' ')
			{
				input[k] = L[i];
				i++;
			} else if (R[j].filedata[col][0] == ' ')
			{
				input[k] = R[j];
				j++;
			} else {
        		         //char leftc[strlen(L[i].filedata[col])];
       				 //strcpy(leftc, L[i].filedata[col]);
				if (R[j].filedata[col] != NULL) 
				{
					rightc = strdup(R[j].filedata[col]);
				}
				if (L[i].filedata[col] != NULL) 
				{
					leftc = strdup(L[i].filedata[col]);
				}
		
         			int ai=0;
				for(ai = 0; leftc[ai]; ai++){
         				leftc[ai] = (char) tolower(leftc[ai]); 
         			}

	 			for(ai = 0; rightc[ai]; ai++){
         				rightc[ai] = (char) tolower(rightc[ai]); 
         			}

				int aSize=strlen(L[i].filedata[col]);
            			if (aSize>strlen(R[j].filedata[col]))
	    				aSize=strlen(R[j].filedata[col]);
	
            			int x=0; int count=0;
            			for (x=0; x<aSize; x++)
            			{
	    				if (leftc[x]==rightc[x])
	    					count++;
	    				else
   						break;
  
           			}
				free(rightc);
				free(leftc);

				if (count>0)
	      			{   
					if (strcmp(L[i].filedata[col], R[j].filedata[col]) < 0)
       					{
        	    				input[k] = L[i];
        	    				i++;
      					}
        			else
        			{
       			        	input[k] = R[j];
        		        	j++;
        	       		}
		      		}
            			else 
	     			{
					if (strcasecmp(L[i].filedata[col], R[j].filedata[col]) < 0)
       					{
            					input[k] = L[i];
            					i++;
      					}	
        				else
        				{
       	        				input[k] = R[j];
                				j++;
               				}
	     	 		}
			}
                	k++;
		} else
		{ //float
			if (L[i].filedata[col][0] == ' ')
			{
				input[k] = L[i];
				i++;
			} else if (R[j].filedata[col][0] == ' ')
			{
				input[k] = R[j];
				j++;
			} else {
				leftf=atof(L[i].filedata[col]);
				rightf=atof(R[j].filedata[col]);
	        		if ( leftf<= rightf)
	       			{
	            			input[k] = L[i];
	            			i++;
	      			}
	        		else
	        		{
	       	        		input[k] = R[j];
	                		j++;
	                	}
			}
	                k++;

	    	}
	}
 
   	 
   	 while (i < n1)
   	 {
   	     input[k] = L[i];
   	     i++;
   	     k++;
   	 }
 	
   	
   	 while (j < n2)
   	 {
   	     input[k] = R[j];
   	     j++;
   	     k++;
   	 }
}



 

void mergeSort(CSV input[], int l, int r, int col, int type)
{
    if (l < r)
    {

        int m = l+(r-l)/2;
 
        mergeSort(input, l, m, col, type);
        mergeSort(input, m+1, r, col, type);
 
        merge(input, l, m, r, col, type);
    }
}

