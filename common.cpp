#include "type.hpp"
#include "common.hpp"
#include <iostream>

using namespace std;

u64 getBits(const u64 *bitvec, u64 i, u64 len)
{
	u64 t = 0;
	u64 word = i>>6;//i/D D= 64
	u64 offset = i & 0x003f;//i%D
	u64 j = D - offset;
	if(j >= len)
	{//在同一个字长内完成取数
		t = bitvec[word]<<offset;
		t = t >> (D - len);
	}
	else
	{//出现了跨字取数
		t = bitvec[word] & ((1<<j)-1);
		u64 k = D - (len - j);
		t = t << (len - j);
		t += bitvec[word + 1] >> k;
	}
	return t; 
}

void setBits(u64 *bitvec, u64 i, u64 value, u64 len)
{
	u64 t = 0;
	u64 word = i>>6;/*i/D D= 64*/
	u64 offset = i & 0x003f;/*i%D*/
	u64 j = D - offset;
	if(j >= len)
	{//在同一个字长内完成设置
		t = value << (j - len);
		bitvec[word] += t;
	}
	else
	{//出现了跨字设置
		t = value >> (len - j);
		bitvec[word] += t;
		u64 k = D - (len - j);
		t = value & ((1<<(len - j)) - 1);
		bitvec[word + 1] += t<<k;
	}	
}

u64 popcount(u64 i)
{//knuth求取的方法
	u64 r;

	r = i;
	r = ((r & 0xaaaaaaaaaaaaaaaa)>>1) + (r & 0x5555555555555555);
	r = ((r & 0xcccccccccccccccc)>>2) + (r & 0x3333333333333333);
	r = ((r>>4) + r) & 0x0f0f0f0f0f0f0f0f;

	r *= 0x0101010101010101;
	r >>= 64-8;

	return r;
}

//DK
DK* DK::instance = 0;

DK::DK()
{
	Os *os = Os::getInstance();
	Cmap *cmap = Cmap::getInstance();
	d = new u16[1u<<B];
	kind = new int[B+1];
	kind[0] = 1;
	for(int i=1; i <= B; i++)
	{
		kind[i] = kind[i-1] + os->getO(i);
	}
	d[0] = 0;
	for(int i = 1; i <(1U<<B); i++)
	{
		int c = popcount(i);
		int o = cmap->getCmap(i);
		d[kind[c-1]+o] = (u16)i;
	}
}

DK* DK::getInstance()
{
	if(instance == 0)
	{
		instance = new DK();
	}
	return instance;
}

u16 DK::getD(u32 i)
{
	return d[i];
}

int DK::getK(int i)
{
	return kind[i];
}

DK::~DK()
{
	delete[] d;
	delete[] kind;
	d = NULL;
	kind = NULL;
}

//Cmap
Cmap* Cmap::instance = 0;

Cmap::Cmap()
{
	int *sum = new int[B+1];
	memset(sum,0,sizeof(int)*(B+1));
	cmap = new int[1<<B];
	memset(cmap,0,sizeof(int)*(1<<B));
	for(u64 i = 0; i < (1<<B); i++)
	{
		cmap[i] = sum[popcount(i)]++;
	}
	delete[] sum;
	sum = NULL;
}

Cmap* Cmap::getInstance()
{
	if(instance == 0)
	{
		instance = new Cmap();
	}
	return instance;
}

int Cmap::getCmap(int i)
{
	return cmap[i];
}

Cmap::~Cmap()
{
	delete[] cmap;
	cmap = NULL;
}

//Os类的方法
Os* Os::instance = 0;

Os* Os::getInstance()
{
	if(instance == 0)
	{
		instance = new Os();
	}
	return instance;
}


Os::Os()
{
	o = new u64[B/2 + 1];
	o[0] = 1ULL;o[1] = 15ULL;o[2] = 105ULL;o[3] = 455ULL;o[4] = 1365ULL;o[5] = 3003ULL;o[6] = 5005ULL;o[7] = 6435ULL;
	oLen = new u64[B/2 + 1];
	oLen[0] = 1ULL;oLen[1] = 4ULL;oLen[2] = 7ULL;oLen[3] = 9ULL;oLen[4] = 11ULL;oLen[5] = 12ULL;oLen[6] = 13ULL;oLen[7] = 13ULL;
}

Os::~Os()
{
	delete[] o;
	delete[] oLen;
	o = NULL;
	oLen = NULL;
}

u64 Os::getO(u64 i)
{
	if(i < 8)
		return o[i];
	else
		return o[B-i];
}

u64 Os::getOLen(u64 i)
{
	if(i < 8)
		return oLen[i];
	else
		return oLen[B - i];
}
