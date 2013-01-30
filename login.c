#include <stdio.h>

#include <signal.h>
#include <time.h>

#include "login.h"
#include "utils.h"


#define TIME_STR_LEN 20

char username[UNAME_LEN];
char password[PWORD_LEN];

void show_login_window();
void get_user_name();
void get_pass_word();
void print_err(int status);

static void init_clock();
static void update_clock();

struct User
{
	char name[UNAME_LEN];
	char password[PWORD_LEN];
} UserList[10] ;

int UserCount;

static void init_user();

/*
 * 2.输入获取
 * 3.处理输入
 */
void login()
{
	int Login_status;
	init_clock();	
	init_user();

	while (1) {

		/* 显示界面 */
		show_login_window();

		/* 获取用户名 */
		get_user_name();
		/* 获取密码 */
		get_pass_word();

		Login_status = check_login();

		/* 
		 * 验证用户名密码
		 * 正确进入系统，
		 * 错误重新登录
		 */
		if ( Login_status == LOGIN_INFO_SUCCESS ) {
			/* 进入系统 */
			sys_main();
		}

		/* 错误提示 */
		print_err(Login_status);
		
	}
}

/*
 * 显示登录界面
 */
void show_login_window()
{
	clear_win();
	move_to(0, 0);

	printf("┌────────────────────────────────────┐\n");
	printf("│           图书销售系统             │\n");
	printf("│           ────────────             │\n");
	printf("│                                    │\n");

	printf("│       ");

	color_on(INVERSE, BLACK, WHITE);
	printf("┌───────────────────┐ ");
	color_off();

	printf("       │\n");
	printf("│       ");
	
	color_on(INVERSE, BLACK, WHITE);
	printf("│ 用户名：          │ ");
	color_off();

	printf("       │\n");
	printf("│       ");
	
	color_on(INVERSE, BLACK, WHITE);
	printf("│ 密  码：          │ ");
	color_off();

	printf("       │\n");
	printf("│       ");
	
	color_on(INVERSE, BLACK, WHITE);
	printf("└───────────────────┘ ");
	color_off();

	printf("       │\n");
	printf("│                                    │\n");
	printf("│                                    │\n");
	printf("│                                    │\n");
	printf("│                                    │\n");
	printf("│  时间:               按TAB键退出   │\n");
	printf("└────────────────────────────────────┘\n");
}

/*
 * 获取用户名
 */
void get_user_name()
{
	move_to(19, 6);
	get_str(username, UNAME_LEN, NORMAL);
}

/*
 * 获取密码
 */
void get_pass_word()
{
	move_to(19, 7);
	get_str(password, PWORD_LEN, STAR);
}

int check_login()
{
	int flag = 0;
	int i;

	for (i = 0; i < UserCount; i++)
	{
		if(0 == strcmp(username, UserList[i].name))
		{
			if (0 == strcmp(password, UserList[i].password))
			{
				flag = 1;
			}
		}
	}

	if (flag)
	{
		return LOGIN_INFO_SUCCESS;
	}else{
		if (0 == strcmp(username, "") || 0 == strcmp(password, ""))
		{
			return LOGIN_ERROR_NULL;
		}

		return LOGIN_ERROR_INCORRECT;
	}
}

/*
 * 打印错误信息
 */
void print_err(int status)
{
	KEY_T key;

	if (status == LOGIN_ERROR_INCORRECT)
	{
		move_to(7, 10);
		color_on(INVERSE, WHITE, RED);
		printf("用户名或密码错误!");
		move_to(7, 11);
		printf("请按任意键继续");
		color_off();
	}else{
		move_to(7, 10);
		color_on(INVERSE, WHITE, RED);
		printf("输入不能为空！");
		move_to(7, 11);
		printf("请按任意键继续");
		color_off();
	}
	

	getch();
}


static void init_clock()
{
	signal(SIGALRM, update_clock);
	update_clock();
}

static void update_clock()
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

	move_to(10, 13);
	printf("%s", time_str);
	fflush(stdout);

	cursor_restore_location();

	alarm(1);
}

static void init_user()
{
	strcpy(UserList[0].name, "admin");
	strcpy(UserList[0].password, "admin");
	UserCount++;

	strcpy(UserList[1].name, "alan");
	strcpy(UserList[1].password, "1234");
	UserCount++;
}


