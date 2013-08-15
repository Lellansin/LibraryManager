#include "sys_main.h"
#include "utils.h"
#include "sale.h"
#include "ctrl.h"
#include "store.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

/*
 * 销售单
 */
const int ITEM_PER_PAGE = 9;
int item_selected;
int cur_page;
sale_t *cur_sale;
char err_str[ERR_LEN];

sale_t *tmp_sale;

int sale_num;
sale_t *pend_sales[MAX_PEND];
int npends;

static void init_clock();
static void update_clock();
static void show_main_window();
static void show_list();
static void show_error_tips();
static void show_page_number();
static void init_system();

/*
 * 1. 显示界面
 * 2. 获得输入
 * 3. 处理输入
 */
void sys_main()
{
	KEY_T key;

	init_system();
	Goods_init();

	while (1) 
	{
		show_main_window();
		key = getkey();
		doit(key);
	}
}

static void init_system()
{
	cur_sale = make_new_sale(1);
	cursor_hide();
	init_clock();
	update_clock();
}

/*
 * 显示主界面
 */
static void show_main_window()
{
	/*
	 * 声明变量username
	 */
	extern char username[];

	clear_win();
	move_to(0, 0);
	printf("┌────────────────────────────────┬─────────────────────────────┐\n");
	printf("│  销售单号:                     │ 收银员:                     │\n");
	printf("├────────────────────────────────┼─────────────────────────────┤\n");
	printf("│  商品条码:                     │ 数  量:                     │\n");
	printf("├────────────────────────────────┴────────┬────────────────────┤\n");
	printf("│  序号 条码 商品名称 单价 折扣 数量 金额 │      提示信息      │\n");
	printf("├─────────────────────────────────────────┤                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│                                         │                    │\n");
	printf("│   第   页 ←上一页→下一页↑上一项↓下一项  │                    │\n");
	printf("├─────────────────────────────────────────┴────────────────────┤\n");
	printf("│   F2删除商品明细  F3结账 F4撤单 F6挂单  F7取单 F8添加        │\n");
	printf("│   F9注销                                 时间:               │\n");
	printf("└──────────────────────────────────────────────────────────────┘\n");

	/*
	 * 显示销售单号
	 */
	move_to(14, 2);
	printf("%05d", cur_sale->sale_num);

	/*
	 * 显示销售员
	 */
	move_to(44, 2);
	printf("%s", username);

	/*
	 * 显示商品列表
	 */
	show_list();

	/*
	 * 显示页码
	 */
	show_page_number();

	/*
	 * 显示错误提示
	 */
	show_error_tips();
}

/*
 * 显示商品列表
 */
static void show_list()
{
	int i, cnt;
	list_t *list;
	line_item_t *item;

	list = &cur_sale->item_list;

	/*
	 * 遍历商品列表
	 */
	i = cur_page * ITEM_PER_PAGE;

	for (cnt = 0; i < list_len(list) &&	cnt < ITEM_PER_PAGE; i++,cnt++) {

		list_get(list, i, &item);
		move_to(2, 8+cnt);

		printf(" ");

		if (cnt == item_selected) {
			color_on(INVERSE, BLACK, WHITE);
		}

		printf("%3d%6s   %-6s%6.2lf%5.1lf%4d %6.2lf",
			cnt,
			item->code,
			item->name,
			item->price,
			item->discount,
			item->count,
			item->total);

		if (cnt == item_selected) {
			color_off();
		}
	}
}

static void show_page_number()
{
	move_to(8, 17);
	printf("%d", cur_page);
}

static void show_error_tips()
{
	move_to(46, 8);
	printf("%s", err_str);
	err_str[0] = '\0';
}

static void init_clock()
{
	signal(SIGALRM, update_clock);
	update_clock();
}

static void update_clock()
{
	show_time();
	alarm(1);
}

void show_time()
{
	static int i = 0;
	char time_str[TIME_STR_LEN] = {0, };
	
	cursor_save_location();

	time_t t = time(NULL);
	struct tm *cur_time;

	cur_time = localtime(&t);
	
	sprintf(time_str, "%02d:%02d:%02d",
		cur_time->tm_hour,
		cur_time->tm_min,
		cur_time->tm_sec);

	move_to(50, 20);
	printf("%s", time_str);
	fflush(stdout);

	cursor_restore_location();
}

