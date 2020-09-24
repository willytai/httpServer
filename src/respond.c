#include "respond.h"

int resp(int new_fd, char* msgBuffer) {
    enum httpMethod method;
    char url[MAX_URL_SIZE];
    parseRequest(msgBuffer, &method, url);
    memset(msgBuffer, '\0', BUFFER_SIZE);

    if (method == GET) return HttpGET(new_fd, msgBuffer, url);
    // else if (*method==POST) return HttpPOST();
    else return RESPOND_ERROR;
}

int HttpGET(int new_fd, char* msgBuffer, const char* url) {
    size_t responseLength;
    if (!strcmp(url, "")) { // navigate to home page
        if ((responseLength=fileRead("index.html", msgBuffer)) == FILE_ERROR) {
            perror("Error in fileRead");
            return RESPOND_ERROR;
        }
    }
    else if (!strcmp(url, "favicon.ico")) {
        // TODO: enforce the favicon image to be placed in favicon/, and find all the imgs that are in it and pick the first
        if ((responseLength=fileRead("favicon/icons8-monitor-80.png", msgBuffer)) == FILE_ERROR) {
            perror("Error in fileRead");
            // TODO: send error message ex: 404 Not Found
            return RESPOND_ERROR;
        }
    }
    else {
        if ((responseLength=fileRead(url, msgBuffer)) == FILE_ERROR) {
            perror("Error in fileRead");
            // TODO: send error message ex: 404 Not Found
            return RESPOND_ERROR;
        }
    }
    if (send(new_fd, msgBuffer, responseLength, 0) == SO_ERROR) {
        perror("Error in send");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int parseRequest(char* msgBuffer, enum httpMethod* method, char* url) {
    char strMethod[7];
    long unsigned int i = 0, start = 0;
    for (; i < strlen(msgBuffer); ++i) {
        if (msgBuffer[i] == ' ') {
            memset(strMethod, '\0', sizeof(strMethod));
            myStrCpy(strMethod, msgBuffer, 0, i);
            setRequestMethod(method, strMethod);
            while (msgBuffer[i] == ' ') ++i;
            start = i;
            break;
        }
    }
    for (i+=1; i < strlen(msgBuffer); ++i) {
        if (msgBuffer[i] == ' ') {
            myStrCpy(url, msgBuffer, start+1, i-start-1);
            break;
        }
    }
    printf("%s %s | ", strMethod, url);
    return 0;
}

void setRequestMethod(enum httpMethod* mm, const char* method) {
    if (!strcmp(method, "GET")) (*mm) = GET;
    else if (!strcmp(method, "HEAD")) (*mm) = HEAD;
    else if (!strcmp(method, "POST")) (*mm) = POST;
    else if (!strcmp(method, "PUT")) (*mm) = PUT;
    else if (!strcmp(method, "DELETE")) (*mm) = DELETE;
    else if (!strcmp(method, "CONNECT")) (*mm) = CONNECT;
    else if (!strcmp(method, "OPTIONS")) (*mm) = OPTIONS;
    else if (!strcmp(method, "TRACE")) (*mm) = TRACE;
    else if (!strcmp(method, "PATCH")) (*mm) = PATCH;
    else (*mm) = UNDEF;
}
