#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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

static int compare(const void *a, const void *b)
{
	if (((Coords *)a)->x < ((Coords *)b)->x)
		return -1;
	if (((Coords *)a)->x == ((Coords *)b)->x)
		return 0;
	if (((Coords *)a)->x > ((Coords *)b)->x)
		return 1;
}

static double linear_inter(Coords_Array coords_array, double input)
{
	Coords *coords = coords_array.coords_array;
	int length = coords_array.length;

	qsort((void *)coords, length, sizeof(*coords), compare);

	double output;

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

static Coords_Array csv_parser(char *csv_file)
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
	unsigned file_length = ftell(file);

	fseek(file, 0x0, SEEK_SET);

	//clear a memory block to store the file data in
	char *file_data = malloc(file_length + 1);
	memset(file_data, 0x0, file_length + 1);

	//copy the file into memory
	fread(file_data, 1, file_length, file);
	fclose(file);

	char *delimiter = ",";
	unsigned rowcnt = 0;
	int i = file_length;
	while (i--)
	{
		if (file_data[i] == delimiter[0])
			rowcnt++;
	}

	char **rows = malloc(sizeof(char *) * rowcnt);
	memset(rows, 0, sizeof(char *) * rowcnt);

	Coords *data = malloc(sizeof(Coords) * rowcnt);
	memset(data, 0, sizeof(Coords) * rowcnt);
	char *temp;
	i = 0;
	for (temp = strtok(file_data, "\n"); temp != NULL; temp = strtok(NULL, "\n"))
	{
		rows[i++] = temp;
	}

	for (i = 0; i < rowcnt; i++)
	{
		temp = strtok(rows[i], delimiter);
		data[i].x = atof(temp);
		temp = strtok(NULL, delimiter);
		data[i].y = atof(temp);
	}

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
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
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

	Coords_Array coords_array = csv_parser(camera_config);

	double output = linear_inter(coords_array, input);

	printf("%f\n", output);

	return 0;
}
