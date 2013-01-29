#ifndef SALE_H
#define SALE_H

#include "list.h"

typedef struct {
	int		sale_num;
	list_t	item_list;
} sale_t;

sale_t *make_new_sale(int num);
double end_sale(sale_t *sale);
double make_payment(double total, double pay);
void enter_line_item(sale_t *sale,
	line_item_t *item);
void delete_sale(sale_t *sale);
int get_sale_item_counts(sale_t *sale);
int get_sale_item(sale_t *sale, int pos, line_item_t **item);
void del_sale_item(sale_t *sale, int pos);

#endif
