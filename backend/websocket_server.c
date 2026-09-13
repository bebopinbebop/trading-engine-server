#include "websocket_server.h"
#include <stdio.h>
#include <libwebsockets.h>
#include <time.h>
#include <stdarg.h>

// HELP DETERMINE IF LOG IS NORMAL OR STDERR
enum log_level {
    LOG_INFO,
    LOG_ERROR
};

// TODO: IMPROVE ONELINE-NESS OF LOG, AND FIND BETTER IDENTIFICATION SYSTEM
// A STANDARDIZED MESSAGE LOG FOR THE SERVER, USING YYYYMMDDHHMMSSNS AS A UNIQUE IDENTIFIER
static void server_log(enum log_level level, const char *message, ...) {

    FILE *stream = (level == LOG_ERROR) ? stderr : stdout;
    const char *label = (level == LOG_ERROR) ? "ERROR" : "INFO";

    // DEFAULT TIME INCASE CAN'T GET TIME
    char timestamp[32]="time unavailable";
    long millis = 0;
    struct timespec now;

    // TIME IS GOOD
    if (timespec_get(&now, TIME_UTC) == TIME_UTC) {
        // GETTING THE TIME FROM EPOCH
        struct tm *local = localtime(&now.tv_sec);

        if (local != NULL) {
            strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", local);
            millis = now.tv_nsec / 1000000L;
        }

    }

    fprintf(stream,"server_log #%s%03ld: %s\n", timestamp,millis,label);

    // HANDLING MULTIPLE INPUT FORMATS
    va_list args;
    va_start(args, message);
    vfprintf(stream, message, args);
    va_end(args);

    fputc('\n', stream);
    fflush(stream);
}

static int callback_heartbeat(
    struct lws *connection,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
) {
    (void)connection;
    (void) user;
    (void)in;
    (void)len;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            server_log(LOG_INFO, "LWS_CALLBACK_ESTABLISHED!!!--->");
            break;
        case LWS_CALLBACK_CLOSED:
            server_log(LOG_INFO, "--->LWS_CALLBACK_CLOSEDxxx");
            break;
        default:
            break;
    }

    return 0;
}

static const struct lws_protocols protocols[] = {
    {
        .name = "heartbeat",
        .callback = callback_heartbeat,
        .per_session_data_size = 0,
        .rx_buffer_size = 0
    },
    {0}
};

int websocket_server_run(int port) {
    struct lws_context_creation_info info = {0};

    info.port = port;
    info.protocols = protocols;

    struct lws_context *context = lws_create_context(&info);

    if (context == NULL) {
        server_log(LOG_ERROR, "lws_create_context() failed");
        return 1;
    }

    server_log(LOG_INFO, "WebSocket server listening on port %d", port);

    while (lws_service(context, 0) >= 0) {



    }

    lws_context_destroy(context);
    return 1;

}