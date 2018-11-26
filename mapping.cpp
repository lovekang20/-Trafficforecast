#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define START_YEAR 2016
#define YEAR 1
#define MONTH 12
#define DATA_LEN 10
#define START 100
using namespace std;

void toCSV(char*,char,char);
void date_init(int*,int);
void init_filename(char filename[20],int year,int month,char* type);
void init_outfp();
void outfile_close();
void init_infp();
void infile_close();

ifstream infp[YEAR];

int main(void){
	int line[32]={0,},pre[32]={0,};
	char tmp[32];
	int sum=0, cnt=0;
	
	init_infp();
	ofstream outfp("mapping2.txt");
	
	for(int y=0;y<YEAR;++y){
		int target=0,from=0,pre_t,pre_f;
		int i=0,j=0;
		while(!infp[y].eof()){
//		for(int m=1; m<=MONTH;++m){
			do{//��ü 
		//	for(int i=0;i<500;++i){//�Ϻ� 
		//		���� �б�
				pre_t=target;
				pre_f=from; 
				for(int i=0;i<DATA_LEN-1&&!infp[y].eof();++i){
					infp[y].getline(tmp,128,',');
					line[i]=atoi(tmp);
				}
				infp[y].getline(tmp,128);
				line[DATA_LEN-1]=atoi(tmp);
//				if(line[7]==502&&line[8]==167){
//					cout<<line[7]<<" "<<line[8]<<" "<<line[9]<<endl;
//					cout<<j<<endl;
//				}
		//		���Ͽ� ���� 
//				if(line[7]>=START&&!infp[y].eof()){
//					for(int i=0; i<DATA_LEN-1;++i)
//						outfp[y]<<line[i]<<',';
//					outfp[y]<<line[DATA_LEN-1]<<endl;					
//				}
				from=line[7];
				target=line[8];
				j++;
			}while(pre_t==target&&!infp[y].eof());
			if(!infp[y].eof()&&j>8250){
				outfp<<pre_f<<" "<<pre_t<<endl;
//				cout<<from<<" "<<pre<<" "<<endl;
//				cout<<pre<<" "<<target<<" "<<endl;
				
//				cout<<j<<endl;
				i++;
			}
//			else{
//				cout<<j<<endl;
//			}
			j=0;
		}
		cout<<i<<endl;
	}
//		}
	infile_close();
	outfp.close();
	
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
//
//void init_outfp(){
//	char filename[30];
//	
//	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
////		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
////			char type[10]="_.csv";
////			
////			init_filename(filename,j+START_YEAR,i,type);//�а� �� ���� �̸� ���
////			outfp[j][i-1].open(filename);
////			if(outfp[j][i-1].fail()){
////				cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
////				return ;
////			}
////		}
//			char type[20]="_";
//			char start[20];
//			
//			itoa(START,start,10);
//			strcat(type,start);
//			strcat(type,".csv");
//			
//			init_filename(filename,j+START_YEAR,0,type);//�а� �� ���� �̸� ���
//			outfp[j].open(filename);
//			if(outfp[j].fail()){
//				cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
//				return ;
//			}
//
//	}
//}
//void outfile_close(){
//	char filename[10];
//	
//	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
////		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
////			outfp[j][i-1].close();
////		}
//			outfp[j].close();
//	}
//}

void init_infp(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
//		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
//			char type[10]=".csv";
//			
//			init_filename(filename,j+START_YEAR,i,type);//�а� �� ���� �̸� ���
//			infp[j][i-1].open(filename);
//			if(infp[j][i-1].fail()){
//				cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
//				return ;
//			}
//		}
			char type[10]=".csv";
			
			init_filename(filename,j+START_YEAR,0,type);//�а� �� ���� �̸� ���
			infp[j].open(filename);
			if(infp[j].fail()){
				cout <<filename<< " ������ ���� �� �����߽��ϴ�." << endl;	
				return ;
			}

	}
}
void infile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//���ĵǴ� ��
//		for(int i=1; i<=MONTH;++i){//���ĵǴ� �� 
//			infp[j][i-1].close();
//		}
			infp[j].close();
	}
}
//void toCSV(char* str, char find_char,char new_char){
//
// 	char *ptr = strchr(str, '|');
//	int len;
//	
//	while (ptr != NULL)              // �˻��� ���ڿ��� ���� ������ �ݺ�
//	{
//    	*ptr=',';     
//    	len=ptr-str;
//    	ptr = strchr(ptr + 1, '|');  // �����Ϳ� 1�� ���Ͽ� a �������� �˻�
//	}
//	str[len]='\0';
//}
