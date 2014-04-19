#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>  
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "type.hpp"
#include "common.hpp"
#include "rrr.hpp"
#include "rsindex.hpp"
#include "rstable.hpp"

using namespace std;

int main()
{
	u64 *bitvec;
	u64 len;
	FILE *fp1;
	fp1=fopen("bits.tmp","rb");

	int FileSize=0;
	fseek(fp1,0L,SEEK_END);
	FileSize=ftell(fp1);
	cout<<"FileSize="<<FileSize<<endl;

	fseek(fp1,0L,SEEK_SET);
	int bufSize=FileSize/8+1;
	bitvec = new u64[bufSize];
	for(u64 i = 0; i < bufSize; i++){
		fread(&bitvec[i],8,1,fp1);
	}
	int size=fread(bitvec,8,bufSize,fp1);
	/*for(u64 i = 0; i < bufSize; i++)
	{
		bitvec[i] = 0xefefefef;
	}*/
	cout<<"size="<<size<<endl;
	len = FileSize*8;
	RRR *rrr = new RRR(bitvec,len);
	rrr->compress();
	rrr->createIndex();
	u64 i,t;
	/*while(1)
	{ 
		cin>>i;
		if(i == 0)	
			break;
		cout<<rrr->rank(i)<<endl;
	}*/
	u64 a[1000000];
	srand((unsigned)time(0));
	for(i = 0; i < 1000000; i++)
	{
		t = rand();
		a[i] = (t<<32 + rand())%len;
	}
	clock_t s = clock();
	for(i = 0; i < 1000000; i++)
	{
		rrr->rank(a[i]);
	}
	clock_t e = clock();
	cout<<(double)(e-s)*1000/CLOCKS_PER_SEC<<endl;
	return 0;
}
