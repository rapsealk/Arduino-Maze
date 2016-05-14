/*
 * Copyright(c) 2015 Jeongseok Kang
 * Program for Maze Algorithm
 * Code Written with C
 */

#include <stdio.h>

typedef enum { UP, DOWN, LEFT, RIGHT } dir_t;

typedef struct {
	int x;
	int y;
} point_t;

char MAP[11][11] = { { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
					 { '1', '0', '#', '0', '#', '0', '#', '0', '#', '0', '1' },
					 { '1', '#', 'W', '#', 'W', '#', 'W', '#', 'W', '#', '1' },
					 { '1', '0', '#', '0', '#', '0', '#', '0', '#', '0', '1' },
					 { '1', '#', 'W', '#', 'W', '#', 'W', '#', 'W', '#', '1' },
					 { '1', '0', '#', '0', '#', '0', '#', '0', '#', '0', '1' },
					 { '1', '#', 'W', '#', 'W', '#', 'W', '#', 'W', '#', '1' },
					 { '1', '0', '#', '0', '#', '0', '0', '#', '0', '0', '1' },
					 { '1', '#', 'W', '#', 'W', '#', 'W', '#', 'W', '#', '1' },
					 { '1', '0', '#', '0', '#', '0', '#', '0', '#', '0', '1' },
					 { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' } };
char MAP2[11][11] = { '0', };
char MAZE[11][11] = { { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
					  { '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '1' },
					  { '1', '0', 'W', '1', 'W', '0', 'W', '1', 'W', '0', '1' },
					  { '1', '0', '1', '0', '1', '0', '0', '0', '1', '0', '1' },
					  { '1', '0', 'W', '1', 'W', '0', 'W', '1', 'W', '0', '1' },
					  { '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1' },
					  { '1', '0', 'W', '1', 'W', '0', 'W', '0', 'W', '0', '1' },
					  { '1', '0', '0', '0', '1', '0', '1', '0', '1', '0', '1' },
					  { '1', '0', 'W', '0', 'W', '0', 'W', '1', 'W', '0', '1' },
					  { '1', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1' },
					  { '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' } };
char MAZE2[11][11] = { '0', };

// 1: wall, 0: road, #: unknown

// (x, y) -> (2x-1, 2y-1)
point_t user = { 0, 0 };
point_t pass = { 0, 0 };
point_t dest = { 0, 0 };

dir_t Direction = RIGHT;

int right = 0;
int left = 0;
int up = 0;
int down = 0;

int count = 0;

// Announce Functions
point_t InitiatePoint();

void ShowMap(); // 지도를 출력하는 함수
void ShowMaze(); // 미로를 출력하는 함수
void ShowPoint(); // 현재 위치를 출력하는 함수
void Search(); // 상하좌우 탐색하는 함수
int Calculate(); // 경로 계획하는 함수
int Move(); // 계획에 따라 이동하는 함수
void TurnMap(dir_t dir); // 지도를 회전하는 함수
void Turn(dir_t dir); // 회전하는 함수

// Main Function
int main()
{
	user = InitiatePoint();
	dest = InitiatePoint();
	pass = InitiatePoint();
	MAP[user.x][user.y] = MAZE[user.x][user.y] = 'S';
	MAP[dest.x][dest.y] = MAZE[dest.x][dest.y] = 'P';
	MAP[pass.x][pass.y] = MAZE[pass.x][pass.y] = 'D';

	ShowMap();

	while (1) {
		printf("Count: %d\n", ++count);
		Search();
		if (!Calculate()) {
			dest = pass;
			break;
		}
		Move();
		if (count == 5)
			return 0;
	}
	count = 0;
	while (1) {
		printf("Count %d\n", ++count);
		Search();
		if (!Calculate()) {
			return 0;
		}
		Move();
		if (count == 3) {
			return 0;
		}
	}
	return 0;
}

// Define Functions ------------------------------------------------------------------------------------------------------------------------

point_t InitiatePoint()
{
	point_t temp;
	printf("Point x y: ");
	scanf("%d %d", &temp.x, &temp.y);
	temp.x = 2 * temp.x - 1;
	temp.y = 2 * temp.y - 1;
	return temp;
}

void ShowMap()
{
	int i, j;
	char temp = MAP[user.x][user.y];
	MAP[user.x][user.y] = '*';
	for (i = 0; i < 11; i++) {
		for (j = 0; j < 11; j++) {
			printf("%c ", MAP[i][j]);
		}
		puts("");
	}
	MAP[user.x][user.y] = temp;
	printf("(x, y): %d, %d\n", (user.x + 1) / 2, (user.y + 1) / 2);
	printf("up: %d\ndown: %d\nright: %d\nleft: %d\n", up, down, right, left);
	puts("");
}

void ShowMaze()
{
	int i, j;
	for (i = 0; i < 11; i++) {
		for (j = 0; j < 11; j++) {
			printf("%c ", MAZE[i][j]);
		}
		puts("");
	}
}

void ShowPoint()
{
	printf("(x, y): %d, %d\n", (user.x + 1) / 2, (user.y + 1) / 2);
}

void Search()
{
	int i, j;
	int num = 0;
	for (i = user.x - 1; i <= user.x + 1; i++) {
		for (j = user.y - 1; j <= user.y + 1; j++) {
			MAP[i][j] = MAZE[i][j];
		}
	}
	if (MAP[user.x][user.y - 1] == '1') {
		num++;
	}
	if (MAP[user.x][user.y + 1] == '1') {
		num++;
	}
	if (MAP[user.x - 1][user.y] == '1') {
		num++;
	}
	if (MAP[user.x + 1][user.y] == '1') {
		num++;
	}
	/*
	if (num >= 3) {
		for (i = user.x - 1; i <= user.x + 1; i++) {
			for (j = user.y - 1; j <= user.y + 1; j++) {
				MAP[i][j] = MAZE[i][j] = '1';
			}
		}
	}*/
}

int Calculate()
{
	up = down = left = right = 0;
	if (user.x == dest.x) {
		up = down = 0;
		if (user.y == dest.y) {
			puts("Mission Complete!");
			return 0;
		}
		else if (user.y > dest.y) {
			if (MAP[user.x][user.y - 1] != '1') {
				left = 1;
			}
		}
		else if (user.y < dest.y) {
			right = 1;
		}
	}
	else if (user.x > dest.x) {
		up = 1;
		if (user.y > dest.y) {
			left = 1;
		}
		else if (user.y < dest.y) {
			right = 1;
		}
	}
	else if (user.x < dest.x) {
		down = 1;
		if (user.y > dest.y) {
			left = 1;
		}
		else if (user.y < dest.y) {
			right = 1;
		}
	}
	return 1;
}

int Move()
{
	while (right > 0) {
		if (MAP[user.x][user.y + 1] != '1') {
			user.y += 2;
			right--;
		}
		else {
			if (MAP[user.x + 1][user.y] != '1') {
				user.x += 2;
				down--;
			}
			else if (MAP[user.x - 1][user.y] != '1') {
				user.x -= 2;
				up--;
			}
			else {
				user.y -= 2;
				left--;
			}
		}
		Search();
		ShowMap();
	}
	while (left > 0) {
		if (MAP[user.x][user.y - 1] != '1') {
			user.y -= 2;
			left--;
		}
		else {
			if (MAP[user.x + 1][user.y] != '1') {
				user.x += 2;
				down--;
			}
			else if (MAP[user.x - 1][user.y] != '1') {
				user.x -= 2;
				up--;
			}
			else {
				user.y += 2;
				right--;
			}
		}
		Search();
		ShowMap();
	}
	while (down > 0) {
		if (MAP[user.x + 1][user.y] != '1') {
			user.x += 2;
			down--;
		}
		else {
			if (MAP[user.x][user.y + 1] != '1') {
				user.y += 2;
				right--;
			}
			else if (MAP[user.x][user.y - 1] != '1') {
				user.y -= 2;
				left--;
			}
			else {
				user.x -= 2;
				up--;
			}
		}
		Search();
		ShowMap();
	}
	while (up > 0) {
		if (MAP[user.x - 1][user.y] != '1') {
			user.x -= 2;
			up--;
		}
		else {
			if (MAP[user.x][user.y + 1] != '1') {
				user.y += 2;
				right--;
			}
			else if (MAP[user.x][user.y - 1] != '1') {
				user.y -= 2;
				left--;
			}
			else {
				user.x += 2;
				down--;
			}
		}
		Search();
		ShowMap();
	}
	return 0;
}

void TurnMap(dir_t dir)
{
	int i, j;
	point_t temp;

	switch (dir) {
	case LEFT:
		for (i = 1; i < 10; i++) {
			for (j = 1; j < 10; j++) {
				MAP2[10 - j][i] = MAP[i][j];
				MAZE2[10 - j][i] = MAZE[i][j];
			}
		}
		temp.x = 10 - user.y;
		temp.y = user.x;
		user = temp;
		break;
	case RIGHT:
		for (i = 1; i < 10; i++) {
			for (j = 1; j < 10; j++) {
				MAP2[j][10 - i] = MAP[i][j];
				MAZE2[j][10 - i] = MAZE[i][j];
			}
		}
		temp.x = user.y;
		temp.y = 10 - user.x;
		user = temp;
		break;
	}

	for (i = 1; i < 10; i++) {
		for (j = 1; j < 10; j++) {
			MAP[i][j] = MAP2[i][j];
			MAZE[i][j] = MAZE2[i][j];
		}
	}
}
