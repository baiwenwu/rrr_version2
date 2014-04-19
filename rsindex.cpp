#include "type.hpp"
#include "common.hpp"
#include "rstable.hpp"
#include "rsindex.hpp"
#include <iostream>
#include <string.h>
using namespace std;

/**
求取rank(i)的方法：
	cnt = 0;
	lbi = i / lbsize;
	cnt += rIndex[lbi * 2];
	sboffset = i - lbi * lbsize;//小块中的偏移量
	sbi = sboffset / sbsize;

	t = rIndex[lbi * 2 + 1];//取出小块的64索引值
	for(u64 j = 7; j >= sbi; j--)
		cnt += (t>>(j*9)) & 0x001f;

	boffset = j - sbi * sbsize;//块中的偏移量
	t = (t>>(j * 4)) & 0x001f;//取出类别，取出对应的o，由(c,o)得到源串，进行位操作即可得到最终的rank   
*/

RSIndex::RSIndex():r(NULL),rlen(0),s(NULL),slen(0)
{
}

RSIndex::RSIndex(RSTable *rsTable)
		:r(rsTable->getR()),rlen(rsTable->getRLen()),s(rsTable->getS()),slen(rsTable->getSLen())
{
	rIndexLen = (rlen * 16 / (32*8) + 1) * 2;
	sIndexLen = rlen * 16 / 32 + 1;
}

RSIndex::~RSIndex()
{
	r = NULL;
	s = NULL;
	delete[] rIndex;
	delete[] sIndex;
	rIndex = NULL;
	sIndex = NULL;
}

const u64* RSIndex::getRIndex()
{
	return this->rIndex;
}

const u64 RSIndex::getRIndexSize()
{
	return rIndexLen;
}
	
const u64* RSIndex::getSIndex()
{
	return this->sIndex;
}

const u64 RSIndex::getSIndexSize()
{
	return sIndexLen;
}

void RSIndex::createRSIndex()
{
	createRIndex();
	createSIndex();
}


/*
	RIndex：R表rank查询的索引
	大块:lb size = 15 * 32 * 8 bits
	小块:sb size = 15 * 32 bits
	块:   b size = 15 bits

	大块表： lbt 每个单元需要64 bits，因为64 = log(n)
	小块表： sbt 每个单元需要9  bits，因为log(15 * 32) = 9
	
	因为1个大块有8个小块，所以存储7个小块索引值，即7 * 9 = 64 bits,交叉存储
*/
void RSIndex::createRIndex()
{
	rIndex = new u64[rIndexLen];//R索引表
	memset(rIndex,0,sizeof(u64)*rIndexLen);

	u64 ri = 0;
	u64 rankl = 0,ranks = 0;
	rIndex[0] = 0;//第一个大块表的值为0，因为第一个大块前面没有bit
	for(u64 i = 0; i < rlen; i++)
	{
		for(u64 j = 15; j >= 0; j--)
		{ 
			ranks += (r[i]>>(j*4))&0x000f;
			if(j == 0)
				break;
		}
		if(i%16 == 15)
		{//每个大块存储一 个rankl
			rankl += ranks;
			rIndex[ri+2] = rankl;
			ri += 2;
			ranks = 0;
		}
		else if(i%2 && i%16 != 15)
		{//每个小块存储一个ranks
			rIndex[ri+1] = rIndex[ri+1]<<9;
			rIndex[ri+1] += ranks;
			rankl += ranks;
			ranks = 0;
		}		
	}
}

void RSIndex::createSIndex()
{ 
	Os *os = Os::getInstance();
	sIndex = new u64[sIndexLen];//S索引表
	memset(sIndex,0,sizeof(u64)*sIndexLen);
	sIndex[0] = 0;//第一个S索引为0
	u64 si = 1;
	u64 len = 0;
	for(u64 i = 0; i < rlen; i++)
	{
		for(u64 j = 15; j >= 0; j--)
		{
			u64 tvalue = (r[i]>>(j*4))&0x000f;
			u64 tlen = os->getOLen(tvalue);
			len += tlen;
			if(j == 0)
				break;
		} 
		if(i%2)
		{ 
			sIndex[si] = len;
			si++;
		}
 	}
}

