#include "websocket_server.h"
#include <stdio.h>
#include <libwebsockets.h>
#include "server_log.h"

struct heartbeat_session {
    unsigned int sequence; // LAST HEARTBEAT NUMBER SENT
    int heartbeat_pending; // WHETHER A TIMER HAS REQUESTED A HEARTBEAT
};

static int callback_heartbeat(
    struct lws *connection,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len
) {

    struct heartbeat_session *session = user;

    (void)in;
    (void)len;

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            session->sequence = 0;
            session->heartbeat_pending = 0;

            server_log(LOG_INFO, "LWS_CALLBACK_ESTABLISHED!!!---...");
            lws_set_timer_usecs(connection, 1000000);
            break;


        case LWS_CALLBACK_TIMER:
            session->heartbeat_pending = 1;
            lws_callback_on_writable(connection);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            if (!session->heartbeat_pending) {break;}

            unsigned char buffer[LWS_PRE +32];
            unsigned char *message = buffer + LWS_PRE;
            unsigned int next_sequence = session->sequence + 1;

            int length = snprintf((char *)message, 128, "{\"type\":\"heartbeat\",\"sequence\":%u}", next_sequence);

            int sent = lws_write(
                connection,
                message,
                (size_t)length,
                LWS_WRITE_TEXT);

            if (sent < length) {
                server_log(LOG_ERROR, "Failed to write to LWS_CALLBACK_SERVER_WRITEABLE");
                return -1;
            }

            session->sequence = next_sequence;
            session->heartbeat_pending = 0;

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
        .per_session_data_size = sizeof(struct heartbeat_session),
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