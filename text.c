#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "text.h"

#define BUFFER_SIZE 256
#define MAX_NUM_MODULES 16
#define MAX_NUM_CONNECTIONS 32

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
processModuleType(char *s, int *iptr, char *b)
{
    int bi = 0;
    if (!isupper(s[*iptr])) return 1;

    b[bi++] = s[(*iptr)++];
    while(1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[bi++] = s[(*iptr)++];
    }
    b[bi] = '\0';

    return 0;
}

static int
processModuleName(char *s, int *iptr, char *b)
{
    int bi = 0;

    if (!islower(s[*iptr])) return 1;
    b[bi++] = s[(*iptr)++];
    while (1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[bi++] = s[(*iptr)++];
    }
    b[bi] = '\0';

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
processModuleConnector(char *s, int *iptr, char *b)
{
    int bi = 0;
    if (!islower(s[*iptr])) return 1;
    b[bi++] = s[(*iptr)++];
    while (1)
    {
        if (!isalpha(s[*iptr]))
            if (!isdigit(s[*iptr]))
                if (s[*iptr] != '_') break;

        b[bi++] = s[(*iptr)++];
    }
    b[bi] = '\0';

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

static ModuleType
getModuleType(char *s)
{
    switch (*s)
    {
        case 'A': return strcmp(s+1, "UDIOFILE") ? M_UNKNOWN : M_AUDIOFILE;
        case 'C': return strcmp(s+1, "LOCK")     ? M_UNKNOWN : M_CLOCK;
        case 'E': return strcmp(s+1, "NVELOPE")  ? M_UNKNOWN : M_ENVELOPE;
        case 'M': return strcmp(s+1, "IXER")     ? M_UNKNOWN : M_MIXER;
        case 'H': return strcmp(s+1, "PF")       ? M_UNKNOWN : M_HPF;
        case 'L': return strcmp(s+1, "PF")       ? M_UNKNOWN : M_LPF;
        case 'R': return strcmp(s+1, "EVERB")    ? M_UNKNOWN : M_REVERB;
        case 'S':
            switch (*(s+1))
            {
                case 'A': return strcmp(s+2, "WOSC")    ? M_UNKNOWN : M_SAWOSC;
                case 'E': return strcmp(s+2, "QUENCER") ? M_UNKNOWN : M_SEQUENCER;
                case 'I': return strcmp(s+2, "NOSC")    ? M_UNKNOWN : M_SINOSC;
                case 'P': return strcmp(s+2, "EAKERS")  ? M_UNKNOWN : M_SPEAKERS;
                case 'Q': return strcmp(s+2, "UOSC")    ? M_UNKNOWN : M_SQUOSC;
                default: return M_UNKNOWN;
            }
        case 'T': return strcmp(s+1, "RIOSC") ? M_UNKNOWN : M_TRIOSC;
        default: return M_UNKNOWN;
    }
}

static int
processConnectionPoint(ParseContext *context, int line, int line_start,
        ConnectionType connection_type, char *s, int *iptr)
{
    char buffer[BUFFER_SIZE];
    ModuleType module_type;
    char module_name[BUFFER_SIZE];
    char module_connector[BUFFER_SIZE];

    module_type = M_UNKNOWN;

    if (isupper(s[*iptr]))
    {
        if (processModuleType(s, iptr, buffer) == 1)
            return raiseError(context, line, *iptr - line_start,
                    "Invalid module type.");
        module_type = getModuleType(buffer);
        if (module_type == M_UNKNOWN)
            return raiseError(context, line, *iptr - line_start,
                    "Unknown module type.");

        processWhiteSpace(s, iptr);
        if (islower(s[*iptr]))
        {
            if (processModuleName(s, iptr, buffer) == 1)
                return raiseError(context, line, *iptr - line_start,
                        "Invalid module name.");
        }
        else
            return raiseError(context, line, *iptr - line_start,
                    "Missing module name.");
    }
    else if (islower(s[*iptr]))
    {
        if (processModuleName(s, iptr, buffer) == 1)
            return raiseError(context, line, *iptr - line_start,
                    "Invalid module name.");
    }
    else
        return raiseError(context, line, *iptr - line_start,
                "Missing module name or module type.");
    strncpy(module_name, buffer, BUFFER_SIZE);

    processWhiteSpace(s, iptr);
    if (s[*iptr] == ':')
        (*iptr)++;
    else
        return raiseError(context, line, *iptr - line_start, "Missing colon.");

    processWhiteSpace(s, iptr);
    if (islower(s[*iptr]))
    {
        if (processModuleConnector(s, iptr, buffer) == 1)
            return raiseError(context, line, *iptr - line_start,
                    connection_type == C_INPUT ? "Invalid module input name."
                    : "Invalid module output name.");
    }
    else
        return raiseError(context, line, *iptr - line_start,
                connection_type == C_INPUT ? "Missing module input name."
                : "Missing module output name");
    strncpy(module_connector, buffer, BUFFER_SIZE);

    //update the context with the new module
    if (context->result.num_modules < MAX_NUM_MODULES)
    {
        context->result.modules[context->result.num_modules].module_type
            = module_type;
        context->result.num_modules++;
    }
    else
        return raiseError(context, line, *iptr - line_start,
                "Maximum number of modules reached.");
    
    return 0;
}

static int
processLine(ParseContext *context, int line, char* s, int *iptr)
{
    int start = *iptr;

    processWhiteSpace(s, iptr);
    if (processConnectionPoint(context, line, start, C_INPUT, s, iptr)) return 1;
    processWhiteSpace(s, iptr);
    if (processArrowOperator(s, iptr) == 1)
        return raiseError(context, line, *iptr - start, "Error processing arrow operator.");
    processWhiteSpace(s, iptr);
    if (processConnectionPoint(context, line, start, C_OUTPUT, s, iptr)) return 1;
    processWhiteSpace(s, iptr);

    if (context->result.num_connections < MAX_NUM_CONNECTIONS)
    {
        context->result.connections[context->result.num_connections] = (Connection){
            .src_module_index = context->result.num_modules - 2,
            .src_module_output_index = 0,
            .dst_module_index = context->result.num_modules - 1,
            .dst_module_input_index = 0
        };
        context->result.num_connections ++;
    }
    else
        return raiseError(context, line, *iptr - start,
                "Maximum number of connections reached.");

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

    //prepare result
    context.result.modules = (AbstractModule*) malloc(sizeof(AbstractModule)
            * MAX_NUM_MODULES);
    context.result.num_modules = 0;
    context.result.connections = (Connection*) malloc(sizeof(Connection)
            * MAX_NUM_CONNECTIONS);
    context.result.num_connections = 0;
    
    while (context.err == 0 && s[i] != '\0')
    {
        processLine(&context, line, s, &i);
        i++;
        line++;
    }

    if (context.err == 0)
        snprintf(context.errmsg, BUFFER_SIZE,
                "Patch processed successfully.");

    return context;
}
