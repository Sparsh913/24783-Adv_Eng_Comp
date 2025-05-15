#include <stdio.h>
int main(void)
{
	char str[256];
	while(nullptr!=fgets(str,255,stdin))
	{
		printf("%s",str);
	}
	return 0;
}
