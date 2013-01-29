#include "list.h"
#include <stdio.h>
#include <stdlib.h>

node_t *node_new(node_t *next, T data)
{
	node_t *new_node;

	new_node = malloc(sizeof(node_t));
	new_node->next = next;
	new_node->data = data;

	return new_node;
}

node_t *find_node(node_t *first, int pos)
{
	int i;
	for (i = 0; i < pos; i++) {
		first = first->next;
	}
	return first;
}

void list_init(list_t *list)
{
	list->len = 0;
	list->head = NULL;
}

void list_free(list_t *list)
{
	node_t *to_be_free;
	node_t *head;

	if (NULL == list->head)
		return ;

	head = list->head->next;
	list->head->next = NULL;

	while (NULL != head) {
		to_be_free = head;
		head = head->next;
		free(to_be_free);
	}

	list->len = 0;
	list->head = NULL;
}

void list_append(list_t *list, T data)
{
	int i;
	node_t *new_node, *prev;
	
	/* 往空链表中加入第一个元素 */
	if (0 == list_len(list)) {
		new_node = node_new(NULL, data);
		new_node->next = new_node;
		list->head = new_node;
	} else {
		/* 在最后一个节点后面插入元素 */
		prev = find_node(list->head, list_len(list)-1);
		new_node = node_new(prev->next, data);
		prev->next =  new_node;
	}
	
	list->len++;
}

void list_prepend(list_t *list, T data)
{

}

void list_delete(list_t *list, int pos)
{
	node_t *prev, *cur;
	if (0 == pos) {
		cur = list->head;
		list->head = cur->next;
		free(cur);
		list->len--;
		return ;
	}
	//找到前一个节点
	prev = find_node(list->head, pos-1);
	if (NULL == prev)
		return ;
	//找到要删除的节点
	cur = find_node(list->head, pos);
	if (NULL == cur)
		return ;
	//前一个节点的next，指向
	//当前节点的next
	prev->next = cur->next;
	free(cur);

	//长度--
	list->len--;
}

int	list_len(list_t *list)
{
	return list->len;
}

void list_for_each(list_t *list, FUNC func)
{
	int i;
	node_t *tmp;
	for (i = 0; i < list_len(list); i++) {
		tmp = find_node(list->head, i);
		func(&tmp->data);
	}
}

int list_get(list_t *list, int pos, T *pdata)
{
	node_t *node;

	node = find_node(list->head, pos);

	if (NULL != node) {
		*pdata = node->data;
		return 1;
	}

	return 0;
}
