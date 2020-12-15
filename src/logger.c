#include "logger.h"
#include <psp2kern/io/fcntl.h>

extern int ksceIoMkdir(const char*, int);

static unsigned int log_buf_ptr = 0;
static char log_buf[16 * 1024];

char*
itoa(int value, char* result, int base) {
    // check that the base if valid
    if(base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char* ptr = result;
    char* ptr1 = result;

    char tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while(value);

    // Apply negative sign
    if (tmp_value < 0)
    {
        *ptr++ = '-';
    }

    *ptr-- = '\0';

    while(ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}

void
logger_write(const char* buffer, const size_t len)
{
    if((log_buf_ptr + len) >= sizeof(log_buf))
    {
        return;
    }

    memcpy(log_buf + log_buf_ptr, buffer, len);
    log_buf_ptr += len;
}

void
logger_reset()
{
    SceUID fd = ksceIoOpen(LOGGER_FILENAME, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 6);
    if (fd < 0) {
        return;
    }

    ksceIoClose(fd);

    log_buf_ptr = 0;
    memset(log_buf, 0, sizeof(log_buf));
}

void
logger_flush()
{
    ksceIoMkdir(LOGGER_PATH, 6);

    SceUID fd = ksceIoOpen(LOGGER_FILENAME, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
    if(fd < 0)
    {
        return;
    }
    ksceIoWrite(fd, log_buf, strlen(log_buf));
    ksceIoClose(fd);

    log_buf_ptr = 0;
    memset(log_buf, 0, sizeof(log_buf));
}
