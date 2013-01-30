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

BOOL is_end_search = FALSE;
int search_sale_item_selected;

static void search_sale_init();
static void show_search_sale_win();
static void search_sale_doit(KEY_T key);
static void search_sale_show_list();
static void search_sale_show_detail();
static void search_sale_up();
static void search_sale_down();
static void search_sale_get();


static void search_by_date();
static void search_by_staff();

int is_method_selected;
int method_selected;
int is_method_obj_selected;
static void get_method();

void search_sale()
{
	KEY_T key;
	
	search_sale_init();
	
	while (!is_end_search) {
		show_search_sale_win();

		if ( !is_method_selected )
		{
			get_method();
		}

		if ( !is_method_obj_selected)
		{
			if ( method_selected )
			{
				// 查询日期
				search_by_date();
			}else
			{
				// 查询员工
				search_by_staff();
			}
		}

		key = getkey();
		search_sale_doit(key);
	}
}

void show_search_sale_win()
{
	clear_win();
	move_to(0, 0);

	printf("┌──────────────────────────────────────────────────────────┐\n");
	printf("│                                                          │\n");
	printf("├─────────────┬────────────────────────────────────────────┤\n");
	printf("│     选项    │   序号 条码 商品名称 单价 折扣 数量 金额   │\n");
	printf("├─────────────┼────────────────────────────────────────────┤\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("│             │                                            │\n");
	printf("├─────────────┴────────────────────────────────────────────┤\n");
	printf("│  上下选择  回车选中  TAB退出             时间:           │\n");
	printf("└──────────────────────────────────────────────────────────┘\n");
	
	/* 显示挂单表 */
	search_sale_show_list();
	
	/* 显示明细 */
	search_sale_show_detail();
	
	/* 显示时间 */
	show_time();
}

static void search_sale_init()
{
	is_end_search = FALSE;
	search_sale_item_selected = 0;
}

static void get_method()
{
	move_to(20, 8);
	printf("┌─────────────────┐\n");
	move_to(20, 9);
	printf("│                 │\n");
	move_to(20, 10);
	printf("│                 │\n");
	move_to(20, 11);
	printf("│                 │\n");
	move_to(20, 12);
	printf("│                 │\n");
	move_to(20, 13);
	printf("│                 │\n");
	move_to(20, 14);
	printf("└─────────────────┘\n");

	move_to(24, 10);

	if ( !method_selected )
		color_on(INVERSE, BLACK, WHITE);

	printf("   按员工   ");
	
	if ( !method_selected )
		color_off();

	move_to(24, 12);

	if ( method_selected )
		color_on(INVERSE, BLACK, WHITE);

	printf("   按时间   ");

	if ( method_selected )
		color_off();
}

void search_sale_doit(KEY_T key)
{
	if (!is_method_selected)
	{
		
		switch (key) {
			case KEY_UP:

			case KEY_DOWN:
				method_selected = !method_selected;
				break;
			case KEY_ENTER:
				is_method_selected = 1;
				break;
		}

	}else{
		switch (key) {
			case KEY_UP:
				search_sale_up();
				break;
			case KEY_DOWN:
				search_sale_down();
				break;
			case KEY_ENTER:
				search_sale_get();
				break;
		}
	}
}

void search_sale_show_list()
{
	int i;
	for (i = 0; i < npends; i++) {
		move_to(4, 4+i);

		if (search_sale_item_selected == i)
		{
			color_on(INVERSE, BLACK, WHITE);
		}

		printf("第 %d 单", pend_sales[i]->sale_num);
		
		if (search_sale_item_selected == i)
		{
			color_off();
		}
	}
}

void search_sale_show_detail()
{
	int i;
	sale_t *sale;
	list_t *list;
	line_item_t *item;
	
	if (npends <= 0) {
		return ;
	}
	
	sale = pend_sales[search_sale_item_selected];
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

static void search_sale_up()
{
	if (search_sale_item_selected > 0)
	{
		search_sale_item_selected--;
	}
}

static void search_sale_down()
{
	if (search_sale_item_selected < npends-1)
	{
		search_sale_item_selected++;
	}	
}

static void search_sale_get()
{
	int i;
	sale_t *toget;
	toget = pend_sales[search_sale_item_selected];
	
	if (npends <= 0) {
		is_end_search = TRUE;
		return ;
	}
	
	for (i = search_sale_item_selected; i < npends-1; i++) {
		pend_sales[i] = pend_sales[i+1];	
	}
	npends--;
	
	// list_free(&cur_sale->item_list);
	// free(cur_sale);
	cur_sale = toget;
	
	is_end_search = TRUE;
}


static void search_by_date()
{

}

static void search_by_staff()
{

}

static void get_date()
{
	move_to(20, 8);
	printf("┌─────────────────┐\n");
	move_to(20, 9);
	printf("│                 │\n");
	move_to(20, 10);
	printf("│                 │\n");
	move_to(20, 11);
	printf("│                 │\n");
	move_to(20, 12);
	printf("│                 │\n");
	move_to(20, 13);
	printf("│                 │\n");
	move_to(20, 14);
	printf("└─────────────────┘\n");

	move_to(24, 10);
	printf("请输入日期：   ");
	move_to(24, 12);
	printf("");
}

static void get_staff()
{
	move_to(20, 8);
	printf("┌─────────────────┐\n");
	move_to(20, 9);
	printf("│                 │\n");
	move_to(20, 10);
	printf("│                 │\n");
	move_to(20, 11);
	printf("│                 │\n");
	move_to(20, 12);
	printf("│                 │\n");
	move_to(20, 13);
	printf("│                 │\n");
	move_to(20, 14);
	printf("└─────────────────┘\n");

	move_to(24, 10);
	printf("请输入日期：   ");
	move_to(24, 12);
	printf("");
}
















