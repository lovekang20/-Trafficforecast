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
ifstream read_queue[10];//radix���ĵǴ� �߰������� ���� 
ofstream outfp[YEAR][MONTH];
//ofstream outfp[YEAR];
ofstream radix_queue[10];//radix���Ŀ� ����� �߰����� 

int main(void){
	int sort_seq[SORT_NUM]={6,5};//6,5��° ������ ���� ����  
	int digit_num[SORT_NUM]={2,2};//�������� �ڸ����� 2,2 �ڸ��� 
	int line[32];
	char tmp[32];
	int digit;
	double total_seq=0,proceed=0;
	
	for(int i=0;i<SORT_NUM;++i){
		total_seq+=digit_num[i]+1;
	}
	
	for(int l=0;l<SORT_NUM;++l){//���ĵǴ� ������ ����
		for(int k=0;k<=digit_num[l];++k){//���ĵǴ� �ڸ��� 
			cout<<"���൵: "<<proceed/total_seq<<endl; 
			proceed++;
		
			//�а� �� queue�� ������ open 
			if(k!=0) read_queue_init(k-1);
			else init_infile(); 
			if(k!=digit_num[l]) radix_queue_init(k);
			else init_outfile();
	
			if(k==0)
				for(int j=0;j<YEAR;++j)//���ĵǴ� ��
					for(int i=0; i<MONTH;++i){//���ĵǴ� �� 
						while(!infp[j][i].eof()){
							for(int ptr=0;ptr<ATTR_NUM;++ptr){//���� ����  
								infp[j][i].getline(tmp,DATA_SIZE,',');
								//cout<<tmp<<endl;
								line[ptr]=atoi(tmp);
							}
							if(!infp[j][i].eof()){
								digit=get_digit(line[sort_seq[l]],k);	//��� queue�� ���� ����
								//cout<<"digit1:"<<digit<<endl;
								write_data(radix_queue[digit],line);//���Ͽ� ���� 
							}
						}
					}
			else if(k==digit_num[l])
				for(int i=0;i<10;++i)
					while(!read_queue[i].eof()){
						for(int ptr=0;ptr<ATTR_NUM&&!read_queue[i].eof();++ptr){//���� ����  
							read_queue[i].getline(tmp,DATA_SIZE,',');
							line[ptr]=atoi(tmp);
						} 
						if(!read_queue[i].eof()){
									//cout<<1<<endl;
							write_file(line);//���Ͽ� ���� 
//							int year,month;
//	//						int date[8];
//							
//							//date_init(date,line[0]);//��¥�����͸� ��,��,�Ϸ� �ɰ�
//							year=line[0]+2000-START_YEAR;
//							month=line[1]-1;
//							write_data(outfp[year][month],line);
						}
					}
			else
				for(int i=0;i<10;++i)
					while(!read_queue[i].eof()){
							//	cout<<2<<endl;
						for(int ptr=0;ptr<ATTR_NUM&&!read_queue[i].eof();++ptr){//���� ����  
							read_queue[i].getline(tmp,DATA_SIZE,',');
							line[ptr]=atoi(tmp);
						} 
						if(!read_queue[i].eof()){
							digit=get_digit(line[sort_seq[l]],k);	//��� queue�� ���� ����
							//cout<<"digit2:"<<digit<<endl;
							write_data(radix_queue[digit],line);//���Ͽ� ����
						}
					}
			//�а� �� �����̳� queue�� close 
			if(k!=0) read_queue_close(k-1);
			else infile_close();
			if(k!=digit_num[l]) radix_queue_close(k);
			else outfile_close();
		}
	}
	
	return 0;
}


//radix������ queue�� ���Ϸ� ����  
void read_queue_init(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		read_queue[i].open(filename);
		if(read_queue[i].fail()){
			cout <<filename<< "������ ���� �� �����߽��ϴ�." << endl;	
			return ;
		}
	}
}
//queue ���� �ݱ� 
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
			cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
			return ;
		}
	}
}
//queue ���� �ݱ� 
void radix_queue_close(int k){
	char filename[20], queue_num[10];
	
	for(int i=0;i<10;++i){
		itoa(k,filename,10);
		itoa(i,queue_num,10);
		strcat(filename,queue_num);
		radix_queue[i].close();
	}
}
//�а� �� ���� �̸� ��� 
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
//���� ����  
void init_infile(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
			init_filename(filename,j+START_YEAR,i);//�а� �� ���� �̸� ���
			infp[j][i-1].open(filename);
			if(infp[j][i-1].fail()){
				cout <<filename<< "������ ���� �� �����߽��ϴ�." << endl;	
				return ;;
			}
		}
	}
}
//���� �ݱ� 
void infile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
			init_filename(filename,j+START_YEAR,i);//�а� �� ���� �̸� ���
			infp[j][i-1].close();
		}
	}
}
void init_outfile(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
			init_filename(filename,j+START_YEAR,i);//�а� �� ���� �̸� ���
//		init_filename(filename,j+START_YEAR,0);
			outfp[j][i-1].open(filename);
//		outfp[j].open(filename);
			if(outfp[j][i-1].fail()){
//		if(outfp[j].fail()){
				cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
				return ;
			}
		}
	}
}
//���� �ݱ� 
void outfile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
//			init_filename(filename,j+START_YEAR,i);//�а� �� ���� �̸� ���
			outfp[j][i-1].close();
		}
//		outfp[j].close();
	}
}
//��� queue�� ���� ���� 
int get_digit(int num,int k){	
	int remainder,digit , divisor=1;
	
	for(int i=0;i<=k;++i)
		divisor*=10;
	remainder=num%divisor;
	digit=remainder*10/divisor;
	return digit;
}

//���Ͽ� ���� 
void write_data(ofstream& outfp,int* line){
	for(int i=0;i<ATTR_NUM;++i){
		outfp<<line[i]<<',';
	}
	outfp<<endl;
}

void write_file(int* line){//���Ͽ� ���� 
	int j,i;
	
	j=line[3]+2000-START_YEAR;
	i=line[4]-1;
	write_data(outfp[j][i],line);
//	write_data(outfp[j],line);
}
