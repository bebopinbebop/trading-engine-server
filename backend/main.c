#include <stdio.h>
#include <libwebsockets.h> // WEBSOCKET LIBRARY CALL

// ADDING A PREPROCESSOR DIRECTIVE TO CONDITIONALLY SET THE CORRECT LIBRARY BY CHECKING THE OS
#ifdef _WIN32
    #include <windows.h>
    #define sleeps(s) Sleep((s)*1000)
#else
    #include <unistd.h>
    #define sleeps(s) sleep(s)
#endif


int main(void) {

    printf("libwebsockets version: %s\n", lws_get_library_version());
    int i = 0;

    while (1) {
        i++;
        printf("I have been alive for %i seconds\n",i);
        fflush(stdout);
        sleeps(1);
    }

    return 0;
}
