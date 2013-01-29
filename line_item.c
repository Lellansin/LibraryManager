#include "line_item.h"
#include <string.h>
#include <stdlib.h>

line_item_t *line_item_new(int num,
	char *code, 
	char *name,
	double price,
	double discount,
	int count)
{
	line_item_t *new_item;

	new_item = (line_item_t *)malloc(sizeof(line_item_t));

	new_item->num = num;
	strcpy(new_item->code, code);
	strcpy(new_item->name, name);
	new_item->price = price;
	new_item->discount = discount;
	new_item->count = count;
	new_item->total = price * count *discount;

	return new_item;
}

void line_item_delete(line_item_t *lt)
{
	free(lt);
}
