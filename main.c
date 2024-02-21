#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <math.h>


#define width 65
#define height 25

typedef struct {
    int x,y;
    int w;
} TPlayer;
typedef struct {
    float x,y;
    int ix, iy;
    float alfa;
    float speed;
}TBall;
char gameFeild[height][width+1];
TPlayer player;
TBall ball;
int hitCount;
int max_hitCount;

void initBall(){
    moveBall(2,2);
    ball.alfa = -1;
    ball.speed = 0.5;
}
void putBall(){
    gameFeild[ball.iy][ball.ix] = '*';
}
void moveBall(float x, float y){
    ball.x = x;
    ball.y = y;
    ball.ix = (int)round(ball.x);
    ball.iy = (int)round(ball.y);
}
void autoMoveBall(){
    if (ball.alfa < 0) ball.alfa += M_PI*2;
    if (ball.alfa > M_PI*2) ball.alfa -= M_PI*2;

    TBall bl = ball;

    moveBall(ball.x + cos(ball.alfa) * ball.speed,
             ball.y + sin(ball.alfa) * ball.speed);

    if((gameFeild[ball.iy][ball.ix] == '#') ||
       (gameFeild[ball.iy][ball.ix] == '@')){
           if (gameFeild[ball.iy][ball.ix] == '@')
                hitCount++;
           if ((ball.ix != bl.ix) && (ball.iy != bl.iy)){
                if(gameFeild[bl.iy][ball.ix] == gameFeild[ball.iy][bl.ix])
                    bl.alfa = bl.alfa + M_PI;
                else{
                    if (gameFeild[bl.iy][ball.ix] == '#')
                        bl.alfa = (2*M_PI - bl.alfa) + M_PI;
                    else
                        bl.alfa = (2*M_PI - bl.alfa);
                }
                          }
           else if (ball.iy == bl.iy)
                bl.alfa = (2*M_PI - bl.alfa) + M_PI;
           else
                bl.alfa = (2*M_PI - bl.alfa);

            ball = bl;
            autoMoveBall();
       }

}
void intitPlayer(){
    player.w = 7;
    player.x = (width - player.w) / 2;
    player.y = height - 1;
}
void putPlayer(){
    for(int i=player.x; i < player.x + player.w; i++)
        gameFeild[player.y][i] = '@';
}

void movePlayer(int x){
    player.x = x;
    if(player.x < 1)
        player.x = 1;
    if(player.x + player.w >= width)
        player.x = width - player.w - 1;
}

void init(){
    for(int i=0; i<width; i++)
        gameFeild[0][i] = '#';

    gameFeild[0][width] = '\0'; //ƒобавление в конец символ конца строки дл€ вывола

    //  опируем первую строку во вторую
    strncpy(gameFeild[1], gameFeild[0], width);

    //вставл€ем символы пробела
    for(int i=1; i<width-1; i++)
        gameFeild[1][i] = ' ';

    // копируем 2 строку во все последующии
    for(int i=2; i<height; i++)
        strncpy(gameFeild[i],gameFeild[1], width);
}
void show(){
    for(int i = 0; i < height; i++){
        //setcur(10,i);
        printf("%s", gameFeild[i]);
        if(i == 3)
            printf("\tmax %i", max_hitCount);
        if(i == 4)
            printf("\thit %i", hitCount);
        if(i < height - 1)
            printf("\n");
    }
}

void setcur (int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main(){
    char c;
    BOOL run = FALSE;
    intitPlayer();
    initBall();
    //movePlayer(100);
    do{
        //system("cls");
        setcur(0,0);
        if(run)
            autoMoveBall();
        if(ball.y >= height){
            run = FALSE;
            if(hitCount > max_hitCount)
                max_hitCount = hitCount;
            hitCount = 0;
        }
        init();
        putPlayer();
        putBall();
        show();
        if (GetKeyState('A') < 0) movePlayer(player.x - 1);
        if (GetKeyState('D') < 0) movePlayer(player.x + 1);
        if (GetKeyState('W') < 0) run = TRUE;
        if(!run)
            moveBall(player.x + player.w / 2, player.y - 1);
        Sleep(10);
    }while(GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
