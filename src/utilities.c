#include "utilities.h"

size_t fileRead(const char* filename, char* buffer) {
    char fileContent[BUFFER_SIZE];
    memset(fileContent, '\0', sizeof(fileContent));
    FILE* fp;
    if ((fp=fopen(filename, "r")) == NULL) { // read & binary
        perror("Unable to open file");
        return FILE_ERROR;
    }
    fseek(fp, 0L, SEEK_END);
    size_t fileLength = (size_t)ftell(fp);
    rewind(fp);
    fread(fileContent, fileLength, sizeof(char), fp);
    fileContent[fileLength] = '\0'; // this is necessary
    fclose(fp);

    // get the suffix of the file, and determine content type
    char suf[8];
    char type[16];
    suffix(filename, suf);
    // printf("suffix %s\n", suf);
    getFileType(suf, type);
    // printf("type %s %ld\n", type, strlen(type));

    // use memcpy in order to concatenate binary data
    int hlen = snprintf(buffer, BUFFER_SIZE, "HTTP/1.1 200 OK\nContent_Type: %sContent-Length: %ld\n\n", type, fileLength);
    memcpy(buffer+hlen, fileContent, fileLength+1); // need to copy the end-of-string character

    printf("sending with type: %s\n", type);
    // printf("this is the buffer\n%s| end\n", buffer);
    return (size_t)hlen+fileLength;
}

// similar to substr
void myStrCpy(char* dst, const char* src, const long unsigned int start, const long unsigned int n) {
    long unsigned int i = 0;
    for (; i < n; ++i) {
        dst[i] = src[start+i];
    }
    dst[i] = '\0';
}

void suffix(const char* filename, char* suf) {
    unsigned long int start = 0;
    while (start < strlen(filename)&&filename[start++]!='.') {}
    myStrCpy(suf, filename, start, strlen(filename)-start);
}

void getFileType(const char* suf, char* type) {
    if (!strcmp(suf, "JPG")) strcpy(type, "image/jpg");
    else if (!strcmp(suf, "jpg")) strcpy(type, "image/jpg");
    else if (!strcmp(suf, "JPEG")) strcpy(type, "image/jpeg");
    else if (!strcmp(suf, "jpeg")) strcpy(type, "image/jpeg");
    else if (!strcmp(suf, "PNG")) strcpy(type, "image/png");
    else if (!strcmp(suf, "png")) strcpy(type, "image/png");
    else if (!strcmp(suf, "GIF")) strcpy(type, "image/gif");
    else if (!strcmp(suf, "gif")) strcpy(type, "image/gif");
    else if (!strcmp(suf, "html")) strcpy(type, "text/html");
    else if (!strcmp(suf, "css")) strcpy(type, "text/css");
    else if (!strcmp(suf, "xml")) strcpy(type, "text/xml");
    else if (!strcmp(suf, "zip")) strcpy(type, "application/zip");
    else if (!strcmp(suf, "js")) strcpy(type, "application/javascript");
    else strcpy(type, "text/plain");
}
