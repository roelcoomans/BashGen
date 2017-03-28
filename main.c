#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "util.h"

list *l = NULL;
char *help = NULL;
size_t size_help = 0;
char *functions = NULL;
size_t size_fuctions = 0;
char *cases = NULL;
size_t size_cases = 0;

static void getInput(void)
{
	bool result;
	char c[100];
	char command[100] = { 0 }, *cm;
	char description[100] = { 0 }, *desc;
	char input[100];

	printf("Provide one character: ");
	fgets(c, sizeof(c) / sizeof(c[0]), stdin);
	printf("Provide a full command name (optional): ");
	cm = fgets(command, sizeof(command) / sizeof(command[0]), stdin);
	dropLineFeed(cm);
	printf("Provide a description (optional): ");
	desc = fgets(description, sizeof(description) / sizeof(description[0]), stdin);
	dropLineFeed(desc);
	do {
		printf("Does it receive an input? (y/n): ");
		fgets(input, sizeof(input) / sizeof(input[0]), stdin);
	} while (input[0] != 'y' && input[0] != 'n');

	if (true == list_add(l, c[0],
			     (cm[0] == '\0') ? NULL : cm,
			     (desc[0] == '\0') ? NULL : desc,
			     (input[0] == 'y') ? true : false)) {
		printf("Item added successfully\n");
	} else {
		printf("Failed to add item\n");
	}
}

/* Create help function */
static void createHelp(node *data)
{
	bool error = false;
	size_t size = 24;

	size += (NULL != data->command) ? strlen(data->command) : 0;
	size += (NULL != data->description) ? strlen(data->description) : 0;
	int out_size;
	char *buffer;

	do {
		buffer = (char*)malloc(size);
		if (NULL == buffer) {
			error = true;
			break;
		}
		if (NULL != data->command && NULL != data->description) {
			out_size = snprintf(buffer, size, "\techo \"-%c --%s %s | %s\"\n",
					    data->c, data->command, (true == data->input) ? "INPUT" : "", data->description);
		} else if (NULL == data->command && NULL != data->description) {
			out_size = snprintf(buffer, size, "\techo \"-%c %s | %s\"\n",
					    data->c, (true == data->input) ? "INPUT" : "", data->description);
		} else if (NULL != data->command && NULL == data->description) {
			out_size = snprintf(buffer, size, "\techo \"-%c --%s %s\"\n",
					    data->c, data->command, (true == data->input) ? "INPUT" : "");
		} else {
			out_size = snprintf(buffer, size, "\techo \"-%c %s\"\n",
					    data->c, (true == data->input) ? "INPUT" : "");
		}
		if (out_size < -1) {
			error = true;
			break;
		}
	} while (0);
	if (false == error) {
		safeCat(&help, &size_help, buffer, out_size);
	}
	if (NULL != buffer) {
		free(buffer);
	}
}


static void createFunctions(node *data)
{
	bool error = false;
	size_t size = 128;

	size += (NULL != data->command) ? strlen(data->command) * 2 : 0;
	int out_size;
	char *buffer;

	do {
		buffer = (char*)malloc(size);
		if (NULL == buffer) {
			error = true;
			break;
		}
		if (NULL != data->command) {
			out_size = snprintf(buffer, size, "function %s() {\n\techo \"function %s\"\n}\n\n",
					    data->command, data->command);
		} else {
			out_size = snprintf(buffer, size, "function f_%c() {\n\techo \"function f_%c\"\n}\n\n",
					    data->c, data->c);
		}
		if (out_size < -1) {
			error = true;
			break;
		}
	} while (0);
	if (false == error) {
		replaceDash(buffer);
		safeCat(&functions, &size_fuctions, buffer, out_size);
	}
	if (NULL != buffer) {
		free(buffer);
	}
}

static void createCases(node *data)
{
	bool error = false;
	size_t size = 128;

	size += (NULL != data->command) ? strlen(data->command) : 0;
	int out_size;
	char *buffer;

	do {
		buffer = (char*)malloc(size);
		if (NULL == buffer) {
			error = true;
			break;
		}
		if (NULL != data->command) {
			if (true == data->input) {
				char *variable = strdup(data->command);
				replaceDash(variable);
				toUpperCase(variable);
				out_size = snprintf(buffer, size, "\t\t-%c=*|--%s=*)\n\t\t\tVAR_%s=\"${i#*=}\"\n\t\t\t%s\n\t\t\t;;\n",
						    data->c, data->command, variable, data->command);
				free(variable);
			} else {
				out_size = snprintf(buffer, size, "\t\t-%c|--%s)\n\t\t\t%s\n\t\t\t;;\n",
						    data->c, data->command, data->command);
			}
		} else {
			if (true == data->input) {
				char variable[6] = "VAR_";
				variable[4] = (char)data->c;
				variable[5] = '\0';
				toUpperCase(variable);
				out_size = snprintf(buffer, size, "\t\t-%c=*)\n\t\t\t%s=\"${i#*=}\"\n\t\t\tf_%c\n\t\t\t;;\n",
						    data->c, variable, data->c);
			} else {
				out_size = snprintf(buffer, size, "\t\t-%c)\n\t\t\tf_%c\n\t\t\t;;\n\n",
						    data->c, data->c);
			}
		}
		if (out_size < -1) {
			error = true;
			break;
		}
	} while (0);
	if (false == error) {
		safeCat(&cases, &size_cases, buffer, out_size);
	}
	if (NULL != buffer) {
		free(buffer);
	}
}

static void writeToFile(void)
{
	do {
		FILE *f = fopen("out.sh", "w+");
		if (f == NULL) {
			printf("Error opening file!\n");
			break;
		}
		fprintf(f, "#!/bin/bash\n\n");

		/* create help */
		char *msg = "function showHelp() {\n\techo \"This bash script provides following argument list, input arguments are equals (=) separated\"\n";
		safeCat(&help, &size_help, msg, strlen(msg));
		list_foreach(l, createHelp);
		msg = "\techo \"-h --help | Show this help description\"\n}\n\n";
		safeCat(&help, &size_help, msg, strlen(msg));
		fprintf(f, help);

		/* create functions */
		list_foreach(l, createFunctions);
		fprintf(f, functions);

		/* create cases */
		msg = "for i in \"$@\"\ndo\n\tcase $i in\n";
		safeCat(&cases, &size_cases, msg, strlen(msg));
		list_foreach(l, createCases);
		msg = "\t\t-h|--help)\n\t\t\tshowHelp\n\t\t\t;;\n";
		safeCat(&cases, &size_cases, msg, strlen(msg));
		msg = "\tesac\ndone\n";
		safeCat(&cases, &size_cases, msg, strlen(msg));
		fprintf(f, cases);

		fclose(f);

		printf("out.sh file created successfully\n");
	} while (0);
}

static void printHelp()
{
	printf("0. Exit\n");
	printf("1. Create new item\n");
	printf("2. Write to file\n");
}

int main(int argc, char const *argv[])
{
	char c[10];

	/* Initialize the output strings, this is needed for the first strcat,
	   they will be realloced according to the correct size */
	help = calloc(1, 1);
	functions = calloc(1, 1);
	cases = calloc(1, 1);

	l = list_new();
	while (NULL != l) {
		printHelp();
		printf("> ");
		fgets(c, sizeof(c) / sizeof(c[0]), stdin);
		switch ((char)c[0]) {
		case '0':
			list_free(l);
			l = NULL;
			free(help);
			help = NULL;
			free(functions);
			functions = NULL;
			free(cases);
			cases = NULL;
			break;
		case '1':
			getInput();
			break;
		case '2':
			writeToFile();
			break;
		}
	}
	return 0;
}
