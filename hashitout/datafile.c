#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "datafile.h"

int
df_load(struct hashtable_t *ht, char *filename)
{
	FILE *file;
	char line[DFLINEBUFLEN];
	char *key, *val;
	int nlines, lineno;
	struct simpledata *databuf;

	/* Open the datafile, or return an error */
	file = fopen(filename, "r");
	if (file == NULL)
		return (-1);

	/*
	 * Calculate the total number of lines in the file.
	 * We will use this to determine the initial size of the
	 * hashtable we will create to store the contents.
	*/
	nlines = 0;
	while (fgets(line, sizeof(line), file) != NULL)
		nlines++;

	/* Set the file stream position back to the beginning of the file */
	rewind(file);

	lineno = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		/* Increment the line number for reporting errors */
		lineno++;

		/* Replace the newline character with a NULL */
		line[strlen(line) - 1] = '\0';

		/* Split the line into a key-value pair */
		key = strtok(line, DELIM);
		val = strtok(NULL, DELIM);

		/* Place the val into our data struct */
		databuf = malloc(sizeof(struct simpledata));
		databuf->id = lineno;
		databuf->text = strndup(val, BUFSIZ);

		/*
		 * Validate the key and value
		 * If the key or value is NULL, then we have hit an issue
		 * with the constistency of the datafile. Until I have
		 * determined exactly how I want to deal with this,
		 * we will just error out, printing the corrupt line
		 * to the user and returning less than 0...
		 */
		if ((key == NULL) || (val == NULL)) {
			fprintf(stderr,
			    "error: could not parse datafile, line: %d\n",
			    lineno);
			return (-1);
		}

		/* Insert the line into the hashtable */
		ht_insert(ht, key, databuf);
	}

	return (0);
}
