#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct node_s {
        struct node_s *next;
        char c;
        char *command;
        char *description;
        bool input;
} node;

typedef struct list_s {
        node *head;
        int nelem;
} list;

/* Create a new list */
list *list_new(void);

/* Free the list and all its elements */
void list_free(list *l);

/* return the number of items in the list */
int list_length(const list *l);

/* add an item to the list */
bool list_add(list *l,
              char c,
              char *command,
              char *description,
              bool input);

/* Callback function for the foreach function */
typedef void (*list_callback)(node *data);

/* Call the callback function for each element in the list */
void list_foreach (list *l,
                   list_callback cb);

#endif /* LIST_H_ */
