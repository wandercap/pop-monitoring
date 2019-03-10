#ifndef _GRAPH_H_
#define _GRAPH_H_


typedef struct node_path {
	char path[100];
	struct node_path *nexthop;
}node_path_t, *node_path_p;

typedef struct info_node {
  char* uf;
  char* name;
  char* dns;
  char* ip;
  node_path_t *pathold;
  node_path_t *pathnew;
} info_node_t, *info_node_p;

typedef struct list_node {
  info_node_t *info;
  struct list_node *vertex;
  struct list_node *next;
} list_node_t, *list_node_p;

typedef struct list {
  int length;
  list_node_t *head;
} list_t, *list_p;

typedef struct graph {
  int vertices;
  list_p array;
} graph_t, *graph_p;

void initGraph(char *links, char *edges, graph_p graph);
info_node_p initInfo();
list_node_p createNode(info_node_p info);
void createVertice(graph_p graph, info_node_p info);
void addEdge(graph_p graph, list_node_p src, list_node_p dest, int index);
graph_p createGraph(int n);
void destroyGraph(graph_p graph);
void printGraph(graph_p graph);  

#endif
