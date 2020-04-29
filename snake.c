#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAXHEIGHT 20 //地图高度
#define MAXWIDTH 60  //地图长度

//蛇的一些基本属性
typedef struct Snake
{
    struct snakenode *xiaobai_head; //链表头节点节点
    int where;                      //1向上，2向右，3向下，4向左
    int where_last;                 //上次方向
} SNAKE, *PSNAKE;

//记录蛇的各个节点位置(用链表实现)
typedef struct snakenode
{
    int x;
    int y;
    struct snakenode *next;
} SNAKENODE, *PSNAKENODE;

//记录果实位置
typedef struct Fruit
{
    int fruit_x;
    int fruit_y;
    int symbol;
} FRUIT;

//地图
typedef enum
{
    level,      //'-'
    vertical,   //'|'
    blank,      //' '
    snakebody,  //'*'
    fruitwhere, //果实,样式随机
} MAP;

MAP map[MAXHEIGHT][MAXWIDTH]; //地图
FRUIT fruit;                  //果实
SNAKE snake;                  //蛇
int num = 0;                  //分数

void begin_map();                  //绘制开始画面
void drawing();                    //绘制运行
void clear_buffer();               //缓冲区释放无效字符
void fruit_rand();                 //生成果实随机位置和样式数
void fruit_print();                //打印果实符号
void draw_map();                   //绘制地图
void remove_snake();               //蛇移动
void in_thing(int in_y, int in_x); //蛇遇到不同事物发生的可能性
void drawing_go();                 //绘制当前map的地图

int main(void)
{
    begin_map();
    drawing();
    draw_map();
    return 0;
}

void begin_map()
{
    fruit.symbol = -1; //设置果实样式
    int choice;        //选择
    printf("_________________________________________________________________________________\n");
    printf("|                                                                                |\n");
    printf("|                                   贪吃蛇                                       |\n");
    printf("|                                  (1)新游戏                                     |\n");
    printf("|                                  (2)退出                                       |\n");
    printf("|                                  (3)关于我们                                   |\n");
    printf("|                                                                                |\n");
    printf("==================================================================================\n");
    printf("请输入您的选择：");
    //消除错误输入
    if (scanf("%d", &choice) != 1)
    {
        clear_buffer(); //消除多余缓冲区元素
        system("cls");
        begin_map();
    }
    else
    {
        clear_buffer();
        if (choice == 1)
        {
            system("cls");
            return;
        }
        else if (choice == 2)
        {
            exit(-1);
        }
        else if (choice == 3)
        {
            system("cls");
            printf("版本：1.0\n");
            printf("创作者：Mr.Yun\n");
            printf("输入任意键返回\n");
            getchar();
            system("cls");
            begin_map();
        }
        else
        {
            printf("请输入正确数字！（输入任意键继续）");
            getchar();
            system("cls");
            begin_map();
        }
    }
}

//缓冲区释放无效字符
void clear_buffer()
{
    while (getchar() != '\n')
    {
    }
}

//0为‘-’，1为‘|’，2为‘ ’,3为蛇，4为果实
void drawing()
{
    snake.xiaobai_head = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next->next->next = NULL;
    snake.xiaobai_head->next->x = MAXWIDTH / 2;  //蛇头X位置定位
    snake.xiaobai_head->next->y = MAXHEIGHT / 2; //蛇头Y位置定位
    snake.xiaobai_head->next->next->x = (MAXWIDTH / 2) - 1;
    snake.xiaobai_head->next->next->y = MAXHEIGHT / 2;
    snake.xiaobai_head->next->next->next->x = (MAXWIDTH / 2) - 2;
    snake.xiaobai_head->next->next->next->y = MAXHEIGHT / 2;
    snake.where = 2; //蛇位移方向初始化
    snake.where_last = 2;
    //检查蛇位置是否与果实位置重合
    do
    {
        fruit_rand();
    } while (snake.xiaobai_head->next->y == fruit.fruit_y && fruit.fruit_x < snake.xiaobai_head->next->x && fruit.fruit_x > snake.xiaobai_head->next->next->x);
    for (int i = 0; i < MAXHEIGHT; i++)
    {
        for (int a = 0; a < MAXWIDTH; a++)
        {
            if (i == 0 || i == 19)
            {
                map[i][a] = 0;
                printf("-");
            }
            else if (a == 0 || a == 59)
            {
                map[i][a] = 1;
                printf("|");
            }
            else if (fruit.fruit_x == a && fruit.fruit_y == i)
            {
                map[i][a] = 4;
                fruit_print();
            }
            else if (!(i == snake.xiaobai_head->next->y && a <= snake.xiaobai_head->next->x && a >= snake.xiaobai_head->next->next->next->x))
            {
                map[i][a] = 2;
                printf(" ");
            }
            else
            {
                map[i][a] = 3;
                printf("*");
            }
        }
        printf("\n");
    }
    printf("分数：%d", num);
    return;
}

//生成果实随机位置和样式
void fruit_rand()
{
    //这是一个很垃圾的检查果实位置随机生成是否合法的方式
    do
    {
        srand(time(NULL));
        fruit.fruit_x = rand() % (MAXWIDTH - 2) + 1;
        srand(time(NULL));
        fruit.fruit_y = rand() % (MAXHEIGHT - 2) + 1;
        fruit.symbol = (fruit.symbol + 1) % 6;
    } while (map[fruit.fruit_y][fruit.fruit_x] == snakebody);
    map[fruit.fruit_y][fruit.fruit_x]=4;
    return;
}

//打印果实符号
void fruit_print()
{
    switch (fruit.symbol)
    {
    case 0:
        printf("@");
        break;
    case 1:
        printf("#");
        break;
    case 2:
        printf("$");
        break;
    case 3:
        printf("+");
        break;
    case 4:
        printf("V");
        break;
    default:
        printf("&");
        break;
    }
    return;
}

//绘制地图，撞墙或者吃到自己结束
void draw_map()
{
    _Bool running = 1;
    int input;
    int time1, time2;
    time1 = clock();
    while (running)
    {
        time2 = clock();
        //0.8s内可改变方向
        while (((time2 - time1) / CLOCKS_PER_SEC) < 0.3)
        {
            if (kbhit()) //上下左右是双键值
            {
                input = getch();
                input = getch();
                //上键1，右键2，下键3，左键4
                switch (input)
                {
                case 72:
                    snake.where = 1;
                    break;
                case 77:
                    snake.where = 2;
                    break;
                case 80:
                    snake.where = 3;
                    break;
                case 75:
                    snake.where = 4;
                    break;
                default:
                    break;
                }
            }
            time2 = clock();
        }
        //与上次值不相反判断
        if (abs(snake.where_last - snake.where) != 2)
        {
            snake.where_last = snake.where;
            remove_snake();
        }
        else
        {
            snake.where = snake.where_last;
            remove_snake();
        }
        //一段时间后重新开始计时
        //绘制地图
        drawing_go();
        time1 = clock(); //更新时间
    }
}

//蛇移动
void remove_snake()
{
    switch (snake.where)
    {
    case 1:
        in_thing(snake.xiaobai_head->next->y - 1, snake.xiaobai_head->next->x);
        break;
    case 2:
        in_thing(snake.xiaobai_head->next->y, snake.xiaobai_head->next->x + 1);
        break;
    case 3:
        in_thing(snake.xiaobai_head->next->y + 1, snake.xiaobai_head->next->x);
        break;
    case 4:
        in_thing(snake.xiaobai_head->next->y, snake.xiaobai_head->next->x - 1);
        break;
    default:
        break;
    }
}

//需简化
//蛇遇到不同事物发生的可能性,传入蛇头将要到达地址
void in_thing(int in_y, int in_x)
{
    PSNAKENODE last = snake.xiaobai_head; //指向最后二节点
    //指向倒数第二节点
    while (last->next->next != NULL)
    {
        last = last->next;
    }
    //遇到空白（释放蛇尾，头插入一个首节点）
    //为蛇尾位置（蛇尾位置减一，头向前移动1格）
    if ((map[in_y][in_x] == 2) || (last->next->x == in_x && last->next->y == in_y))
    {
        //头插入一个新节点
        PSNAKENODE two = (PSNAKENODE)malloc(sizeof(SNAKENODE));
        two->next = snake.xiaobai_head->next;
        snake.xiaobai_head->next = two;
        two->x = in_x; //赋值
        two->y = in_y; //赋值
        //地图信息修改
        map[last->next->y][last->next->x] = 2;
        map[in_y][in_x] = 3;
        //释放最后一个节点
        free(last->next);
        last->next = NULL;
    }
    //遇到果实（头插入尾不变），刷新
    else if (map[in_y][in_x] == 4)
    {
        PSNAKENODE two = (PSNAKENODE)malloc(sizeof(SNAKENODE));
        two->x = in_x; //赋值
        two->y = in_y; //赋值
        two->next = snake.xiaobai_head->next;
        snake.xiaobai_head->next = two;
        map[in_y][in_x] = 3;
        num++; //分数增加
        //生成一个随机果实
        do
        {
            fruit_rand();
        } while (snake.xiaobai_head->next->y == fruit.fruit_y && fruit.fruit_x < snake.xiaobai_head->next->x && fruit.fruit_x > snake.xiaobai_head->next->next->x);
    }
    //碰墙壁或者吃到自己
    else
    {
        exit(-1); //游戏结束
    }
}

//绘制当前map的地图
void drawing_go()
{
    system("cls");
    for (int map_y = 0; map_y < MAXHEIGHT; map_y++)
    {
        for (int map_x = 0; map_x < MAXWIDTH; map_x++)
        {
            switch (map[map_y][map_x])
            {
            case 2:
                
                printf(" ");
                break;
            case 0:
                printf("-");
                break;
            case 1:
                printf("|");
                break;
            case 3:
                printf("*");
                break;
            default:
                fruit_print();
                break;
            }
        }
        printf("\n");
    }
    printf("分数：%d", num);
    return;
}