#ifndef WT_NODE_H
#define WT_NODE_H

#include "type.hpp"
#include "rrr.hpp"
#include <iostream>

using namespace std;

class WT_Node
{
public:
	/**
	 * 小波树节点
	 * @ bitbuff 节点的0,1串，用u64格式存储
	 * @ len 0/1串的长度
	 * @ level 该节点所在的层数
	 * @ block_size 块的大小
	 * @ label 节点的字母，只有叶节点有意义
	 */
	WT_Node(u64 *bitbuff, int len, int level, int block_size= 32 * 15, unsigned char label='\0');
	
	/**
	 * 析构函数
	 */
	~WT_Node();

	/**
	 * rank查询
	 * @ pos 0 到 pos 的0/1串中1的个数
	 */
	int Rank(int pos);

	/**
	* get查询
	* @ pos 从0数第pos的bit
	*/
	int Bit(int pos);
	/**
	 * 设置左孩子
	 * @ left 左孩子指针
	 */
	void Left(WT_Node *left);

	/**
	 * 设置右孩子
	 * @ right 右孩子指针
	 */
	void Right(WT_Node *right);

	/**
	 * 返回左孩子
	 */
	WT_Node *Left();

	/**
	 * 返回右孩子
	 */
	WT_Node *Right();
private:
	/*不实现*/
	WT_Node(const WT_Node &);
	WT_Node & operator=(const WT_Node &right);

	int level;//该节点所在层数
	u8 label;//只有叶节点有意义
	u64 *bitvec;//指向外部数据的指针
	u64 *data;//压缩后的数据
	int bitLen;//该节点的长度
	WT_Node *left;//左孩子指针
	WT_Node *right;//右孩子指针

	RRR *rrr;

};


#endif
