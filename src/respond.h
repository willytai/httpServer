#ifndef __RESPOND_H__
#define __RESPOND_H__

#include <sys/socket.h>
#include "utilities.h"

enum httpMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNDEF
};
void setRequestMethod(enum httpMethod* mm, const char* method);
int parseRequest(char* msgBuffer, enum httpMethod* method, char* url);
int resp(int new_fd, char* msgBuffer);

int HttpGET(int new_fd, char* msgBuffer, const char* url);

#endif /* __RESPOND_H__ */
