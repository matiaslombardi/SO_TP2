#include <string.h>

void itoa(int value, char *buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[length - i - 2] = value % 10 + '0';
        value /= 10;
    }
    buffer[length - 1] = 0;
}

void itoaTruncate(int value, char * buffer, int length) {
    uint64_t len = numlen(value);
    if(len > length - 1) {
        return;
    }
    itoa(value, buffer, len + 1);
}

/*
void dateToString(date myDate, char *buffer, char separator, int length) {

    itoa(myDate->day, buffer, 3);
    buffer[2] = separator;
    itoa(myDate->month, buffer + 3, 3);
    buffer[5] = separator;
    itoa(myDate->year, buffer + 6, 3);
    buffer[8] = ' ';
    itoa(myDate->hour, buffer + 9, 3);
    buffer[11] = ':';
    itoa(myDate->minutes, buffer + 12, 3);
    buffer[14] = ':';
    itoa(myDate->seconds, buffer + 15, 3);
    buffer[18] = 0;
}*/

char *strcpy(char *destination, char *source) {
    char *start = destination;

    while (*source != 0) {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = 0;
    return start;
}

int strlen(char *string) {
    int count = 0;
    while (string[count] != 0) {
        count++;
    }
    return count;
}

uint64_t numlen(uint64_t num) {
    uint64_t len = 0;
    if(num == 0) {
        return 1;
    } else {
        while(num != 0) {
            len++;
            num /= 10;
        }
    }
    return len;
}

int strcmp(char *stringA, char *stringB) {
    int i = 0;
    for (; stringA[i] == stringB[i]; i++) {
        if (stringA[i] == 0)
            return 0;
    }
    return stringA[i] - stringB[i];
}

int strtoks(char *string, char delimiter, char buffer[][25]) {
    int i = 0;
    int j = 0, z = 0;
    while (string[i] != 0) {
        z = 0;
        while (string[i] != delimiter && string[i] != 0) {
            buffer[j][z++] = string[i++];
        }
        buffer[j][z] = 0;
        if (z != 0) j++;
        i++;
    }
    return j; //Cantidad de tokens
}

/*uint64_t turnToBaseN(uint64_t value, int base, char *buffer, int bufferLength) {
    if (base < 2 || base > 26) return -1;

    uint64_t digits = 0;
    int pos = bufferLength - 2;
    //Calculate characters for each digit
    do {
        uint64_t remainder = value % base;
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    while (pos >= 0) {
        if ((pos + 1) % 5 == 0) {
            buffer[pos--] = ' ';
        }
        buffer[pos--] = '0';
    }
    buffer[bufferLength - 1] = 0;
    return digits;
}*/

//Convierte un string en hexa a un int.
uint64_t stringToInt(char *num) {
    toMayus(num);
    uint64_t value = 0;
    for (int i = 0; num[i] != '\0'; i++) {
        value *= 16;
        value += IS_DIGIT(num[i]) ? (num[i] - '0') : (num[i] - 'A' + 10);
    }
    return value;
}

//Convierte un string en decimal a un int.
uint64_t string10ToInt(char *num) {
    toMayus(num);
    uint64_t value = 0;
    for (int i = 0; num[i] != '\0'; i++) {
        value *= 10;
        value += num[i] - '0';
    }
    return value;
}

void toMayus(char *num) {
    int i = 0;
    while (num[i] != 0) {
        if (num[i] >= 'a' && num[i] <= 'z') {
            num[i] -= ('a' - 'A');
        }
        i++;
    }
}
