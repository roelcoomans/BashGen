#include <stdlib.h>
#include <string.h>
#include "list.h"

list *list_new(void)
{
	list *l = NULL;

	l = calloc(1, sizeof(list));
	return l;
}

void list_free(list *l)
{
	if (NULL != l) {
		node *item = l->head;
		while (NULL != item) {
			node *next = item->next;
			if (NULL != item->command) {
				free(item->command);
			}
			if (item->description) {
				free(item->description);
			}
			free(item);
			item = next;
		}
		free(l);
	}
}

int list_length(const list *l)
{
	int nelem = -1;

	if (NULL != l) {
		nelem = l->nelem;
	}
	return nelem;
}

bool list_add(list *l,
	      char c,
	      char *command,
	      char *description,
	      bool input)
{
	bool result = false;
	node *item = NULL;

	do {
		if (NULL == l) {
			break;
		}
		item = calloc(1, sizeof(*item));
		if (NULL == item) {
			break;
		} else {
			item->c = c;
			if (NULL != command) {
				item->command = strdup(command);
			}
			if (NULL != description) {
				item->description = strdup(description);
			}
			item->input = input;
			item->next = l->head;
			l->head = item;
			l->nelem++;
			result = true;
		}
	} while (0);
	return result;
}

void list_foreach(list *l,
		  list_callback cb)
{
	node *prev = NULL, *it;

	it = l->head;
	while (NULL != it) {
		cb(it);
		it = it->next;
	}
}
