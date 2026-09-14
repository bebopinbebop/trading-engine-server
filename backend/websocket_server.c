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

    while (lws_service(context, 0) >= 0) {}

    lws_context_destroy(context);
    return 1;

}