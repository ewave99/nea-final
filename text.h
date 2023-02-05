#include "application.h"

typedef struct
{
    int num_modules;
    AbstractModule* modules;
    int num_connections;
    Connection* connections;
    int error_code;
    char error_message[256];
} ParseResult;

ParseResult processText(char*);
