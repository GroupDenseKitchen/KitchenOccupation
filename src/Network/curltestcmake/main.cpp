#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

    char data[1337];
  /* get a curl handle */
    char* nOcc = "DBL_MAX";
    char*  roomName = "INF";
    sprintf(data, "{\"queStatus\" : \"%s\", \"numOccupants\" : \"%s\"}", roomName, nOcc);
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    printf("curl initialized\n");

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers );
    curl_easy_setopt(curl, CURLOPT_VERBOSE, CURLINFO_HEADER_OUT);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    printf("curl options set\n");

    curl_easy_setopt(curl, CURLOPT_URL, "http://api.densekitchen.bloip.se/api/roomstate/1");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    printf("%s \n",data);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);


    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}

