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

typedef struct coords_array
{
	Coords *coords_array;
	int length;
} Coords_Array;

double linear_inter(Coords_Array coords_array, double input)
{

	Coords *coords = coords_array.coords_array;
	int length = coords_array.length;

	printf("input: %f\n", input);

	double output;
	printf("length: %f\n", length);
	printf("0.x: %f, last.x: %f\n", coords[0].x, coords[length - 1].x);

	if (input < coords[0].x || input > coords[length - 1].x)
	{
		fprintf(stderr, "Error: Outside bounds\n");
		exit(0);
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

Coords_Array csv_parser(char *csv_file)
{

	FILE *file = fopen(csv_file, "r");

	//ensure we have a file or error
	if (file == NULL)
	{
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	// go to the end of the file to get length
	fseek(file, 0x0, SEEK_END);
	unsigned file_length = ftell(f);

	fseek(f, 0x0, SEEK_SET);

	//clear a memory block to store the file data in
	char *file_data = malloc(file_length + 1);
	memset(file_data, 0x0, file_length + 1);

	//copy the file into memory
	fread(file_data, 1, file_length, file);
	fclose(file);

	char delimiter = ',';
	unsigned rowcnt = 0;
	int i = file_length;
	while (i--)
	{
		if (file_data[i] == delimiter)
			rowcnt++;
	}

	//char **rows = malloc(sizeof(char*) * rowcnt);
	//memset(rows, 0, sizeof(char*) * rowcnt);

	Coords *data = malloc(sizeof(Coords) * rowcnt);
	memset(data, 0, sizeof(Coords) * rowcnt);

	for (i = 0; i < rowcnt; i++)
	{
		temp = strtok(file_data, '\n');
		data[i].x = strtok(temp, delimiter);
		data[i].y = temp;
	}

	//======================================================================
	/*
		char tmp[1024] = {0x0};
	int fldcnt = 0;
	char arr[MAXFLDS][MAXFLDSIZE] = {0x0};

	while (fgets(tmp, sizeof(tmp), in) != 0) // read a record
	{
		int i = 0;
		rowcnt++;
		printf("Record number: %d\n", rowcnt);
		parse(tmp, ",", arr, &fldcnt); // whack record into fields
		for (i = 0; i < fldcnt; i++)
		{ // print each field
			printf("\tField number: %3d==%s\n", i, arr[i]);
		}
	}
	fclose(in);
*/
	/*
	Coords *coords = malloc(sizeof(Coords) * 11);

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
	coords[9].y = 236;*/

	return (Coords_Array){.coords_array = data, .length = rowcnt};
}

int main(int argc, char **argv)
{
	char *camera_config = NULL;
	double input;

	int c;

	while ((c = getopt(argc, argv, "i:c:")) != -1)
	{
		switch (c)
		{
		case 'c':
			camera_config = strdup(optarg);
			break;
		case 'i':
			input = atof(optarg);
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

	Coords_Array coords_array = csv_parser(camera_config);

	double output = linear_inter(coords_array, input);

	printf("%f\n", output);

	return 0;
}
