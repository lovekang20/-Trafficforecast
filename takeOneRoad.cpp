#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>

#define START_YEAR 2016
#define YEAR 1
#define MONTH 12
#define DATA_LEN 10
#define START 501
#define END 502
using namespace std;

void toCSV(char*,char,char);
void date_init(int*,int);
void init_filename(char filename[20],int year,int gate,char* type);
void init_ofilename(char filename[20],int year,char* gate,char* type);
void init_outfp(int s,int y);
void outfile_close();
void init_infp();
void infile_close();
void set_mapping();

ifstream infp[YEAR];
ofstream outfp;

ifstream map("mapping2.txt");
int mapping[1300][2]={0,};

int main(void){
	int line[32]={0,},pre[32]={0,};
	char tmp[32];
	int sum=0, cnt=0;
	
	set_mapping();
	init_infp();
		
	for(int y=0;y<YEAR;++y){
		for(int s=0;mapping[s][0]!=0&&!infp[y].eof();++s){
	//		for(int m=1; m<=MONTH;++m){
			for(int i=0;i<DATA_LEN-1&&!infp[y].eof();++i){
				infp[y].getline(tmp,128,',');
				line[i]=atoi(tmp);
			}
			infp[y].getline(tmp,128);
			line[DATA_LEN-1]=atoi(tmp);
			
//			if(mapping[s][0]==509&&mapping[s][1]==152){
//					cout<<line[7]<<" "<<line[8]<<" "<<line[9]<<endl;
//					cout<<mapping[s][0]<<" "<<mapping[s][1]<<endl;
//				}
//			
//			if(mapping[s][0]==502&&mapping[s][1]==167){
//					cout<<line[7]<<" "<<line[8]<<" "<<line[9]<<endl;
//					cout<<mapping[s][0]<<" "<<mapping[s][1]<<endl;
//				}
			while((mapping[s][0]!=line[7]||mapping[s][1]!=line[8])&&!infp[y].eof()){//전체
		//	for(int i=0;i<500;++i){//일부 
		//		파일 읽기		//		파일에 적기 
		
				for(int i=0;i<DATA_LEN-1&&!infp[y].eof();++i){
					infp[y].getline(tmp,128,',');
					line[i]=atoi(tmp);
				}
				infp[y].getline(tmp,128);
				line[DATA_LEN-1]=atoi(tmp);
				
//				if(mapping[s][0]==502&&mapping[s][1]==167){
//					cout<<line[7]<<" "<<line[8]<<" "<<line[9]<<endl;
//					cout<<mapping[s][0]<<" "<<mapping[s][1]<<endl;
//				}
//				if(line[7]==502&&line[8]==167){
//					cout<<line[7]<<" "<<line[8]<<" "<<line[9]<<endl;
//					cout<<mapping[s][0]<<" "<<mapping[s][1]<<endl;
//				}
			}
			init_outfp(s,y);
			while(mapping[s][0]==line[7]&&mapping[s][1]==line[8]&&!infp[y].eof()){//전체
		//	for(int i=0;i<500;++i){//일부 
		//		파일 읽기		//		파일에 적기 
		
				
				if(!infp[y].eof()){
					for(int i=0; i<DATA_LEN-1;++i)
						outfp<<line[i]<<',';
					outfp<<line[DATA_LEN-1]<<endl;					
				}
				for(int i=0;i<DATA_LEN-1&&!infp[y].eof();++i){
					infp[y].getline(tmp,128,',');
					line[i]=atoi(tmp);
				}
				infp[y].getline(tmp,128);
				line[DATA_LEN-1]=atoi(tmp);
			}
			outfp.close();
		}
			
	}
	infile_close();
	
	return 0	;
}

void set_mapping(){
	int i=0;
	char tmp[10];
	
	while(!map.eof()){
		map>>tmp;
		if(!map.eof()){
			mapping[i][0]=atoi(tmp);
			map>>tmp;
			mapping[i][1]=atoi(tmp);		
			i++;
		}
	}
	map.close();
	
}
void init_ofilename(char filename[20],int year,char* gate,char* type,int s){
	char year_str[10],month_str[10];
	
//	if(mapping[s][0]<502)
		strcpy(filename,"files/");
//	else if(mapping[s][0]<504)
//		strcpy(filename,"files2/");
//	else if(mapping[s][0]<506)
//		strcpy(filename,"files3/");
//	else if(mapping[s][0]<508)
//		strcpy(filename,"files4/");
//	else
//		strcpy(filename,"files5/");
	
	itoa(year,year_str,10);
	strcat(filename,year_str);
	strcat(filename,"_");
	strcat(filename,gate);
	strcat(filename,type);
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

void init_outfp(int s,int y){
	char filename[30];
	
//		}
	char type[20]="\0";
	char gate[20];
	char target[20];
	
	strcat(type,".csv");
	
	itoa(mapping[s][0],gate,10);
	itoa(mapping[s][1],target,10);				
	strcat(gate,"_");
	strcat(gate,target);
	init_ofilename(filename,y+START_YEAR,gate,type,s);//읽고 쓸 파일 이름 계산

	outfp.open(filename);
	if(outfp.fail()){
		cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
		return ;
	
	}

}
void outfile_close(){
	char filename[10];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
//		for(int i=1; i<=MONTH;++i){//정렬되는 월 
//			outfp[j][i-1].close();
//		}
			outfp.close();
	}
}

void init_infp(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
//		for(int i=1; i<=MONTH;++i){//정렬되는 월 
//			char type[10]=".csv";
//			
//			init_filename(filename,j+START_YEAR,i,type);//읽고 쓸 파일 이름 계산
//			infp[j][i-1].open(filename);
//			if(infp[j][i-1].fail()){
//				cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
//				return ;
//			}
//		}
			char type[10]=".csv";
			
			init_filename(filename,j+START_YEAR,0,type);//읽고 쓸 파일 이름 계산
			infp[j].open(filename);
			if(infp[j].fail()){
				cout <<filename<< " 파일을 여는 데 실패했습니다." << endl;	
				return ;
			}

	}
}
void infile_close(){
	char filename[20];
	
	for(int j=0;j<YEAR;++j){//정렬되는 년
//		for(int i=1; i<=MONTH;++i){//정렬되는 월 
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
//	while (ptr != NULL)              // 검색된 문자열이 없을 때까지 반복
//	{
//    	*ptr=',';     
//    	len=ptr-str;
//    	ptr = strchr(ptr + 1, '|');  // 포인터에 1을 더하여 a 다음부터 검색
//	}
//	str[len]='\0';
//}
