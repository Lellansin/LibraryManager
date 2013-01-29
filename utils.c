#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*
 * 关于终端的转义序列，请使用man手册:
 * man console_codes
 */

/*
 * 清屏函数
 */
void clear_win()
{
	printf("\033[2J");
}

/*
 * 光标移动函数
 */
void move_to(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

/*
 *	\033[x;y;zm
 *		<text>
 *	\033[0m
 *
 *	参数y, z表示背景颜色与前景颜色，
 *	可选颜色：红色、绿色、黄色、蓝色、洋红、
 *	青色和白色。 他们对应的颜色代码是：
 *	30（黑色）、31（红色）、32（绿色）、
 *	33（黄色）、34（蓝色）、35（洋红）、
 *	36（青色）、37（白色）。
 *
 *	参数x的取值为： 
 *	0、1、22、4、24、5、25、7、27，
 *	分别表示：默认值、粗体、非粗体、下划线、
 *	非下划线、闪烁、非闪烁、 反显、非反显。
 */
void color_on(ATTR attr, COLOR bg, COLOR fg)
{
	printf("\033[%d;%d;%dm",
		attr, bg, fg);
}

void color_off()
{
	printf("\033[0m");
}

/*
 * 获取一个按键，不回显，
 * 并且关闭行缓冲。
 *
 * 行缓冲：只有输入一行并且
 * 回车后，输入才有效。
 */
int getch()
{
	int key;
	struct termios oldtm;	/* 保存原配置 */
	struct termios newtm;	/* 保存新配置 */

	/*
	 * 获取终端配置
	 */
	tcgetattr(STDIN_FILENO, &oldtm);
	newtm = oldtm;
	/*
	 * 修改配置，去掉行缓冲与回显
	 * ICANON	: 行缓冲
	 * ECHO		：回显
	 */
	newtm.c_lflag &= (~(ICANON | ECHO));
	/*
	 * 设置终端配置
	 */
	tcsetattr(STDIN_FILENO, TCSANOW, &newtm);

	key = getchar();

	/*
	 * 回复之前的配置
	 */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldtm);

	return key;
}

/*
 * linux下特殊键扫描码 
 *	F1		0x1B 0x5B 0x31 0x31			- \033[11~
 *	F2		0x1B 0x5B 0x31 0x32			- \033[12~
 *	F3		0x1B 0x5B 0x31 0x33			- \033[13~
 *	F4		0x1B 0x5B 0x31 0x34			- \033[14~
 *	F5		0x1B 0x5B 0x31 0x35			- \033[15~
 *
 *	F6		0x1B 0x5B 0x31 0x37 0x7E	- \033[17~
 *	F7		0x1B 0x5B 0x31 0x38 0x7E    - \033[18~
 *	F8		0x1B 0x5B 0x31 0x39 0x7E	- \033[19~
 *
 *	F9		0x1B 0x5B 0x32 0x30 0x7E	- \033[20~
 *	F10		0x1B 0x5B 0x32 0x31 0x7E	- \033[21~
 *	F11		0x1B 0x5B 0x32 0x33 0x7E	- \033[23~
 *	F12		0x1B 0x5B 0x32 0x34 0x7E	- \033[24~
 *
 *	Home	0x1B 0x5B 0x31 0x7E			- \033[1~
 *	Insert	0x1B 0x5B 0x32 0x7E			- \033[2~
 *	Delete	0x1B 0x5B 0x33 0x7E			- \033[3~
 *	End		0x1B 0x5B 0x34 0x7E			- \033[4~
 *	PageUp	0x1B 0x5B 0x35 0x7E			- \033[5~
 *	PageD	0x1B 0x5B 0x36 0x7E			- \033[6~
 *
 *	Up		0x1B 0x5B 0x41				- \033[A
 *	Down	0x1B 0x5B 0x42				- \033[B
 *	Left	0x1B 0x5B 0x44				- \033[C
 *	Right	0x1B 0x5B 0x43				- \033[D
 *	
 *	0x1B - \033
 *  0x5B - [
 *
 *	0x31 - 1
 *  0x32 - 2
 *  0x33 - 3
 *  0x34 - 4
 *  0x35 - 5
 *  0x36 - 6
 *  0x37 - 7
 *  0x38 - 8
 *  0x39 - 9
 *  0x40 - @
 *
 *  0x41 - A
 *  0x42 - B
 *  0x43 - C
 *  0x44 - D
 *
 *  0x7e - ~
 */

/*
 * F1-F8的捕获函数
 */
static KEY_T key_f1_f8()
{
	int key = getch();
	getch();
	return KEY_F1 + (key - '1');
}

/* 
 * F9-F12的捕获函数
 */
static KEY_T key_f9_f12()
{	
	int key = getch();
	getch();
	return KEY_F9 + (key - '0');
}

/*
 * 特殊按键捕获函数
 */
static KEY_T ex_key()
{
	KEY_T key; 

	/* 比如：F1键的码是 \033[11~
	 * 前面已经获取了\033现在要用getch()
	 * 先获取掉[，然后再次调用getch()
	 * 获取到第一个 '1'
	 */
	getch();
	key = getch();

	if ('1' == key) {
		/*
		 * 当是'1'的时候说明是F1-F8，
		 * 调用F1-F8的捕获函数
		 */
		key = key_f1_f8();
	}  else if ('2' == key) {
		/*
		 * 当是'2'的时候说明是F9-F12
		 * 调用F9-F12的捕获函数
		 */
		key = key_f9_f12(); 
	} else if (key >= 'A' && key <= 'D') {
		/*
		 * 否则为方向键
		 */
		key = KEY_UP + (key - 'A');
	}

	return key;
}

/*
 * 获取按键，包括特殊按键
 */
KEY_T getkey()
{
	KEY_T key = getch();

	/*
	 * 以\033开始的是特殊按键，
	 * 调用特殊按键捕获函数
	 * ex_key()
	 *
	 * \033为ESC按键的八进制
	 */ 
	if ('\033' == key) {
		key = ex_key();
	}

	return key;
}

/*
 * 往后退一格
 */
static void eatback()
{
	printf("\033[1D");	/* 光标向左移动一格 */
	putchar(' ');		/* 输出一个空格     */
	printf("\033[1D");	/* 光标向左移动一格 */
}

/*
 * 从屏幕上获取一个字符串
 *
 * 参数str:
 *		存放获取到的字符串
 * 参数len:
 *		要获取的字符串的长度
 * 参数echo_type:
 *		回显的类型
 *		1.NORMAL输出原字符
 *		2.STAR输出星号
 */
void get_str(char *str, int len, ECHO_TYPE echo_type)
{
	int key;				/*    保存按键    */
	char *beg = str;		/* 字符串开始位置 */
	char *end = str + len;  /* 字符串结束位置 */

	/*
	 * 不断的获取按键，直到获取到
	 * 的个数超出了个数操作len
	 */
	while (beg < end) {
		key = getkey();

		// printf("%d\n", key);

		/*
		 * 回车结束
		 */
		if (KEY_ENTER == key) {
			break;
		} else if (KEY_BACKSPACE == key ||
			/*
			 * 当按下退格键时，退格
			 */
			KEY_DEL == key) {
			if (beg > str) {
				/*
				 * 屏幕光标退格
				 * 指针也要后退
				 */
				eatback();
				beg--;
			}
		} else if (KEY_TAB == key){
			printf("退出操作");
			sleep(100);
			move_to(0,0);
			clear_win();
			exit(0);
		} else {
			/*
			 * 当为普通按键时，保存
			 * 并根据echo_type,输出
			 * 相应的字符
			 */

			if (NORMAL == echo_type) {
				putchar(key);
			} else {
				putchar('*');
			}
			
			*beg++ = key;
		}
	}

	*beg = '\0';
}

/*
 * 隐藏光标
 */
void cursor_hide()
{
	printf("\033[?25l");
}

/*
 * 显示光标
 */
void cursor_show()
{
	printf("\033[?25h");
}

void cursor_save_location()
{
	printf("\033[s");
	fflush(stdout);
}

void cursor_restore_location()
{
	printf("\033[u");
	fflush(stdout);
}
