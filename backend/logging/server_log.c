#include "server_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>


// A STANDARDIZED MESSAGE LOG FOR THE SERVER, USING YYYYMMDDHHMMSSNS TAGGED WITH A INCREMENTING LOG INT AS A UNIQUE IDENTIFIER
void server_log(enum log_level level, const char *message, ...) {

    FILE *stream = (level == LOG_ERROR) ? stderr : stdout;
    const char *label = (level == LOG_ERROR) ? "ERROR" : "INFO";

    // DEFAULT TIME INCASE CAN'T GET TIME
    char timestamp[32]="time unavailable";
    long millis = 0;
    struct timespec now;

    static unsigned long long sequence = 0;

    // TIME IS GOOD
    if (timespec_get(&now, TIME_UTC) == TIME_UTC) {
        // GETTING THE TIME FROM EPOCH
        struct tm *local = localtime(&now.tv_sec);

        if (local != NULL) {
            strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", local);
            millis = now.tv_nsec / 1000000L;
        }


    }

    ++sequence;
    fprintf(stream,"server_log #%s%03ld-%llu: [%s]:", timestamp,millis,sequence,label);

    // HANDLING MULTIPLE INPUT FORMATS
    va_list args;
    va_start(args, message);
    vfprintf(stream, message, args);
    va_end(args);

    fputc('\n', stream);
    fflush(stream);
}