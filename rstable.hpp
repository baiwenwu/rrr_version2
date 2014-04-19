/**
* 创建R，S表
* R表：存储类别c
* S表：存储类别偏移量o
* R，S表的对应，可得出源0/1串中这个位置的(c,o)，继而可以解码
*/

#ifndef _RSTABLE_H
#define _RSTABLE_H

#include "type.hpp"

class RSTable
{
public:
	RSTable();
	/**
	* 构造函数
	* @ bitvec 初始0/1串
	* @ len 初始0/1串的长度
	*/
	RSTable(u64 *bitvec, u64 len);

	/*析够函数*/
	~RSTable();

	/**
	* 返回R表
	* @ return 返回R表头指针
	*/
	const u64* getR();


	/**
	* 返回R表的长度
	* @ return R表的长度
	*/
	const u64 getRLen();


	/**
	* 返回S表
	* @ return 返回S表头指针
	*/
	const u64* getS();

		/**
	* 返回S表的长度
	* @ return S表的长度
	*/
	const u64 getSLen();
	/**
	* 创建RS表
	* 由于R表的创建先于S表的创建，我不想让类用户关心这个问题，所以提供这个函数，类似外观模式
	*/
	void createTable();
private:
	/**
	* 创建R表
	*/
	void createRTable();

	/**
	* 创建S表
	*/
	void createSTable();
private:
	u64* r;//R表头指针
	u64* s;//S表头指针

	u64 rlen;//R表的长度
	u64 slen;//S表的长度

	u64 *bitvec;//源串指针
	u64 len;//源串长度
};

#endif
