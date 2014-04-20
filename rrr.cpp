#include "common.hpp"
#include "type.hpp"
#include "rstable.hpp"
#include "rsindex.hpp"
#include "rrr.hpp"
#include <string.h>
#include <iostream>

using namespace std;

RRR::RRR()
{
}

RRR::RRR(u64 *bitvec, int len):bitvec(bitvec),len(len)
{
	os = Os::getInstance();
	cmap = Cmap::getInstance();//这两个是全局的，不应该在此进行初始化，重构到全局函数中
	dk = DK::getInstance();
}

RRR::~RRR()
{
	r = NULL;
	s = NULL;
	cmap = NULL;
	os = NULL;
	dk = NULL;
	rIndex = NULL;
	sIndex = NULL;
	delete[] bitvec;
	bitvec = NULL;
}

/*void RRR::createD()
{ 
	d = new u16[1u<<B];
	kind=new int[B+1];
	kind[0] = 1;
	for(int i=1;i<=B;i++)
	{ 
		kind[i]=kind[i-1]+os->getO(i);
	}
	d[0] = 0;
	for(int i=1;i<(1U<<B);i++)
	{  
		int c=popcount(i);
		int o=cmap->getCmap(i);
		d[kind[c-1]+o]=(u16)i;
	}
}*/

void RRR::compress()
{
	rsTable = new RSTable(bitvec,len);
	rsTable->createTable();
	r = rsTable->getR();
	s = rsTable->getS();
	cout<<"compress size is :"<<(double)((rsTable->getRLen() + rsTable->getSLen()) * 64)/(double)len<<endl;
}

u64* RRR::getCompressData()
{
	u64 rlen = rsTable->getRLen(); 
	u64 slen = rsTable->getSLen();
	u64 datalen = rlen + slen;//要返回的空间长度
	u64 *data = new u64[datalen];
	memset(data,0,sizeof(u64)*datalen);
	for(u64 i = 0; i < datalen; i++)
	{
		if(i < rlen)
			data[i] = r[i];
		else
			data[i] = s[i-rlen];
	}
	delete rsTable;//删除重复的r,s表
	r = data;
	s = &data[rlen];
	return data;
}

void RRR::createIndex()
{
	/*RSTable *rsTable = new RSTable(bitvec,len);
	rsTable->createTable();*/
	RSIndex *rsIndex = new RSIndex(rsTable);
	rsIndex->createRSIndex();
	rIndex = rsIndex->getRIndex();
	sIndex = rsIndex->getSIndex();
	cout<<"index size is :"<<(double)((rsIndex->getRIndexSize() + rsIndex->getSIndexSize())*64)/(double)len<<endl;
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
u64 RRR::rank(u64 i)
{ 
	u64 cnt = 0,t,j,blocknum = 0;
	const u64 LBSIZE = 15 * 32 * 8;
	const u64 SBSIZE = 15 * 32;
	const u64 BSIZE = 15;

	u64 lboffset = i/LBSIZE;//i所在大块号
	cnt += rIndex[lboffset * 2];//i所在大块之前的大块中的rank值
	u64 sbi = i - lboffset * LBSIZE;//i在其所在大块中的偏移量
	u64 sboffset = sbi / SBSIZE;//i所在的小块，从它所在的大块开始计数
	t = rIndex[lboffset * 2 + 1];
	j = 0;
	while(j < sboffset)
	{
		cnt += (t>>((6 - j)*9)) & 0x1ff;
		j++;
	}
	//到此为止，将i所在小块之前的rank值全部求出来了

	u64 bi = sbi - sboffset * SBSIZE;//i所在块在小块中的偏移量
	u64 boffset = bi / BSIZE;//i所在块
	j = lboffset * 32 * 8 + sboffset * 32;
	u64 start = j/16 * 64 + j % 16 * 4;
	j = 0;
	u64 sxlen = 0;
	while(j < boffset)
	{
		t = getBits(r,start + j * 4,4);
		cnt += t;
		sxlen += os->getOLen(t);
		j++;
	} 
	//到此为止，将i所在的15bit块之前的rank值全部计算出来了
	//下面开始找c值
	u64 c = getBits(r,start + j * 4, 4);
	//下面开始求o值
	u64 sx;
	sx = sIndex[i/SBSIZE];
	start = sx + sxlen;
	u64 o = getBits(s,start,os->getOLen(c));
	//根据c,o求取原始串
	u32 x;
	if(c>0)
		//x = d[kind[c-1] + o];
		x = dk->getD(dk->getK(c-1) + o);
	else
		x = 0;
	cnt += popcount((u64)(x&(~((1<<(15-i%15))-1))));
	return cnt;
}

u64 RRR::get(u64 index)
{
	if(rank(index-1) != rank(index))
		return 1;
	else
		return 0;
}
