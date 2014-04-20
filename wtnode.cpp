#include "WT_Node.h"
#include <iostream>
#include "type.hpp"
#include "rrr.hpp"

using namespace std;

WT_Node::WT_Node(u64 *bitbuff, int len, int level, int block_size, unsigned char label)
		:bitvec(bitbuff),bitLen(len),level(level),label(label)
{
	if(bitvec != NULL)
	{
		rrr = new RRR(bitvec,bitLen);
		rrr->compress();//先压缩
		delete[] bitvec;
		bitvec = NULL;
		rrr->createIndex();//后创建索引
		data = rrr->getCompressData();//将压缩后的数据放到当前数据段，并且释放原来的数据
	}
}

WT_Node::~WT_Node()
{
	if(left != NULL)
		delete left;
	else if(right != NULL)
		delete right;
	delete[] data;
}

int WT_Node::Rank(int pos)
{
	return rrr->rank(pos+1);
}

int WT_Node::Bit(int pos)
{
	return rrr->get(pos+1);
}


void WT_Node::Left(WT_Node *left)
{
	this->left = left;
}

void WT_Node::Right(WT_Node *right)
{
	this->right = right;
}

WT_Node* WT_Node::Left()
{
	return left;
}

WT_Node* WT_Node::Right()
{
	return right;
}
