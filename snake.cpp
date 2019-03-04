#include<ctime>
#include<cstdlib>
#include<thread>
#include"snake.h"


int main(){
	 thread t(print_snake);
    t.detach();
    srand((unsigned)time(NULL));
    Snake snake;
	print_map();
    
    snake.initMap();
    snake.start();
    system("pause");

    return 0;

}