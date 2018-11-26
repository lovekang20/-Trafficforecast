#include "sol2lun.h"
#include <iostream>

using namespace std;

int main(void){
	int result[3];
	
	getLunarDate( getTotalDaySolar( 17, 3,15 ), result );
	cout<<result[0]<<endl<<result[1]<<endl<<result[2]<<endl;

	return 0;
}
