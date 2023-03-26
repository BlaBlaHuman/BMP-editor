#ifndef CLIST_H_
#define CLIST_H_

#define container_of(ptr, type, member) (type*)((char*)(ptr) - offsetof(type, member))

struct intrusive_node {
  struct intrusive_node *next;
  struct intrusive_node *prev;
};

struct intrusive_list {
  struct intrusive_node* head;
};

struct pixel {
  int x, y;
  char color;
  struct intrusive_node node;
};


void init_list(struct intrusive_list* list);
struct intrusive_node* add_node(struct intrusive_list* head, struct intrusive_node* node);
struct intrusive_node* remove_node(struct intrusive_list* head, struct intrusive_node* node);
struct intrusive_node* get_node_by_index(struct intrusive_list* list, int index);

int get_length(struct intrusive_list* head);

void remove_all_pixels(struct intrusive_list* list);

#endif
