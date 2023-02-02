#include <stdlib.h>
#include "text.h"
#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 256

static int
processModuleType(char *ptr, char *bptr)
{
    int err = 0;
    while(1)
    {
        if (*ptr == '\0') break;
        if (*ptr == ' ') break;

        if (!isupper(*ptr)) err = 1;

        *(bptr++) = *(ptr++);
    }
    *bptr = '\0';
    return err;
}

static int
processModuleName(char *ptr, char *bptr)
{
    int err = 0;
    while (1)
    {
        if (*ptr == '\0') break;
        if (*ptr == ' ') break;

        if (!islower(*ptr))
            if (!isdigit(*ptr)) err = 1;

        *(bptr++) = *(ptr++);
    }
    *bptr = '\0';
    return err;
}

ParseResult
processText(const char* input)
{
    ParseResult result;
    char *ptr;
    char buffer[BUFFER_SIZE];
    char *bptr;
    int err; //non-error is 0, error is 1

    ptr = input;
    bptr = buffer;
    err = 0;

    if (isupper(*ptr))
    {
        if (processModuleType(ptr, bptr) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module type: %s", buffer);
            return result;
        }
    }
    else if (islower(*ptr))
    {
        if (processModuleName(ptr, bptr) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module name: %s", buffer);
            return result;
        }
    }

    //test to see if program handles result from this function correctly
    result.num_modules = 3;
    result.modules = (AbstractModule*) malloc(sizeof(AbstractModule)
            * result.num_modules);
    int m = 0;
    result.modules[m++] = (AbstractModule){ .module_type = M_SAWOSC };
    result.modules[m++] = (AbstractModule){ .module_type = M_MIXER };
    result.modules[m++] = (AbstractModule){ .module_type = M_SPEAKERS };
    
    result.num_connections = 2;
    result.connections = (Connection*) malloc(sizeof(Connection)
            * result.num_connections);
    int c = 0;
    result.connections[c++] = (Connection){ 0, 0, 1, 0 };
    result.connections[c++] = (Connection){ 1, 0, 2, 0 };

    /* returns 0 if successful, else 1 */
    result.error_code = 0;
    snprintf(result.error_message, BUFFER_SIZE,
            "Patch processed successfully.");
    return result;
}
