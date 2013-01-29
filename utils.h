#ifndef UTILS_H

	#define UTILS_H

	#define CODE_LEN 5
	#define NAME_LEN 8
	#define ERR_LEN  20
	#define FILE_NAME_LEN 50

	#define MAX_PEND 5

	#define ITEM_SHOWS 10

/*
 * 文字颜色枚举
 */
typedef enum { 
	BLACK = 30, RED, GREEN, YELLOW, BLUE, CARMINE, CYAN, WHITE
}COLOR;

/*
 * 文字属性枚举
 */
typedef enum { 
	DEFAULT=0, BOLD=1, NONBOLD=22, UNDER_LINE=4, NONUNDER_LINE=24, BLINK=5, NONBLINK=25, INVERSE=7, NONINVERSE=27
} ATTR;

/*
 * 按键的枚举：
 * 这里的按键的取值是有规律的，
 * 因为linux下按键的扫描码中——-
 * f1-f5是连续的 f6-f8是连续的-
 * f9-f10是连续的 f11-f12是连续
 * 的，这里我们让枚举的值之间的
 * 差据跟扫描码一样，这样，我们
 * 就可以通过加减来把扫描码转化
 * 为我们的枚举值
 */
typedef enum {
	KEY_TAB = 9,
	KEY_BACKSPACE = '\b', KEY_ENTER = '\n', KEY_DEL = 127,
	KEY_UP = 1000, KEY_DOWN, KEY_RIGHT, KEY_LEFT,
	KEY_F1 = 2000,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,
	KEY_F6 = 2006,	KEY_F7,	KEY_F8,	
	KEY_F9 = 3000,	KEY_F10, KEY_F11 = 3003, KEY_F12,
} KEY_T;

/*
 * 回显类型
 */
typedef enum { NORMAL, STAR } ECHO_TYPE;

typedef enum { FALSE, TRUE } BOOL;

void clear_win();
void move_to();
void color_on(ATTR, COLOR, COLOR);
void color_off();
int getch();
void get_str(char *str, int len, ECHO_TYPE echo_type);
KEY_T getkey();
void cursor_hide();
void cursor_show();
void cursor_save_location();
void cursor_restore_location();

#endif
