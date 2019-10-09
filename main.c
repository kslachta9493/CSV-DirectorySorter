
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"
#include "builder.h"
#include <dirent.h>

extern struct node *BST;
extern int maxColMain;


int threadcount = 0;
int totalcsv = 0;
int threadmax = 5000;
int coltosorton = -1;
SORTERARGS* argstruct;

pthread_t *threads;
pthread_mutex_t lock;
char *userinputdirectory = NULL;
char *useroutputdirectory = NULL;

//Function Declarations
void *dirsort(void*);
void *sort(void*);
void doublethread();
void printHeader(FILE* fps);

char colnames[28][50] = {
	"color",
	"director_name",
	"num_critic_for_reviews",
	"duration",
	"director_facebook_likes",
	"actor_3_facebook_likes",
	"actor_2_name",
	"actor_1_facebook_likes",
	"gross",
	"genres",
	"actor_1_name",
	"movie_title",
	"num_voted_users",
	"cast_total_facebook_likes",
	"actor_3_name",
	"facenumber_in_poster",
	"plot_keywords",
	"movie_imdb_link",
	"num_user_for_reviews",
	"language",
	"country",
	"content_rating",
	"budget",
	"title_year",
	"actor_2_facebook_likes",
	"imdb_score",
	"aspect_ratio",
	"movie_facebook_likes"
};



int main (int argc, char* argv[])
{

	int i = 0;
	int parentpid = getpid();
	int freeinput = 0;
	argstruct = (SORTERARGS *) malloc (sizeof(SORTERARGS) * threadmax);

	for (i = 0; i < threadmax; i++)
	{
		(argstruct + i)->argc = argc;
	}

	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("MUTEX INIT FAILED");
		free(argstruct);
		exit(0);
	}
	printf("Initial PID: %d\n", parentpid);
	printf("Child TIDS: ");
	fflush(stdout);


	int valid = validflags(argc, argv);
	if (valid == -1)
	{
		free(argstruct);
		pthread_mutex_destroy(&lock);
		exit(-1);
	} 
	else
	{
		coltosorton = valid;
		for (i = 0; i < threadmax; i++)
		{
			(argstruct + i)->coltosorton = valid;
		}
	}

	if (userinputdirectory == NULL)
	{
		userinputdirectory = (char*) malloc (2*sizeof(char));
		userinputdirectory[0] = '.';
		userinputdirectory[1] = '\0';
		for (i = 0; i < threadmax; i++)
		{
			(argstruct + i)->inputdirectory = strdup(userinputdirectory);
		}

		//free(userinputdirectory);
	} else {
		FILE* fp;
		fp = fopen(userinputdirectory, "r");
	
		if (fp == NULL)
		{
			printf("\nInvalid input directory %s\n", userinputdirectory);
			exit(-1);
		}
		close(fp);
	}
	if (useroutputdirectory == NULL)
	{
		
		useroutputdirectory = (char*) malloc (2*sizeof(char));
		useroutputdirectory[0] = '.';
		useroutputdirectory[1] = '\0';
		
		//useroutputdirectory = strdup(userinputdirectory);
	} else
	{
		FILE* fp;
		fp = fopen(useroutputdirectory, "r");
	
		if (fp == NULL)
		{
			printf("\nInvalid output directory %s\n", useroutputdirectory);
			exit(-1);
		}
		close(fp);
	}
	char *outfile;
	outfile = (char*) malloc (strlen(useroutputdirectory) + 17*sizeof(char) + strlen(colnames[argstruct->coltosorton]));

	strcat(outfile, useroutputdirectory);
	strcat(outfile, "/All-sorted-");
	strcat(outfile, colnames[coltosorton]);
	strcat(outfile, ".csv");

	FILE* fps;
	fps = fopen(outfile, "w");
	if (fps == NULL)
	{
		printf("Error creating output file\n");
		close(fps);
	}

	threads = (pthread_t *) malloc (threadmax * sizeof(pthread_t));
	printf("%s\n", userinputdirectory);
	populateDIR(userinputdirectory);

	//wait for all threads

	for (i = 0; i < threadcount; i++)
	{
		pthread_join(threads[i], NULL);
		if (i == 0)
			printf(" %d", threads[i]);
		else
			printf(", %d", threads[i]);
	}

	printf("\nTotal Threads: %d\n", threadcount);

	printHeader(fps);
	fflush(fps);
	printBST(BST, maxColMain, fps);
	close(fps);
	if (freeinput)
		free(userinputdirectory);

	return 0;
}
void printHeader(FILE* fps)
{
	int i = 0;
	for (i = 0; i < 28; i++)
	{
		if (i == 27)
			fprintf(fps, "%s", colnames[i]);
		else
			fprintf(fps, "%s,", colnames[i]);
	}

}
int validflags(int argc, char* argv[])
{
	int i = 0;
	int acceptedargs = -1;
	int j = 0;
	int k = 0;
	int c = -1;
	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-c") == 0)
		{
			 c = 1;
			if (argv[i + 1] != NULL)
			{
				for (k = 0; k < 28; k++)
				{
					if (strcmp(colnames[k], argv[i + 1]) == 0)
					{
						acceptedargs = k;
						for (j = 0; j < threadmax; j++)
						{
							(argstruct + j)->colname = strdup(argv[i + 1]);
						}
					}
				}
				if (acceptedargs == -1)
				{
					printf("\nInvalid sorting string %s. Be sure it matches a column header! CASE SENSITIVE!\n", argv[i + 1]);
				}
			}
		}
		if (strcmp(argv[i], "-d") == 0)
		{
			if (argv[i + 1] != NULL)
				userinputdirectory = strdup(argv[i + 1]);
		}
		if (strcmp(argv[i], "-o") == 0)
		{
			if (argv[i + 1] != NULL)
				useroutputdirectory = strdup(argv[i + 1]);
		}
	}
	if (c == -1)
		printf("\nYou must include a col name to sort on! use -c colname to define it\n");

	return acceptedargs;
}

int populateDIR(char *name)
{
    	DIR *dir;
    	struct dirent *entry;

   	if (!(dir = opendir(name)))
        	return 0;

	while ((entry = readdir(dir)) != NULL) 
	{
		if (entry->d_type == DT_DIR) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
			} else
			{
		    		char *path = (char*) malloc ((strlen(name) + strlen(entry->d_name) + 2) * sizeof(char));
				strcpy(path, name);
				strcat(path, "/");
				strcat(path, entry->d_name);

				pthread_mutex_lock(&lock);
				if (threadcount == threadmax)
				{
					doublethread();
				}
				pthread_create(&threads[threadcount], NULL, dirsort, (void *) path);
				threadcount++;
				pthread_mutex_unlock(&lock);

			}
        	} else if (entry->d_type == DT_REG) 
		{
			int length = strlen(entry->d_name);
			if (entry->d_name[length - 1] == 'v' && entry->d_name[length - 2] == 's' && entry->d_name[length - 3] == 'c' && entry->d_name[length - 4] == '.')
			{
				if (!strstr(entry->d_name, "-sorted-"))
				{
					pthread_mutex_lock(&lock);

					if (threadcount == threadmax)
					{
						doublethread();
					}

					(argstruct + totalcsv)->filename = strdup(entry->d_name);
					(argstruct + totalcsv)->inputdirectory = strdup(name);
					pthread_create(&threads[threadcount], NULL, sort, (void *) (argstruct + totalcsv));
					totalcsv++;
					threadcount++;

					pthread_mutex_unlock(&lock);
				}
			}
       		}
    	}
	closedir(dir);
	return 0;
}
void *dirsort(void *args)
{
	char *dirname = (char *) args;
	populateDIR(dirname);
	pthread_exit(0);
}
void *sort(void *args)
{
	initsorting((void *) args);
	//loadAndSort();
	pthread_exit(0);
}
void doublethread()
{
	int large = threadmax * 2;
	pthread_t *tmp = (pthread_t *) realloc (threads, large*sizeof(pthread_t));
	if (tmp == NULL)
	{
		printf("Malloc FAILED!\n");
		exit(-1);
	} else 
	{
		threads = tmp;
		threadmax = large;
	}
}

