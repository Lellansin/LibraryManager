#ifndef STORE_H
#define STORE_H

#include "utils.h"

typedef struct good {
	char name[NAME_LEN];
	char code[CODE_LEN];
	double price;
} good_t ;

typedef struct {
	good_t Goods[10];
	int Count;
} Store;

Store StoreGoods;

int store_fetch_item(char *code, int counts, good_t *good);
void Goods_init();

#endif
