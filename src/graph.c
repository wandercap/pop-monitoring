#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/util.h"

void initGraph(char *links, char *edges, graph_p graph) {
  FILE* l;
  FILE* e;
  char buffer[1000];
  info_node_p info;
  int node, i;

  l = fopen(links, "r");
  if(!l)
      err_exit("Unable to open the file!");

  e = fopen(edges, "r");
  if(!e)
      err_exit("Unable to open the file!");

  while(fgets(buffer, 1000, l)) {
    info = initInfo();

    sscanf(buffer, "%s %s %s %s", info->uf, info->name, info->dns, info->ip);
    
    createVertice(graph, info);
  }

  i = 0;
  node = 0;
  while(!feof(e)) {
    while (fscanf(e, "%d", &node) && node != -1) {
      addEdge(graph, graph->array[i].head, graph->array[node].head, i);
    }

    i++;
  }

  fclose(l);
  fclose(e);
}

info_node_p initInfo() {
  info_node_p info = (info_node_p)malloc(sizeof(info_node_t));
  info->uf = (char *)malloc(sizeof(char));
  info->name = (char *)malloc(sizeof(char));
  info->dns = (char *)malloc(sizeof(char));
  info->ip = (char *)malloc(sizeof(char));

  return info;
}

list_node_p createNode(info_node_p info) {
  list_node_p node = (list_node_p)malloc(sizeof(list_node_t));
  if(!node)
    err_exit("Unable to create a new Node!");

  node->info = info;
  node->next = NULL;

  return node;
}

void createVertice(graph_p graph, info_node_p info) {
  graph->vertices++;

  graph->array = (list_p)realloc(graph->array, (graph->vertices)*sizeof(list_t));
  if(!graph->array)
    err_exit("Unable to realloc the Graph!");

  list_node_p newNode = createNode(info);

  graph->array[graph->vertices-1].head = newNode;
  graph->array[graph->vertices-1].length = 0;

}

void addEdge(graph_p graph, list_node_p src, list_node_p dest, int index) {

  list_node_p node = createNode(dest->info);
  
  list_node_p aux = src;
  
  while(aux->next) {
    aux = aux->next;
  }

  aux->next = node;
  aux->next->vertex = dest;

  graph->array[index].length++;
  
}

graph_p createGraph(int n) {
  int i;

  graph_p graph = (graph_p)malloc(sizeof(graph_t));
  if(!graph)
    err_exit("Unable to create a new Graph!");

  graph->vertices = n;

  graph->array = (list_p)malloc(n*sizeof(list_t));
  if(!graph->array)
    err_exit("Unable to create a new Array!");

  for(i=0; i<n; i++) {
    graph->array[i].head = NULL;
    graph->array[i].length = 0;
  }

  return graph;
}

void destroyGraph(graph_p graph) {
  if(graph) {
    if(graph->array) {
      int v;
    
      for(v=0; v<graph->vertices; v++) {
        list_node_p listPtr = graph->array[v].head;

        while(listPtr) {
          list_node_p tmp = listPtr;
          listPtr = listPtr->next;
          free(tmp);
        }

      }

      free(graph->array);
    }

    free(graph);
  } else {
    err_exit("The Graph is empty!");
  }

}

void printGraph(graph_p graph) {
  if(!graph)
    err_exit("The Graph is empty!");

  int i;

  for(i=0; i<graph->vertices; i++) {
    list_node_p listPtr = graph->array[i].head;
    printf("\nVertice %s: ", listPtr->info->uf);
    listPtr = listPtr->next;
    while(listPtr) {
      printf("%s", listPtr->info->uf);
      listPtr = listPtr->next;
      if(listPtr)
        printf("->");
    }
    
    //printf("NULL");
  }
}
