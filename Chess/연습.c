#include <stdio.h>

int main(void) {
	int x = 0;
	int y = 3;
	int z = 3;
	printf("%d %d %d", x, y, z);
	while (x != y) {
		if (x == z)
			printf("���� %d", x);
		else
			printf("��� %d", x);
		x++;
	}
	return 0;
}