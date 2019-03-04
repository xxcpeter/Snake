#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <sys/time.h>
#include <thread>
#include <mutex>

#define DELAY_TIME 1000
#define SCREEN 0
#define RPI true
#define RED 0
#define GREEN 2
#define BLUE 1
#define LINE 3
#define nullptr NULL

#ifndef RPI
#include <windows.h>
#endif

using namespace std;

mutex m;
int map[8][8];
//empty:0
//snake No.1 : 1
//snake No.2 : 2
//apple:3
struct pos
{
    int x, y;
};

void print_snake();

void print_map()
{
    system("clear");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << map[i][j];
        }
        cout << endl;
    }

}

class Snake
{
  public:
    class Node
    {
      public:
        pos node_pos;
        Node *next;
        int stat;
        Node(int input_x, int input_y);
        void left() { node_pos.y = (node_pos.y - 1 + 8) % 8; };
        void right() { node_pos.y = (node_pos.y + 1 + 8) % 8; };
        void up() { node_pos.x = (node_pos.x - 1 + 8) % 8; };
        void down() { node_pos.x = (node_pos.x + 1 + 8) % 8; };
        void update();
    };

    Node *head;
    //snake's direction
    int head_dir;
    int food_x;
    int food_y;
    //Direction{UP,DOWN,LEFT,RIGHT};

    Snake();
    void update();
    void setDir(int dir);
    void move();
    void start();
    void getFoodLocation();
    void createFood();
    bool isTouchFood();
    void eatFood();
    bool isGameOver();
    void initMap();
};

Snake::Node::Node(int input_x, int input_y) : node_pos({input_x, input_y}), stat(1)
{
    next = nullptr;
}

void Snake::Node::update()
{
    if (stat)
    {
        map[node_pos.x][node_pos.y] = 1;
    }
    else
    {
        map[node_pos.x][node_pos.y] = 0;
    }
}

//birth point:(0,0) direction:RIGHT
Snake::Snake()
{
    head = new Node(0, 2);
    Node *mid = new Node(0, 1);
    Node *tail = new Node(0, 0);
    head_dir = 0;
    food_x = 4;
    food_y = 4;
    head->next = mid;
    mid->next = tail;
    tail->next = nullptr;
    m.lock();
    head->update();
    mid->update();
    tail->update();
    m.unlock();
}

void Snake::start()
{
    update();
    //direction from joystick
    int dir;
    srand((unsigned)time(NULL));
    createFood();
    while (1)
    {
    	m.lock();
        dir = rand() % 4;
        switch (dir)
        {
        case 0:
        case 1:
            if (head_dir == 2 || head_dir == 3)
                head_dir = dir;
            break;
        case 2:
        case 3:
            if (head_dir == 0 || head_dir == 1)
                head_dir = dir;
            break;
        }
        move();
        if (isTouchFood())
        {
            eatFood();
            createFood();
        }
        if (SCREEN)
            print_map();
        m.unlock();
#ifdef RPI
    delay(DELAY_TIME);
#else
    Sleep(DELAY_TIME);
#endif
    }
}

void Snake::setDir(int dir)
{
    head_dir = dir;
}

void Snake::update()
{
    Node *p = head;
    do
    {
        p = p->next;
    } while (p != nullptr);
}

void Snake::move()
{
    Node *p = head;
    Node *newtail = nullptr;
    //find tail
    if (p->next != nullptr)
    {
        while (p->next->next != nullptr)
        {
            p = p->next;
        }
    }
    newtail = p;
    p = p->next;
    newtail->next = nullptr;
    p->stat = 0;
    p->update();
    p->node_pos = head->node_pos;
    p->next = head->next;
    head->next = p;

    switch (head_dir)
    {
    case 0:
        head->up();
        break;
    case 1:
        head->down();
        break;
    case 2:
        head->left();
        break;
    case 3:
        head->right();
        break;
    }

    if (isGameOver())
    {
        //cout << "game over";
        //system("pause");
    }
    head->update();
}

void Snake::getFoodLocation()
{
    srand((unsigned)time(NULL));
    food_x = rand() % 8;
    food_y = rand() % 8;
    while (map[food_x][food_y] != 0)
    {
        food_x = rand() % 8;
        food_y = rand() % 8;
    }
}

void Snake::createFood()
{
    getFoodLocation();
    map[food_x][food_y] = 3;
}

bool Snake::isTouchFood()
{
    Node *p = head;
    if (p->node_pos.x == food_x && p->node_pos.y == food_y)
    {
        return true;
    }
    return false;
}

void Snake::eatFood()
{
    Node *p = head;
    while (p->next != nullptr)
    {
        p = p->next;
    }
    Node *newnode = new Node(p->node_pos.x, p->node_pos.y);
    p->next = newnode;
    newnode->next = nullptr;
}

bool Snake::isGameOver()
{
    Node *p = head;
    if (map[p->node_pos.x][p->node_pos.y] == 1 || map[p->node_pos.x][p->node_pos.y] == 2)
    {
        return true;
    }
    return false;
}

void Snake::initMap()
{
    memset(map, 0, sizeof(map));
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void print_snake()
{
    wiringPiSetup();
    wiringPiSPISetup(0, 500000);
    
	 while(1){
	     uint8_t data[4] = {0x0, 0x0, 0x0, 0x0};
        uint8_t d[3][8] = {0};
	     m.lock();
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                switch (map[i][j])
                {
                // snake RED
                case 1:
                    d[0][i] += 0x01 << 7 - j;
                    break;
                // snake BLUE
                case 2:
                    d[2][i] += 0x01 << 7 - j;
                    break;
                // apple GREEN
            		case 3:
                	 d[1][i] += 0x01 << 7 - j;
                	 break;
            	   }
            }
        }
        //print_map();
    	  m.unlock();
        // iterate every line
        for (int i = 0; i < 8; i++)
        {
            data[RED] = ~d[0][i];
            data[GREEN] = ~d[1][i];
            data[BLUE] = ~d[2][i];
            data[LINE] = 0x01 << 7 - i;

            // send data SPI
            wiringPiSPIDataRW(0, data, sizeof(data));
            delay(2);
        }
	     
	     //delay(10);
    }
}