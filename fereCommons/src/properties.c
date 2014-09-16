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

#define BUFFER_LENGTH 16
#define PROPERTIES_LENGTH 5

#define TRUE 1
#define FALSE 0
#define SLEEP_MILI "RETARDO"
#define KERNEL_IP "IP_KERNEL"
#define KERNEL_PORT "PUERTO_KERNEL"
#define MSP_IP "IP_MSP"
#define MSP_PORT "PUERTO_MSP"
#define CPU_CONFIG_FILE_NAME "cpu.config.txt"

int readConfigFile();
char* getPropertyValue(char*);

char** properties = NULL;

char* getProperty(const char* propertyName) {
	int i;
	if (properties != NULL) {
		for (i = 0; *(properties + i); i++) {
			if (strstr(properties[i], propertyName) != NULL ) {
				return getPropertyValue(properties[i]);
			}
		}
	} else {
		readConfigFile();
		return getProperty(propertyName);
	}
	return NULL ;
}

int readConfigFile() {
	int c;
	char* property;
	char** ptrPoperties;
	FILE *file;

	properties = (char**) malloc(
			sizeof(char) * BUFFER_LENGTH * PROPERTIES_LENGTH);
	ptrPoperties = properties;

	file = fopen(CPU_CONFIG_FILE_NAME, "r");
	if (file) {
		property = (char*) malloc(sizeof(char) * BUFFER_LENGTH);
		while ((c = getc(file)) != EOF) {
			if (strcmp((char*) &c, "\n") == 0) {
				*ptrPoperties = property;
				ptrPoperties += 1;
				property = (char*) malloc(sizeof(char) * BUFFER_LENGTH);
			} else {
				strcat(property, (char*) &c);
			}
		}
		fclose(file);
	} else {
		return FALSE;
	}

	return TRUE;
}

char* getPropertyValue(char* propertyName) {
	char** tokens;
	char* value;
	tokens = str_split(propertyName, '=');
	value = tokens[1];
	strcat(propertyName, "=");
	strcat(propertyName, value);
	free(tokens);
	return value;

}
