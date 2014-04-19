/**
* 创建R，S表
* R表：存储类别c
* S表：存储类别偏移量o
* R，S表的对应，可得出源0/1串中这个位置的(c,o)，继而可以解码
*/

#include "type.hpp"
#include "rstable.hpp"
#include "common.hpp"
#include <iostream>
#include <string.h>

using namespace std;

RSTable::RSTable()
{
	this->bitvec = NULL;
	this->len = 0;
}
/**
* 构造函数
* @ bitvec 初始0/1串
* @ len 初始0/1串的长度
*/
RSTable::RSTable(u64 *bitvec, u64 len):bitvec(bitvec),len(len),rlen(0),slen(0),r(NULL),s(NULL)
{
}

/*析够函数*/
RSTable::~RSTable()
{
	this->bitvec = NULL;//不能释放空间
	delete[] r;
	delete[] s;
	r = NULL;
	s = NULL;
}

/**
* 返回R表
* @ return 返回R表头指针
*/
const u64* RSTable::getR()
{ 
	return this->r;
}

/**
* 返回S表
* @ return 返回S表头指针
*/
const u64* RSTable::getS()
{
	return this->s;
}

/**
* 创建R表
*/
void RSTable::createRTable()
{ 
	/*
	创建R表：各个块的类别连接起来存储在R中，即c0,c1,c2,...,ci,...,cj,...其中每个ci，占据 4bits。
	字长为64 bits，所以一个字里面可以存储16个这样的值。
	15是rrr结构的要求
	*/

	u64 clen = this->len / 15 + ((this->len % 15) ? 1 : 0);//c的个数
	rlen = clen / 16 + ((clen % 16) ? 1 : 0);//R表的长度，最后可能会有少于1个64位字的空间没有用到

	this->r = new u64[rlen];//R表
	memset(this->r,0,sizeof(u64)*rlen);
	for(u64 i = 0; i < rlen; i++)
	{
		u64 t = 0;
		for(u64 j = 0; j < 16; j++)
		{//每16个c组成一个64位的字
			t = t << 4;
			if(i*15 + 15 <= len)
			{//取出的位未越界
				t += popcount(getBits(bitvec,i*15*16+j*15,15));
			}
			else if(i*15 < len)
			{//取出的位部分越界
				t += popcount(getBits(bitvec,i*15*16+j*15,len - i*15*16 - j*15));
			}
			else//取出的位全部越界
				t += 0;
		}
		r[i] = t;
	}
}

/**
* 创建S表
*/
void RSTable::createSTable()
{ 
	/*
	创建S表：S表存储(c,o)中o的连接。即o1,o2,...oi,..oj,..，每个oi需要log(C(15,ci))位存储，重构时，可以写成两个函数
	*/
	Os *os = Os::getInstance();
	Cmap *cmap = Cmap::getInstance();
	u64 t,cnt = 0;
	for(u64 i = 0; i < rlen; i++)
	{
		for(u64 j = 15; j >= 0; j--)
		{
			t = (r[i]>>(j<<2))&0x000f;//得到类t
			cnt += os->getOLen(t);//存储类t对应的o所需的bit长度
			if(j == 0)
				break;
		}
	}

	slen = cnt / D + ((cnt % D) ? 1 : 0);//S表的长度
	this->s = new u64[slen];//S表
	memset(s,0,sizeof(u64)*slen);
	//将类对应的o值依次按照计算出的长度存储到s当中
	cnt = 0;
	for(u64 i = 0; i < rlen; i++)
	{ 
		for(u64 j = 15; j >= 0; j--)
		{ 
			t = (r[i]>>(j<<2))&0x000f;//得到类t
			//u64 tvalue = os->getO(t);//类t对应的值C(15,t)
			u64 tvalue = getBits(bitvec,i*15*16+(15-j)*15,15);
			u64 tlen = os->getOLen(t);//类t对应的值所需的bit长度log(C(15,t))
			setBits(s,cnt,cmap->getCmap(tvalue),tlen);//这里不是tvalue，应该是相对偏移量，存储长度不变

			cnt += tlen;
			if(j == 0)
				break;
		}
	}
	
}

void RSTable::createTable()
{ 
	createRTable();
	createSTable();
}


const u64 RSTable::getRLen()
{ 
	return rlen;
}


const u64 RSTable::getSLen()
{ 
	return slen;
}

