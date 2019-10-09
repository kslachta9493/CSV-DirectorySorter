#include <stdio.h>
#include "builder.h"
#include "sorter.h"
#include <stdlib.h>
#include <string.h>



void buildCSV(char* holder, CSV input[], char* structinput, int linesize, int rows, int cols)
{
	int i = 0;
	int j = 0;
	int q = 0;
	int line = 0;
	int removewhite = 0;
	int ignore = 0;
	int structline = 0;


	for (i = 0; i < linesize; i++)
	{
		structinput[i] = '\0';
	}
	i = 0;
	while (line < rows)
	{
		while (holder[i] != '\n')
		{
			if (holder[i] == '"' && ignore == 0)
			{
				ignore = 1;
				i++;
			} else if (holder[i] == '"' && ignore == 1)
			{
				ignore = 0;
				i++;
			}
			
			if (removewhite == 1 && holder[i] != ' ')
			{
				removewhite = 0;
			}

			if (ignore == 1)
			{
				structinput[j] = holder[i];
				j++;
			} else if (removewhite == 0) 
			{
				if (holder[i] == ',')
				{
					j = 0;
					if (structinput[0] == '\0')
					{
						//if the first input is null terminated nothing was found between the commas so we set a default value we can use to sort on later
						structinput[0] = ' ';
					}
      					input[line].filedata[structline] = strdup(structinput);
					
					
					
					structline++;
					//Cleaning the structinput so it is always null terminated before using it
					for (q = 0; q < linesize; q++)
					{
						structinput[q] = '\0';
					}
					removewhite = 1;
				} else {
					structinput[j] = holder[i];
					j++;
				}
			}
			i++;
		}

     		input[line].filedata[structline] = strdup(structinput);

		j = 0;
		structline++;
		if (structline == cols)
		{
			structline = 0;
		}
		//Cleaning the structinput so it is always null terminated before using it
		for (q = 0; q < linesize; q++)
		{
			structinput[q] = '\0';
		}
		line++;
		i++;
	}

}



void printSorted (CSV input[], int rows, int cols, char* useroutputdirectory)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			while (input[i].filedata[j][strlen(input[i].filedata[j])-1] == ' ')
    			{
  			  input[i].filedata[j][strlen(input[i].filedata[j])-1] = '\0';
  		        }
			
			if (strchr(input[i].filedata[j], ','))
			{
				int cursize = strlen(input[i].filedata[j]);
				char *tmp = (char*) malloc ((cursize + 2) * sizeof(char));
				int start;
				for (start = 0; start < cursize + 2; start++)
				{
					tmp[start] = '\0';
				}

				for (start = 1; start < cursize; start++) {
					tmp[start] = input[i].filedata[j][start - 1];
				}
					tmp[0] = '"';
					tmp[cursize - 1] = '"';
					input[i].filedata[j] = strdup(tmp);
					free(tmp);
			}
		}
			
	}
				
	FILE* fp;
	fp = fopen(useroutputdirectory, "w");
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (input[i].filedata[j][0] == ' ') { 
				input[i].filedata[j] = "\0";
			}
			if (j==cols-1)
			{
				fprintf(fp, "%s", input[i].filedata[j]);
				//printf("%s", input[i].filedata[j]);
			} else
			{
				fprintf(fp, "%s,", input[i].filedata[j]);
				//printf("%s,", input[i].filedata[j]);
			}
			
		}

		fprintf(fp, "\n");
		//printf("\n");
		fflush(fp);
	}
	fclose(fp);
}

