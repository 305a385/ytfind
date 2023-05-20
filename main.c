#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>
#include <json-c/json.h>

#include "util.h"
#include "config.h"

int printTitles(void){
	json_object *root = json_object_from_file(QUERY_RESULT_FILE);
	if (!root) {
		return 1;
	}

	int n = json_object_array_length(root);
	for (int i=1; i<n; i++) {
		json_object *type = json_object_object_get(json_object_array_get_idx(root, i), "type");
		//make sure a video is being chosen (not a channel, playlist, etc)
		if(! strcmp(json_object_get_string(type), "video")) {
			json_object *title = json_object_object_get(json_object_array_get_idx(root, i), "title");
			json_object *videoId = json_object_object_get(json_object_array_get_idx(root, i), "videoId");
			json_object *author = json_object_object_get(json_object_array_get_idx(root, i), "author");
			
			printf("Title: %s\nAuthor: %s\nID: %s\n\n", json_object_get_string(title), json_object_get_string(author), json_object_get_string(videoId));
		}
	}
	json_object_put(root);
	return 0;
}


int main(int argc, char *argv[]) {
	
	if( argc < 2 ) {
		printf("usage: try '%s [query]' to make a search.\n", NAME);
		return 1;
	}

	/* get random instance */
	srand(time(NULL));
	const char *INSTANCE = (INSTANCES[rand() % ARR_SIZE(INSTANCES)]);
	
	/* if memory cannot be allocated */
	if(INSTANCE == NULL) {
		printf("Error! memory not allocated.");
		exit(0);
	}

	char *query;
	query = (char*) malloc(sizeof(char*) * (sizeof(*argv) + sizeof(INSTANCE)));
	strcat(query, INSTANCE);
	strcat(query, API_PATH);
	
	/* if memory cannot be allocated */
	if(query == NULL) {
		printf("Error! memory not allocated.");
		exit(0);
	}
	/* add all arguments to query */
	for (short i=1; i < argc; i++){
		/* add space before current argument if not first argument*/
		if (i != 1) {
			strcat(query, "%20");
		}
		strcat(query, argv[i]);
	}
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0;

	curl_handle = curl_easy_init();
	if(curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_URL, query);
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "fetch-agent/0.1");

		res = curl_easy_perform(curl_handle);

		if(res != CURLE_OK) {
			fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
		} else {
			/* create pointer to query_RESULT_FILE
			 * (where the query will be stored,
			 * you can change the value in config.h) */
			FILE *tmpFilePtr = fopen(QUERY_RESULT_FILE,"w");
			fprintf(tmpFilePtr, "%s", chunk.memory);
			fclose(tmpFilePtr);
		}

		curl_easy_cleanup(curl_handle);
		/* dealocate memory*/
		free(chunk.memory);
		free(query);

		/* print titles of videos to stdout */
		 printTitles();
	}

	return 0;
}

