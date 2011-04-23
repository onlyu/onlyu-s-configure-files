#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  printf("start recv data\n");
  char *content = (char *)malloc(size*nmemb + 1);
  memset(content, 0, size*nmemb + 1);
  memcpy(content, (char *)ptr, size * nmemb);
  printf("recv data:%s\n", content);
  free(content);
  return size;
} 

int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
 
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* Fill in the file upload field */ 
  /* set the machine id */
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "mid",
               CURLFORM_COPYCONTENTS, "1",
               CURLFORM_END);
 
  /* Fill in the filename field */ 
  /* set the program id */
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "pid",
               CURLFORM_COPYCONTENTS, "2",
               CURLFORM_END);
  
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 

  if(curl) {
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://nusm1i1.chinaw3.com/cgi-bin/pull_data.cgi");
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
    res = curl_easy_perform(curl);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
  return 0;
}
