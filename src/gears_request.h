#ifndef GEARS_REQUEST_H_INC
#define GEARS_REQUEST_H_INC

// prototypes
// send HTTPs request via indicated method (get/put/post)
// returns HTTP error code as number
int proto_getRequest(const char* url, const char* token);
int proto_putRequest(const char* url, const char* token);
int proto_postRequest(const char* url, const char* token, const char* postData);

#endif // GEARS_REQUEST_H_INC
