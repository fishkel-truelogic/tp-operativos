/*
 * properties.c
 *
 *  Created on: 16/09/2014
 *      Author: lfishkel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fereString.h"
#include "fereTypes.h"

#define BUFFER_LENGTH 16
#define PROPERTIES_LENGTH 5

#define CONFIG_FILE_NAME "config.txt"

#define FILE_ERROR "There was an error reading the %s, please check if that file exists\n"

typedef struct Property {
	String name;
	String value;
} Property;

Property* newProperty(String, String);
int readConfigFile();


Property* properties[PROPERTIES_LENGTH] = { NULL };

String getProperty(const String propertyName) {
	int i;
	if (properties[0] != NULL) {
		for (i = 0; i < PROPERTIES_LENGTH; i++) {
			if (strcmp(properties[i]->name, propertyName) == 0) {
				return properties[i]->value;
			}
		}
	} else {
		if (readConfigFile()) {
			return getProperty(propertyName);
		} else {
			printf(FILE_ERROR, CONFIG_FILE_NAME);
		}
	}
	return NULL ;
}

int readConfigFile() {
	Int32U fileCharacter;
	Int8U count = 0;
	String name;
	String value;
	FILE *file;
	Boolean isName = TRUE;

	file = fopen(CONFIG_FILE_NAME, "r");
	if (file) {
		name = newString(BUFFER_LENGTH);
		value = newString(BUFFER_LENGTH);
		while ((fileCharacter = getc(file)) != EOF) {
			if (strcmp((String) &fileCharacter, "\n") == 0) {
				isName = TRUE;
				properties[count] = newProperty(name, value);
				count += 1;
				name = newString(BUFFER_LENGTH);
				value = newString(BUFFER_LENGTH);
			} else {
				if (strcmp((String) &fileCharacter, "=") == 0) {
					isName = FALSE;
				} else {
					if (isName) {
						strcat(name, (String) &fileCharacter);
					} else{
						strcat(value, (String) &fileCharacter);
					}
				}
			}
		}
		fclose(file);
	} else {
		return FALSE;
	}
	return TRUE;
}

Property* newProperty(String name, String value) {
	Property* p = (Property*) malloc(sizeof(Property));
	p->name = name;
	p->value = value;
	return p;
}
