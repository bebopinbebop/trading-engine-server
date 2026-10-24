#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <curl/curl.h>

// TESTING ALPACA API CALLING IN C TO FETCH REST DATA ON OPTIONS FOR SPECIFIED STOCKS USING API CREDENTIALS SAVED IN ENV
int main(void) {

    // SAVING ENV PARAM FOR ALPACA API CALLING
    const char *key = getenv("ALPACA_KEY");
    const char *secret = getenv("ALPACA_SECRET");

    // SETTING A STOCK TICKER THAT CAN BE INPUTTED INTO THE URL FOR LATER
    const char *symbol = "AAPL";

    // BUILDING THE BASE OF THE URL TO CALL WITH THE API
    const char *url_format =
    "https://data.alpaca.markets/v1beta1/options/snapshots/%s"
    "?feed=indicative&limit=5";

    // CALCULATING THE URL LENGTH
    int url_length = snprintf(NULL, 0, url_format, symbol);

    // CHECK TO MAKE SURE THAT THERE IS URL STRING TO DETERMINE SIZE
    if (url_length < 0) {
        fprintf(stderr, "Cannot format url\n");

        return EXIT_FAILURE;
    }

    // BUILDING MEMORY FOR URL, WITH A BYTE ADDED TO ACCOUNT FOR '\0'
    char *url = malloc((size_t)url_length + 1);

    // CHECK IF URL EXISTS
    if (url == NULL) {
        fprintf(stderr, "Cannot allocate memory for url\n");
        return EXIT_FAILURE;
    }

    //PRINTING THE SIZE OF THE URL MEMORY
    snprintf(url, (size_t)url_length + 1, url_format, symbol);

    // CHECK TO SEE IF THE API KEY AND SECRET EXISTS
    if (key == NULL || secret == NULL) {
        fprintf(stderr, "Alpaca API Key or Secret missing\n");
        return EXIT_FAILURE;
    }

    // BUILDING THE HTTPS HEADERS USING THE API DATA ENTERED, W/ THE CHALLENGE OF USING MALLOC AND FREE TO
    // char key_header[256];
    // char secret_header[256];

    // PREDEFINING THE HEADER STRINGS THAT WILL LATER BE USED FOR THE API CALL
    const char *key_prefix = "APCA-API-KEY-ID: ";
    const char *secret_prefix = "APCA-API-SECRET-KEY: ";

    // CALCULATING THE MEMORY SIZE NEEDED FOR THE HEADER, WHICH IS WHY WE CHECK FOR THE EXISTENCE OF THESE ENV PARAMS
    // THERE NEEDS TO BE AN ADDITIONAL '+1' IN THE SIZE TO ACCOUNT FOR THE TERMINATING CHARACTER '\0' IN THE ENV PARAMS
    size_t key_size = strlen(key_prefix) + strlen(key) + 1;
    size_t secret_size = strlen(secret_prefix) + strlen(secret) + 1;

    char *key_header = malloc(key_size);
    char *secret_header = malloc(secret_size);

    // CHECKING TO SEE IF SUCCESSFUL MEMORY BUILD FOR BOTH KEY & SECRET WAS MADE
    if (key_header == NULL || secret_header == NULL) {
        fprintf(stderr, "Alpaca API Key or Secret missing\n");
        free(key_header);
        free(secret_header);
        return EXIT_FAILURE;
    }

    snprintf(key_header, key_size, "%s%s", key_prefix, key);
    snprintf(secret_header, secret_size, "%s%s", secret_prefix, secret);

    // CHECKING IF CURL IS OKAY TO BE USED, AND ALSO INITIALZING IT
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        fprintf(stderr, "Cannot use curl, check libcurl package on local device\n");
        return EXIT_FAILURE;
    }

    // STARTING THE CURL SETUP
    CURL *request = curl_easy_init();

    // CHECKING TO SEE IF CURL INIT BREAKS
    if (request == NULL) {
        fprintf(stderr, "Cannot init curl\n");
        curl_easy_cleanup(request); // GOT TO CLEAN UP ANY MEMORY ALOC
        return EXIT_FAILURE;
    }

    // BUILDING THE HEADERS FOR CURL, PIECE BY PIECE WITH VERIFICATION
    struct curl_slist *headers = NULL;

    // ADDING THE KEY TO THE HEADER
    struct curl_slist *updated = curl_slist_append(headers, key_header);

    if (updated == NULL) {
        curl_easy_cleanup(request);
        curl_global_cleanup();
        fprintf(stderr, "key_header didn't work\n");
        return EXIT_FAILURE;
    }

    headers = updated;

    //ADDING THE SECRET TO THE HEADER NOW
    updated = curl_slist_append(headers, secret_header);

    if (updated == NULL) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(request);
        curl_global_cleanup();
        fprintf(stderr, "secret_header didn't work, and now deleting headers\n");
        return EXIT_FAILURE;
    }

    // HEADERS NOW FULLY BUILT
    headers = updated;

    // BUILDING THE CURL COMMAND
    curl_easy_setopt(request, CURLOPT_URL, url);
    curl_easy_setopt(request, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(request, CURLOPT_TIMEOUT, 30L);

    // RELEASING MEMORY FOR URL
    free(url);

    // GETTING OUTPUT FROM CURL
    CURLcode result = curl_easy_perform(request);

    long http_status = 0;

    if (result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
    }else {
        curl_easy_getinfo(request, CURLINFO_RESPONSE_CODE, &http_status);
        fprintf(stderr, "curl_easy_getinfo() returned %ld\n", http_status);
    }

    // CLEANING UP MEMORY SPACE
    curl_easy_cleanup(request);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    // CLOSING THE SCRIPT, DOING A CURL AND HTTP CHECK TO DETERMINE THE SUCCESS OF THE OPERATION
    return result == CURLE_OK && http_status == 200 ? EXIT_SUCCESS : EXIT_FAILURE;
}