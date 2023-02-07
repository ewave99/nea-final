#include "application.h"

typedef struct
{
    int num_modules;
    AbstractModule* modules;
    int num_connections;
    Connection* connections;
    int err;
}
ParseResult;

typedef struct
{
    int line;
    int col;
    int err;
    char errmsg[256];
    ParseResult result;
}
ParseContext;

ParseContext processText(char*);
