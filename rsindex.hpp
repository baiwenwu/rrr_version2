/**
* 创建R表和S表的查询索引
*/

#ifndef _RSINDEX_H
#define _RSINDEX_H
#include "type.hpp"
#include "rstable.hpp"

/*
* 函数命名说明：
* 
*/
class RSIndex
{
public:
	RSIndex();
	/**
	* 构造函数，为R,S表建立索引
	* @ r R表头指针
	* @ s S表头指针
	*/
	RSIndex(RSTable *rsTable);
	/*析够函数*/
	~RSIndex();

	/**
	* 返回R表的查询表
	* @ return R表查询表的表头指针
	*/
	const u64* getRIndex();

	/**
	* 返回R表的查询表的大小
	* @ return R表查询表的大小
	*/
	const u64 getRIndexSize();

	/**
	* 返回S表的查询表
	* @ return S表查询表的表头指针
	*/
	const u64* getSIndex();

	/**
	* 返回S表的查询表的大小
	* @ return S表查询表的大小
	*/
	const u64 getSIndexSize();

	/**
	* 创建RS表的查询表
	*/
	void createRSIndex();	

private:
	/**
	* 创建R表的查询表
	*/
	void createRIndex();
	
	/**
	* 创建S表的查询表
	*/
	void createSIndex();

private:
	const u64 *r;//R表
	const u64 *s;//S表
	u64* rIndex;//R表的索引
	u64* sIndex;//S表的索引

	const u64 rlen;
	const u64 slen;

	u64 rIndexLen;
	u64 sIndexLen;
};

#endif
