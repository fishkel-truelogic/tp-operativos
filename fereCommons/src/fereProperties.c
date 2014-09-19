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
#include "commons/collections/list.h"

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


t_list* properties;

String getProperty(const String propertyName) {
	int i;
	if (properties != NULL && !list_is_empty(properties)) {
		for (i = 0; i < list_size(properties); i++) {
			Property* property = (Property*) list_get(properties, i);
			if (property != NULL && strcmp(property->name, propertyName) == 0) {
				return property->value;
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
	String name;
	String value;
	FILE *file;
	Boolean isName = TRUE;

	properties = list_create();
	file = fopen(CONFIG_FILE_NAME, "r");
	if (file) {
		name = newString(BUFFER_LENGTH);
		value = newString(BUFFER_LENGTH);
		while ((fileCharacter = getc(file)) != EOF) {
			if (strcmp((String) &fileCharacter, "\n") == 0) {
				isName = TRUE;
				Property* property = newProperty(name, value);
				list_add(properties, property);
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
