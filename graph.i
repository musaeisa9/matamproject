%module graph
%{
#include "graph.h"
%}
Graph* create();

void destroy(Graph* graph_ptr);

Graph* addVertex(Graph* graph_ptr, char* vertex);

Graph* addEdge(Graph* graph_ptr, char* vertex1, char* vertex2);

void disp(Graph* graph_ptr);

Graph* graphUnion(Graph* graph_ptr1, Graph* graph_ptr2, Graph* graph_ptr_out);

Graph* graphIntersection(Graph* graph_ptr1, Graph* graph_ptr2, Graph* graph_ptr_out);

Graph* graphDifference(Graph* graph_ptr1, Graph* graph_ptr2, Graph* graph_ptr_out);

Graph* graphProduct(Graph* graph_ptr1, Graph* graph_ptr2, Graph* graph_ptr_out);

Graph* graphComplement(Graph* graph_ptr, Graph* graph_ptr_out);