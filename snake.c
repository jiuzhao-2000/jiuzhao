#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAXHEIGHT 20 //��ͼ�߶�
#define MAXWIDTH 60  //��ͼ����

//�ߵ�һЩ��������
typedef struct Snake
{
    struct snakenode *xiaobai_head; //����ͷ�ڵ�ڵ�
    int where;                      //1���ϣ�2���ң�3���£�4����
    int where_last;                 //�ϴη���
} SNAKE, *PSNAKE;

//��¼�ߵĸ����ڵ�λ��(������ʵ��)
typedef struct snakenode
{
    int x;
    int y;
    struct snakenode *next;
} SNAKENODE, *PSNAKENODE;

//��¼��ʵλ��
typedef struct Fruit
{
    int fruit_x;
    int fruit_y;
    int symbol;
} FRUIT;

//��ͼ
typedef enum
{
    level,      //'-'
    vertical,   //'|'
    blank,      //' '
    snakebody,  //'*'
    fruitwhere, //��ʵ,��ʽ���
} MAP;

MAP map[MAXHEIGHT][MAXWIDTH]; //��ͼ
FRUIT fruit;                  //��ʵ
SNAKE snake;                  //��
int num = 0;                  //����

void begin_map();                  //���ƿ�ʼ����
void drawing();                    //��������
void clear_buffer();               //�������ͷ���Ч�ַ�
void fruit_rand();                 //���ɹ�ʵ���λ�ú���ʽ��
void fruit_print();                //��ӡ��ʵ����
void draw_map();                   //���Ƶ�ͼ
void remove_snake();               //���ƶ�
void in_thing(int in_y, int in_x); //��������ͬ���﷢���Ŀ�����
void drawing_go();                 //���Ƶ�ǰmap�ĵ�ͼ

int main(void)
{
    begin_map();
    drawing();
    draw_map();
    return 0;
}

void begin_map()
{
    fruit.symbol = -1; //���ù�ʵ��ʽ
    int choice;        //ѡ��
    printf("_________________________________________________________________________________\n");
    printf("|                                                                                |\n");
    printf("|                                   ̰����                                       |\n");
    printf("|                                  (1)����Ϸ                                     |\n");
    printf("|                                  (2)�˳�                                       |\n");
    printf("|                                  (3)��������                                   |\n");
    printf("|                                                                                |\n");
    printf("==================================================================================\n");
    printf("����������ѡ��");
    //������������
    if (scanf("%d", &choice) != 1)
    {
        clear_buffer(); //�������໺����Ԫ��
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
            printf("�汾��1.0\n");
            printf("�����ߣ�Mr.Yun\n");
            printf("�������������\n");
            getchar();
            system("cls");
            begin_map();
        }
        else
        {
            printf("��������ȷ���֣������������������");
            getchar();
            system("cls");
            begin_map();
        }
    }
}

//�������ͷ���Ч�ַ�
void clear_buffer()
{
    while (getchar() != '\n')
    {
    }
}

//0Ϊ��-����1Ϊ��|����2Ϊ�� ��,3Ϊ�ߣ�4Ϊ��ʵ
void drawing()
{
    snake.xiaobai_head = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next->next = (PSNAKENODE)malloc(sizeof(SNAKENODE));
    snake.xiaobai_head->next->next->next->next = NULL;
    snake.xiaobai_head->next->x = MAXWIDTH / 2;  //��ͷXλ�ö�λ
    snake.xiaobai_head->next->y = MAXHEIGHT / 2; //��ͷYλ�ö�λ
    snake.xiaobai_head->next->next->x = (MAXWIDTH / 2) - 1;
    snake.xiaobai_head->next->next->y = MAXHEIGHT / 2;
    snake.xiaobai_head->next->next->next->x = (MAXWIDTH / 2) - 2;
    snake.xiaobai_head->next->next->next->y = MAXHEIGHT / 2;
    snake.where = 2; //��λ�Ʒ����ʼ��
    snake.where_last = 2;
    //�����λ���Ƿ����ʵλ���غ�
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
    printf("������%d", num);
    return;
}

//���ɹ�ʵ���λ�ú���ʽ
void fruit_rand()
{
    //����һ���������ļ���ʵλ����������Ƿ�Ϸ��ķ�ʽ
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

//��ӡ��ʵ����
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

//���Ƶ�ͼ��ײǽ���߳Ե��Լ�����
void draw_map()
{
    _Bool running = 1;
    int input;
    int time1, time2;
    time1 = clock();
    while (running)
    {
        time2 = clock();
        //0.8s�ڿɸı䷽��
        while (((time2 - time1) / CLOCKS_PER_SEC) < 0.3)
        {
            if (kbhit()) //����������˫��ֵ
            {
                input = getch();
                input = getch();
                //�ϼ�1���Ҽ�2���¼�3�����4
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
        //���ϴ�ֵ���෴�ж�
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
        //һ��ʱ������¿�ʼ��ʱ
        //���Ƶ�ͼ
        drawing_go();
        time1 = clock(); //����ʱ��
    }
}

//���ƶ�
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

//���
//��������ͬ���﷢���Ŀ�����,������ͷ��Ҫ�����ַ
void in_thing(int in_y, int in_x)
{
    PSNAKENODE last = snake.xiaobai_head; //ָ�������ڵ�
    //ָ�����ڶ��ڵ�
    while (last->next->next != NULL)
    {
        last = last->next;
    }
    //�����հף��ͷ���β��ͷ����һ���׽ڵ㣩
    //Ϊ��βλ�ã���βλ�ü�һ��ͷ��ǰ�ƶ�1��
    if ((map[in_y][in_x] == 2) || (last->next->x == in_x && last->next->y == in_y))
    {
        //ͷ����һ���½ڵ�
        PSNAKENODE two = (PSNAKENODE)malloc(sizeof(SNAKENODE));
        two->next = snake.xiaobai_head->next;
        snake.xiaobai_head->next = two;
        two->x = in_x; //��ֵ
        two->y = in_y; //��ֵ
        //��ͼ��Ϣ�޸�
        map[last->next->y][last->next->x] = 2;
        map[in_y][in_x] = 3;
        //�ͷ����һ���ڵ�
        free(last->next);
        last->next = NULL;
    }
    //������ʵ��ͷ����β���䣩��ˢ��
    else if (map[in_y][in_x] == 4)
    {
        PSNAKENODE two = (PSNAKENODE)malloc(sizeof(SNAKENODE));
        two->x = in_x; //��ֵ
        two->y = in_y; //��ֵ
        two->next = snake.xiaobai_head->next;
        snake.xiaobai_head->next = two;
        map[in_y][in_x] = 3;
        num++; //��������
        //����һ�������ʵ
        do
        {
            fruit_rand();
        } while (snake.xiaobai_head->next->y == fruit.fruit_y && fruit.fruit_x < snake.xiaobai_head->next->x && fruit.fruit_x > snake.xiaobai_head->next->next->x);
    }
    //��ǽ�ڻ��߳Ե��Լ�
    else
    {
        exit(-1); //��Ϸ����
    }
}

//���Ƶ�ǰmap�ĵ�ͼ
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
    printf("������%d", num);
    return;
}