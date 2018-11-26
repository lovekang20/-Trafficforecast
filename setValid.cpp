#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define START_YEAR 2015
#define YEAR 2
#define MONTH 12
#define DATA_LEN 11
using namespace std;

void toCSV(char*,char,char);
void date_init(int*,int);
void init_filename(char filename[20],int year,int month,char* type);
void init_outfp();
void outfile_close();
void init_infp();
void infile_close();

ifstream infp[YEAR][MONTH];
ofstream outfp[YEAR][MONTH];
int min_val[1000][1000]={0,};

int main(void){
	int line[32],lunar[3];
	char tmp[32];
	
	init_infp();
	
	for(int y=0;y<YEAR;++y)
		for(int m=1; m<=MONTH;++m){
			while(!infp[y][m-1].eof()){//전체 
		//	for(int i=0;i<500;++i){//일부 
		//		파일 읽기 
				for(int i=0;i<DATA_LEN&&!infp[y][m-1].eof();++i){
					infp[y][m-1].getline(tmp,128,',');
					line[i]=atoi(tmp);
				}
				if(min_val[line[7]][line[8]]<=0)
					min_val[line[7]][line[8]]=line[DATA_LEN-1];
				if(line[DATA_LEN-1]>0&&min_val[line[7]][line[8]]>line[DATA_LEN-1])
					min_val[line[7]][line[8]]=line[DATA_LEN-1];
//				outfp[y][m-1]<<line[5]<<endl;
			}
		}
	infile_close();
	init_infp();
	init_outfp();
	
	for(int y=0;y<YEAR;++y)
		for(int m=1; m<=MONTH;++m){
			while(!infp[y][m-1].eof()){//전체 
		//	for(int i=0;i<500;++i){//일부 
		//		파일 읽기 
				for(int i=0;i<DATA_LEN&&!infp[y][m-1].eof();++i){
					infp[y][m-1].getline(tmp,128,',');
					line[i]=atoi(tmp);
				}
		//		파일에 적기 
				if(!infp[y][m-1].eof()){
					for(int i=0; i<DATA_LEN-1;++i)
						outfp[y][m-1]<<line[i]<<',';
					if(line[DATA_LEN-1]<=0) outfp[y][m-1]<<min_val[line[7]][line[8]];
					else outfp[y][m-1]<<line[DATA_LEN-1];
					outfp[y][m-1]<<','<<endl;
	//				outfp[y][m-1]<<line[5]<<endl;
				}
			}
		}
	infile_close();
	outfile_close();
	
	return 0;
}

void init_filename(char filename[20],int year,int month,char* type){
	char year_str[10],month_str[10];
		
	filename[0]='\0';
	itoa(month,month_str,10);
	itoa(year,year_str,10);
	strcat(filename,year_str);
	if(month<10){
		filename[4]='0';
		filename[5]='\0';
	}
	strcat(filename,month_str);
	strcat(filename,type);
}

void init_outfp(){
	char filename[10];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			char type[10]=".csv";
			
			init_filename(filename,j+START_YEAR,i,type);//읽고 쓸 파일 이름 계산
			outfp[j][i-1].open(filename);
			if(outfp[j][i-1].fail()){
				cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
				return ;
			}
		}
	}
}
void outfile_close(){
	char filename[10];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			outfp[j][i-1].close();
		}
	}
}

void init_infp(){
	char filename[10];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			char type[10]="_.csv";
			
			init_filename(filename,j+START_YEAR,i,type);//읽고 쓸 파일 이름 계산
			infp[j][i-1].open(filename);
			if(infp[j][i-1].fail()){
				cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
				return ;
			}
		}
	}
}
void infile_close(){
	char filename[10];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			infp[j][i-1].close();
		}
	}
}
//void toCSV(char* str, char find_char,char new_char){
//
// 	char *ptr = strchr(str, '|');
//	int len;
//	
//	while (ptr != NULL)              // 검색된 문자열이 없을 때까지 반복
//	{
//    	*ptr=',';     
//    	len=ptr-str;
//    	ptr = strchr(ptr + 1, '|');  // 포인터에 1을 더하여 a 다음부터 검색
//	}
//	str[len]='\0';
//}
