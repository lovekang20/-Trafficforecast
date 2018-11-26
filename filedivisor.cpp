#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define SAMPLE_NUM 25000
#define SAMPLE_OFFSET 24999
#define FILE_NAME "201500"
#define TYPE ".csv"

using namespace std;

int main(void){
	char line[128];
	char type[30]=(TYPE);
	char filename[30]=(FILE_NAME);
	char inStr[60];
	char outStr[60];
	
	strcpy(inStr,filename);
	strcpy(outStr,filename);
	strcat(inStr,type);
	cout<<inStr<<endl;
	
	ifstream infp(inStr);
	if(infp.fail()){
		cout<<"file open error"<<endl;
		return -1;
	}
	int repeat = 0;
	char repeat_str[30];
	ofstream outfp;
	infp.getline(line,128);
	
	int i=0;
	for(i=0;!infp.eof();++i){
		if(i%SAMPLE_NUM==0){
			strcpy(outStr,filename);
			itoa(repeat,repeat_str,10);
			strcat(outStr,repeat_str);
			strcat(outStr,type);
			outfp.close();
			outfp.open(outStr);
			repeat++;
		}
		outfp<<line<<endl;
		infp.getline(line,128);
	}
	
	return 0;
}

