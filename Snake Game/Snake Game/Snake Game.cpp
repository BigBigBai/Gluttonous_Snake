// Snake Game.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
using namespace std;

typedef struct { int x, y; }Point;

char map[22][22];					//define a 22 * 22 map (include the border)
Point snake[400], food, Next;		//define Snake, Food, Next Position of Snake Head
int head, tail;						//store the coordinate of Head and Tail
int grade, length, autotime;		//Game level, Snake Length, Time for auto-movement
char direction;						//Direction of movement

									//用inline定义内联函数节省程序运行时的调用开销
									//Use inline to define functions to save the cost
									//Refresh the map
inline void Update(char map[][22], int grade, int length, int autotime)
{
	system("cls");					//Clear the Console
	int i, j;
	printf("\n");
	for (i = 0; i < 22; i++)
	{
		printf("\t");
		for (j = 0; j < 22; j++)
			printf("%c", map[i][j]);
		if (i == 0)
			printf("\tGrade: %d", grade);
		if (i == 2)
			printf("\tSnake Length: %d", length);
		if (i == 6)
			printf("\tAutoMovement for");
		if (i == 8)
			printf("\tIntervel between Time: %d ms", autotime);
		printf("\n");
	}
}

//Welcome Interface
inline void hello()
{
	puts("\n\n\n\t\t\tGluttonous Snake will begin!");		//prepare to begin
	double start;
	for (int i = 3; i >= 0; i--)
	{
		start = (double)clock() / CLOCKS_PER_SEC;			//Get Current time for game
		while ((double)clock() / CLOCKS_PER_SEC - start <= 1);		//After 1 second
		if (i > 0)
		{
			system("cls");
			printf("\n\n\n\t\t\tCount Down: %d\n", i);		//Count Down			
		}
		else
			Update(map, grade, length, autotime);			//Refresh Map
	}
}

//Randomly Create Food	
inline void f()
{
	srand(int(time(0)));								//Call Seed Function
	do {
		food.x = rand() % 20 + 1;
		food.y = rand() % 20 + 1;
	} while (map[food.x][food.y] != ' ');
	map[food.x][food.y] = '!';							//Food is "!"
}

//Initialize
inline void init()
{
	int i, j;
	for (i = 1; i <= 20; i++)
		for (j = 1; j <= 20; j++)
			map[i][j] = ' ';
	for (i = 0; i <= 21; i++)
		map[0][i] = map[21][i] = map[i][0] = map[i][21] = '*';		//Border
	map[1][1] = map[1][2] = 'O';									//Snake Body(include tail)
	map[1][3] = '@';												//Snake Head
	head = 2; tail = 0;					//Subscript for Head and Tail when start
	snake[head].x = 1; snake[head].y = 3;	//Subscript for Head on map when start
	snake[tail].x = 1; snake[tail].y = 1;	//Subscript for Tail on map when start
	snake[1].x = 1; snake[1].y = 2;			//Subscript for body on map when start
	f();									//Random position for food
	grade = 1; length = 3; autotime = 500;	//Level,Length,Auto-movement Time when start
	direction = 77;							//initial direction is right
}

//Pre-movement
inline int GO()
{
	bool timeover = true;
	double start = (double)clock() / CLOCKS_PER_SEC;		//Get Current Time for Game

															//Auto move in 1 second or Wait for keyboard input in 1 second
	while ((timeover = ((double)clock() / CLOCKS_PER_SEC - start <= autotime / 1000.0)) && !_kbhit());
	//Keyboard input
	if (timeover)
	{
		_getch();
		direction = _getch();					//Get direction
	}
	switch (direction)
	{
	case 72:
		Next.x = snake[head].x - 1; Next.y = snake[head].y;		//Upward
		break;
	case 80:
		Next.x = snake[head].x + 1; Next.y = snake[head].y;		//Downward
		break;
	case 75:
		Next.x = snake[head].x; Next.y = snake[head].y - 1;		//left
		break;
	case 77:
		Next.x = snake[head].x; Next.y = snake[head].y + 1;		//right
		break;
	default:
		puts("\tGame over!");									//Fail if press non-directional keyboard
		return 0;
	}
	if (Next.x == 0 || Next.x == 21 || Next.y == 0 || Next.y == 21)	//Fail if Touch the border
	{
		puts("\tGame over!");
		return 0;
	}
	if (map[Next.x][Next.y] != ' ' && !(Next.x == food.x && Next.y == food.y))	//Fail if Touch the body
	{
		puts("\tGame over!");
		return 0;
	}
	if (length == 400)												//Max Snake Length
	{
		puts("\tGame over!");
		return 0;
	}
	return 1;
}

//Successfully Eat Food
inline void EAT()
{
	length++;								//Length increase by 1
	int _grade = length / 10 + 1;			//Calculate the level
	if (_grade != grade)
	{
		grade = _grade;
		if (autotime >= 100)
			autotime = 550 - grade * 50;	//Increase 1 level will decrease 50ms
	}
	map[Next.x][Next.y] = '@';				//Change of Snake Head 
	map[snake[head].x][snake[head].y] = 'O';	//Original Head change to Body
	head = (head + 1) % 400;					//Subscript of Head increase by 1
	snake[head].x = Next.x; snake[head].y = Next.y;  //Subscript of Head Change
	f();											 //Random food position
	Update(map, grade, length, autotime);			 //Refresh map
}

//Unsuccessfully Eat Food
inline void FAILURE()
{
	map[snake[tail].x][snake[tail].y] = ' ';	//Original Tail Position becomes " "
	tail = (tail + 1) % 400;					//Subscript of Tail increase by 1
	map[Next.x][Next.y] = '@';					//Change of Snake Head
	map[snake[head].x][snake[head].y] = 'O';	//Original Head change to Body
	head = (head + 1) % 400;					//Subscript of Head increase by 1
	snake[head].x = Next.x; snake[head].y = Next.y;  //Subscript of Head Change
	Update(map, grade, length, autotime);			 //Refresh map
}

//main function
int main()
{
	init();			//Initialize
	hello();		//Welcome Interface
	while (1)
	{
		if (GO())	//Pre-movement
		{
			if (Next.x == food.x && Next.y == food.y)
				EAT();				//Successfully Eat Food
			else
				FAILURE();			//Unsuccessfully Eat Food
		}
		else
			return 0;				//success or fail, game over
	}
	return 0;
}

