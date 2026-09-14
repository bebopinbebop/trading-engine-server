#include <stdio.h>
#include <libwebsockets.h> // WEBSOCKET LIBRARY CALL
#include "websocket_server.h"
#include <unistd.h>

int main(void) {

    websocket_server_run(8080);

    printf("libwebsockets version: %s\n", lws_get_library_version());

    static long long unsigned int i = 0;

    while (1) {
        i++;
        printf("I have been alive for %llu seconds\n",i);
        fflush(stdout);
        sleep(1);

    }

}
