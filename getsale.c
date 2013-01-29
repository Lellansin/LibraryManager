#include "getsale.h"
#include "utils.h"
#include "sys_main.h"
#include "sale.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

extern int npends;
extern sale_t *pend_sales[];
extern sale_t *cur_sale;

BOOL is_end = FALSE;
int get_sale_item_selected;

static void get_sale_init();
static void show_get_sale_win();
static void get_sale_doit(KEY_T key);
static void get_sale_show_list();
static void get_sale_show_detail();
static void get_sale_up();
static void get_sale_down();
static void get_sale_get();

void get_sale()
{
	KEY_T key;
	
	get_sale_init();
	
	while (!is_end) {
		show_get_sale_win();
		key = getkey();
		get_sale_doit(key);
	}
}

void show_get_sale_win()
{
	clear_win();
	move_to(0, 0);

	printf("┌───────────┬─────────────────────────────────────────┐\n");
	printf("│           │ 明细 ：                                 │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("│           │                                         │\n");
	printf("└─────────────────────────────────────────────────────┘\n");
	
	/* 显示挂单表 */
	get_sale_show_list();
	
	/* 显示明细 */
	get_sale_show_detail();
	
	/* 显示时间 */
	show_time();
}

static void get_sale_init()
{
	is_end = FALSE;
	get_sale_item_selected = 0;
}

void get_sale_doit(KEY_T key)
{
	switch (key) {
	case KEY_UP:
		get_sale_up();
		break;
	case KEY_DOWN:
		get_sale_down();
		break;
	case KEY_ENTER:
		get_sale_get();
		break;
	}
}

void get_sale_show_list()
{
	int i;
	for (i = 0; i < npends; i++) {
		move_to(4, 4+i);

		if (get_sale_item_selected == i)
		{
			color_on(INVERSE, BLACK, WHITE);
		}

		printf("第 %d 单", pend_sales[i]->sale_num);
		
		if (get_sale_item_selected == i)
		{
			color_off();
		}
	}
}

void get_sale_show_detail()
{
	int i;
	sale_t *sale;
	list_t *list;
	line_item_t *item;
	
	if (npends <= 0) {
		return ;
	}
	
	sale = pend_sales[get_sale_item_selected];
	list = &sale->item_list;
	for (i = 0; i < ITEM_SHOWS && i < list_len(list); i++) {
		list_get(list, i, &item);
		move_to(19, 4+i);
		printf(" %d  %s  %s  %.2lf  %.2lf %d %.2lf",
			i,
			item->code,
			item->name,
			item->price,
			item->discount,
			item->count,
			item->total);
	}
	
}

static void get_sale_up()
{
	if (get_sale_item_selected > 0)
	{
		get_sale_item_selected--;
	}
}

static void get_sale_down()
{
	if (get_sale_item_selected < npends-1)
	{
		get_sale_item_selected++;
	}	
}

static void get_sale_get()
{
	int i;
	sale_t *toget;
	toget = pend_sales[get_sale_item_selected];
	
	if (npends <= 0) {
		is_end = TRUE;
		return ;
	}		
	
	for (i = get_sale_item_selected;
		i < npends-1; i++) {
		pend_sales[i] = pend_sales[i+1];	
	}
	npends--;
	
	list_free(&cur_sale->item_list);
	free(cur_sale);
	cur_sale = toget;
	
	is_end = TRUE;
}
















