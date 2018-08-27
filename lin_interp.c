#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXFLDS 200   /* maximum possible number of fields */
#define MAXFLDSIZE 32 /* longest possible field + 1 = 31 byte field */

//https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html#Argument-Syntax

typedef struct coords
{
	double x;
	double y;
} Coords;

double linear_inter(Coords *coords, double input)
{
	float length = sizeof(coords) / sizeof(*coords);
	double output;

	if (input < coords[0].x || input > coords[(int)length - 1].x)
	{
		printf("Error: Outside bounds\n");
		return 1;
	}

	int i;
	for (i = 0; i < length; i++)
	{
		if (input == coords[i].x)
		{
			output = coords[i].y;
			break;
		}
		if (input < coords[i].x)
		{
			output = (coords[i].y - coords[i - 1].y) * (input - coords[i - 1].x) / (coords[i].x - coords[i - 1].x) + coords[i - 1].y;
			break;
		}
	}

	return output;
};

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

	Cords *coords = malloc(sizeof(Cords) * 10);

	coords[1].x = 1;
	coords[2].x = 100;
	coords[3].x = 200;
	coords[4].x = 450;
	coords[5].x = 600;
	coords[6].x = 800;
	coords[7].x = 1000;
	coords[8].x = 1300;
	coords[9].x = 1600;

	coords[1].y = 550;
	coords[2].y = 509.5;
	coords[3].y = 468;
	coords[4].y = 437;
	coords[5].y = 398;
	coords[6].y = 361;
	coords[7].y = 331;
	coords[8].y = 265.5;
	coords[9].y = 236;

	return coords;
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
