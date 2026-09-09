#include <stdio.h>

// ADDING A PREPROCESSOR DIRECTIVE TO CONDITIONALLY SET THE CORRECT LIBRARY BY CHECKING THE OS
#ifdef _WIN32
    #include <windows.h>
    #define sleeps(s) Sleep((s)*1000)
#else
    #include <unistd.h>
    #define sleeps(s) sleep(s)
#endif


int main(void) {

    int i = 0;

    while (1) {
        i++;
        printf("I have been alive for %i seconds\n",i);
        sleeps(1);
    }

    return 0;
}
