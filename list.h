#ifndef LIST_H

	#define LIST_H

	#include "line_item.h"

	typedef line_item_t * T;

	typedef struct node {
		T data;
		struct node *next;
	} node_t;

	typedef struct list {
		int len;
		node_t *head;
	} list_t;

	typedef void(*FUNC)(T *data);

	void list_init(list_t *list);
	void list_free(list_t *list);
	void list_append(list_t *list, T data);
	void list_prepend(list_t *list, T data);
	void list_delete(list_t *list, int pos);
	int  list_get(list_t *list, int pos, T *pdata);
	int	 list_len(list_t *list);
	void list_for_each(list_t *list, FUNC func);

#endif
