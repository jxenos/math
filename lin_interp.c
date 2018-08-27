#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
	printf("input: %f\n", input);
	float length = sizeof(coords) / sizeof(coords[0]);
	double output;
	printf("length: %f\n", length);
	printf("0.x: %f, last.x: %f\n", coords[0].x, coords[(int)length - 1].x);

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

	Coords *coords = malloc(sizeof(Coords) * 10);

	coords[0].x = 1;
	coords[1].x = 100;
	coords[2].x = 200;
	coords[3].x = 300;
	coords[4].x = 450;
	coords[5].x = 600;
	coords[6].x = 800;
	coords[7].x = 1000;
	coords[8].x = 1300;
	coords[9].x = 1600;

	coords[0].y = 550;
	coords[1].y = 509.5;
	coords[2].y = 468;
	coords[3].y = 437;
	coords[4].y = 398;
	coords[5].y = 361;
	coords[6].y = 331;
	coords[7].y = 297;
	coords[8].y = 265.5;
	coords[9].y = 236;

	return coords;
}

int main(int argc, char **argv)
{
	//float x[] = {1, 100, 200, 300, 450, 600, 800, 1000, 1300, 1600};
	//float y[] = {550, 509.5, 468, 437, 398, 361.5, 331, 297, 265.5, 236};
	char *camera_config;
	double input;

	int c;

	while ((c = getopt(argc, argv, "c:i:")) != -1)
	{
		switch (c)
		{
		case 'c':
			strcpy(camera_config, optarg);
			break;
		case 'i':
			sscanf(optarg, "%lf", input);
			break;
		case '?':
			if (optopt == 'c')
			{
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			}
			else if (optopt == 'i')
			{
				fprintf(stderr, "Option -%i requires an argument.\n", optopt);
			}
			else if (isprint(optopt))
			{
				fprintf(stderr, "Unkown option '-%c'.\n", optopt);
			}
			else
			{
				fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
			}
			return 1;
		default:
			abort();
		}
	}

	printf("c: %s\ni: %f\n", camera_config, input);

	Coords *coords = csv_parser("camera_data.cfg");

	int x;

	for (x = 0; x < 10; x++)
	{
		printf("%f %f\n", coords[x].x, coords[x].y);
	}

	double output = linear_inter(coords, input);

	printf("%f\n", output);

	return 0;
}
