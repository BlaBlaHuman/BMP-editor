#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void remove_all_pixels(struct intrusive_list* list) {
    while (list->head->next != NULL) {
      struct pixel* p = container_of(list->head->next, struct pixel, node);
      remove_node(list, list->head->next);
      free(p);
    }
}

void init_list(struct intrusive_list* list) {
	list->head = malloc(sizeof(*(list->head)));
	list->head->next = NULL;
	list->head->prev = NULL;
}

struct intrusive_node* add_node(struct intrusive_list* list, struct intrusive_node* node) {
	struct intrusive_node* iterator = list->head;
	while (iterator->next != NULL) { 
		iterator = iterator->next;
 	}
 	iterator->next = node;
 	node->prev = iterator;
 	node->next = NULL;
 	return node;
}

struct intrusive_node* remove_node(struct intrusive_list* list, struct intrusive_node* node) {
	struct intrusive_node* iterator = list->head->next;
	while (iterator != NULL) {
	    if (iterator->next == node->next && iterator->prev == node->prev) {
	    	iterator->prev->next = iterator->next;
	    	if (iterator->next != NULL)
	    		iterator->next->prev = iterator->prev;
	    	return iterator->next;
	    }
	    else 
	    	iterator = iterator->next;
 	}
 	return iterator;
}



int get_length(struct intrusive_list* list) {
	struct intrusive_node* iterator = list->head;
	int n = 0;
	while (iterator->next) {
		n += 1;
		iterator = iterator->next;
	}
	return n;
}

struct intrusive_node* get_node_by_index(struct intrusive_list* list, int index) {
	struct intrusive_node* iterator = list->head;
	int n = 0;
	while (iterator->next) {
		n += 1;
		iterator = iterator->next;
		if (n - 1 == index)
			return iterator;
	}
	return NULL;

}
