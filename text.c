#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "text.h"

#define BUFFER_SIZE 256

typedef enum
{
    C_INPUT,
    C_OUTPUT
}
ConnectionType;

static int
processWhiteSpace(char *s, int *iptr)
{
    while (1)
    {
        if (!isspace(s[*iptr])) break;
        if (s[*iptr] == '\n') break;
        (*iptr)++;
    }
    return 0;
}

static int
processModuleType(char *s, int *iptr, char *b, int *biptr)
{
    if (!isupper(s[*iptr])) return 1;

    b[(*biptr)++] = s[(*iptr)++];
    while(1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[(*biptr)++] = s[(*iptr)++];
    }
    b[*biptr] = '\0';

    return 0;
}

static int
processModuleName(char *s, int *iptr, char *b, int *biptr)
{
    if (!islower(s[*iptr])) return 1;
    b[(*biptr)++] = s[(*iptr)++];
    while (1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[(*biptr)++] = s[(*iptr)++];
    }
    b[*biptr] = '\0';

    return 0;
}

static int
processModuleOutput(char *s, int *iptr, char *b, int *biptr)
{
    if (!islower(s[*iptr])) return 1;
    b[(*biptr)++] = s[(*iptr)++];
    while (1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[(*biptr)++] = s[(*iptr)++];
    }
    b[*biptr] = '\0';

    return 0;
}

static int
processArrowOperator(char *s, int *iptr)
{
    if (s[(*iptr)++] != '-') return 1;
    if (s[(*iptr)++] != '>') return 1;
    return 0;
}

static int
processModuleInput(char *s, int *iptr, char *b, int *biptr)
{
    if (!islower(s[*iptr])) return 1;
    b[(*biptr)++] = s[(*iptr)++];
    while (1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[(*biptr)++] = s[(*iptr)++];
    }
    b[*biptr] = '\0';

    return 0;
}

static int
raiseError(ParseContext *context, int line, int col, char *errmsg)
{
    context->err = 1;
    snprintf(context->errmsg, BUFFER_SIZE, "Line %d, Col %d: %s",
            line, col, errmsg);
    return 1;
}

static int
processConnectionPoint(ParseContext *context, int line,
        ConnectionType connection_type, char *s, int *iptr)
{
    int start = *iptr;
    char buffer[256];
    int bi = 0;

    if (isupper(s[*iptr]))
    {
        if (processModuleType(s, iptr, buffer, &bi) == 1)
            return raiseError(context, line, *iptr - start, "Invalid module type.");
        processWhiteSpace(s, iptr);
        if (islower(s[*iptr]))
        {
            if (processModuleName(s, iptr, buffer, &bi) == 1)
                return raiseError(context, line, *iptr - start, "Invalid module name.");
        }
        else
            return raiseError(context, line, *iptr - start, "Missing module name.");
    }
    else if (islower(s[*iptr]))
    {
        if (processModuleName(s, iptr, buffer, &bi) == 1)
            return raiseError(context, line, *iptr - start, "Invalid module name.");
    }
    else
        return raiseError(context, line, *iptr - start, "Missing module name or module type.");

    processWhiteSpace(s, iptr);
    if (s[*iptr] == ':')
        (*iptr)++;
    else
        return raiseError(context, line, *iptr - start, "Missing colon.");

    processWhiteSpace(s, iptr);
    if (connection_type == C_INPUT)
    {
        if (islower(s[*iptr]))
        {
            if (processModuleInput(s, iptr, buffer, &bi) == 1)
                return raiseError(context, line, *iptr - start, "Invalid module input name.");
        }
        else
            return raiseError(context, line, *iptr - start, "Missing module input name.");
    }
    else
    {
        if (islower(s[*iptr]))
        {
            if (processModuleOutput(s, iptr, buffer, &bi) == 1)
                return raiseError(context, line, *iptr - start, "Invalid module output name.");
        }
        else
            return raiseError(context, line, *iptr - start, "Missing module output name.");
    }

    return 0;
}

static int
processLine(ParseContext *context, int line, char* s, int *iptr)
{
    int start = *iptr;

    processWhiteSpace(s, iptr);
    if (processConnectionPoint(context, line, C_INPUT, s, iptr)) return 1;
    processWhiteSpace(s, iptr);
    if (processArrowOperator(s, iptr) == 1)
        return raiseError(context, line, *iptr - start, "Error processing arrow operator.");
    processWhiteSpace(s, iptr);
    if (processConnectionPoint(context, line, C_OUTPUT, s, iptr)) return 1;
    processWhiteSpace(s, iptr);

    return 0;
}

ParseContext
processText(char* s)
{
    ParseContext context;
    int i = 0;
    int line = 0;

    context.err = 0;
    context.col = 0;
    
    if (processLine(&context, line, s, &i))
        return context;

    if (s[i] == '\0' || s[i] == '\n')
    {
        //test to see if program handles result from this function correctly
        context.result.num_modules = 3;
        context.result.modules = (AbstractModule*) malloc(sizeof(AbstractModule)
                * context.result.num_modules);
        int m = 0;
        context.result.modules[m++] = (AbstractModule){ .module_type = M_SAWOSC };
        context.result.modules[m++] = (AbstractModule){ .module_type = M_MIXER };
        context.result.modules[m++] = (AbstractModule){ .module_type = M_SPEAKERS };
        
        context.result.num_connections = 2;
        context.result.connections = (Connection*) malloc(sizeof(Connection)
                * context.result.num_connections);
        int c = 0;
        context.result.connections[c++] = (Connection){ 0, 0, 1, 0 };
        context.result.connections[c++] = (Connection){ 1, 0, 2, 0 };

        /* returns 0 if successful, else 1 */
        context.err = 0;
        snprintf(context.errmsg, BUFFER_SIZE,
                "Patch processed successfully.");
    }

    return context;
}
