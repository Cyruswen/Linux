
#include <stdio.h>
#include "sub.h"
#include "add.h"

int main()
{
	int x = 100;
	int y = 30;
	int z = mysub(x, y);
	int f = myadd(x, y);

	printf("%d, %d\n", z, f);
}
