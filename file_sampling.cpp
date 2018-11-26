#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define SAMPLE_NUM 25
#define SAMPLE_OFFSET 24999
#define FILE_NAME "mapping2.txt"

using namespace std;

int main(void){
	char line[SAMPLE_NUM][128];
	
	ifstream infp(FILE_NAME);
	if(infp.fail()){
		cout<<"file open error"<<endl;
		return -1;
	}
	
	int i;
	for(i=0;!infp.eof();++i){
//		for(int j=1;j<SAMPLE_NUM;++j)
//			strcpy(line[j-1],line[j]);
		infp.getline(line[SAMPLE_NUM-1],128);
//		if(i%SAMPLE_OFFSET<SAMPLE_NUM)
//			cout<<line[SAMPLE_NUM-1]<<endl;		
	}
//	for(int i=0;i<SAMPLE_NUM;++i)
//		cout<<line[i]<<endl;
	cout<<i<<endl;
	
	return 0;
}

