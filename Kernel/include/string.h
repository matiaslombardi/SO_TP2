//
// Created by matias on 25/4/21.
//

#ifndef SO_TP2_STRING_H
#define SO_TP2_STRING_H

#include <stdint.h>
#include <naiveConsole.h>

#define IS_ALPHA(c) ( (c <= '9' && c >= '0') || (c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a') )
#define IS_DIGIT(c) (c >= '0' && c <= '9')

void itoa(int value, char *buffer, int length);

//void dateToString(date myDate, char *buffer, char separator, int length); PAra poner esta funcion definir date

char *strcpy(char *destination, char *source);

int strlen(char *string);

int strcmp(char *stringA, char *stringB);

int strtoks(char *string, char delimiter, char buffer[][25]);

//uint64_t turnToBaseN(uint64_t value, int base, char *buffer, int bufferLength); //Esta en naiveConsole

uint64_t stringToInt(char *num);

void toMayus(char *string);


#endif //SO_TP2_STRING_H
