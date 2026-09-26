#ifndef SERVER_LOG_H
#define SERVER_LOG_H

// HELP DETERMINE IF LOG IS NORMAL OR STDERR
enum log_level {
    LOG_INFO,
    LOG_ERROR
};

// SERVER LOG, USES LOG_LEVEL TO CLASSIFY WHAT IS HAPPENING, THE CHAR FOR THE MESSAGE ITSELF
void server_log(enum log_level, const char *message, ...);

#endif
