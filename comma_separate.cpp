#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define START_YEAR 2015
#define YEAR 2
#define MONTH 12
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

int main(void){
	int line[32],date[8];
	char tmp[32];
	
	init_infp();
	init_outfp();
	
	for(int y=0;y<YEAR;++y)
		for(int m=1; m<=MONTH;++m){
			while(!infp[y][m-1].eof()){//전체 
		//	for(int i=0;i<500;++i){//일부 
		//		파일 읽기 
				for(int i=0;i<6&&!infp[y][m-1].eof();++i){
					infp[y][m-1].getline(tmp,128,'|');
					line[i]=atoi(tmp);
				}
		//		파일에 적기 
				if(!infp[y][m-1].eof()){
		        	date_init(date,line[0]);   
					for(int i=0;i<3;++i)
						outfp[y][m-1]<<date[i]<<',';
					for(int i=1; i<6;++i)
						outfp[y][m-1]<<line[i]<<',';
					outfp[y][m-1]<<endl;
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
			char type[10]=".txt";
			
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

void date_init(int* div_date,int date){
	div_date[2] = date%100;
	date = (date-div_date[2])/100;
	div_date[1] = date%100;
	date = (date-div_date[1])/100;
	div_date[0] = date-2000;
}
