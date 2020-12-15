#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string.h>

#define LOGGER_PATH "ux0:tmp/"
#define LOGGER_FILENAME LOGGER_PATH "xvita.log"

char* itoa(int value, char* result, int base);

void logger_write(const char* buffer, const size_t len);
void logger_reset();
void logger_flush();

#define LOG(...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), ##__VA_ARGS__); \
        logger_write(buffer, strlen(buffer)); \
    } while(0)

#endif // LOGGER_H
