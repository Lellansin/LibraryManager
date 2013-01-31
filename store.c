#include "store.h"
#include <string.h>


/*
 *   | good_t | member | 含义 |
 *   |--------+--------+------|
 *   |        | name   | 名字 |
 *   |        | code   | 条码 |
 *   |        | price  | 价格 |
 */


void Goods_init()
{
	StoreGoods.Count = 0;

	strncpy(StoreGoods.Goods[0].name, "linux", NAME_LEN);
	strncpy(StoreGoods.Goods[0].code, "1", CODE_LEN);
	StoreGoods.Goods[0].price = 65;

	StoreGoods.Count++;

	strncpy(StoreGoods.Goods[1].name, "Java", NAME_LEN);
	strncpy(StoreGoods.Goods[1].code, "2", CODE_LEN);
	StoreGoods.Goods[1].price = 32.5;

	StoreGoods.Count++;

	strncpy(StoreGoods.Goods[2].name, "Python", NAME_LEN);
	strncpy(StoreGoods.Goods[2].code, "3", CODE_LEN);
	StoreGoods.Goods[2].price = 15;

	StoreGoods.Count++;
}




int getGoodByCode(char *code, good_t *good)
{
	int number = atoi(code);

	if (number > StoreGoods.Count)
	{
		return 0;
	}

	if (&StoreGoods.Goods[number-1] != NULL)
	{
		strncpy(good->name, StoreGoods.Goods[number-1].name, NAME_LEN);
		strncpy(good->code, StoreGoods.Goods[number-1].code, CODE_LEN);
		good->price = StoreGoods.Goods[number-1].price;
		return 1;
	}
	
}


int store_fetch_item(char *code, int counts, good_t *good)
{
	int flag;

	flag = getGoodByCode(code, good);

	// printf("%d\n", flag );	

	if (counts > 10)
		return -2;

	if (0 == strcmp(code, "dame") || flag == 0 )
		return -1;

	return 0;
}
