#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;
#define YEAR 1
#define MONTH 12
#define SORT_NUM 2
#define START_YEAR 2016
#define ATTR_NUM 11
#define DATA_SIZE 64

void read_queue_init(int k);
void read_queue_close(int k);
void radix_queue_init(int k);
void radix_queue_close(int k);
void init_filename(char filename[10],int,int);
void init_infile();
void infile_close();
void init_outfile();
void outfile_close();
int get_digit(int,int);
//void date_init(int*,int);
void write_data(ofstream& outfp,int* line);
void write_file(int* line);

ifstream infp[YEAR][MONTH];
ifstream read_queue[10];//radix정렬되는 중간파일을 읽음 
ofstream outfp[YEAR][MONTH];
//ofstream outfp[YEAR];
ofstream radix_queue[10];//radix정렬에 사용할 중간파일 

int main(void){
	int sort_seq[SORT_NUM]={6,5};//6,5번째 순으로 값을 정렬  
	int digit_num[SORT_NUM]={2,2};//데이터의 자릿수가 2,2 자리임 
	int line[32];
	char tmp[32];
	int digit;
	double total_seq=0,proceed=0;
	
	for(int i=0;i<SORT_NUM;++i){
		total_seq+=digit_num[i]+1;
	}
	
	for(int l=0;l<SORT_NUM;++l){//정렬되는 데이터 종류
		for(int k=0;k<=digit_num[l];++k){//정렬되는 자릿수 
			cout<<"진행도: "<<proceed/total_seq<<endl; 
			proceed++;
		
			//읽고 쓸 queue나 파일을 open 
			if(k!=0) read_queue_init(k-1);
			else init_infile(); 
			if(k!=digit_num[l]) radix_queue_init(k);
			else init_outfile();
	
			if(k==0)
				for(int j=0;j<YEAR;++j)//정렬되는 년
					for(int i=0; i<MONTH;++i){//정렬되는 월 
						while(!infp[j][i].eof()){
							for(int ptr=0;ptr<ATTR_NUM;++ptr){//파일 읽음  
								infp[j][i].getline(tmp,DATA_SIZE,',');
								//cout<<tmp<<endl;
								line[ptr]=atoi(tmp);
							}
							if(!infp[j][i].eof()){
								digit=get_digit(line[sort_seq[l]],k);	//어느 queue에 쓸지 선택
								//cout<<"digit1:"<<digit<<endl;
								write_data(radix_queue[digit],line);//파일에 적음 
							}
						}
					}
			else if(k==digit_num[l])
				for(int i=0;i<10;++i)
					while(!read_queue[i].eof()){
						for(int ptr=0;ptr<ATTR_NUM&&!read_queue[i].eof();++ptr){//파일 읽음  
							read_queue[i].getline(tmp,DATA_SIZE,',');
							line[ptr]=atoi(tmp);
						} 
						if(!read_queue[i].eof()){
									//cout<<1<<endl;
							write_file(line);//파일에 적음 
//							int year,month;
//	//						int date[8];
//							
//							//date_init(date,line[0]);//날짜데이터를 년,월,일로 쪼갬
//							year=line[0]+2000-START_YEAR;
//							month=line[1]-1;
//							write_data(outfp[year][month],line);
						}
					}
			else
				for(int i=0;i<10;++i)
					while(!read_queue[i].eof()){
							//	cout<<2<<endl;
						for(int ptr=0;ptr<ATTR_NUM&&!read_queue[i].eof();++ptr){//파일 읽음  
							read_queue[i].getline(tmp,DATA_SIZE,',');
							line[ptr]=atoi(tmp);
						} 
						if(!read_queue[i].eof()){
							digit=get_digit(line[sort_seq[l]],k);	//어느 queue에 쓸지 선택
							//cout<<"digit2:"<<digit<<endl;
							write_data(radix_queue[digit],line);//파일에 적음
						}
					}
			//읽고 쓴 파일이나 queue를 close 
			if(k!=0) read_queue_close(k-1);
			else infile_close();
			if(k!=digit_num[l]) radix_queue_close(k);
			else outfile_close();
		}
	}
	
	return 0;
}


//radix정렬할 queue를 파일로 구현  
void read_queue_init(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		read_queue[i].open(filename);
		if(read_queue[i].fail()){
			cout <<filename<< "파일을 여는 데 실패했습니다." << endl;	
			return ;
		}
	}
}
//queue 파일 닫기 
void read_queue_close(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		read_queue[i].close();
	}
}
void radix_queue_init(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		radix_queue[i].open(filename);
		if(radix_queue[i].fail()){
			cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
			return ;
		}
	}
}
//queue 파일 닫기 
void radix_queue_close(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		radix_queue[i].close();
	}
}
//읽고 쓸 파일 이름 계산 
void init_filename(char filename[10],int year,int month){
	char month_str[20];
		
	itoa(month,month_str,10);
	itoa(year,filename,10);
	if(month<10){
		filename[4]='0';
		filename[5]='\0';
	}
	strcat(filename,month_str);
	strcat(filename,"_.csv");
}
//파일 열기  
void init_infile(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			init_filename(filename,j+START_YEAR,i);//읽고 쓸 파일 이름 계산
			infp[j][i-1].open(filename);
			if(infp[j][i-1].fail()){
				cout <<filename<< "파일을 여는 데 실패했습니다." << endl;	
				return ;;
			}
		}
	}
}
//파일 닫기 
void infile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			init_filename(filename,j+START_YEAR,i);//읽고 쓸 파일 이름 계산
			infp[j][i-1].close();
		}
	}
}
void init_outfile(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
			init_filename(filename,j+START_YEAR,i);//읽고 쓸 파일 이름 계산
//		init_filename(filename,j+START_YEAR,0);
			outfp[j][i-1].open(filename);
//		outfp[j].open(filename);
			if(outfp[j][i-1].fail()){
//		if(outfp[j].fail()){
				cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
				return ;
			}
		}
	}
}
//파일 닫기 
void outfile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
		for(int i=1; i<=MONTH;++i){//정렬되는 월 
//			init_filename(filename,j+START_YEAR,i);//읽고 쓸 파일 이름 계산
			outfp[j][i-1].close();
		}
//		outfp[j].close();
	}
}
//어느 queue에 쓸지 선택 
int get_digit(int num,int k){	
	int remainder,digit , divisor=1;
	
	for(int i=0;i<=k;++i)
		divisor*=10;
	remainder=num%divisor;
	digit=remainder*10/divisor;
	return digit;
}

//파일에 적음 
void write_data(ofstream& outfp,int* line){
	for(int i=0;i<ATTR_NUM;++i){
		outfp<<line[i]<<',';
	}
	outfp<<endl;
}

void write_file(int* line){//파일에 적음 
	int j,i;
	
	j=line[3]+2000-START_YEAR;
	i=line[4]-1;
	write_data(outfp[j][i],line);
//	write_data(outfp[j],line);
}
