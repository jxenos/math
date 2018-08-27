#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXFLDS 200     /* maximum possible number of fields */
#define MAXFLDSIZE 32   /* longest possible field + 1 = 31 byte field */
void parse( char *record, char *delim, char arr[][MAXFLDSIZE],int *fldcnt)
{
    char*p=strtok(record,delim);
    int fld=0;
    
    while(p)
    {
        strcpy(arr[fld],p);
		fld++;
		p=strtok('\0',delim);
	}		
	*fldcnt=fld;
}
int main(int argc, char *argv[])
{
	char tmp[1024]={0x0};
	int fldcnt=0;
	char arr[MAXFLDS][MAXFLDSIZE]={0x0};
	int recordcnt=0;	
	FILE *in=fopen(argv[1],"r");         /* open file on command line */
	
	if(in==NULL)
	{
		perror("File open error");
		exit(EXIT_FAILURE);
	}
	while(fgets(tmp,sizeof(tmp),in)!=0) /* read a record */
	{
	    int i=0;
	    recordcnt++;
		printf("Record number: %d\n",recordcnt);
		parse(tmp,",",arr,&fldcnt);    /* whack record into fields */
		for(i=0;i<fldcnt;i++)
		{                              /* print each field */
			printf("\tField number: %3d==%s\n",i,arr[i]);
		}
	}
    fclose(in);
    return 0;	
}
