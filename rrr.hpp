/**
* rrr结构有两种用途，1，压缩，2，进行rank查询
*/

#ifndef _RRR_H
#define _RRR_H

#include "common.hpp"
#include "type.hpp"
#include "rstable.hpp"
#include "rsindex.hpp"

class RRR
{
public:
	/**
	* 默认构造函数
	*/
	RRR();

	/**
	* 构造函数
	* @ bitvec 原始0/1串
	* @ len 原始0/1串的长度
	*/
	RRR(u64 *bitvec, int len);

	/*析够函数*/
	~RRR();

	/**
	* 利用RRR压缩0/1串
	*/
	void compress();

	/**
	* 为压缩后的结构创建索引
	*/
	void createIndex();


	/**
	* rank查询
	* @ index 位置
	* @ return 返回0到index-1中1的个数
	*/
	u64 rank(u64 index);
/*private:
	void createD();*/

private:
	const u64 *r;//R表
	const u64 *s;//S表
	//u16 *d;//D表
	//int* kind;
	Cmap *cmap;
	Os *os;
	DK *dk;
	const u64 *rIndex;//R表索引
	const u64 *sIndex;//S表索引
	u64 *bitvec;
	u64 len;
};

#endif
