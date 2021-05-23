#ifndef STRING_H
#define STRING_H

#include <types.h>
#include <stdint.h>

#define IS_ALPHA(c) ( (c <= '9' && c >= '0') || (c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') )


void itoa(int value, char *buffer, int length);

void itoaTruncate(int value, char * buffer, int length);

void dateToString(date myDate, char *buffer, char separator, int length);

char *strcpy(char *destination, char *source);

int strlen(char *string);

uint64_t numlen(uint64_t num);

int strcmp(char *stringA, char *stringB);

int strtoks(char *string, char delimiter, char buffer[][25]);

uint64_t turnToBaseN(uint64_t value, int base, char *buffer, int bufferLength);

uint64_t stringToInt(char *num);

uint64_t string10ToInt(char *num);

void toMayus(char *string);

void strcat(char * dest, char * src);

void removeSpaces(char * str);

#endif