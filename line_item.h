#ifndef LINE_ITEM_H
#define LINE_ITEM_H

#include "utils.h"

typedef struct {
	int		num;
	char	code[CODE_LEN];
	char	name[NAME_LEN];
	double	price;
	double	discount;
	int		count;
	double	total;
} line_item_t;

line_item_t *line_item_new(int num, char *code, 
	char *name, double price, double discount,
	int count);
void line_item_delete(line_item_t *lt);


#endif
