#include <stdlib.h>
#include "text.h"

ParseResult
processText(const char* input)
{
    ParseResult result;
    AdjacencyListEntry* adjacency_list;

    adjacency_list = (AdjacencyListEntry*) malloc(
            sizeof(AdjacencyListEntry) * 2);

    adjacency_list[0].source = 0;
    adjacency_list[0].destination = 1;
    
    // like a null-terminator
    adjacency_list[1].source = -1;
    adjacency_list[1].destination = -1;

    result.adjacency_list = adjacency_list;
    result.error_code = 1;
    result.error_message = "Some error has occurred.";

    return result;
}
