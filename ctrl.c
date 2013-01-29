#include "ctrl.h"
#include "sale.h"
#include "line_item.h"
#include "store.h"

#include "getsale.h"


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char username[];
extern int cur_page;
extern sale_t *cur_sale;
extern const int ITEM_PER_PAGE;
extern int item_selected;
extern char err_str[];

extern int sale_num;
extern sale_t *pend_sales[];
extern int npends;

static void on_key_left();
static void on_key_right();
static void on_key_up();
static void on_key_down();
static void on_key_f8();
static void on_key_f2();
static void on_key_f3();
static void on_key_f4();

static void on_key_f6();
static void on_key_f7();

static void check_out();
static void save_data();
static void save_sale();
static void save_some_sale(FILE *file, sale_t *sale);

void show_check_out_box(double total);
double get_payment();
void show_change(double change);

void doit(KEY_T key)
{
	switch (key) {
		
		case KEY_LEFT:
			on_key_left();
		break;
		
		case KEY_RIGHT:
			on_key_right();
		break;
		
		case KEY_UP:
			on_key_up();
		break;
		
		case KEY_DOWN:
			on_key_down();
		break;
		
		case KEY_F8:
			on_key_f8();
		break;
		
		case '2'://KEY_F2:
			on_key_f2();
		break;
		
		case '3':
			on_key_f3();
		break;

		case '4':
			on_key_f4();
		break;
		
		case '6':
			on_key_f6();
		break;

		case '7':
			on_key_f7();
			break;

		case KEY_TAB:
			printf("退出操作");
			sleep(1000);
			move_to(0,0);
			clear_win();
			exit(0);
		break;
		
		default:
		break;
	}
}

static void on_key_left()
{
	if (cur_page > 0) {
		cur_page--;
	}
}

static void on_key_right()
{
	int first_num_of_next_page;

	first_num_of_next_page = (cur_page + 1)* ITEM_PER_PAGE;

	if (first_num_of_next_page < 
		get_sale_item_counts(cur_sale)) {
		cur_page++;
		item_selected = 0;
	}
}

static void on_key_up()
{
	if (item_selected > 0) {
		item_selected--;
	}
}

static void on_key_down()
{
	int pos_of_item_selected;

	pos_of_item_selected = cur_page * ITEM_PER_PAGE + item_selected;
	if (item_selected < ITEM_PER_PAGE - 1 && 
		pos_of_item_selected < get_sale_item_counts(cur_sale)-1) {
		item_selected++;
	}
}

static void get_code(char *code)
{
	move_to(14, 4);
	get_str(code, CODE_LEN, NORMAL);
}

static void get_counts(int *counts)
{
	move_to(44, 4);
	scanf("%d", counts);
	getch();
}

/*
 * linus talk is cheap show me the code;
 */
static void on_key_f8()
{
	good_t good;

	int counts = 3, err;
	char code[CODE_LEN] = {0, };

	line_item_t *line_item;
	
	cursor_show();
	get_code(code);
	get_counts(&counts);
	cursor_hide();

	err = store_fetch_item(code, counts, &good);

	if (0 == err) {
		/* 把货物结构题，填充到一个line_item里，
		 * 调用enter_line_item添加
		 */
		line_item = line_item_new(
			0,
			good.code,
			good.name,
			good.price,
			0.9,
			counts
			);
		
		enter_line_item(cur_sale, line_item);
	} else if (-1 == err) {
		/* 不存在该货物 */
		strncpy(err_str, "nonexist goods", ERR_LEN);
	} else if (-2 == err) {
		/* 商品数量不足 */
		strncpy(err_str, "not enough goods", ERR_LEN);
	}
}

/*
 * 删除商品
 */
static void on_key_f2()
{
	int nums_to_del;
	line_item_t *item;

	/*
	 * 没有要删除的商品
	 */
	if (0 == get_sale_item(cur_sale, cur_page * ITEM_PER_PAGE + item_selected, &item) )
	{
		move_to(46, 8);
		printf("没有商品");
		move_to(46, 9);
		printf("请按任意键");
		getch();

		return ;
	}

	/* 
	 * 获取要删除的数量 
	 */

	move_to(45, 8);
	printf("   删除商品明细");
	move_to(45, 9);
	printf("┌─────────────────┐");
	move_to(45, 10);
	printf("│                 │");
	move_to(45, 11);
	printf("│                 │");
	move_to(45, 12);
	printf("└─────────────────┘");



	move_to(47, 10);
	//printf("输入删除的数量");
	printf("条码: %s", item->code);

	move_to(47, 11);
	printf("数量: ");
	scanf("%d", &nums_to_del);

	/*
	 * 删除的个数小于原个数
	 */
	if (nums_to_del < item->count) {
		item->count -= nums_to_del;

		return ;
	}

	/*
	 * 要删除掉该条目
	 */
	del_sale_item(cur_sale, cur_page * ITEM_PER_PAGE + item_selected);
}

static void on_key_f3()
{
	check_out();
	save_data();
}

static void check_out()
{
	double total, payment, change;

	/* 计算总金额 */
	total = end_sale(cur_sale);

	/* 显示支付对话框 */
	show_check_out_box(total);

	/* 获取实收金额 */
	payment = get_payment();

	/* 支付 */
	change = make_payment(total, payment);

	/* 显示应找金额 */
	show_change(change);
}

static void save_data()
{
	/* 保存销售单 */
	save_sale();
	/* 删除销售单 */
	/* 新建销售单 */
}

/*
 * 显示结账对话框
 */
void show_check_out_box(double total)
{
	move_to(45, 8);
	printf("┌────────────┐");
	move_to(45, 9);
	printf("│ 应收:      │");
	move_to(45, 10);
	printf("│            │");
	move_to(45, 11);
	printf("│ 实收:      │");
	move_to(45, 12);
	printf("│            │");
	move_to(45, 13);
	printf("│ 应找:      │");
	move_to(45, 14);
	printf("└────────────┘");
	move_to(52, 9);
	printf("%.2lf", total);
}

/*
 * 获取实收金额
 */ 
double get_payment()
{
	double payment = 0;

	cursor_show();
	move_to(52, 11);
	scanf("%lf", &payment);
	cursor_hide();

	return payment;
}

/*
 * 显示找零
 */
void show_change(double change)
{
	move_to(52, 13);
	printf("%.2lf", change);
	move_to(45, 15);
	printf("按任意键继续");
	getch();
	getch();
}

static void save_sale()
{
	time_t secs;
	struct tm *t;

	FILE *file;
	char filename[FILE_NAME_LEN];
	
	secs = time(NULL);
	t = localtime(&secs);

	sprintf(filename, "%d%02d%02d(%s)%05d.dat",
		t->tm_year+1900,
		t->tm_mon+1,
		t->tm_mday,
		username,
		cur_sale->sale_num);

	/* 打开文件 */
	file = fopen(filename, "w");
	if (NULL == file) {
		//err
		return;
	}

	/* 保存销售单到文件 */
	save_some_sale(file, cur_sale);

	/* 关闭文件 */
	fclose(file);
}

static void save_some_sale(FILE *file, sale_t *sale)
{
	int i;
	list_t *list;
	line_item_t *item;
	list = &sale->item_list;

	for (i = 0; i < list_len(list); i++) {
		list_get(list, i, &item);
		fprintf(file, "%d %s %s %.2lf %.2lf %d %.2lf\n", 
			i,
			item->code,
			item->name,
			item->price,
			item->discount,
			item->count,
			item->total
		);
	}
}


/*
 * 撤单
 * 
 * 把链表中的内容清空，把销售界面清空，等待下一次销售
 */
static void on_key_f4()
{
	delete_sale(cur_sale);
	cur_sale->sale_num++;
	list_init(&cur_sale->item_list);
}

/*
 * 挂单
 */
static void on_key_f6()
{
	/* 错误提示 */
	if (list_len(&cur_sale->item_list) == 0)
	{
		move_to(46, 8);
		printf("清单为空不能悬挂");
		move_to(46, 9);
		printf("  按任意键继续");
		getchar();
		return ;
	}

	pend_sales[npends++] = cur_sale;
	// printf("sale_num:%d\n", sale_num);
	// getchar();
	cur_sale = make_new_sale( ++(cur_sale->sale_num) );
	sale_num++;

	printf("%d\n", npends);
	getchar();
}

static void on_key_f7()
{
	get_sale();
}

