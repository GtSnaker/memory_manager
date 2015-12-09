#include <stdio.h>

int main()
{
	FILE* fp;
	char buffer[16] = {0}; 
	remove("matriz");
	fp = fopen("matriz", "w");

	for(int i = 0; i < 10000; i++){
		sprintf(buffer, "%d", i);
  		fputs(buffer, fp);
  		fputs("\n", fp);
	}


	return 0;
}