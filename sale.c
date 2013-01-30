#include "sale.h"
#include <stdlib.h>

/*
 * 新建一个销售单
 * 参数num：
 *		销售单号
 */
sale_t * make_new_sale(int num)
{
	sale_t *new_sale;

	new_sale = malloc(sizeof(sale_t));
	new_sale->sale_num = num;
	
	list_init(&new_sale->item_list);

	return new_sale;
}

/*
 * 结束一次销售，计算总金额
 */
double end_sale(sale_t *sale)
{
	int i;
	list_t *list;
	line_item_t *item;
	double total = 0;

	list = &sale->item_list;

	for (i = 0; i < list_len(list); i++) {
		list_get(list, i, &item);
		total += item->total;
	}

	return total;
}

/*
 * 结账
 * 参数total:
 *		应付的金额
 * 参数pay:
 *		支付了的金额
 */
double make_payment(double total, double pay)
{
	/* 
	 * update_outcome(total-pay);
	 * update_income(pay);
	 */
	if (pay >= total)
	{
		return pay - total;
	}else{
		return -1;
	}
}

/*
 * 往销售单里添加一个新的行
 */
void enter_line_item(sale_t *sale, line_item_t *item)
{
	list_append(&sale->item_list, item);
}

/*
 * 删除当前的销售单
 */
void delete_sale(sale_t *sale)
{
	list_free( &sale->item_list );
}


/*
 * 获取销售单中元素个数
 */
int get_sale_item_counts(sale_t *sale)
{
	return list_len(&sale->item_list);
}


/*
 * 获取销售单第pos个位置上的元素
 */
int get_sale_item(sale_t *sale, int pos, line_item_t **item)
{
	return list_get(&sale->item_list,
		pos, item);
}

/*
 * 删除销售单中的第pos个位置上的元素
 */
void del_sale_item(sale_t *sale, int pos)
{
	list_delete(&sale->item_list, pos);
}
