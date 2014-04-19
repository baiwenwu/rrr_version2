#ifndef _COMMON_H
#define _COMMON_H

#include <string.h>
#include "type.hpp"
//定义全局函数，这些函数不属于某个类

/**
* 从bitvec中取出第i位到第i+len-1位组成的整数
* @ bitvec 初始0/1串
* @ i 0/1串中开始取数的位置
* @ len 取出的长度
* @ return 0/1串中从第i位到第i+len-1位的0/1子串组成的数
*/
u64 getBits(const u64 *bitvec, u64 i, u64 len);


/**
* 用value的低len位更新bitvec的第i到第i+len-1位
* @ bitvec 初始0/1串
* @ i 0/1串中开始设置新bit的位置
* @ value 设置的值
* @ len 设置的值的低len位有效
*/
void setBits(u64 *bitvec, u64 i, u64 value, u64 len);


/**
* 得64位整型数i的2进制表示中1的个数
* @ i 64位整型数
* @ return i的2进制表示中1的个数
*/
u64 popcount(u64 i);

class Cmap
{ 
public:
	~Cmap();
	static Cmap* getInstance();
	int getCmap(int i);
private:
	Cmap();
private:
	int *cmap;
	static Cmap* instance;
};

//将求15以内组合数和组合数所需存储长度放在一个类中
class Os
{ 
public:
	~Os();
	/**
	* 得到存储C(15,i)
	*/
	u64 getO(u64 i);
	/**
	* 得到存储C(15,i)所需的位长度
	*/
	u64 getOLen(u64 i);
	static Os* getInstance();
private:
	Os();
private:
	/**
	* 存储C(15,i)的值
	*/
	u64 *o;
	/**
	* 存储C(15,i)所需的位长度
	*/
	u64 *oLen;
	static Os* instance;
};

class DK
{
public:
	~DK();
	static DK* getInstance();
	u16 getD(u32 i);
	int getK(int i);
private:
	DK();
private:
	u16 *d;
	int *kind;
	static DK* instance;
};

#endif
