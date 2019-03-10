#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../include/graph.h"
#include "../include/util.h"

#define INTERFACE "enp4s0"
#define IFCONFIG "sudo ifconfig"
#define IPROUTEADD "sudo ip route add"
#define ROUTEDEL "sudo route del"
#define TRACEROUTE "sudo traceroute -I"
#define TENMIN 600 // 600 s
#define DAY 144 // 144x10min

void execTraceroute(graph_p graph, int index);
void putInterfaces(graph_p graph);
void savePath (info_node_p node, int index);

int main(int argc, char **argv) {
    int i;
    graph_p graph = createGraph(0);

    initGraph(argv[1], argv[2], graph);

    //putInterfaces(graph);
    for(i=0; i<DAY; i++) {
        execTraceroute(graph, i);
        sleep(TENMIN);
    }

    //printGraph(graph);

}

void execTraceroute(graph_p graph, int index) {
    int i;
    char command[1000];

    for(i=0; i<graph->vertices; i++) {
        sprintf(command, "%s %s > trace/traceroute-%s-%d.txt", TRACEROUTE, graph->array[i].head->info->ip, graph->array[i].head->info->uf, index);
        system(command);

        savePath(graph->array[i].head->info, index);

        node_path_p aux;
        aux = graph->array[i].head->info->pathnew;

        while (aux->nexthop != NULL){
			printf("caminho: %s\n",aux->path);
			aux = aux->nexthop;
        }
        
        comparePath(graph->array[i].head->info);
    }
}


void comparePath(info_node_p node){
	if (node->pathold == NULL){
		node->pathold = node->pathnew;
		return;
	}
	
	node_path_p old = node->pathold;
	node_path_p new = node->pathnew;
	
	while ((strcmp(old->path,new->path) == 0) && (old->nexthop != NULL)){
		old = old->nexthop;
		new = new->nexthop;
    }
    
    if (strcmp(old->path,new->path) != 0) 
        printf("Route changed! Enlace %s is down\n",old->path);
}

void savePath (info_node_p node, int index){
	char caminho[1000];
	char reader[100];
    char hop[2];
    FILE *log;
    
    sprintf(caminho,"trace/traceroute-%s-%d.txt",node->uf, index);
    
    log = fopen(caminho,"r");
    
	node->pathnew = (node_path_p)malloc(sizeof(node_path_t));
	node_path_p pathaux;
    pathaux = node->pathnew;	
    
	for (int j = 0; j < 10; j++)
        fscanf(log,"%s",reader);
        
	while (fscanf(log,"%s",reader) != EOF){
		strcpy(hop,reader);
		if (strcmp(reader,"*") != 0){
			fscanf(log,"%s",reader);
			if (strcmp(reader,"*") != 0){
				strcpy(pathaux->path,reader);
				//printf("%s\n",pathaux->path);
				pathaux->nexthop = (node_path_p ) malloc (sizeof (node_path_t));
				pathaux = pathaux->nexthop;
				for (int j = 0; j < 7; j++){
					fscanf(log,"%s",reader);
					if (strcmp(reader,"*") == 0)
						j++;
				}
					
		    }
		}
	}
	if ((strcmp(reader,"*") == 0) && (strcmp(hop,"30") == 0)){
		pathaux->nexthop = (node_path_p ) malloc (sizeof (node_path_t));
		strcpy(pathaux->path,"DESTINATION NOT REACHED.");
		pathaux = pathaux->nexthop;
	}
	
	
	fclose(log);
	free(pathaux);
	return;

}

void putInterfaces(graph_p graph) {
    int i;
    char command[1000];

    for(i=0; i<graph->vertices; i++) {
        sprintf(command, "%s %s:%d %s", IFCONFIG, INTERFACE, i, graph->array[i].head->info->ip);
        //printf("%s \n", command);
        system(command);
    }

}
