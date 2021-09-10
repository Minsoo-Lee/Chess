#include <stdio.h>
#include <string.h>
#include <math.h>

static char star[5] = " * ";
//P 0~7, R 8~9, N 10~11, V 12~13, Q 14, K 15
struct point {
	char name[10];
	int x;
	int y;
	int move;
};
typedef struct point Point;

void show(Point* w, Point* b, Point(*board)[12]) {
	int x, y, i;
	printf("\n     ");
	for (i = 0; i < 8; i++) {
		printf("%d   ", i);
	}
	printf("\t\t죽은 흰 말\t죽은 검은 말\n\n");
	for (x = 0; x < 8; x++) {
		printf(" %d ", x);
		for (y = 0; y < 12; y++) {
			for (i = 0; i < 16; i++) {
				if ((w[i].x == x) && (w[i].y == y)) {
					strcpy(board[x][y].name, w[i].name);
					break;
				}
				else if ((b[i].x == x) && (b[i].y == y)) {
					strcpy(board[x][y].name, b[i].name);
					break;
				}
				else
					strcpy(board[x][y].name, star);
			}
			if (y < 8)
				printf(" %s", board[x][y].name);
			else if (y == 8)
				printf("\t\t %s", board[x][y].name);
			else if ((y == 9) || (y == 11))
				printf(" %s", board[x][y].name);
			else
				printf("\t  %s", board[x][y].name);
		}
		printf("\n\n");
	}
}

void check1(int* x, int* y, Point* team, Point* enemy, Point(*board)[12], int* result, int* n) {
	int x1, y1;
	printf("움직일 말의 현재 위치를 입력하세요: ");
	scanf("%d %d", &x1, &y1);
	for (int i = 0; i < 16; i++) {
		if ((team[i].x == x1) && (team[i].y == y1)) {
			*result = 0;
			*n = i;
			printf("\n움직일 말: %c %d\n\n", team[i].name[2], *n);
			break;
		}
		else if ((enemy[i].x == x1) && (enemy[i].y == y1)) {
			printf("그 위치는 적군의 말입니다.\n");
			*result = 1;
		}
		else if (strcmp(board[x1][y1].name, star) == 0) {
			printf("그 위치에는 아무것도 없습니다.\n");
			*result = 1;
			break;
		}
		else if ((x1 > 7) || (y1 > 7)) {
			printf("보드판을 벗어났습니다.\n");
			*result = 1;
			break;
		}
		else if ((x1 < 0) && (y1 < 0)) {
			printf("초기화를 시작합니다.\n");
			*result = 2;
			break;
		}
	}
	*x = x1;
	*y = y1;
}

void check_line(int x1, int y1, int x2, int y2, Point w[], Point b[], Point board[][12], int* res) {
	int xc, yc, xi, yi;
	for (int i = 0; i < 16; i++) {
		if (x1 == x2) {
			yc = (abs(y2 - y1)) / (y2 - y1);
			xi = x1;
			yi = y1 + yc;
			while(yi!=y2) {
				if ((strcmp(board[x1][yi].name, w[i].name) == 0) || (strcmp(board[x1][yi].name, b[i].name) == 0)) {
					*res= 1;
					break;
				}
				else {
					*res = 0;
				}
				yi += yc;
			}
			if (*res == 1)
				break;
		}
		else if (y1 == y2) {
			xc = (abs(x2 - x1)) / (x2 - x1);
			xi = x1 + xc;
			yi = y1;
			while(xi!=x2) {
				if ((strcmp(board[xi][y1].name, w[i].name) == 0) || (strcmp(board[xi][y1].name, b[i].name) == 0)) {
					*res = 1;
					break;
				}
				else {
					*res = 0;
				}
				xi += xc;
			}
			if (*res == 1)
				break;
		}
	}
}

void check_cross(int x1, int y1, int x2, int y2, Point w[], Point b[], Point board[][12], int* res) {
	int xc, yc, xi, yi;
	xc = (abs(x2 - x1)) / (x2 - x1);
	yc = (abs(y2 - y1)) / (y2 - y1);
	printf("%d %d %d %d", xc, yc, x2, y2);
	for (int i = 0; i < 16; i++) {
		yi = y1+yc;
		xi = x1+xc;
		while (yi != y2) {
			if (((w[i].x==xi)&&(w[i].y==yi)) || ((b[i].x==xi)&&(b[i].y==yi))){
				*res = 1;
				break;
			}
			else
				*res = 0;
			xi += xc;
			yi += yc;
		}
		if (*res == 1)
			break;
	}
}

void check2(int x1, int y1, int* x, int* y, Point* team, Point* enemy, int* result, int n, int turn, Point (*board)[12]) {
	if ((*result) == 1)
		goto end;
	int x2, y2, count = 0, res;
	printf("움직일 말의 이동 위치를 입력하세요: ");
	scanf("%d %d", &x2, &y2);
	if ((x1 == x2) && (y1 == y2)) {
		printf("제자리입니다.\n");
		*result = 1;
		goto end;
	}
	if ((x2 > 7) || (y2 > 7)) {
		printf("out of board\n");
		*result = 1;
	}
	else{
		switch (team[(n)].name[2]) {
		case 'P':
			if (team[n].move > 1) {
				if ((y2 == y1) && (x2 == x1 - turn)) {
					for (int i = 0; i < 16; i++) {
						if ((enemy[i].x == x2) && (enemy[i].y == y2)) {
							*result = 1;
							printf("잡을 수 없습니다.\n");
							break;
						}
						else 
							*result = 0;
					}
				}
				else if ((x2 == x1 - turn) && ((y2 == y1 - turn) || (y2 == y1 + turn))) {
					for (int i = 0; i < 16; i++) {
						if ((enemy[i].x == x2) && (enemy[i].y == y2)) {
							break;
						}
						else
							count++;
					}
					if (count < 16)
						*result = 0;
					else {
						printf("이동이 불가능합니다.\n");
						*result = 1;
					}
				}
				else {
					printf("이동이 불가능합니다.\n");
					*result = 1;
				}
			}
			else {
				if ((y2 == y1) && (x2 == x1 - 2 * turn)) {
					*result = 0;
					team[n].move++;
				}
			}
			break;
		case 'R':
			if ((y2 == y1) || (x2 == x1)) {
				check_line(x1, y1, x2, y2, team, enemy, board, &res);
				if (res == 1) {
					*result = 1;
					printf("이동이 불가능합니다.\n");
				}
				else
					*result = 0;
			}
			else {
				printf("이동이 불가능합니다.\n");
				*result = 1;
			}
			break;
		case 'N':
			if (((x2 == x1 + 1) || (x2 == x1 - 1)) && ((y2 == y1 + 2) || (y2 == y1 - 2)))
				*result = 0;
			else if (((x2 == x1 + 2) || (x2 == x1 - 2)) && ((y2 == y1 + 1) || (y2 == y1 - 1))) 
				*result = 0;
			else {
				printf("이동이 불가능합니다.\n");
				*result = 1;
			}
			break;
		case 'V':
			if (abs(x2 - x1) == abs(y2 - y1)) {
				check_cross(x1, y1, x2, y2, team, enemy, board, &res);
				if (res == 1) {
					*result = 1;
					printf("이동이 불가능합니다.\n");
				}
				else
					*result = 0;
			}
			else {
				printf("이동이 불가능합니다.\n");
				*result = 1;
			}
			break;
		case 'Q':
			if ((x2 == x1) || (y2 == y1) || ((x2 - x1) == (y2 - y1))) {
				check_line(x1, y1, x2, y2, team, enemy, board, &res);
				if (res == 1) {
					printf("이동이 불가능합니다.\n");
					*result = 1;
				}
				else {
					check_cross(x1, y1, x2, y2, team, enemy, board, &res);
					if (res == 1) {
						printf("이동이 불가능합니다.\n");
						*result = 1;
					}
					else
						*result = 0;
				}
			}
			else {
				printf("이동이 불가능합니다.\n");
				*result = 1;
			}
			break;
		case 'K':
			if (((x2 == x1) || (x2 == x1 + 1) || (x2 == x1 - 1)) && ((y2 == y1) || (y2 == y1 + 1) || (y2 == y1 - 1)))
				*result = 0;
			else {
				printf("이동이 불가능합니다.\n");
				*result = 1;
			}
			break;
		}
	}
	*x = x2;
	*y = y2;
end:
	;
}

void check3(int x2, int y2, Point* team, Point* enemy, Point(*board)[12], int* result, int n, int turn) {
	if ((*result) == 1)
		goto end;
	else {
		for (int i = 0; i < 16; i++) {
			if (strcmp(board[x2][y2].name, team[i].name) == 0) {
				printf("같은 편이 있는 곳입니다.\n");
				*result = 1;
				break;
			}
			else if ((enemy[i].x==x2)&&(enemy[i].y==y2)) {
				printf("잡았습니다.\n");
				*result = 0;
				if (turn == -1) {
					enemy[i].x = i % 8;
					enemy[i].y = i / 8 + 8;
					printf("%d %d", enemy[i].x, enemy[i].y);
				}
				else {
					enemy[i].x = i % 10;
					enemy[i].y = i / 10 + 10;
				}
				team[n].x = x2;
				team[n].y = y2;
				break;
			}
			else if (strcmp(board[x2][y2].name, star) == 0) {
				team[n].x = x2;
				team[n].y = y2;
				*result = 0;
				break;
			}
		}
	}
end:
	;
}

void checkmate_rule(int n, Point* team, Point* enemy, Point(*board)[12], int turn, int* checkmate) {
	int k = 12;
	int ex, ey, tx, ty, xc, yc, count;
	int xi = tx;
	int yi = ty;
	if (ex != tx)
		xc = abs(ex - tx) / (ex - tx);
	else
		xc = 0;
	if (ey != ty)
		yc = abs(ey - ty) / (ey - ty);
	else
		yc = 0;
	ex = enemy[k].x;
	ey = enemy[k].y;
	tx = team[n].x;
	ty = team[n].y;
	switch (team[n].name[2]) {
	case 'P':
		if (((ey == ty - 1) || (ey == ty + 1)) && (ex == tx - turn))
			(*checkmate)++;
		break;
	case'R':
		count = 0;
		if (ex == tx) {
			yi += yc;
			while (yi != ey) {
				if (strcmp(board[tx][yi].name, star) != 0) {
					count++;
					break;
				}
				yi += yc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		if (ey == ty) {
			xi += xc;
			while (xi != ex) {
				if (strcmp(board[xi][ty].name, star) != 0) {
					count++;
					break;
				}
				xi += xc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		break;
	case 'N':
		if (((ex == tx - 1) || (ex == tx + 1)) && ((ey == ty - 2) || (ey == ty + 2)))
			(*checkmate)++;
		else if (((ex == tx - 2) || (ex == tx + 2)) && ((ey == ty - 1) || (ey == ty + 1)))
			(*checkmate)++;
		break;
	case 'V':
		if (abs(ex - tx) == abs(ey - ty)) {
			while (xi != ex) {
				if (strcmp(board[xi][yi].name, star) != 0) {
					count++;
					break;
				}
				xi += xc;
				yi += yc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		break;
	case 'Q':
		count = 0;
		if (abs(ex - tx) == abs(ey - ty)) {
			while (xi != ex) {
				if (strcmp(board[xi][yi].name, star) != 0) {
					count++;
					break;
				}
				xi += xc;
				yi += yc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		else if (ex == tx) {
			yi += yc;
			while (yi != ey) {
				if (strcmp(board[tx][yi].name, star) != 0) {
					count++;
					break;
				}
				yi += yc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		else if (ey == ty) {
			xi += xc;
			while (xi != ex) {
				if (strcmp(board[xi][ty].name, star) != 0) {
					count++;
					break;
				}
				xi += xc;
			}
			if (count > 0)
				break;
			else
				(*checkmate)++;
		}
		break;
	}
end:
	;
}

void checkmate(Point* white, Point* black, Point* board, int turn) {
	int xw, yw, xb, yb, nw, nb;
	int w_checkmate, b_checkmate;
	w_checkmate = 0;
	b_checkmate = 0;
	white[12].x = xw;
	white[12].y = yw;
	black[12].x = xb;
	black[12].y = yb;
	for (int i = 0; i < 16; i++) {
		if (white[i].y < 8)
			nw = i;
		if (black[i].y < 8)
			nb = i;
		else
			continue;
		checkmate_rule(nw, white, black, board, turn, &w_checkmate);
		checkmate_rule(nb, black, white, board, turn, &b_checkmate);
	}
	if ((w_checkmate > 0)||(b_checkmate>0))
		printf("체크메이트!\n");
}
int main(void) { // 다시 입력은 1 반환
	int x1, x2, y1, y2, result, n, turn = 1, turn_count = 1;
	Point white[16];
	Point black[16];
	Point board[8][12];
	

	for (int i = 0; i < 16; i++) {
		white[i].move = 1;
		black[i].move = 1;
		if (i < 8) {
			white[i].x = 6;
			white[i].y = i;
			black[i].x = 1;
			black[i].y = i;
			strcpy(white[i].name, "w_P");
			strcpy(black[i].name, "b_P");
		}
		else {
			white[i].x = 7;
			white[i].y = 15 - i;
			black[i].x = 0;
			black[i].y = 15 - i;
			if ((i == 8) || (i == 15)) {
				strcpy(white[i].name, "w_R");
				strcpy(black[i].name, "b_R");
			}
			else if ((i == 9) || (i == 14)) {
				strcpy(white[i].name, "w_N");
				strcpy(black[i].name, "b_N");
			}
			else if ((i == 10) || (i == 13)) {
				strcpy(white[i].name, "w_V");
				strcpy(black[i].name, "b_V");
			}
			else if (i == 11) {
				strcpy(white[i].name, "w_Q");
				strcpy(black[i].name, "b_Q");
			}
			else if (i == 12) {
				strcpy(white[i].name, "w_K");
				strcpy(black[i].name, "b_K");
			}
		}
	}

	while (1) {
		printf("\n<%d번째 턴>\n", turn_count);
		show(white, black, board);
		printf("\n\n");
		if (turn == 1) {
			printf("하얀색 차례입니다.\n");
			check1(&x1, &y1, white, black, board, &result, &n);
			check2(x1, y1, &x2, &y2, white, black, &result, n, turn, board);
			check3(x2, y2, white, black, board, &result, n, turn);
			if (result == 1)
				continue;
		}
		else if (turn == -1) {
			printf("검은색 차례입니다.\n");
			check1(&x1, &y1, black, white, board, &result, &n);
			check2(x1, y1, &x2, &y2, black, white, &result, n, turn, board);
			check3(x2, y2, black, white, board, &result, n, turn);
			if (result == 1)
				continue;
			turn_count++;
		}
		turn *= -1;
		if ((black[11].y == 10) || (black[11].y == 11)) {
			printf("흰색이 이겼습니다!\n");
			break;
		}
		else if ((white[12].y == 8) || (white[12].y == 9)) {
			printf("검은색이 이겼습니다!\n");
			break;
		}
	}

	return 0;
}