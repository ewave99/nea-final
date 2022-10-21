typedef struct 
{
    int source;
    int destination;
} AdjacencyListEntry;

typedef struct
{
    AdjacencyListEntry* adjacency_list;

    int error_code;
    char* error_message;
} ParseResult;

ParseResult processText(const char*);
