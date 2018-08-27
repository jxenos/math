#include <stdio.h>
#include <errno.h>

//https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html#Argument-Syntax

typedef struct coords
{
	double x;
	double y;
} Coords;

double linear_inter(Coords *coords, double input)
{
	float length = sizeof(coords->x) / sizeof(coords->*x);
	double output;

	if (input < coords->x[0] || input > coords->x[(int)length - 1])
	{
		printf("Error: Outside bounds\n");
		return 1;
	}

	int i;
	for (i = 0; i < length; i++)
	{
		if (input == coords->x[i])
		{
			output = coords->y[i];
			break;
		}
		if (input < coords->x[i])
		{
			output = (coords->y[i] - coords->y[i - 1]) * (input - coords->x[i - 1]) / (coords->x[i] - coords->x[i - 1]) + coords->y[i - 1];
			break;
		}
	}

	return output;
};

#include <stdlib.h>
#include <string.h>
#define MAXFLDS 200   /* maximum possible number of fields */
#define MAXFLDSIZE 32 /* longest possible field + 1 = 31 byte field */
void parse(char *record, char *delim, char arr[][MAXFLDSIZE], int *fldcnt)
{
	char *p = strtok(record, delim);
	int fld = 0;

	while (p)
	{
		strcpy(arr[fld], p);
		fld++;
		p = strtok('\0', delim);
	}
	*fldcnt = fld;
}

Coords *csv_parser(char *csv_file)
{
	char tmp[1024] = {0x0};
	int fldcnt = 0;
	char arr[MAXFLDS][MAXFLDSIZE] = {0x0};
	int recordcnt = 0;
	FILE *in = fopen(csv_file, "r"); /* open file on command line */

	if (in == NULL)
	{
		perror("File open error");
		exit(EXIT_FAILURE);
	}
	while (fgets(tmp, sizeof(tmp), in) != 0) /* read a record */
	{
		int i = 0;
		recordcnt++;
		printf("Record number: %d\n", recordcnt);
		parse(tmp, ",", arr, &fldcnt); /* whack record into fields */
		for (i = 0; i < fldcnt; i++)
		{ /* print each field */
			printf("\tField number: %3d==%s\n", i, arr[i]);
		}
	}
	fclose(in);
	return 0;
}

int main(int argc, char **argv)
{
	//float x[] = {1, 100, 200, 300, 450, 600, 800, 1000, 1300, 1600};
	//float y[] = {550, 509.5, 468, 437, 398, 361.5, 331, 297, 265.5, 236};

	Coords *coords = csv_parser("camera_data.cfg");

	double input = 0;

	linear_inter(coords, input);

	return 0;
}
