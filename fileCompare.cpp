#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define SAMPLE_NUM 25
#define SAMPLE_OFFSET 24999
#define FILE_NAME1 "mapping1.txt"
#define FILE_NAME2 "mapping2.txt"

using namespace std;

int main(void){
	char line1[128]="";
	char line2[128]="";
	
	ifstream infp1(FILE_NAME1);
	if(infp1.fail()){
		cout<<"file open error"<<endl;
		return -1;
	}
	
	ifstream infp2(FILE_NAME2);
	if(infp2.fail()){
		cout<<"file open error"<<endl;
		return -1;
	}
	
	int i;
	int cmp;
	for(i=0;!infp1.eof()&&!infp2.eof();++i){
//		for(int j=1;j<SAMPLE_NUM;++j)
//			strcpy(line[j-1],line[j]);
		cmp=strcmp(line1,line2);
		if(cmp==0){
			infp1.getline(line1,128);
			infp2.getline(line2,128);
		}
		else if(cmp>0){
			cout<<"2: "<<line2<<endl;
			infp2.getline(line2,128);
		}
		else{
			cout<<"1: "<<line1<<endl;
			infp1.getline(line1,128);
		}
//		if(i%SAMPLE_OFFSET<SAMPLE_NUM)
//			cout<<line[SAMPLE_NUM-1]<<endl;		
	}
//	for(int i=0;i<SAMPLE_NUM;++i)
//		cout<<line[i]<<endl;
	cout<<i<<endl;
	
	return 0;
}

