#include <stdio.h>
#include <libwebsockets.h> // WEBSOCKET LIBRARY CALL
#include "server/websocket_server.h"
#include <unistd.h>

int main(void) {

    // THE ACTUAL WEBSOCKET SERVER CALLED FROM THE WEBSOCKET_SERVER FILE
    websocket_server_run(8080); // TODO: MAKE THE PORT NUMBER A VARIABLE THAT CAN BE PULLED FROM THE DOCKER INIT

    }
