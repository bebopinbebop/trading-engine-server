#include "websocket_server.h"
#include <stdio.h>
#include <libwebsockets.h>
#include "server_log.h"

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
            server_log(LOG_INFO, "LWS_CALLBACK_ESTABLISHED!!!---...");
            lws_set_timer_usecs(connection, 1000000);
            break;


        case LWS_CALLBACK_TIMER:
            lws_callback_on_writable(connection);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:

            unsigned char buffer[LWS_PRE +32];
            unsigned char *message = buffer + LWS_PRE;

            int length = snprintf((char *)message, 32, "heartbeat");

            int sent = lws_write(
                connection,
                message,
                (size_t)length,
                LWS_WRITE_TEXT);

            if (sent < length) {
                return -1;
            }

            lws_set_timer_usecs(connection, 1000000);

            break;

        case LWS_CALLBACK_CLOSED:
            server_log(LOG_INFO, "...--->LWS_CALLBACK_CLOSED");
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

    while (lws_service(context, 0) >= 0) {}

    lws_context_destroy(context);
    return 1;

}