#include <cstdio>
#include <conio.h>
#include <iostream>
#include <cstdlib>
using namespace std;
const int ONE_SECOND = 1e4;
const int N = 10 + 5;
int number;
void init();
void display();
void deal_with_input();
void deal_with_time();
void meet_item();
int count = 0;
int reset = 0; 
int stop;
int nemu = 0;
int cheat = 0;
char map[16][16] = {
{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
{'1',' ',' ',' ','2',' ',' ',' ',' ','1',' ',' ','2',' ',' ','1'},
{'1',' ','1',' ','2',' ','1','1',' ','2','2','2','1',' ',' ','1'},
{'1',' ',' ',' ','2',' ',' ',' ',' ','1',' ',' ','1',' ',' ','1'},
{'1','2','2','2','1','1','1','1','2','1',' ',' ','1',' ',' ','1'},
{'1',' ',' ',' ','1',' ',' ',' ',' ','2','1','2','1','1','2','1'},
{'1',' ','2',' ','1',' ',' ',' ',' ','2',' ',' ','2',' ',' ','1'},
{'1',' ','2',' ','1','2','1','1','2','1','2','2','1',' ',' ','1'},
{'1',' ',' ',' ','1',' ',' ',' ',' ','1',' ',' ','1','2','2','1'},
{'1','2','2','2','1',' ',' ',' ',' ','2','2','2','1',' ',' ','1'},
{'1',' ',' ',' ','1',' ',' ',' ',' ','2',' ',' ','2',' ',' ','1'},
{'1',' ','1',' ','1',' ',' ',' ',' ','1',' ',' ','2',' ',' ','1'},
{'1',' ','1',' ','2','2','2','2','2','1','2','1','1',' ',' ','1'},
{'1',' ',' ',' ','2',' ',' ',' ',' ','2',' ',' ','2',' ',' ','1'},
{'1',' ',' ',' ','2',' ','1','1',' ','2',' ',' ','2',' ',' ','1'},
{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
};
class Wall
{
	public:
		char kid; //  空气，1硬墙，2软墙 
		int item;// 0 没有，1加速，2威力 
		pair<int, int> location;
		bool have_player;
		char player;
		bool have_flash;
		bool have_boom;
};
Wall wall[16][16]; 
class Item
{
	public:
		int kid; 
		pair<int ,int> location;
 }; 
 
class Boom: public Item
{
	public:
		int time;
		Boom *next;
		Boom *prev;
		char owner;
		void init();
		
};
Boom *Head[N];
Boom *Tail[N];
class Player
{
	private:
    	pair<int ,int> location;
    	char symbol;
	public:
		int score;
		int live;
		int power;
		int speed;
		bool move;
		bool have_boom;
		void update_location(int type);
		Player(int x, int y, char sym)
		{
			symbol = sym;
			location = make_pair(x, y);
		}
		pair<int, int> get_location()
		{
			return location;
		}
		char get_symbol()
		{
			return symbol;
		}
		void put_Boom()
		{
			if(have_boom)
			{
				int x = location.first;
				int y = location.second;
				wall[x][y].have_boom = true;
				have_boom = false;
				Boom *node = new Boom();
				node->owner = symbol;
				node->location = location;
				node->time = 8;
				node->kid = power;
				if(Head[0] == NULL)
				{
					Head[0] = Tail[0] = node;
				}
				else
				{
					node->prev = NULL;
					node->next = Head[0];
					Head[0]->prev = node;
					Head[0] = node;
				}
				display();
			}
		}
};

class Robot: public Player
{
	public:
		bool have_way;
		void random()
		{
			pair<int, int> location = get_location();
			char symbol = get_symbol();
			int x = location.first;
			int y = location.second;
			int array[N];
			int num = 0;
			if(wall[x][y + 1].kid == ' ' && !wall[x][y + 1].have_boom)
			{
				if(!wall[x][y + 2].have_boom && !wall[x + 1][y + 1].have_boom && !wall[x - 1][y + 1].have_boom)
				{
					array[num] = 4;
					num++;
				}
			}
			if(wall[x][y - 1].kid == ' ' && !wall[x][y - 1].have_boom)
			{
				if(!wall[x][y - 2].have_boom && !wall[x + 1][y - 1].have_boom && !wall[x - 1][y - 1].have_boom)
				{
					array[num] = 3;
					num++;
				}
			}
			if(wall[x + 1][y].kid == ' ' && !wall[x + 1][y].have_boom)
			{
				if(!wall[x + 2][y].have_boom && !wall[x + 1][y + 1].have_boom && !wall[x + 1][y - 1].have_boom)
				{
					array[num] = 2;
					num++;
				}
			}
			if(wall[x - 1][y].kid == ' ' && !wall[x - 1][y].have_boom)
			{
				if(!wall[x - 2][y].have_boom && !wall[x - 1][y + 1].have_boom && !wall[x - 1][y - 1].have_boom)
				{
					array[num] = 1;
					num++;
				}
			}
			if(num != 0)
			{
				int a = rand() % num;
				update_location(array[a]);
			}
		    else
		    {
		    	if(wall[x][y + 1].have_boom || wall[x][y - 1].have_boom || wall[x + 1][y].have_boom || wall[x - 1][y].have_boom)
		    	{
		    		Boom *node = new Boom();
					node->owner = symbol;
					node->time = 3;
					have_way = true;
					if(y < 13 && wall[x][y + 1].have_boom && wall[x][y + 3].kid == ' ' && wall[x][y + 2].kid == ' ')
					{
						node->kid = 4;
					}
					if(y > 2 && wall[x][y - 1].have_boom && wall[x][y - 3].kid == ' ' && wall[x][y + 2].kid == ' ')
					{
						node->kid = 3;
					}
					if(x < 13 && wall[x + 1][y].have_boom && wall[x + 3][y].kid == ' ' && wall[x + 2][y].kid == ' ')
					{
						node->kid = 2;
					}
					if(x > 2 && wall[x - 1][y].have_boom && wall[x - 3][y].kid == ' ' && wall[x - 2][y].kid == ' ')
					{
						node->kid = 1;
					}
					if(Head[3] == NULL)
					{
						Head[3] = Tail[3] = node;
					}
					else
					{
						node->prev = NULL;
						node->next = Head[3];
						Head[3]->prev = node;
						Head[3] = node;
					}
				}
			}
			put_Boom();
		}
		Robot(int x, int y, char symbol) : Player(x, y, symbol){}
	
 }; 
Player player1 = Player(1, 1, '+');
Player player2 = Player(1, 14, '-');
Robot robot1 = Robot(14, 1, '*');
Robot robot2 = Robot(14, 14, '/');
void Player::update_location(int type)
{
	int x = location.first;
	int y = location.second; 
	if(!move)
	{
		return;
	}
	if(type == 1 && wall[x - 1][y].kid == ' ')
	{
		move = false;
		x--;
		location = make_pair(x, y);
		wall[x + 1][y].have_player = false;
		wall[x][y].have_player = true;
		wall[x][y].player = symbol;
		display();
	}
	else if(type == 2 && wall[x + 1][y].kid == ' ')
	{
		move = false;
		x++;
		location = make_pair(x, y);
		wall[x - 1][y].have_player = false;
		wall[x][y].have_player = true;
		wall[x][y].player = symbol;
		display();
	}
	else if(type == 3 && wall[x][y - 1].kid == ' ')
	{
		move = false;
		y--;
		location = make_pair(x, y);
		wall[x][y + 1].have_player = false;
		wall[x][y].have_player = true;
		wall[x][y].player = symbol;
		display();
	}
	else if(type == 4 && wall[x][y + 1].kid == ' ')
	{
		move = false;
		y++;
		location = make_pair(x, y);
		wall[x][y - 1].have_player = false;
		wall[x][y].have_player = true;
		wall[x][y].player = symbol;
		display();
	}
	if(!move)
	{
		Boom *node = new Boom();
		node->owner = symbol;
		node->time = speed;
		if(Head[1] == NULL)
		{
			Head[1] = Tail[1] = node;
		}
		else
		{
			node->prev = NULL;
			node->next = Head[1];
			Head[1]->prev = node;
			Head[1] = node;
		}
		if(wall[x][y].item != 0)
		{
			Boom *node = new Boom();
			node->owner = symbol;
			node->time = 6;
			node->kid = wall[x][y].item;
			wall[x][y].item = 0;
			if(node->owner == '+')
			{
				if(node->kid == 1)
				{
					player1.speed = 1;
				}
				else if(node->kid == 2)
				{
					player1.power = 2;
				}
			}
			else if(node->owner == '-')
			{
				if(node->kid == 1)
				{
					player2.speed = 1;
				}
				else if(node->kid == 2)
				{
					player2.power = 2;
	    		}
			}
			else if(node->owner == '*')
			{
				if(node->kid == 1)
				{
			    	robot1.speed = 1;
				}
				else if(node->kid == 2)
				{
					robot1.power = 2;
				}
			}
			else if(node->owner == '/')
			{
				if(node->kid == 1)
				{
					robot2.speed = 1;
		    	}
				else if(node->kid == 2)
				{
					robot2.power = 2;
				}
			}
			if(Head[2] == NULL)
			{
				Head[2] = Tail[2] = node;
			}
			else
			{
				node->prev = NULL;
				node->next = Head[2];
				Head[2]->prev = node;
				Head[2] = node;
			}
		}
	}
}
void meet_flash(Boom *node, int direct, int distance)
{
	pair<int, int> location = node->location;
	int x;
	int y;
	if(direct == 0)
	{
		x = location.first;
		y = location.second + distance;
	}
	else if(direct == 1)
	{
		x = location.first;
		y = location.second - distance;
	}
	else if(direct == 2)
	{
		x = location.first + distance;
		y = location.second;
	}
	else if(direct == 3)
	{
		x = location.first - distance;
		y = location.second;
	}
	if(wall[x][y].have_player)
	{
		if(node->owner == '+')
		{
			player1.score++;
		}
		else if(node->owner == '-')
		{
			player2.score++;
		}
		else if(node->owner == '*')
		{
			robot1.score++;
		}
		else if(node->owner == '/')
		{
			robot2.score++;
		}
		if(wall[x][y].player == '+')
		{
			player1.live--;
			if(player1.live == 0)
			{
				wall[x][y].have_player = false;
				number--;
			}
		}
		else if(wall[x][y].player == '-')
		{
			player2.live--;
			if(player2.live == 0)
			{
				wall[x][y].have_player = false;
				number--;
			}
		}
		else if(wall[x][y].player == '*')
		{
			robot1.live--;
			if(robot1.live == 0)
			{
				wall[x][y].have_player = false;
				number--;
			}
		}
		else if(wall[x][y].player == '/')
		{
			robot2.live--;
			if(robot2.live == 0)
			{
				wall[x][y].have_player = false;
				number--;
			}
		}
		display();	
	} 
}
void Boom_explode(Boom *node)
{
	int x = node->location.first;
	int y = node->location.second;
	int t = y;
	int power = node->kid;
	wall[x][y].have_boom = false;
	wall[x][y].have_flash = true;
	meet_flash(node, 0, 0);
	for(int i = 0; i < power; i++)
	{
		t++;
		if(wall[x][t].kid == '1')
		{
			break;
		}
		wall[x][t].have_flash = true;
		wall[x][t].kid = ' ';
		meet_flash(node, 0, t - y);
	}
	t = y;
	for(int i = 0; i < power; i++)
	{
		t--;
		if(wall[x][t].kid == '1')
		{
			break;
		}
		wall[x][t].have_flash = true;
		wall[x][t].kid = ' ';
		meet_flash(node, 1, y - t);
	}
	t = x;
	for(int i = 0; i < power; i++)
	{
		t++;
		if(wall[t][y].kid == '1')
		{
			break;
		}
		wall[t][y].have_flash = true;
		wall[t][y].kid = ' ';
		meet_flash(node, 2, t - x);
	}
	t = x;
	for(int i = 0; i < power; i++)
	{
		t--;
		if(wall[t][y].kid == '1')
		{
			break;
		}
		wall[t][y].have_flash = true;
		wall[t][y].kid = ' ';
		meet_flash(node, 3, x - t);
	}
	if(node->owner == '+')
	{
		player1.have_boom = true;
	}
	else if(node->owner == '-')
	{
		player2.have_boom = true;
	}
	else if(node->owner == '*')
	{
		robot1.have_boom = true;
	}
	else if(node->owner == '/')
	{
		robot2.have_boom = true;
	}
	display();
}
void flash_loss(Boom *node)
{	
	int x = node->location.first;
	int y = node->location.second;
	int power = node->kid;
	wall[x][y].have_flash = false;
	for(int i = 1; i <= power; i++)
	{
		if(wall[x][y + i].have_flash)
		{
			wall[x][y + i].have_flash = false;
		}
		if(wall[x][y - i].have_flash)
		{
			wall[x][y - i].have_flash = false;
		}
		if(wall[x + i][y].have_flash)
		{
			wall[x + i][y].have_flash = false;
		}
		if(wall[x - i][y].have_flash)
		{
			wall[x - i][y].have_flash = false;
		}
	}
	display();
}
void display()
{
	system("cls");
	if(nemu == 0){
		printf("按z键开始游戏\n");
		return;
	}else if(nemu == 1){
		printf("操作指南:\n");
		printf("作弊模式:x\n");
		printf("暂停游戏:c\n");
		printf("重新开始:q\n");
		printf("退出游戏:p\n");
		printf("\n");
		printf("player1:\n");
		printf("上:w 下:s 左:a 右:d\n");
		printf("放置炸弹:space\n");
		printf("player2:\n");
		printf("上:i 下:k 左:j 右:l\n");
		printf("放置炸弹:Enter\n");
		printf("按z继续\n");
		return;
	}
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			if(player1.get_location() == make_pair(i, j) && player1.live > 0)
			{
				cout << '+';
			}
			else if(player2.get_location() == make_pair(i, j) && player2.live > 0)
			{
				cout << '-';
			}
			else if(robot1.get_location() == make_pair(i, j) && robot1.live > 0)
			{
				cout << '*';
			}
			else if(robot2.get_location() == make_pair(i, j) && robot2.live > 0)
			{
				cout << '/';
			}
			else if(wall[i][j].have_boom)
			{
				cout << 'Q';
			}
			else if(wall[i][j].have_flash)
			{
				cout << '|';
			}
			else if(wall[i][j].kid != ' ')
			{
				cout << wall[i][j].kid;
			}
			else 
			{
				if(wall[i][j].item == 1)
				{
					cout << 'S';
				}
				else if(wall[i][j].item == 2)
				{
					cout << 'P';
				}
				else
				{
					cout << ' ';
				}
			}
			
		}
		printf("\n");
	}
	if(number > 1)
	{
		printf("生命显示:\n");
		printf("  player1:%d     player2:%d\n", player1.live, player2.live);
		printf("  robot1:%d     robot2:%d\n", robot1.live, robot2.live);	
		printf("得分显示:\n");
		printf("  player1:%d     player2:%d\n", player1.score, player2.score);
		printf("  robot1:%d     robot2:%d\n", robot1.score, robot2.score);
		if(cheat != 0){
			if(cheat == 3){
				printf("请输入作弊码:\n");
			}else{
				if(cheat == 2){
					stop = 0;
					player1.live = 99;
					player2.live = 99;
				}
				cheat--;
				printf("作弊模式已开启！！！\n");
			}
		}
		else if(reset != 0){
			printf("游戏已重置！！！\n");
			reset--; 
		}
		else if(stop == 1){
			printf("游戏已暂停！！！按c键继续游戏\n");
		}
	}
	else 
	{
		if(player1.score >= player2.score && player1.score >= robot1.score && player1.score >= robot2.score)
		{
			printf("  player1 won!\n");
		}
		else if(player2.score >= player1.score && player2.score >= robot1.score && player2.score >= robot2.score)
		{
			printf("  player2 won!\n");
		}
		else if(robot1.score >= player1.score && robot1.score >= player2.score && robot1.score >= robot2.score)
		{
			printf("  robot1 won!\n");
		}
		else if(robot2.score >= player1.score && robot2.score >= player2.score && robot2.score >= robot1.score)
		{
			printf("  robot2 won!\n");
		}
	}
}

void init()
{
	stop = 1;
	number = 4;
	cheat = 0;
	reset = 0;
	Head[0] = Head[1] = Head[2] = NULL;
	Tail[0] = Tail[1] = Tail[2] = NULL;
	player1 = Player(1, 1, '+');
	player2 = Player(1, 14, '-');
	robot1 = Robot(14, 1, '*');
	robot2 = Robot(14, 14, '/');
	player1.live = player2.live = 3;
	player1.speed = player2.speed = 2;
	player1.power = player2.power = 1;
	player1.move = player2.move = true; 
	player1.have_boom = player2.have_boom = true;
	robot1.live = robot2.live = 3;
	robot1.speed = robot2.speed = 2;
	robot1.power = robot2.power = 1;
	robot1.move = robot2.move = true;
	robot1.have_boom = robot2.have_boom = true;
	robot1.have_way = robot2.have_way = false;
	player1.score = player2.score = robot1.score = robot2.score = 0;
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			wall[i][j].kid = map[i][j];
			wall[i][j].location = make_pair(i, j);
			wall[i][j].have_player = false;
			wall[i][j].item = 0;
			wall[i][j].have_boom = false;
			wall[i][j].have_flash = false;
			if(wall[i][j].kid == '2')
			{
				int x = 1 + rand() % 10;
				if(x <= 8)
				{
					wall[i][j].item = 0;
				}
				else if(x == 9)
				{
					wall[i][j].item = 1;
				}
				else 
				{
					wall[i][j].item = 2;
				}
			}
		}
	}
	wall[1][1].have_player = true;
	wall[1][1].player = '+';
	wall[1][14].have_player = true;
	wall[1][14].player = '-';
	wall[14][1].have_player = true;
	wall[14][1].player = '*';
	wall[14][14].have_player = true;
	wall[14][14].player = '/';
}

void deal_with_input()
{
	char ch;
	if(_kbhit())
	{
		ch = _getch();
		if(ch == 'z'){
			if(nemu == 0){
				nemu = 1;
			}else if(nemu == 1){
				nemu = 2;
			}
		}
		if(nemu == 0){
			return;
		}
		if(ch == 'p'){
			init();
			nemu = 0;
		}
		if(nemu == 1){
			return;
		}
		if(cheat != 0){
			if(ch == 'b'){
				cheat = 2;
			}
			return;
		} 
		if(ch == 'x'){
			cheat = 3;
			stop = 1;
		}
		else if(ch == 'c'){
			if(stop == 1){
				stop = 0;
			}else if(stop == 0){
				stop = 1;
			}
		}
		else if(ch == 'q'){
			init();
			reset = 2;
		}
		if(stop == 1){
			return;
		}
		if(ch == 'w' && player1.live != 0)
		{
			player1.update_location(1);	
		}
		else if(ch == 's' && player1.live != 0)
		{
			player1.update_location(2);
		}
		else if(ch == 'a' && player1.live != 0)
		{
			player1.update_location(3);
		}
		else if(ch == 'd' && player1.live != 0)
		{
			player1.update_location(4);
		}
		else if(ch == 'i' && player2.live != 0)
		{
			player2.update_location(1);
		}
		else if(ch == 'k' && player2.live != 0)
		{
			player2.update_location(2);
		}
		else if(ch == 'j' && player2.live != 0)
		{
			player2.update_location(3);
		}
		else if(ch == 'l' && player2.live != 0)
		{
			player2.update_location(4);
		}
		else if(ch == ' ' && player1.live != 0)
		{
			player1.put_Boom();
		}
		else if(ch == '\r' && player2.live != 0)
		{
			player2.put_Boom();
		}
	} 
	
}

void deal_with_time()
{
	player1.move = true;
	player2.move = true;
	robot1.move = true;
	robot2.move = true;
	if(robot1.live != 0 && !robot1.have_way)
	{
		robot1.random();
	}
	if(robot2.live != 0 && !robot2.have_way)
	{
		robot2.random();
	}
	Boom *node = new Boom();
	node = Head[0];
	while(node != NULL)
	{
		node->time--;
		node = node->next;
	}
	node = Tail[0]; 
	while(node != NULL && node->time == 0)
	{
		flash_loss(node);
		if(node->prev == NULL)
		{
			Head[0] = Tail[0] = NULL;
		}
		else
		{
			node->prev->next = NULL;
			Tail[0] = node->prev;
			node->prev == NULL;
		}
		node = node->prev;
	}
	while(node != NULL && node->time == 1)
	{
		node = node->prev;
	}
	while(node != NULL && node->time == 2)
	{
		Boom_explode(node);
		node = node->prev;
	}
	node = Head[1];
	while(node != NULL)
	{
		node->time--;
		node = node->next;
	}
	node = Tail[1]; 
	while(node != NULL && node->time == 0)
	{
		if(node->owner == '+')
		{
			player1.move = true;
		}
		else if(node->owner == '-')
		{
			player2.move = true;
		}
		else if(node->owner == '*')
		{
			robot1.move = true;
		}
		else if(node->owner == '/')
		{
			robot2.move = true;
		}
		if(node->prev == NULL)
		{
			Head[1] = Tail[1] = NULL;
		}
		else
		{
			node->prev->next = NULL;
			Tail[1] = node->prev;
			node->prev == NULL;
		}
		node = node->prev;
	}
	node = Head[2];
	while(node != NULL)
	{
		node->time--;
		node = node->next;
	}
	node = Tail[2]; 
	while(node != NULL && node->time == 0)
	{
		if(node->owner == '+')
		{
			if(node->kid == 1)
			{
				player1.speed = 2;
			}
			else if(node->kid == 2)
			{
				player1.power = 1;
			}
		}
		else if(node->owner == '-')
		{
			if(node->kid == 1)
			{
				player2.speed = 2;
			}
			else if(node->kid == 2)
			{
				player2.power = 1;
			}
		}
		else if(node->owner == '*')
		{
			if(node->kid == 1)
			{
				robot1.speed = 2;
			}
			else if(node->kid == 2)
			{
				robot1.power = 1;
			}
		}
		else if(node->owner == '/')
		{
			if(node->kid == 1)
			{
				robot2.speed = 2;
			}
			else if(node->kid == 2)
			{
				robot2.power = 1;
			}
		}
		if(node->prev == NULL)
		{
			Head[2] = Tail[2] = NULL;
		}
		else
		{
			node->prev->next = NULL;
			Tail[2] = node->prev;
			node->prev == NULL;
		}
		node = node->prev;
		node = Head[1];
		while(node != NULL)
		{
			node->time--;
			node = node->next;
		}
	}
	node = Head[3];
	while(node != NULL)
	{
		node->time--;
		if(node->owner == '*')
		{
			robot1.update_location(node->kid);
		}
		else if(node->owner == '/')
		{
			robot2.update_location(node->kid); 
		}
		node = node->next;
	}
	node = Tail[3]; 
	while(node != NULL && node->time == 0)
	{
		if(node->owner == '*')
		{
			robot1.have_way = false;
		}
		else if(node->owner == '/')
		{
			robot2.have_way = false;
		}
		if(node->prev == NULL)
		{
			Head[3] = Tail[3] = NULL;
		}
		else
		{
			node->prev->next = NULL;
			Tail[3] = node->prev;
			node->prev == NULL;
		}
		node = node->prev;
	}
}

int main()
{
	init();
	display();
	while(1)
	{	
	    deal_with_input();
		count = count + 1;
		if( count == ONE_SECOND)
		{
			if(stop == 0){
				deal_with_time();
			}else{
				display();
			}
			count = 0;
		}
	}
	return 0;
 } 
