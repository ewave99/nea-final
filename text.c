#include <stdlib.h>
#include "text.h"
#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 256

static int
processWhiteSpace(char *s, int *iptr, char *b, int *biptr)
{
    int i = *iptr;
    int bi = *biptr;

    while (1)
    {
        if (!isspace(s[i])) break;
        if (s[i] == '\n') break;
        i++;
    }

    *iptr = i;
    *biptr = bi;
    return 0;
}

static int
processModuleType(char *s, int *iptr, char *b, int *biptr)
{
    int err = 0;
    int i = *iptr;
    int bi = *biptr;

    if (!isupper(s[i]))
        err = 1;
    b[bi++] = s[i++];
    while(1)
    {
        if (!isalpha(s[i]))
            if (!isdigit(s[i]))
                if (s[i] != '_') break;

        b[bi++] = s[i++];
    }
    b[bi] = '\0';

    *iptr = i;
    *biptr = bi;
    return err;
}

static int
processModuleName(char *s, int *iptr, char *b, int *biptr)
{
    int err = 0;
    int i = *iptr;
    int bi = *biptr;

    if (!islower(s[i]))
        err = 1;
    b[bi++] = s[i++];
    while (1)
    {
        if (!isalpha(s[i]))
            if (!isdigit(s[i]))
                if (s[i] != '_') break;

        b[bi++] = s[i++];
    }
    b[bi] = '\0';

    *iptr = i;
    *biptr = bi;
    return err;
}

static int
processModuleOutput(char *s, int *iptr, char *b, int *biptr)
{
    int err = 0;
    int i = *iptr;
    int bi = *biptr;

    if (!islower(s[i]))
        err = 1;
    b[bi++] = s[i++];
    while (1)
    {
        if (!isalpha(s[i]))
            if (!isdigit(s[i]))
                if (s[i] != '_') break;

        b[bi++] = s[i++];
    }
    b[bi] = '\0';

    *iptr = i;
    *biptr = bi;
    return err;
}

static int
processArrowOperator(char *s, int *iptr, char *b, int *biptr)
{
    int err = 0;
    int i = *iptr;
    int bi = *biptr;

    if (s[i] != '-') err = 1;
    b[bi++] = s[i++];
    if (s[i] != '>') err = 1;
    b[bi++] = s[i++];

    b[bi] = '\0';

    *iptr = i;
    *biptr = bi;
    return err;
}

static int
processModuleInput(char *s, int *iptr, char *b, int *biptr)
{
    int err = 0;
    int i = *iptr;
    int bi = *biptr;

    if (!islower(s[i]))
        err = 1;
    b[bi++] = s[i++];
    while (1)
    {
        if (!isalpha(s[i]))
            if (!isdigit(s[i]))
                if (s[i] != '_') break;

        b[bi++] = s[i++];
    }
    b[bi] = '\0';

    *iptr = i;
    *biptr = bi;
    return err;
}

ParseResult
processText(char* input)
{
    ParseResult result;
    char buffer[BUFFER_SIZE];

    int i; //input index
    int bi; //buffer index

    i = 0;
    bi = 0;

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (isupper(input[i]))
    {
        if (processModuleType(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module type: %s", buffer);
            return result;
        }
        if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
        if (islower(input[i]))
        {
            if (processModuleName(input, &i, buffer, &bi) == 1)
            {
                result.error_code = 1;
                snprintf(result.error_message, BUFFER_SIZE,
                        "Invalid module name: %s", buffer);
                return result;
            }
        }
        else
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Error: should be module type.");
            return result;
        }
    }
    else if (islower(input[i]))
    {
        if (processModuleName(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module name: %s", buffer);
            return result;
        }
    }
    else
    {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: line should start with module name or module type.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (input[i] == ':') i++;
    else {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: should be a colon.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (islower(input[i]))
    {
        if (processModuleOutput(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Error: invalid module output name.");
            return result;
        }
    }
    else
    {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: should be name of module output.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (input[i] == '-')
    {
        if (processArrowOperator(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Error processing arrow operator.");
            return result;
        }
    }
    else
    {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: should be an arrow operator.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (isupper(input[i]))
    {
        if (processModuleType(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module type: %s", buffer);
            return result;
        }
        if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
        if (islower(input[i]))
        {
            if (processModuleName(input, &i, buffer, &bi) == 1)
            {
                result.error_code = 1;
                snprintf(result.error_message, BUFFER_SIZE,
                        "Invalid module name: %s", buffer);
                return result;
            }
        }
        else
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Error: should be module type.");
            return result;
        }
    }
    else if (islower(input[i]))
    {
        if (processModuleName(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Invalid module name: %s", buffer);
            return result;
        }
    }
    else
    {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: connection destination should start with module name or module type.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (input[i] == ':') i++;
    else {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: should be a colon.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (islower(input[i]))
    {
        if (processModuleInput(input, &i, buffer, &bi) == 1)
        {
            result.error_code = 1;
            snprintf(result.error_message, BUFFER_SIZE,
                    "Error: invalid module input name.");
            return result;
        }
    }
    else
    {
        result.error_code = 1;
        snprintf(result.error_message, BUFFER_SIZE,
                "Error: should be name of module input.");
        return result;
    }

    if (isspace(input[i])) processWhiteSpace(input, &i, buffer, &bi);
    if (input[i] == '\0' || input[i] == '\n')
    {
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
    }

    return result;
}
