#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "conio.h"
#include <unistd.h>

#define password_max_length 25
#define encryption_code 3


using namespace std;

struct things   //物品结构体 编号、名字、朝代、简短介绍、价格
{
    int id;
    char name[50];
    char time[20];
    char introduce[300];
    double price;
    int type;
    struct things *next;
    struct yonghu *yonghu;

} thing1, *pp1;

struct yonghu  //  用户结构体 编号、姓名、密码、电话、邮件、积分
{
    int id;
    char name[50];
    char password[password_max_length];
    char phone[12];
    char email[30];
    int score;
    struct yonghu *next;
    struct yhthings *yhthings;
} yonghu1;

struct adm    //  管理员结构体 编号、姓名、密码、电话、邮件、积分
{
    int id;
    char name[50];
    char password[password_max_length];
    char phone[12];
    int score;
    char email[30];
    struct yhthings *yhthings;
    struct adm *next;
} adm1;

struct yhthings //用户物品结构体 物品编号、物品名称、物品原价格、物品竞拍后价格
{
    int id;
    int id1;
    char name[50];
    char time[20];
    char yonghu_name[50];
    double f_price;
    double n_price;
    struct yhthings *next;
    struct yhthings *follow;
    struct yhthings *adm;
} yhthings1;

struct times {
    int sec;
    int min;
    int hour;
    int mday;
    int mon;
    int year;
    int yday;

} time1;

struct paimairen {
    int id;
    double price;
    struct paimairen *next;
} renyuan1;

struct paimaipin {
    int id;
    int id1;
    int id2;
    char name[50];
    char yonghu_name[50];
    double f_price;
    double n_price;
    char introduce[300];
    char time[20];
    int situ;
    struct times tb;
    struct times te;
    struct paimairen *ren;
    struct paimaipin *next;

} pai1;

struct order  // 为了用户积分排名
{
    struct yonghu *pointer;
    struct order *next;
};

struct info //信息结构体
{
    int num;  // the amount of yonghu or yhthings or things
} thing_info, yhthings_info, yonghu_info, adm_info, paimaipin_info;


FILE *yonghu_file, *thing_file, *yhthings_file, *adm_file, *paimaipin_file, *log_file;
char log_message[255];
char log_message_clear[255] = {'\0'};
char password_clear[password_max_length] = {'\0'};

struct things *head = NULL, *p1, *p2;
struct yonghu *head_yonghu = NULL;
struct adm *head_adm = NULL;
struct yhthings *head_yhthings = NULL, *yhthingsp, *headc;
struct paimaipin *head_pai = NULL, *paimaipinsp;


//-------------------------------------------function list-------------------------------------------//

void copy_things(struct things *p1, struct things thing1);// 用于复制物品信息
void copy_paimaipin(struct paimaipin *p1, struct paimaipin wupin1);// 用于复制拍卖物品信息 减少代码量
void buy(struct yonghu *p);//竞购自己想要的物品
void checkpai(struct yonghu *p);//查看可以竞购的物品的相关信息
void del_things();//删除用户信息
void del_yhthings();//删除用户物品信息
void del_yonghu();//删除用户信息
void copy_yonghu(struct yonghu *p1, struct yonghu yonghu1);// 用于复制用户信息
void insert_thing(struct things thing1); //物品结构体插入
void file_open();  //用来打开文件和构建链表
void file_close();  //关闭文件
void input_thing(); //物品信息录入
void input_yonghu();//用户录入
void input_yhthings();//用户物品信息录入
void insert_adm(struct adm adm1); //管理员结构体插入
void insert_yhthings1(struct yhthings yhthings1); //将物品信息插入用户
void insert_yhthings2(struct yhthings yhthings1); //将物品信息插入管理员
void insert_paimaipin1(struct paimaipin wupin1); //将拍卖物品结构体插入
void jingpai(struct yonghu *p);//竞拍功能
void jinggou(struct yonghu *p);//竞购物品功能界面
void password_input(char *password);//以星号形式接受密码并放到地址password上
void password_yonghu();//用户ID与密码验证
void password_adm();//管理员ID与密码验证
int idc(char *id);  //id装换 char 到int
void modify_thing_adm();// 管理员修改物品信息
void modify_yonghu(struct yonghu *p1);// 用户修改自己信息
void modify_yonghu_adm();//管理员进入某一用户系统 修改用户信息 如密码
void yonghu_ui(struct yonghu *p);//学生界面
void input_adm();  //管理员录入
void adm_ui_1();//管理员界面1
void adm_ui_2();//管理员界面2
void adm_password();//管理员密码修改
void st_password_yonghu(struct yonghu *p);//用户密码修改
void paimai(struct yonghu *p);//用户拍卖物品
void pairesult(struct yonghu *p);//用户拍卖的物品的结果，并删除用户物品信息
void zuigaojia(struct paimaipin *p, struct yonghu *ppp);//获得竞购的商品结果，并完成用户的物品插入
void show_thing_with_kind(); // 根据类别查看物品 管理员功能
void skim_yhthings(struct yonghu *p);//查看所有用户物品信息
void pfree_yhthings(struct yonghu *p_yonghu, struct things *p_things, struct yhthings yhthings1);//解锁及释放用户物品内存
void skim_thing_all();//查看所有物品信息
void insert_yonghu(struct yonghu yonghu1); //用户结构体插入
void help();//帮助
void develop();//作者
void password_encryption(char *password); //密码加密
void logging(int id, char *action); //日志记录
void show_log();  //管理员显示日志
void user_register(); //用户注册

//-------------------------------------------function list-------------------------------------------//




void password_encryption(char *password) {
    for (int i = 0; i < strlen(password); ++i) {
        password[i] = (password[i] - 'a' + encryption_code) % 26 + 'a';
    }
}


void logging(int id, char *action) {
    time_t time_p;
    time(&time_p);
    //printf("%s", ctime(&timep));
    fprintf(log_file, "%sID:%-8d%s\n", ctime(&time_p), id, action);
    strcpy(log_message, log_message_clear);   //清空日志实例信息
}


void show_log() {
    fseek(log_file, 0, SEEK_SET);
    char buffer[1024] = {'\0'};
    char clear_buffer[1024] = {'\0'};
    while (!feof(log_file)) {
        strcpy(buffer, clear_buffer);
        fgets(buffer, 1024, log_file);
        if (strlen(buffer) > 0)
            cout << buffer;
    }
    printf("按任意键退出");
    getch();
    system("cls");
}


void password_input(char *password) {
    char c;
    int i = 0;
    printf("请输入密码：\n");
    while ((c = getch()) != 13 || i == 0) {
        system("cls");
        printf("请输入密码：\n");
        switch (c) {
            case 8: {
                if (i > 0) {
                    password[i - 1] = '\0';
                    i--;
                } else {
                    password[i] = '\0';
                }
                break;
            }
            default: {
                if (i < 24) {
                    password[i] = c;
                    i++;
                }
            }
        }
        for (int j = 0; j < i && j < 25; j++) {
            cout << "*";
        }
    }
}

void file_open()  //用来打开文件和构建链表
{
    struct things thing1;
    struct yonghu yonghu1;
    struct yhthings yhthings1;
    struct adm adm1;
    //struct yhthings *cp;
    int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
    log_file = fopen("logs.txt", "a+");
    if ((thing_file = fopen("things", "rb")) == NULL || (yhthings_file = fopen("yhthings", "rb")) == NULL ||
        (yonghu_file = fopen("yonghu", "rb")) == NULL || (adm_file = fopen("adm", "rb")) == NULL ||
        (paimaipin_file = fopen("paimaipin", "rb")) == NULL) //can't open the file
    {
        if ((thing_file = fopen("things", "wb")) == NULL || (yhthings_file = fopen("yhthings", "wb")) == NULL ||
            (yonghu_file = fopen("yonghu", "wb")) == NULL || (adm_file = fopen("adm", "wb")) == NULL ||
            (paimaipin_file = fopen("paimaipin", "wb")) == NULL)
            printf("\n文件打开失败");
        exit(0);
    } else  // open file successfully
    {
        fread(&thing_info, sizeof(struct info), 1, thing_file); // things
        fread(&yhthings_info, sizeof(struct info), 1, yhthings_file); // yhthings
        fread(&yonghu_info, sizeof(struct info), 1, yonghu_file); // yonghu
        fread(&adm_info, sizeof(struct info), 1, adm_file); // adm
        fread(&paimaipin_info, sizeof(struct info), 1, paimaipin_file); // paimaipin

        if (thing_info.num <= 0)//没物品数据或者没有初始化0

        {
            fclose(thing_file);
            thing_info.num = 0;
            head = NULL;
        } else {
            a1 = thing_info.num;
            thing_info.num = 0;
            do {
                fread(&thing1, sizeof(struct things), 1, thing_file);
                insert_thing(thing1);
                a1--;
            } while (a1 != 0);
            fclose(thing_file);
        }
        // 加入管理员结构
        if (adm_info.num <= 0)// 没管理员数据或没有初始化
        {
            fclose(adm_file);
            adm_info.num = 0;
            head_adm = NULL;
        } else {
            a4 = adm_info.num;
            adm_info.num = 0;
            do {
                fread(&adm1, sizeof(struct adm), 1, adm_file);
                insert_adm(adm1);
                a4--;
            } while (a4 != 0);
            fclose(adm_file);
        }

        // 加入用户结构
        if (yonghu_info.num <= 0)// 没用户数据或没有初始化
        {
            fclose(yonghu_file);
            yonghu_info.num = 0;
            head_yonghu = NULL;
        } else {
            a3 = yonghu_info.num;
            yonghu_info.num = 0;
            do {
                fread(&yonghu1, sizeof(struct yonghu), 1, yonghu_file);
                insert_yonghu(yonghu1);
                a3--;
            } while (a3 != 0);
            fclose(yonghu_file);
        }
        // 加入用户物品结构
        if (yhthings_info.num <= 0)// 没用户物品数据或者没有初始化
        {
            fclose(yhthings_file);
            yhthings_info.num = 0;
            head_yhthings = NULL;
        } else {
            a2 = yhthings_info.num;
            yhthings_info.num = 0;
            do {
                fread(&yhthings1, sizeof(struct yhthings), 1, yhthings_file);
                insert_yhthings1(yhthings1);
                insert_yhthings2(yhthings1);
                a2--;
            } while (a2 != 0);
            fclose(yhthings_file);
        }
        // 加入拍卖物品结构
        if (paimaipin_info.num <= 0 || head_pai == NULL)// 没拍卖物品数据或者没有初始化
        {
            fclose(yhthings_file);
            paimaipin_info.num = 0;
        } else {
            a5 = paimaipin_info.num;
            paimaipin_info.num = 0;
            do {
                fread(&pai1, sizeof(struct paimaipin), 1, paimaipin_file);
                insert_paimaipin1(pai1);
                a5--;
            } while (a5 != 0);
            fclose(paimaipin_file);
        }

    }
}


void file_close()  //关闭文件,将缓存保存在文件
{

    struct things *p1, *p11;
    struct yhthings *p2, *p21;
    struct yonghu *p3, *p31;
    struct adm *p4, *p41;
    struct paimaipin *p5, *p51;
    thing_file = fopen("things", "wb");
    yhthings_file = fopen("yhthings", "wb");
    yonghu_file = fopen("yonghu", "wb");
    adm_file = fopen("adm", "wb");
    paimaipin_file = fopen("paimaipin", "wb");
    fclose(log_file);
    fwrite(&thing_info, sizeof(struct info), 1, thing_file);
    fwrite(&yhthings_info, sizeof(struct info), 1, yhthings_file);
    fwrite(&yonghu_info, sizeof(struct info), 1, yonghu_file);
    fwrite(&adm_info, sizeof(struct info), 1, adm_file);
    fwrite(&paimaipin_info, sizeof(struct info), 1, paimaipin_file);

    p1 = head;
    while (p1 != NULL) {
        fwrite(&(*p1), sizeof(struct things), 1, thing_file);
        p11 = p1;
        p1 = p1->next;
        free(p11);
    }

    p4 = head_adm;
    while (p4 != NULL) {
        fwrite(&(*p4), sizeof(struct adm), 1, adm_file);
        p41 = p4;
        p4 = p4->next;
        free(p41);
    }

    p2 = head_yhthings;
    while (p2 != NULL) {
        fwrite(p2, sizeof(struct yhthings), 1, yhthings_file);
        p21 = p2;
        p2 = p2->next;
        free(p21);
    }
    p3 = head_yonghu;
    while (p3 != NULL) {
        fwrite(&(*p3), sizeof(struct yonghu), 1, yonghu_file);
        p31 = p3;
        p3 = p3->next;
        free(p31);
    }
    p5 = head_pai;
    while (p5 != NULL) {
        fwrite(&(*p5), sizeof(struct paimaipin), 1, paimaipin_file);
        p51 = p5;
        p5 = p5->next;
        free(p51);
    }


}


void copy_things(struct things *p1, struct things thing1)// 用于复制物品信息 减少代码量
{
    p1->id = thing1.id;
    p1->type = thing1.type;
    p1->price = thing1.price;
    strcpy(p1->name, thing1.name);
    strcpy(p1->time, thing1.time);
    strcpy(p1->introduce, thing1.introduce);
}


void copy_paimaipin(struct paimaipin *p1, struct paimaipin wupin1)// 用于复制拍卖物品信息 减少代码量
{
    p1->id = wupin1.id;
    p1->id2 = wupin1.id2;
    p1->f_price = wupin1.f_price;
    p1->n_price = wupin1.n_price;
    strcpy(p1->name, wupin1.name);
    strcpy(p1->time, wupin1.time);
    strcpy(p1->introduce, wupin1.introduce);
    p1->tb = wupin1.tb;
    p1->te = wupin1.te;

}


void insert_thing(struct things thing1) //物品结构体插入
{
    struct things *p3, *p4;
    if (head == NULL)  // empty linked list
    {
        p1 = (struct things *) malloc(sizeof(struct things));
        head = p1;
        copy_things(p1, thing1);
        p1->next = NULL;
        p1->yonghu = NULL;
        thing_info.num++;
    } else {
        p3 = head;
        p4 = head;
        while (p3 != NULL) {
            if (p3->id >= thing1.id) break;
            p4 = p3;
            p3 = p3->next;
        }
        if (p3 == NULL || p3->id > thing1.id) {
            p3 = (struct things *) head;
            if (p3->id > thing1.id)  //insert at the head
            {
                p1 = (struct things *) malloc(sizeof(struct things));
                copy_things(p1, thing1);
                p1->next = head;
                p1->yonghu = NULL;
                thing_info.num++;
                head = p1;
            } else //insert at the middle or at the end
            {

                while (p3->id < thing1.id && p3->next != NULL) // to find the point to insert
                {
                    if (p3->id == thing1.id) break;
                    p4 = p3;
                    p3 = p3->next;
                }
                if (p3->next == NULL && p3->id < thing1.id) // insert at the end
                {
                    p1 = (struct things *) malloc(sizeof(struct things));
                    copy_things(p1, thing1);
                    p1->next = NULL;
                    p1->yonghu = NULL;
                    thing_info.num++;
                    p3->next = p1;
                } else //insert at the middle
                {
                    p1 = (struct things *) malloc(sizeof(struct things));
                    copy_things(p1, thing1);
                    p1->next = p3;
                    p1->yonghu = NULL;
                    thing_info.num++;
                    p4->next = p1;
                }

            }
        } else printf("\n该物品已存在。");

    }
}


void insert_yhthings2(struct yhthings yhthings1)  //将用户物品信息插入用户物品链表中
{
    struct yhthings *p5, *p3, *p4;
    struct yhthings *p;
    if (head_yhthings == NULL)  // empty linked list
    {
        p5 = (struct yhthings *) malloc(sizeof(struct yhthings));
        p5->id = yhthings1.id;
        p5->id1 = yhthings1.id1;
        strcpy(p5->name, yhthings1.name);
        strcpy(p5->yonghu_name, p->yonghu_name);
        strcpy(p5->time, yhthings1.time);
        p5->f_price = yhthings1.f_price;
        p5->n_price = yhthings1.n_price;
        head_yhthings = p5;
        p5->next = NULL;
        yhthings_info.num++;
    } else {
        p3 = head_yhthings;
        p4 = head_yhthings;
        while (p3 != NULL) {
            if (p3->id >= yhthings1.id) break;
            p4 = p3;
            p3 = p3->next;
        }
        if (p3 == NULL || p3->id > yhthings1.id) {
            p3 = (struct yhthings *) head_yhthings;
            if (p3->id > yhthings1.id)  //insert at the head_yhthings
            {
                p5 = (struct yhthings *) malloc(sizeof(struct yhthings));
                p5->id = yhthings1.id;
                p5->id1 = yhthings1.id1;
                strcpy(p5->name, yhthings1.name);
                strcpy(p5->yonghu_name, p->yonghu_name);
                strcpy(p5->time, yhthings1.time);
                p5->f_price = yhthings1.f_price;
                p5->n_price = yhthings1.n_price;
                p5->next = head_yhthings;
                yhthings_info.num++;
                head_yhthings = p5;
            } else //insert at the middle or at the end
            {

                while (p3->id < yhthings1.id && p3->next != NULL) // to find the point to insert
                {
                    if (p3->id == yhthings1.id) break;
                    p4 = p3;
                    p3 = p3->next;
                }
                if (p3->next == NULL && p3->id < yhthings1.id) // insert at the end
                {
                    p5 = (struct yhthings *) malloc(sizeof(struct yhthings));
                    p5->id = yhthings1.id;
                    p5->id1 = yhthings1.id1;
                    strcpy(p5->name, yhthings1.name);
                    strcpy(p5->yonghu_name, p->yonghu_name);
                    strcpy(p5->time, yhthings1.time);
                    p5->f_price = yhthings1.f_price;
                    p5->n_price = yhthings1.n_price;
                    p5->next = NULL;
                    yhthings_info.num++;
                    p3->next = p5;
                } else //insert at the middle
                {
                    p5 = (struct yhthings *) malloc(sizeof(struct yhthings));
                    p5->id = yhthings1.id;
                    p5->id1 = yhthings1.id1;
                    strcpy(p5->name, yhthings1.name);
                    strcpy(p5->yonghu_name, p->yonghu_name);
                    strcpy(p5->time, yhthings1.time);
                    p5->f_price = yhthings1.f_price;
                    p5->n_price = yhthings1.n_price;
                    p5->next = p3;
                    yhthings_info.num++;
                    p4->next = p5;
                }

            }
        } else printf("\n该用户物品已存在。");

    }

}


void input_thing()  //物品信息录入
{
    system("cls");
    struct things thing1;
    printf("\n请输入物品数据,输入0结束输入.");
    printf("\n请输入物品ID(8位数字):");
    scanf("%d", &thing1.id);
    while (thing1.id != 0) {
        //initialize and create
        printf("\n请输入物品名称:");
        scanf("%s", thing1.name);
        printf("\n请输入物品朝代(如唐代):");
        scanf("%s", thing1.time);
        printf("\n请输入物品价格：");
        scanf("%lf", &thing1.price);
        printf("请输入物品类型，虚拟物品为0，实体物品为1：");
        scanf("%d", &thing1.type);
        printf("\n请输入物品介绍(140字以内):");
        scanf("%s", thing1.introduce);
        thing1.next = NULL;
        thing1.yonghu = NULL;
        insert_thing(thing1);
        //select_n(p1);
        sprintf(log_message, "thing_input\nname:%s, time:%s, type:%d,", thing1.name, thing1.time,
                thing1.type); //格式化日志输出
        logging(thing1.id, log_message);
        printf("\n请输入物品ID(8位数字):");
        scanf("%d", &thing1.id);
    }
}


void input_yonghu()  //用户录入
{
    system("cls");
    struct yonghu yonghu1;
    char password[password_max_length] = {'\0'};
    char password2[password_max_length] = {'\0'};
    printf("\n请输入用户数据,输入0结束输入.");
    printf("\n请输入用户ID(8位数字):");
    scanf("%d", &yonghu1.id);
    while (yonghu1.id != 0) {
        printf("\n请输入用户姓名:");
        scanf("%s", yonghu1.name);
        printf("\n请输入用户电话号码(11位手机号码):");
        scanf("%s", yonghu1.phone);
        printf("\n请输入用户的邮箱地址:");
        scanf("%s", yonghu1.email);
        while (1) {
            printf("\n请输入用户密码:");
            password_input(password);
            system("cls");
            password_input(password2);
            if ((strcmp(password, password2)) == 0) {
                password_encryption(password);
                strcpy(yonghu1.password, password);
                break;
            }
            printf("\n两次密码不一样，请重新输入。。。");
            strcpy(password, password_clear);
            strcpy(password2, password_clear);
        }
        printf("\n请输入用户积分:");
        scanf("%d", &yonghu1.score);
        yonghu1.next = NULL;
        yonghu1.yhthings = NULL;
        insert_yonghu(yonghu1);
        sprintf(log_message, "user_input\nname:%s, phone:%s, email:%s, score:%d", yonghu1.name, yonghu1.phone,
                yonghu1.email,
                yonghu1.score);
        logging(yonghu1.id, log_message);
        printf("\n请输入用户ID(8位数字):");
        scanf("%d", &yonghu1.id);
    }
}

void user_register()  //用户注册
{
    system("cls");
    struct yonghu yonghu1;
    char password[password_max_length] = {'\0'};
    char password2[password_max_length] = {'\0'};
    printf("\n请输入用户数据,输入0结束输入.");
    printf("\n请输入用户ID(8位数字):");
    scanf("%d", &yonghu1.id);
    while (yonghu1.id != 0) {
        printf("\n请输入用户姓名:");
        scanf("%s", yonghu1.name);
        printf("\n请输入用户电话号码(11位手机号码):");
        scanf("%s", yonghu1.phone);
        printf("\n请输入用户的邮箱地址:");
        scanf("%s", yonghu1.email);
        while (1) {
            printf("\n请输入用户密码:");
            password_input(password);
            printf("\n请确认\n:");
            password_input(password2);
            if ((strcmp(password, password2)) == 0) {
                password_encryption(password);
                strcpy(yonghu1.password, password);
                break;
            }
            printf("\n两次密码不一样，请重新输入。。。");
            strcpy(password, password_clear);
            strcpy(password2, password_clear);
        }
        yonghu1.score = 0;
        yonghu1.next = NULL;
        yonghu1.yhthings = NULL;
        insert_yonghu(yonghu1);
        sprintf(log_message, "user_input\nname:%s, phone:%s, email:%s, score:%d", yonghu1.name, yonghu1.phone,
                yonghu1.email,
                yonghu1.score);
        logging(yonghu1.id, log_message);
        printf("\n请输入用户ID(8位数字):");
        scanf("%d", &yonghu1.id);
    }
    system("cls");
}


void input_yhthings()  //用户物品信息录入
{
    struct yhthings yhthings1;
    struct things *thing_p;
    int flag;
    //struct yhthings *cp;
    printf("\n请输入用户物品信息数据,输入0结束输入.");
    printf("\n请输入目标物品ID(8位数字):");
    scanf("%d", &yhthings1.id);
    while (yhthings1.id != 0) {
        //initialize and create
        flag = 0;
        thing_p = head;
        while (thing_p != NULL) {
            if (thing_p->id == yhthings1.id)break;
            thing_p = thing_p->next;
        }
        if (thing_p != NULL) {
            strcpy(yhthings1.time, thing_p->time);
            strcpy(yhthings1.name, thing_p->name);
        } else {
            flag = 1;
        }
        if (flag) printf("\n该物品还未登记，请在物品区录入详细信息后再进行用户物品的录入~");
        else {
            printf("\n请输入目标用户ID：");
            scanf("%d", &yhthings1.id1);
            printf("\n请输入用户物品原价格:");
            scanf("%lf", &yhthings1.f_price);
            printf("\n请输入用户物品竞拍后价格:");
            scanf("%lf", &yhthings1.n_price);
            yhthings1.next = NULL;
            yhthings1.follow = NULL;
            insert_yhthings1(yhthings1);
            insert_yhthings2(yhthings1);
            sprintf(log_message, "user_ownership\nname:%s, price_before:%lf, price_now:%lf", thing_p->name,
                    yhthings1.f_price, yhthings1.n_price);
            logging(yhthings1.id, log_message);
        }

        printf("\n请输入用户物品ID(8位数字):");
        scanf("%d", &yhthings1.id);
    }
}


void print_yonghu(struct yonghu *p1)  //输出用户信息
{
    struct yhthings *p2;
    printf("\n------------------------------------------------------------");
    printf("\nID>>>>>>>>名字>>>>>>>>>积分>>>>手机号码>>>>>>>>邮件>>>>>>>>>>>>>");
    printf("\n------------------------------------------------------------");
    printf("\n%-8d  %-10s   %-5d    %-11s    %s", p1->id, p1->name, p1->score, p1->phone, p1->email);
    if (p1->yhthings == NULL) {
        printf("\n暂无他的物品信息!");
    } else {
        p2 = (struct yhthings *) p1->yhthings;
        printf("\n------------------------------------------------------------");
        printf("\n物品ID>>>>物品名称>>>物品原价格>>物品现价格>>>>>");
        printf("\n------------------------------------------------------------");
        while (p2 != NULL) {
            printf("\n%d   %12s   %.2lf     %.2lf", p2->id, p2->name, p2->f_price, p2->n_price);
            p2 = p2->next;
        }
    }
    getchar();
    getchar();
}


void print_things(struct things *p1)  //输出物品
{
    printf("\n------------------------------------------------------------");
    printf("\nID>>>>>>>>名字>>>>>>>>>朝代>>>>>价格>>>>>>>>>");
    printf("\n------------------------------------------------------------");
    printf("\n%-8d  %-10s  %-7s   %lf", p1->id, p1->name, p1->time, p1->price);
    printf("\n物品简短介绍>>>>>>>>>>>>>>>>>>>");
    printf("\n%s", p1->introduce);
    getchar();
    getchar();
}


void skim_things(struct things *head)   //按ID或名字查看物品信息
{
    char id[60] = {'\0'};
    int id1;
    bool is_num = 1;
    struct things *p1;
    system("cls");
    if (head == NULL) {
        printf("\n目前无物品数据,请返回录入.\n");
    } else {
        printf("\n请输入要查询的物品ID或姓名(8位数字或者汉字,输入0停止查询):");
        scanf("%s", id);
        while (id[0] != '0') {
            p1 = head;
            for (int i = 0; i < strlen(id); i++) {
                if (!('0' <= id[i] && id[i] <= '9')) {
                    is_num = 0;
                    break;
                }
            }
            if (is_num) {    //如果是id
                id1 = idc(id);
                while (p1->id != id1 && p1->next != NULL && p1->id < id1) {
                    p1 = p1->next;
                }
                if ((p1->next == NULL && p1->id != id1) || p1->id > id1) {
                    printf("\n没有这项物品哦~请重新输入物品ID(8位数字,输入0停止查询):");
                    scanf("%s", id);
                } else {
                    print_things(p1);
                    printf("\n继续查询请再次输入物品ID或名称(8位数字或者汉字,输入0停止查询):");
                    scanf("%s", id);
                }
            } else {
                while (p1 != NULL && strcmp(id, p1->name) != 0)    //先判断是否到了链表尾，否者查询不到是会出崩溃
                    p1 = p1->next;
                if (p1 == NULL) {
                    printf("\n没有这项物品哦!请重新输入物品ID(8位数字,输入0停止查询):");
                    scanf("%s", id);
                } else {
                    print_things(p1);
                    printf("\n继续查询请再次输入物品ID或名称(8位数字或者汉字,输入0停止查询):");
                    fflush(stdin);
                    scanf("%s", id);
                }

            }
        }
    }
}


int idc(char *id)  //id转换 char 到int
{
    int z = 0;
    int i = 0;
    int l, n = 1;
    l = strlen(id);
    for (i = 0; i < l; i++) {
        n *= 10;
    }
    i = 0;
    n /= 10;
    while (id[i] != '\0') {
        z = z + ((int) id[i] - 48) * n;
        i++;
        n /= 10;
    }
    return z;
}


void password_yonghu()//用户ID与密码验证
{
    int z = 0;
    char password1[password_max_length] = {'\0'};
    int id;
    struct yonghu *p;
    system("cls");
    do {
        printf("\n\t\t\t|&|*************用户登录**************|&|");
        printf("\n\t\t\t|&|----------------------------------|&|");
        printf("\n\t\t\t|&|                      ++++        |&|");
        printf("\n\t\t\t|&|                     ++++++       |&|");
        printf("\n\t\t\t|&|                 +  +++++++++     |&|");
        printf("\n\t\t\t|&|   ++++++ +++ +++++++++++++++++   |&|");
        printf("\n\t\t\t|&| ++++++++++++++++++++++++++   +   |&|");
        printf("\n\t\t\t|&|   ++++++++++++++++++++++++++++   |&|");
        printf("\n\t\t\t|&|                 +  +++++++++     |&|");
        printf("\n\t\t\t|&|                     ++++++       |&|");
        printf("\n\t\t\t|&|----------------------------------|&|");

        printf("\n请输入用户ID(最多8位数字，0为退出):");
        scanf("%d", &id);
        fflush(stdin);
        if (id == 0)
            return;
        printf("\n请输入密码(24位以内):");
        password_input(password1);
        password_encryption(password1);
        fflush(stdin);
        p = head_yonghu;
        while (p != NULL) {
            if (p->id >= id) break;
            p = p->next;
        }
        if (p != NULL) {
            if (strcmp(p->password, password1) == 0) {
                yonghu_ui(p);
                sprintf(log_message, "user_login\nlogin successfully");
                logging(p->id, log_message);
                break;
            } else {
                z++;
                printf("\n账号或密码有误，请重新输入(你还有%d次机会)：", 5 - z);
                sprintf(log_message, "user_login\nlogin failed");
                logging(p->id, log_message);
                getchar();
                fflush(stdin);
            }
        } else {
            printf("该用户不存在。");
            break;
        }
    } while (z != 5);
}


void password_adm()//管理员ID与密码验证
{
    int z = 0;
    char password1[password_max_length] = {'\0'};
    int id;
    struct adm *p;
    system("cls");
    do {
        printf("\n\t\t\t|&|*************用户登录**************|&|");
        printf("\n\t\t\t|&|----------------------------------|&|");
        printf("\n\t\t\t|&|                      ++++        |&|");
        printf("\n\t\t\t|&|                     ++++++       |&|");
        printf("\n\t\t\t|&|                 +  +++++++++     |&|");
        printf("\n\t\t\t|&|   ++++++ +++ +++++++++++++++++   |&|");
        printf("\n\t\t\t|&| ++++++++++++++++++++++++++   +   |&|");
        printf("\n\t\t\t|&|   ++++++++++++++++++++++++++++   |&|");
        printf("\n\t\t\t|&|                 +  +++++++++     |&|");
        printf("\n\t\t\t|&|                     ++++++       |&|");
        printf("\n\t\t\t|&|----------------------------------|&|");
        printf("\n请输入管理员ID(8位数字，0为退出):");
        scanf("%d", &id);
        fflush(stdin);
        if (id == 0)
            return;
        printf("\n请输入密码(24位以内):");
        password_input(password1);
        password_encryption(password1);
        fflush(stdin);
        p = head_adm;
        while (p != NULL) {
            if (p->id >= id) break;
            p = p->next;
        }
        if (p != NULL) {
            if (strcmp(p->password, password1) == 0) {
                sprintf(log_message, "admin_login\nlogin successfully");
                logging(p->id, log_message);
                adm_ui_1();
                break;
            } else {
                z++;
                sprintf(log_message, "admin_login\nlogin failed");
                logging(p->id, log_message);
                printf("\n账号或密码有误，请重新输入(你还有%d次机会)：", 5 - z);
                getchar();
                fflush(stdin);
            }
        } else {
            printf("\n对不起，该ID不存在，请查证后再输入。");
            break;
        }
    } while (z != 5);

}


void modify_yonghu(struct yonghu *p1)// 用户修改自己信息
{
    int z;
    system("cls");
    printf("\n\t\t\t**************************");
    printf("\n\t\t\t**\t修改项目\t**");
    printf("\n\t\t\t**************************");
    printf("\n\t\t\t**\t(1)邮件    \t**");
    printf("\n\t\t\t**\t(2)电话号码\t**");
    printf("\n\t\t\t**************************");
    printf("\n请输入数字(输入0停止修改):");
    scanf("%d", &z);
    switch (z) {
        case 0:
            break;
        case 1:
            printf("请输入邮件:");
            scanf("%s", p1->email);
            sprintf(log_message, "modify_user\nchange email to%s", p1->email);
            logging(p1->id, log_message);
            fflush(stdin);
            break;
        case 2:
            printf("请输入电话号码:");
            scanf("%s", p1->phone);
            sprintf(log_message, "modify_user\nchange phone to%s", p1->phone);
            logging(p1->id, log_message);
            fflush(stdin);
            break;
        default :
            printf("\n请正确输入相应数字。");
            break;
    }
    printf("\n已完成修改。按任意键返回。");
    getchar();
    getchar();
    system("cls");
}


void modify_yonghu_adm()//管理员进入某一用户系统 修改用户信息 如密码
{
    struct yonghu *p1;
    int id;
    p1 = head_yonghu;
    system("cls");
    printf("\n请输入要修改用户的ID(8位id,输入0停止输入):");
    scanf("%d", &id);
    fflush(stdin);
    while (p1 != NULL) {
        if (p1->id >= id) break; //减少检索次数
        p1 = p1->next;
    }
    if (p1 != NULL) {
        if (p1->id == id) yonghu_ui(p1);
        sprintf(log_message, "admin_modify_user_info\n");
        logging(p1->id, log_message);
    } else
        printf("\n这位用户还没出现过噻~");
}


void del_yonghu()//删除用户信息
{
    struct yonghu *p1, *p11, *p31;
    struct yhthings *p2, *p22;
    struct things *p3;
    int id;
    system("cls");
    printf("\n请输入要删除用户的ID(8位ID,输入0为停止删除):");
    scanf("%d", &id);
    fflush(stdin);
    while (id != 0) {
        p1 = head_yonghu;
        while (p1 != NULL) {
            if (p1->id == id) break;
            p1 = p1->next;
        }
        if (p1 != NULL) {
            p2 = p1->yhthings;
            while (p2 != NULL) {
                p3 = head;
                while (p3 != NULL) {
                    p31 = p3->yonghu;
                    if (strcmp(p3->name, p2->name) == 0) {
                        while (p31 != NULL) {
                            if (p31->id == p2->id1) break;
                            p31 = p31->next;
                        }
                    }
                    if (p31 != NULL && p31->id == p2->id1) break;
                    p3 = p3->next;
                }
                p22 = p2->next;
                if (p3 != NULL) {
                    sprintf(log_message, "user_delete\n");
                    logging(id, log_message);
                    pfree_yhthings(p1, p3, (*p2));
                }
                p2 = p22;
            }
            printf("\n成功删除该用户信息.");

        }

        p1 = p11 = head_yonghu;
        while (p1 != NULL) //释放用户内存
        {
            if (p1->id == id) {
                if (p1 == p11) head_yonghu = p1->next;
                else p11->next = p1->next;
                free(p1);
                yonghu_info.num--;
                break;
            }
            p11 = p1;
            p1 = p1->next;
        }
        printf("\n请输入要删除用户的ID(8位ID,输入0为停止删除):");
        scanf("%d", &id);
    }
}


void del_yhthings()//删除用户物品信息
{
    struct yhthings yhthings1;
    struct yonghu *p_yonghu;
    struct things *p, *p_things;
    system("cls");
    printf("\n请输入用户物品的名称:");
    scanf("%s", yhthings1.name);
    fflush(stdin);
    printf("\n请输入用户物品的ID:");
    scanf("%d", &yhthings1.id);
    fflush(stdin);
    while (yhthings1.id != 0) {
        p_yonghu = head_yonghu;
        p_things = head;
        while (p_yonghu != NULL) {
            if (yhthings1.id == p_yonghu->id) break;
            p_yonghu = p_yonghu->next;
        }
        while (p_things != NULL) {
            if (strcmp(p_things->name, yhthings1.name) == 0) {
                p = p_things;
                while (p != NULL) {
                    if (p->id == yhthings1.id) break;
                    p = p->next;
                }
                if (p != NULL && p_yonghu != NULL) {
                    sprintf(log_message, "user_ownership_delete\ndeleted_thing_id:", yhthings1.id);
                    logging(p_yonghu->id, log_message);
                    pfree_yhthings(p_yonghu, p_things, yhthings1);
                    break;
                }
            }
            p_things = p_things->next;
        }
        printf("\n请输入用户物品的名称:");
        scanf("%s", yhthings1.name);
        printf("\n请输入用户物品的id:");
        scanf("%d", &yhthings1.id);
    }
}


void pfree_yhthings(struct yonghu *p_yonghu, struct things *p_things, struct yhthings yhthings1)//解锁及释放用户物品内存
{
    struct yhthings *p2, *p21;
    struct things *p3, *p31;
    p2 = p21 = p_yonghu->yhthings;
    while (p2 != NULL) {
        if (strcmp(p2->name, yhthings1.name) == 0) {
            if (p2 == p21) {
                p_yonghu->yhthings = p2->next;
            } else {
                p21->next = p2->next;
            }
            break;
        }
        p21 = p2;
        p2 = p2->next;
    }
    p3 = p31 = p_things;
    while (p3 != NULL) {
        if (p3->id == yhthings1.id) {
            if (p3 == p31) {
                p_things->next = p3->next;
            } else {
                p31->next = p3->next;
            }
            break;
        }
        p31 = p3;
        p3 = p3->next;
    }
    p2 = p21 = head_yhthings;
    while (p2 != NULL) {
        if (p2->id == yhthings1.id && strcmp(p2->name, yhthings1.name) == 0) {
            if (p2 == p21) {
                headc = p2->next;
            } else {
                p21->next = p2->next;

            }
            free(p2);
            yhthings_info.num--;
            break;
        }
        p21 = p2;
        p2 = p2->next;
    }

}


void skim_yonghu(struct yonghu *head)   //按ID或名字查看用户信息
{
    char id[60];
    bool is_num = 1;
    int id1;
    struct yonghu *p1;
    system("cls");
    if (head == NULL) {
        printf("\n目前无用户数据,请返回录入.\n");
    } else {
        printf("\n请输入要查询的用户ID或姓名(8位数字或者汉字,输入0停止查询):");
        scanf("%s", id);
        while (id[0] != '0') {
            for (int i = 0; i < strlen(id); i++) {
                if (!('0' <= id[i] && id[i] <= '9')) {
                    is_num = 0;
                    break;
                }
            }
            p1 = head;
            if (is_num) {
                id1 = idc(id);
                while (p1->id != id1 && p1->next != NULL && p1->id < id1) {
                    p1 = p1->next;
                }
                if ((p1->next == NULL && p1->id != id1) || p1->id > id1) {
                    printf("\n查无此用户!请重新输入用户ID(8位数字,输入0停止查询):");
                    scanf("%s", id);
                } else {
                    print_yonghu(p1);
                    printf("\n继续查询请再次输入用户ID或姓名(8位数字或者汉字,输入0停止查询):");
                    scanf("%s", id);
                }
            } else {
                while (p1 != NULL && strcmp(id, p1->name) != 0) {
                    p1 = p1->next;
                }
                if (p1 != NULL) {
                    print_yonghu(p1);
                    printf("\n继续查询请再次输入用户ID或姓名(8位数字或者汉字,输入0停止查询):");
                    scanf("%s", id);
                } else {
                    printf("\n查无此用户!请重新输入用户ID(8位数字,输入0停止查询):");
                    scanf("%s", id);
                }
            }

        }
    }

}


void skim_yonghu_all()//查看所有用户积分及所拥有物品
{
    struct yonghu *p1;
    struct yhthings *p2;
    system("cls");
    p1 = head_yonghu;
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>用户姓名>>>>>>>积分>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n%d  %s  \t  %d", p1->id, p1->name, p1->score);
        p2 = p1->yhthings;
        if (p2 == NULL) printf("\n该用户没有个人物品。");
        else {
            printf("\n该用户的物品如下：");
            while (p2 != NULL) {
                printf("\n %s  ", p2->name);
                p2 = p2->next;
            }

        }
        printf("\n");
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void adm_password()//管理员密码修改
{
    int z = 0;
    struct adm *p;
    int ID;
    char password0[password_max_length] = {'\0'};
    char password2[password_max_length]{'\0'};
    system("cls");
    do {
        printf("\n请输入管理员ID（8位数字）：");
        scanf("%d", &ID);
        printf("\n请输入原密码（10位以内）:");
        password_input(password0);
        password_encryption(password0);
        fflush(stdin);
        p = head_adm;
        while (p != NULL) {
            if (p->id >= ID) break;
            p = p->next;
        }
        if (p != NULL) {
            if (strcmp(p->password, password0) == 0) {
                break;
            } else {
                z++;
                sprintf(log_message, "admin_password_changing\nfail  reason: password wrong");
                logging(p->id, log_message);
                printf("\n密码错误请重新输入(你还有%d次机会).", 5 - z);
            }
        }

    } while (z != 5);
    if (z != 5) {
        do {
            printf("\n请输入新密码:");
            password_input(password0);
            fflush(stdin);
            printf("\n请再次输入新密码:");
            password_input(password2);
            fflush(stdin);
            if (strcmp(password0, password2) == 0) {
                password_encryption(password0);
                strcpy(p->password, password0);
                sprintf(log_message, "admin_password_changing\nsuccessfully");
                logging(p->id, log_message);
                break;
            } else {
                sprintf(log_message,
                        "admin_password_changing\nfailed  reason:new passwords ard not the same in confirmation");
                logging(p->id, log_message);
                printf("\n两次密码不一致，请重新输入,不再修改请输入0,继续修改请输入1.");
                scanf("%d", &z);
            }
        } while (z != 0);
        system("cls");
    } else exit(0);
}


struct order *list(struct yonghu *head)// 用户积分排序 返回链表头   为了建立用户的链表
{
    struct order *p1 = NULL, *p2, *p3, *p4;
    struct yonghu *p;
    p = head_yonghu;
    while (p != NULL) {
        if (p1 == NULL)//the head
        {
            p2 = (struct order *) malloc(sizeof(struct order));
            p2->pointer = p;
            p2->next = NULL;
            p1 = p2;
        } else {
            if (p->score > p1->pointer->score) {
                p2 = (struct order *) malloc(sizeof(struct order));
                p2->pointer = p;
                p2->next = p1;
                p1 = p2;
            } else {
                p3 = p1;
                while (p3 != NULL) {
                    if (p->score > p3->pointer->score) break;
                    p4 = p3;
                    p3 = p3->next;
                }
                p2 = (struct order *) malloc(sizeof(struct order));
                p2->pointer = p;
                if (p3 == NULL) p2->next = NULL;
                else p2->next = p3;
                p4->next = p2;

            }
        }
        p = NULL;
    }
    return p1;
}


void yonghu_ui(struct yonghu *p)//用户界面
{
    int z;
    system("cls");
    do {

        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+        %s,欢迎你          |\n", p->name);
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----修改个人信息       |\n");
        printf("\t\t\t+    [2]----参与竞拍           |\n");
        printf("\t\t\t+    [3]----查询所有用户信息   |\n");
        printf("\t\t\t+    [4]----修改密码           |\n");
        printf("\t\t\t+    [5]----查看个人物品信息   |\n");
        printf("\t\t\t+    [0]----返回主界面         |\n");
        printf("\t\t\t--------------------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &z);
        switch (z) {
            case 0 :
                break;
            case 1 :
                modify_yonghu(p);
                break;
            case 2 :
                jingpai(p);
                break;
            case 3 :
                print_yonghu(p);
                break;
            case 4 :
                st_password_yonghu(p);
                break;
            case 5:
                skim_yhthings(p);
            default:
                printf("\n无效选项!");
        }
    } while (z != 0);
    system("cls");
}


void st_password_yonghu(struct yonghu *p)//用户密码修改
{
    char password1[11] = {'\0'};
    char password2[11] = {'\0'};
    int z = 1;
    system("cls");
    do {
        printf("\n请输入新密码:");
        scanf("%s", password1);
        fflush(stdin);
        printf("\n请再次输入新密码:");
        scanf("%s", password2);
        fflush(stdin);
        if (strcmp(password1, password2) == 0) {
            password_encryption(password1);
            strcpy(p->password, password1);
            sprintf(log_message, "user_password_changing\nsuccessfully");
            logging(p->id, log_message);
            printf("\n密码修改成功!");
            break;
        } else {
            printf("\n两次密码不一致，请重新输入,不再修改请输入0,继续修改请输入任何其余值.");
            sprintf(log_message,
                    "user_password_changing\nfailed  reason:new passwords ard not the same in confirmation");
            logging(p->id, log_message);
            scanf("%d", &z);
            fflush(stdin);
        }
    } while (z != 0);
    system("cls");
}


void skim_thing_all()//查看所有物品信息
{
    struct things *p1;
    system("cls");
    p1 = head;
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>物品信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n物品ID：  %d", p1->id);
        printf("\n物品名称：%s", p1->name);
        printf("\n物品朝代：%s", p1->time);
        printf("\n物品价格：%lf", p1->price);
        printf("\n物品介绍: %s\n", p1->introduce);
        if (p1->type == 1)
            printf("\n物品类型: 实体");
        else printf("\n物品类型: 虚拟");
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void show_thing_with_kind()//查看所有物品信息
{
    struct things *p1;
    system("cls");
    p1 = head;
    int type;
    printf("请输入物品类型，0为虚拟物品，1为实体物品");
    scanf("%d", &type);
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>物品信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        if (type == p1->type) {
            printf("\n物品ID：  %d", p1->id);
            printf("\n物品名称：%s", p1->name);
            printf("\n物品朝代：%s", p1->time);
            printf("\n物品价格：%lf", p1->price);
            printf("\n物品介绍: %s\n", p1->introduce);
            if (p1->type == 1)
                printf("\n物品类型: 实体");
            else printf("\n物品类型: 虚拟");
        }
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void skim_yhthings(struct yonghu *p)//查看所有用户物品信息
{
    struct yhthings *p1;
    system("cls");
    p1 = p->yhthings;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>>>>>>>>>>>>>>>>>用户物品信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n\t\t物品ID：  %d", p1->id);
        printf("\n\t\t物品名称：%s", p1->name);
        printf("\n\t\t物品朝代：%s", p1->time);
        printf("\n\t\t物品价格：%lf\n", p1->n_price);
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}

void skim_yhthings1()//查看所有用户物品信息
{
    struct yhthings *p1;
    system("cls");
    p1 = head_yhthings;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>>>>>>>>>>>>>>>>>用户物品信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n\t\t物品ID：  %d", p1->id);
        printf("\n\t\t所有人ID：  %d", p1->id1);
        printf("\n\t\t所有人名字：  %s", p1->yonghu_name);
        printf("\n\t\t物品名称：%s", p1->name);
        printf("\n\t\t物品朝代：%s", p1->time);
        printf("\n\t\t物品价格：%lf\n", p1->n_price);
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void result(struct yonghu *p) {
    system("cls");
    time_t t;
    struct tm *p7;
    struct paimaipin *p3;
    struct paimairen *p4;
    int flag = 0, flag1 = 0;
    int dd1, tt1, dd2, tt2;
    p3 = head_pai;
    while (p3 != NULL) {
        p4 = p3->ren;
        while (p4 != NULL) {
            if (p4->id == p->id) {
                flag = 1;
                printf("\n\t系统当前时间为：");
                t = time(NULL);
                p7 = localtime(&t);
                printf(ctime(&t));
                printf("\n物品ID：  %d", p3->id);
                printf("\n物品名称：%s", p3->name);
                printf("\n物品朝代：%s", p3->time);
                printf("\n物品价格：%lf", p3->f_price);
                printf("\n物品介绍: %s\n", p3->introduce);
                printf("\n该物品开始竞拍时间：%d年%d月%d日 %d时%d分%d秒", p3->tb.year, p3->tb.mon, p3->tb.mday, p3->tb.hour, p3->tb.min,
                       p3->tb.sec);
                printf("\n该物品结束竞拍时间：%d年%d月%d日 %d时%d分%d秒", p3->te.year, p3->te.mon, p3->te.mday, p3->te.hour, p3->te.min,
                       p3->te.sec);
                dd1 = (p7->tm_year + 1900) * 400 + (p7->tm_mon + 1) * 31 + (p7->tm_mday);
                dd2 = (p3->te.year) * 400 + (p3->te.mon) * 31 + (p3->te.mday);
                if (dd1 > dd2) flag1 = 1;
                else {
                    tt1 = (p7->tm_hour) * 100 + (p7->tm_min) * 60 + (p7->tm_sec);
                    tt2 = (p3->te.hour) * 100 + (p3->te.min) * 60 + (p3->te.sec);
                    if (tt1 > tt2) flag1 = 1;
                }
                if (!flag1) {
                    printf("\n竞拍还未结束，请等待竞拍结束后查看结果。");
                } else {
                    zuigaojia(p3, p);
                }
            }
            p4 = p4->next;
        }
        p3 = p3->next;
    }
    if (!flag) printf("\n没有你参与竞购的物品哦~");
    return;

}

void modify_thing_adm()// 管理员修改物品信息
{
    struct things *p1;
    int z;
    int id1 = 0;
    system("cls");
    printf("\n\t\t\t请输入要修改的物品id：");
    scanf("%d", &id1);
    p1 = head;
    while (p1 != NULL) {
        if (p1->id >= id1) break;
        p1 = p1->next;
    }
    if (p1 != NULL) {
        if (p1->id == id1) {
            printf("\n\t\t\t**************************");
            printf("\n\t\t\t**\t修改项目\t**");
            printf("\n\t\t\t**************************");
            printf("\n\t\t\t**\t(1)名称    \t**");
            printf("\n\t\t\t**\t(2)朝代    \t**");
            printf("\n\t\t\t**\t(3)价格    \t**");
            printf("\n\t\t\t**\t(4)简短介绍\t**");
            printf("\n\t\t\t**\t(5)物品类型\t**");
            printf("\n\t\t\t**************************");
            printf("\n请输入数字(输入0停止修改):");
            scanf("%d", &z);
            switch (z) {
                case 0:
                    break;
                case 1:
                    printf("请输入新名字:");
                    scanf("%s", p1->name);
                    sprintf(log_message, "thing_name_changing\n new_name:%s", p1->name);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 2:
                    printf("请输入朝代；");
                    scanf("%s", p1->time);
                    sprintf(log_message, "thing_time_changing\n new_time:%s", p1->time);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 3:
                    printf("请输入价格：");
                    scanf("%lf", &p1->price);
                    sprintf(log_message, "thing_price_changing\n new_price:%lf", p1->price);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 4:
                    printf("请输入新介绍：");
                    scanf("%s", p1->introduce);
                    sprintf(log_message, "thing_introduction_changing\n successfully");
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 5:
                    printf("请输入新物品种类,虚拟物品为0，实体物品为1：");
                    scanf("%d", &p1->type);
                    sprintf(log_message, "thing_type_changing\n new_price:%d", p1->type);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                default :
                    printf("\n请正确输入相应数字。");
                    break;
            }
            printf("\n已完成修改。按任意键返回。");
            getchar();
            getchar();
            system("cls");
        } else
            printf("该物品不存在。");
    }
}


void skim_thing_kind() // 根据类别查看物品 管理员功能
{
    struct things *p1, *p;
    int id1, id2;
    int choice;
    int n = 0;
    p1 = head;
    do {
        system("cls");
        printf("\n请选择你想要查询的类别：");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("请输入物品朝代：");
                scanf("%s", p->time);
                printf("\n-------------------------------------------------------------------");
                printf("\nID>>>物品名称>>>物品朝代>>>物品价格>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                printf("\n-------------------------------------------------------------------");
                p1 = head;
                n = 0;
                while (p1 != NULL) {
                    if (strcmp(p1->time, p->time) == 0) //寻找同一朝代的物品
                    {
                        printf("\n%s\t%s\t%lf\t", p1->name, p1->time, p1->price);
                        n = 1;  // 有班级存在 n作为判断数
                    }
                    p1 = p1->next;
                }
                if (n == 0)
                    printf("\n无该朝代的物品");
                getchar();
                getchar();
                break;
            case 2:
                printf("请输入物品价格区间：");
                scanf("%d%d", &id1, &id2);
                printf("\n-------------------------------------------------------------------");
                printf("\nID>>>物品名称>>>物品朝代>>>物品价格>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                printf("\n-------------------------------------------------------------------");
                p1 = head;
                n = 0;
                while (p1 != NULL) {
                    if (p1->price < id2 && p1->price > id1) //寻找同一朝代的物品
                    {
                        printf("\n%s\t%s\t%lf\t", p1->name, p1->time, p1->price);
                        n = 1;  // 有物品存在 n作为判断数
                    }
                    p1 = p1->next;
                }
                if (n == 0)
                    printf("\n无该朝代的物品");
                getchar();
                getchar();
                break;
            case 0:
                break;
        }
    } while (choice != 0);


}


void del_things()//删除用户信息
{
    struct things *p1, *p11;
    struct yonghu *p2, *p3;
    struct yhthings *p21, *p22;
    int id;
    int flag = 0;
    system("cls");
    printf("\n请输入要删除物品的ID(8位,输入0为停止删除):");
    scanf("%d", &id);
    fflush(stdin);
    while (id != 0) {
        flag = 0;
        p1 = p11 = head;
        while (p1 != NULL) {
            if (p1->id == id) break;
            p1 = p1->next;
        }
        if (p1 != NULL) {
            p2 = p1->yonghu;
            while (p2 != NULL) {
                p3 = head_yonghu;
                while (p3 != NULL) {
                    if (p3->id == p2->id) break;
                    p3 = p3->next;
                }
                if (p3 != NULL) {
                    p22 = p3->yhthings;
                    p21 = p3->yhthings;
                    while (p21 != NULL) {
                        if (strcmp(p21->name, p1->name) == 0) {
                            if (p22 == p21) {
                                p21 = p21->next;
                                p3->yhthings = p21;
                                free(p22);
                                flag = 1;
                                break;
                            } else {
                                p22->next = p21->next;
                                free(p21);
                                flag = 1;
                                break;
                            }
                        } else {
                            if (p21 == p22)
                                p21 = p22->next;
                            else {
                                p22 = p22->next;
                                p21 = p22->next;
                            }
                        }
                    }
                }
                printf("\n成功删除该物品信息.");
            }
            while (p1 != NULL) //释放things内存
            {
                if (p1->id == id) {
                    if (p1 == p11) head = p1->next;
                    else p11->next = p1->next;
                    sprintf(log_message, "thing_delete\nsuccessfully");
                    logging(p1->id, log_message);
                    free(p1);
                    thing_info.num--;
                    break;
                }
                p11 = p1;
                p1 = p1->next;
            }

        } else {
            printf("\n抱歉没有该物品~");
        }
        printf("\n请输入要删除物品的ID(8位学号,输入0为停止删除):");
        scanf("%d", &id);
    }
}


void input_adm()  //管理员录入
{
    struct adm adm1;
    char password[password_max_length] = {'\0'};
    printf("\n请输入管理员数据,输入0结束输入.");
    printf("\n请输入管理员ID(8位数字):");
    scanf("%d", &adm1.id);
    while (adm1.id != 0) {
        //initialize and create
        printf("\n请输入管理员姓名:");
        scanf("%s", adm1.name);
        printf("\n请输入管理员电话号码(11位手机号码):");
        scanf("%s", adm1.phone);
        printf("\n请输入管理员邮箱地址：");
        scanf("%s", adm1.email);
        printf("\n请输入管理员积分：");
        scanf("%d", &adm1.score);
        printf("\n请输入管理员密码:");
        password_input(password);
        password_encryption(password);
        strcpy(adm1.password, password);
        adm1.next = NULL;
        adm1.yhthings = NULL;
        insert_adm(adm1);
        sprintf(log_message, "admin_input\nname:%s, phone:%s, email:%s, score:%d", adm1.name, adm1.phone, adm1.email,
                adm1.score);
        logging(adm1.id, log_message);
        //select_n(p1);
        printf("\n请输入管理员ID(8位数字):");
        scanf("%d", &adm1.id);
    }
    system("cls");
}


void adm_ui_1()//管理员界面1
{
    int z;

    system("cls");
    do {
        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+            管理员            |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----录入物品信息       \n");
        printf("\t\t\t+    [2]----录入用户信息       \n");
        printf("\t\t\t+    [3]----录入管理员信息     \n");
        printf("\t\t\t+    [4]----查询物品信息       \n");
        printf("\t\t\t+    [5]----查询用户信息       \n");
        printf("\t\t\t+    [6]----删除物品信息       \n");
        printf("\t\t\t+    [7]----删除用户信息       \n");
        printf("\t\t\t+    [8]----录入用户物品信息   \n");
        printf("\t\t\t+    [9]----下一页             \n");
        printf("\t\t\t+    [0]----退出系统           \n");
        printf("\t\t\t--------------------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &z);
        switch (z) {
            case 0 :
                break;
            case 1 :
                input_thing();
                break;
            case 2 :
                input_yonghu();
                break;
            case 3 :
                input_adm();
                break;
            case 4 :
                skim_things(head);
                break;
            case 5 :
                skim_yonghu(head_yonghu);
                break;
            case 6 :
                del_things();
                break;
            case 7 :
                del_yonghu();
                break;
            case 8:
                input_yhthings();
                break;
            case 9 :
                adm_ui_2();
                break;
            default:
                printf("\n客官，没有这个选项呢!");
        }
    } while (z != 0);
    system("cls");
}


void adm_ui_2()//管理员界面2
{
    int z;
    system("cls");
    do {
        system("cls");
        printf("\n\t\t\t-----------------------------------\n");
        printf("\t\t\t+            管理员                |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+  [1]----修改物品信息              \n");
        printf("\t\t\t+  [2]----修改用户信息              \n");
        printf("\t\t\t+  [3]----统计所有物品信息           \n");
        printf("\t\t\t+  [4]----根据种类统计所有物品信息    \n");
        printf("\t\t\t+  [5]----统计所有用户信息           \n");
        printf("\t\t\t+  [6]----统计所有用户物品信息       \n");
        printf("\t\t\t+  [7]----管理员密码修改            \n");
        printf("\t\t\t+  [8]----显示日志                 \n");
        printf("\t\t\t+  [0]---- 上一页                  \n");
        printf("\t\t\t-----------------------------------\n");
        printf("请输入您的选择：");
        scanf("%d", &z);
        switch (z) {
            case 0 :
                break;
            case 1 :
                modify_thing_adm();
                break;
            case 2 :
                modify_yonghu_adm();
                break;
            case 3 :
                skim_thing_all();
                break;
            case 4 :
                show_thing_with_kind();
                break;
            case 5 :
                skim_yonghu_all();
                break;
            case 6:
                skim_yhthings1();
                break;
            case 7 :
                adm_password();
                break;
            case 8 :
                show_log();
                break;
            default:
                printf("\n对不起，没有这个选项!");
        }
    } while (z != 0);
    system("cls");
}


void skim_yonghu_score(struct yonghu *p)//按积分查看用户
{
    struct yonghu *p3;
    struct order *p11, *p1, *head11;
    p1 = list(p);
    head11 = p1;
    system("cls");
    while (p1 != NULL) {
        p3 = p1->pointer;
        printf("\n---------------------------------------------------------------------");
        printf("\n用户ID>>用户姓名>>用户积分>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        printf("\n---------------------------------------------------------------------");
        printf("\n%d  %s    %d\t", p3->id, p3->name, p3->score);
        p1 = p1->next;
    }
    p1 = head11;
    while (p1 != NULL) {
        p11 = p1;
        p1 = p1->next;
        free(p11);
    }
    getchar();
    getchar();
}


void buy(struct yonghu *p)//竞购自己想要的物品
{
    int id;
    int dd1, dd2, tt1, tt2;
    int flag;
    system("cls");
    time_t t;
    struct tm *p7;
    struct paimairen *p6, *p8, *p9;
    struct paimaipin *p3;
    printf("请输入您要竞购的物品id(输入0则返回上一层菜单）：");
    scanf("%d", &id);
    do {
        system("cls");
        flag = 0;
        p3 = head_pai;
        while (p3 != NULL && p3->id < id) {
            p3 = p3->next;
        }
        if (p3 != NULL) {
            printf("\n物品ID：  %d", p3->id);
            printf("\n物品名称：%s", p3->name);
            printf("\n物品朝代：%s", p3->time);
            printf("\n物品价格：%lf", p3->f_price);
            printf("\n物品介绍: %s\n", p3->introduce);
            printf("\n该物品开始竞拍时间：%d年%d月%d日 %d时%d分%d秒", p3->tb.year, p3->tb.mon, p3->tb.mday, p3->tb.hour, p3->tb.min,
                   p3->tb.sec);
            printf("\n该物品结束竞拍时间：%d年%d月%d日 %d时%d分%d秒", p3->te.year, p3->te.mon, p3->te.mday, p3->te.hour, p3->te.min,
                   p3->te.sec);
            printf("\n\t系统当前时间为：");
            t = time(NULL);
            p7 = localtime(&t);
            printf(ctime(&t));
            dd1 = (p7->tm_year + 1900) * 400 + (p7->tm_mon + 1) * 31 + (p7->tm_mday);
            dd2 = (p3->te.year) * 400 + (p3->te.mon) * 31 + (p3->te.mday);
            if (dd1 > dd2) flag = 1;
            else if (dd1 == dd2) {
                tt1 = (p7->tm_hour) * 100 + (p7->tm_min) * 60 + (p7->tm_sec);
                tt2 = (p3->te.hour) * 100 + (p3->te.min) * 60 + (p3->te.sec);
                if (tt1 > tt2) flag = 1;
            }
            dd1 = (p7->tm_year + 1900) * 400 + (p7->tm_mon + 1) * 31 + (p7->tm_mday);
            dd2 = (p3->tb.year) * 400 + (p3->tb.mon) * 31 + (p3->tb.mday);
            if (dd1 < dd2) flag = 2;
            else if (dd1 == dd2) {
                tt1 = (p7->tm_hour) * 100 + (p7->tm_min) * 60 + (p7->tm_sec);
                tt2 = (p3->tb.hour) * 100 + (p3->tb.min) * 60 + (p3->tb.sec);
                if (tt1 < tt2) flag = 2;
            }
            if (flag == 2) {
                sprintf(log_message, "buying\nfailed reason:can't buy ahead of time");
                logging(p3->id, log_message);
                printf("还不能竞拍此物品了呦~");
            } else if (flag == 1) {
                sprintf(log_message, "buying\nfailed reason:time up");
                logging(p3->id, log_message);
                printf("已经不能竞拍此物品了呦~");
            } else {
                p8 = (struct paimairen *) malloc(sizeof(struct paimairen));

                if (p3->ren == NULL) {
                    p9 = (struct paimairen *) malloc(sizeof(struct paimairen));
                    p9->id = p->id;
                    printf("请输入您的出价：");
                    scanf("%lf", &p9->price);
                    sprintf(log_message, "buying\nsuccessfully, now_price:%lf", p9->price);
                    logging(p9->id, log_message);
                    p3->ren = p9;
                    p9->next = NULL;
                } else {
                    p6 = p3->ren;
                    p8 = p3->ren;
                    while (p6 != NULL) {
                        if (p6->id == p->id) break;
                        p8 = p6;
                        p6 = p6->next;
                    }
                    if (p6 == NULL) {
                        p9 = (struct paimairen *) malloc(sizeof(struct paimairen));
                        p9->id = p->id;
                        printf("请输入您的出价：");
                        scanf("%lf", &p9->price);
                        sprintf(log_message, "buying\nsuccessfully,thing_id:%d , pre_price:%lf, now_price:%lf", p9->id,
                                p3->n_price, p9->price);
                        logging(p9->id, log_message);

                        p8->next = p9;
                        p6 = p9;
                        p6->next = NULL;
                    } else if (p6->id == p->id) {
                        printf("请输入您的出价：");
                        scanf("%lf", &p6->price);
                        sprintf(log_message, "buying\nsuccessfully,thing_id:%d , pre_price:%lf, now_price:%lf", p9->id,
                                p3->n_price, p9->price);
                        logging(p9->id, log_message);
                    }
                }
                printf("您的竞拍请求已经提交，请在竞拍结束后查看结果。");
            }
        }
        printf("请输入您要竞购的物品id(输入0则返回上一层菜单）：");
        scanf("%d", &id);
    } while (id != 0);
    system("cls");
    return;

}


void copy_yonghu(struct yonghu *p1, struct yonghu yonghu1)// 用于复制用户信息 减少代码量
{
    strcpy(p1->email, yonghu1.email);
    strcpy(p1->name, yonghu1.name);
    strcpy(p1->phone, yonghu1.phone);
    p1->score = yonghu1.score;
    strcpy(p1->password, yonghu1.password);
}


void checkpai(struct yonghu *p)//查看可以竞购的物品的相关信息
{
    struct paimaipin *p3;
    system("cls");
    p3 = head_pai;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>竞购物品信息>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p3 != NULL && p3->id1 != p->id) {
        printf("\n物品ID：  %d", p3->id);
        printf("\n物品名称：%s", p3->name);
        printf("\n物品朝代：%s", p3->time);
        printf("\n物品价格：%lf", p3->f_price);
        printf("\n物品介绍: %s\n", p3->introduce);
        p3 = p3->next;
    }
    getchar();
    getchar();
    system("cls");
}


void insert_yonghu(struct yonghu yonghu1) //用户结构体插入
{
    struct yonghu *p1, *p2, *p3;
    if (head_yonghu == NULL)  // 空表时
    {
        p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
        head_yonghu = p1;
        p1->id = yonghu1.id;
        copy_yonghu(p1, yonghu1);
        p1->next = NULL;
        yonghu_info.num++;
        p1->yhthings = NULL;
    } else {
        p2 = head_yonghu;
        p3 = head_yonghu;
        while (p2 != NULL) {
            if (p2->id >= yonghu1.id) break;
            p3 = p2;
            p2 = p2->next;
        }
        if (p2 == NULL || p2->id > yonghu1.id) {
            p2 = (struct yonghu *) head_yonghu;
            if (p2->id > yonghu1.id)  //在表头插入
            {
                p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
                p1->id = yonghu1.id;
                copy_yonghu(p1, yonghu1);
                p1->next = head_yonghu;
                p1->yhthings = NULL;
                yonghu_info.num++;
                head_yonghu = p1;
            } else //在表中或表尾插入
            {
                while (p2->id < yonghu1.id && p2->next != NULL) // 找到要插入的位置
                {
                    p3 = p2;
                    p2 = p2->next;
                }

                if (p2->next == NULL && p2->id < yonghu1.id) // 在表尾进行插入
                {
                    p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
                    p1->id = yonghu1.id;
                    copy_yonghu(p1, yonghu1);
                    p1->next = NULL;
                    p1->yhthings = NULL;
                    yonghu_info.num++;
                    p2->next = p1;
                } else //在表中进行插入
                {
                    p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
                    p1->id = yonghu1.id;
                    copy_yonghu(p1, yonghu1);
                    p1->next = p2;
                    p1->yhthings = NULL;
                    yonghu_info.num++;
                    p3->next = p1;
                }

            }
        } else printf("\n该用户已存在。");

    }
}


void copy_admin(struct adm *p1, struct adm adm1)// 用于复制管理员信息 减少代码量
{
    strcpy(p1->email, adm1.email);
    strcpy(p1->name, adm1.name);
    strcpy(p1->phone, adm1.phone);
    p1->score = adm1.score;
    strcpy(p1->password, adm1.password);
}


void insert_adm(struct adm adm1) //管理员结构体插入
{
    struct adm *p1, *p2, *p3;
    if (head_adm == NULL)  // 空表时
    {
        p1 = (struct adm *) malloc(sizeof(struct adm));
        head_adm = p1;
        p1->id = adm1.id;
        copy_admin(p1, adm1);
        p1->next = NULL;
        adm_info.num++;
        p1->yhthings = NULL;
    } else {
        p2 = head_adm;
        p3 = head_adm;
        while (p2 != NULL) {
            if (p2->id >= adm1.id) break;
            p3 = p2;
            p2 = p2->next;
        }
        if (p2 == NULL || p2->id > adm1.id) {
            p2 = (struct adm *) head_adm;
            if (p2->id > yonghu1.id)  //在表头插入
            {
                p1 = (struct adm *) malloc(sizeof(struct adm));
                p1->id = adm1.id;
                copy_admin(p1, adm1);
                p1->next = head_adm;
                p1->yhthings = NULL;
                adm_info.num++;
                head_adm = p1;
            } else //在表中或表尾插入
            {
                while (p2->id < adm1.id && p2->next != NULL) // 找到要插入的位置
                {
                    p3 = p2;
                    p2 = p2->next;
                }

                if (p2->next == NULL && p2->id < adm1.id) // 在表尾进行插入
                {
                    p1 = (struct adm *) malloc(sizeof(struct adm));
                    p1->id = adm1.id;
                    copy_admin(p1, adm1);
                    p1->next = NULL;
                    p1->yhthings = NULL;
                    adm_info.num++;
                    p2->next = p1;
                } else //在表中进行插入
                {
                    p1 = (struct adm *) malloc(sizeof(struct adm));
                    p1->id = adm1.id;
                    copy_admin(p1, adm1);
                    p1->next = p2;
                    p1->yhthings = NULL;
                    adm_info.num++;
                    p3->next = p1;
                }

            }
        } else printf("\n该管理员已存在。");

    }
}


void insert_yhthings1(struct yhthings yhthings1)  //将物品信息插入用户
{
    struct yhthings *p1, *p2, *p3;
    struct yonghu *p8;
    p8 = head_yonghu;
    while (p8->id != yhthings1.id1 && p8->next != NULL)  //寻找物品所属用户
    {
        p8 = p8->next;
    }
    if (p8->next == NULL && p8->id != yhthings1.id1) {
        printf("\n没有这个用户呢!信息插入不进去噻！");
    } else {
        p1 = (struct yhthings *) p8->yhthings;
        p3 = (struct yhthings *) p8->yhthings;
        while (p1 != NULL) //p3为前一个指针 p1为当前地址
        {
            if (p1->id == yhthings1.id) break;
            p3 = p1;
            p1 = p1->next;

        }
        if (p1 != NULL && p1->id == yhthings1.id) {
            p1->f_price = yhthings1.f_price;
            p1->n_price = yhthings1.n_price;
        } else {
            p2 = (struct yhthings *) malloc(sizeof(struct yhthings));
            p2->id = yhthings1.id;
            p2->id1 = yhthings1.id1;
            strcpy(p2->name, yhthings1.name);
            strcpy(p2->yonghu_name, p8->name);
            strcpy(p2->time, yhthings1.time);
            p2->f_price = yhthings1.f_price;
            p2->n_price = yhthings1.n_price;
            p2->next = NULL;
            if (p3 == NULL) p8->yhthings = p2;
            else p3->next = p2;
        }
    }
    return;
}


void insert_paimaipin1(struct paimaipin wupin1) {
    struct paimaipin *p3, *p4, *p5;
    if (head_pai == NULL)  // empty linked list
    {
        p3 = (struct paimaipin *) malloc(sizeof(struct paimaipin));
        head_pai = p3;
        copy_paimaipin(p3, wupin1);
        p3->next = NULL;
        p3->ren = NULL;
        paimaipin_info.num++;
    } else {
        p3 = head_pai;
        p4 = head_pai;
        while (p3 != NULL) {
            if (p3->id == wupin1.id) break;
            p4 = p3;
            p3 = p3->next;
        }
        if (p3 == NULL || p3->id > wupin1.id) {
            p3 = (struct paimaipin *) head_pai;
            if (p3->id > wupin1.id)  //insert at the head
            {
                p3 = (struct paimaipin *) malloc(sizeof(struct paimaipin));
                copy_paimaipin(p3, wupin1);
                p3->next = head_pai;
                p3->ren = NULL;
                paimaipin_info.num++;
                head_pai = p3;
            } else //insert at the middle or at the end
            {

                while (p3->id < wupin1.id && p3->next != NULL) // to find the point to insert
                {
                    if (p3->id == wupin1.id) break;
                    p4 = p3;
                    p3 = p3->next;
                }
                if (p3->next == NULL && p3->id < wupin1.id) // insert at the end
                {
                    p5 = (struct paimaipin *) malloc(sizeof(struct paimaipin));
                    copy_paimaipin(p5, wupin1);
                    p5->next = NULL;
                    p5->ren = NULL;
                    paimaipin_info.num++;
                    p3->next = p5;
                } else //insert at the middle
                {
                    p5 = (struct paimaipin *) malloc(sizeof(struct paimaipin));
                    copy_paimaipin(p5, wupin1);
                    p5->next = p3;
                    p5->ren = NULL;
                    paimaipin_info.num++;
                    p4->next = p5;
                }

            }
        } else printf("\n该拍卖物品已存在。");

    }
}


void jingpai(struct yonghu *p)//竞拍功能
{
    int z = 1;
    time_t t;
    do {
        t = time(NULL);
        system("cls");
        printf("\n\t系统当前时间为：");
        printf(ctime(&t));
        printf("\n\t\t     ****************************** ");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t         竞拍功能        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     ------------------------------  ");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (1)拍卖物品        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (2)竞购物品        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (3)拍卖结果        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (0)返回上一层      |*");
        printf("\n\t\t     ****************************** \n");
        printf("\n\t\t     请输入你的选择：");

        scanf("%d", &z);
        switch (z) {
            case 1:
                paimai(p);
                break;
            case 2:
                jinggou(p);
                break;
            case 3:
                pairesult(p);
            case 0:
                break;
            default:
                printf("\n无效选项哦~请重新输入~");

        }
    } while (z != 0);
    system("cls");
    return;


}


void paimai(struct yonghu *p)//用户拍卖物品
{
    system("cls");
    struct yhthings *p1, *p2;
    struct paimaipin wupin1;
    int id;
    time_t t;
    t = time(NULL);
    printf("\n\t系统当前时间为：");
    printf(ctime(&t));
    printf("\n请输入要拍卖的商品编号：");
    scanf("%d", &id);
    p1 = p->yhthings;
    p2 = p1;
    while (p2 != NULL) {
        if (p2->id == id) break;
        p2 = p2->next;
    }
    if (p2 != NULL) {
        wupin1.id = p2->id;
        wupin1.id1 = p2->id1;
        wupin1.id2 = p->id;
        strcpy(wupin1.name, p2->name);
        wupin1.f_price = p2->f_price;
        wupin1.n_price = p2->n_price;
        wupin1.next = NULL;
        printf("\n请设置拍卖的起始时间：");
        printf("\n年份 月份 日期:");
        scanf("%d%d%d", &wupin1.tb.year, &wupin1.tb.mon, &wupin1.tb.mday);
        printf("\n小时 分钟 秒：");
        scanf("%d%d%d", &wupin1.tb.hour, &wupin1.tb.min, &wupin1.tb.sec);
        printf("\n请设置拍卖的结束时间：");
        printf("\n年份 月份 日期:");
        scanf("%d%d%d", &wupin1.te.year, &wupin1.te.mon, &wupin1.te.mday);
        printf("\n小时 分钟 秒：");
        scanf("%d%d%d", &wupin1.te.hour, &wupin1.te.min, &wupin1.te.sec);
        wupin1.situ = 0;
        printf("\n请输入物品的简单介绍：");
        scanf("%s", wupin1.introduce);
        insert_paimaipin1(wupin1);
        sprintf(log_message, "auction_add\nname:%s", wupin1.name);
        logging(wupin1.id, log_message);
        printf("\n您的物品已经进入拍卖物品列表中，请于物品拍卖结束后查看物品结果。");

    } else {
        printf("\n您没有权力拍卖该物品或该物品不存在！");
        sprintf(log_message, "auction_add\nfailed  reason: unauthenticated or thing does not exist");
        printf("\n如仍需要拍卖请回到用户界面添加用户物品！");
    }
    system("cls");
    return;


}



//void pairesult2(struct yonghu *p){
//
//}



void pairesult(struct yonghu *p)//用户拍卖的物品的结果，并删除用户物品信息；
{

    struct paimaipin *p3, *p5;
    time_t t;
    int dd1, dd2, tt1, tt2, flag = 0;
    struct tm *p7;
    p3 = head_pai;
    p5 = head_pai;
    while (p3 != NULL) {
        if (p3->id2 == p->id) {
            flag = 0;
            printf("\n\t系统当前时间为：");
            t = time(NULL);
            p7 = localtime(&t);
            printf(ctime(&t));
            dd1 = (p7->tm_year + 1900) * 400 + (p7->tm_mon + 1) * 31 + (p7->tm_mday);
            dd2 = (p3->te.year) * 400 + (p3->te.mon) * 31 + (p3->te.mday);
            if (dd1 < dd2) flag = 1;
            else if (dd1 == dd2) {
                tt1 = (p7->tm_hour) * 100 + (p7->tm_min) * 60 + (p7->tm_sec);
                tt2 = (p3->te.hour) * 100 + (p3->te.min) * 60 + (p3->te.sec);
                if (tt1 < tt2) flag = 1;
            }
            if (flag) {
                printf("\n此物品的竞拍尚未结束，结果尚未知晓。");
            } else {
                double max;
                int winner;
                struct paimairen *p4;
                struct yhthings *p6, *p9;
                p4 = p3->ren;
                winner = p4->id;
                max = p4->price;
                p4 = p4->next;
                while (p4 != NULL) {
                    if (p4->price > max) {
                        max = p4->price;
                        winner = p4->id;
                    }
                    p4 = p4->next;
                }
                p6 = p->yhthings;
                p9 = p6;
                while (p6 != NULL) {
                    if (p6->id == p3->id) break;
                    p9 = p6;
                    p6 = p6->next;
                }
                if (p6 != NULL && p6->id == p3->id) {
                    p9->next = p6->next;
                    p->score += max;
                    free(p6);
                }
                p6 = head_yhthings;
                p9 = p6;
                while (p6 != NULL) {
                    if (p6->id == p3->id) break;
                    p9 = p6;
                    p6 = p6->next;
                }
                if (p6 != NULL && p6->id == p3->id) {
                    p9->next = p6->next;
                    free(p6);
                }
                printf("\n%d物品已被ID为%d用户以%lf元的价钱拍下~", p3->id, winner, max);
            }
        }
        p5 = p3;
        p3 = p3->next;
    }
    sleep(3);
    return;
}

void jinggou(struct yonghu *p)//竞购物品功能界面
{
    int z;
    system("cls");
    time_t t;
    do {
        t = time(NULL);
        printf("\n\t系统当前时间为：");
        printf(ctime(&t));
        printf("\n\t\t     ************************************ ");
        printf("\n\t\t     *|                                |* ");
        printf("\n\t\t     *|\t         竞购的时间到了        |*");
        printf("\n\t\t     *|                                |* ");
        printf("\n\t\t     -------------------------------------");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (1)查看可以竞购的物品   ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (2)竞购想要的物品       ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (3)查看竞购的结果       ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (0)返回上一层界面       ||*");
        printf("\n\t\t     ************************************ \n");
        printf("\n\t\t请输入你的选择：");

        scanf("%d", &z);
        switch (z) {
            case 1:
                checkpai(p);
                break;
            case 2:
                buy(p);
                break;
            case 3:
                result(p);
                break;
            case 0:
                break;
            default:
                printf("无效选项哦~请重新输入~");
                break;
        }
    } while (z != 0);
    system("cls");
    return;


}

void zuigaojia(struct paimaipin *p, struct yonghu *ppp)//获得竞购的商品结果，并完成用户的物品插入
{
    double max;
    int winner;
    int flag = 0;
    struct paimairen *p4;
    struct yonghu *p6;
    struct yhthings p8;
    struct yhthings *p9;
    p4 = p->ren;
    winner = p4->id;
    max = p4->price;
    p4 = p4->next;
    while (p4 != NULL) {
        if (p4->price > max) {
            max = p4->price;
            winner = p4->id;
        }
        p4 = p4->next;
    }
    if (winner != ppp->id) {
        printf("\n抱歉，您在此次竞购中失败了。该物品的拥有者是%d，以%lf元竞得。", winner, max);
        p6 = head_yonghu;
        while (p6 != NULL) {
            if (p6->id == winner) break;
            p6 = p6->next;
        }
        if (p6 != NULL) {
            p9 = p6->yhthings;
            while (p9 != NULL) //p3为前一个指针 p1为当前地址
            {
                if (p9->id == p->id) break;
                p9 = p9->next;

            }
            if (p9 != NULL && strcmp(p9->name, p->name) == 0) {
                flag = 1;
            }
            if (!flag) p6->score -= max;
            p8.id = p->id;
            p8.id1 = winner;
            strcpy(p8.name, p->name);
            strcpy(p8.time, p->time);
            strcpy(p8.yonghu_name, p6->name);
            p8.f_price = p->f_price;
            p8.n_price = max;
            insert_yhthings1(p8);

        }
    } else {
        p6 = head_yonghu;
        while (p6 != NULL) {
            if (p6->id == winner) break;
            p6 = p6->next;
        }
        if (p6 != NULL) {
            p9 = p6->yhthings;
            while (p9 != NULL) //p3为前一个指针 p1为当前地址
            {
                if (p9->id == p->id) break;
                p9 = p9->next;

            }
            if (p9 != NULL && strcmp(p9->name, p->name) == 0) {
                flag = 1;
            }
            if (!flag) p6->score -= max;
            p8.id = p->id;
            p8.id1 = ppp->id;
            strcpy(p8.name, p->name);
            strcpy(p8.time, p->time);
            strcpy(p8.yonghu_name, ppp->name);
            p8.f_price = p->f_price;
            p8.n_price = max;
            insert_yhthings1(p8);
            printf("\n该物品已被您竞得，竞得的价钱是：%lf\n", max);
        }
    }
    printf("5秒后自动退出本界面");
    sleep(5);
    system("cls");
    return;
}


void help()//帮助
{
    printf("\n\t\t***************************************");
    printf("\n\t\t*+*\t\t帮  助 \t\t*+*");
    printf("\n\t\t***************************************");
    printf("\n\t\t*+*(n(*≧▽≦*)n)");
    printf("\n\t\t*+*用户可以在任意界面输入0退出当前操作");
    printf("\n\t\t*+*登录后才能进行竞拍");
    printf("\n\t\t*+*未登录的用户请注册或者联系管理员进行相关信息登记");
    printf("\n\t\t*+*如果要将自己的物品挂上竞拍平台，请联系管理员");
    printf("\n\t\t****************************************");
    develop();
    getchar();
    getchar();
    system("cls");
}


void develop()//作者
{
    printf("\n\t\t       Ross");
    printf("\n\n2两秒后退出");
    sleep(2);
    system("cls");
}


int main() {
    int z;
    char ch;
    file_open();    //打开文件
    if (head_adm == NULL) {
        printf("目前无数据，是否录入?(y/n)\n");
        scanf("%c", &ch);
        if (ch == 'y') input_adm();
        else exit(0);
    }
    do {
        system("cls");
        cout << " \t__          ________ _      _____ ____  __  __ ______ \n";
        cout << " \t\\ \\        / /  ____| |    / ____/ __ \\|  \\/  |  ____|\n";
        cout << " \t \\ \\  /\\  / /| |__  | |   | |   | |  | | \\  / | |__   \n";
        cout << " \t  \\ \\/  \\/ / |  __| | |   | |   | |  | | |\\/| |  __|  \n";
        cout << " \t   \\  /\\  /  | |____| |___| |___| |__| | |  | | |____ \n";
        cout << " \t    \\/  \\/   |______|______\\_____\\____/|_|  |_|______|\n";
        cout << " \t                                                      \n";
        cout << " \t                                                      ";
        printf("\n\t\t\t+------------------------------+\n");
        printf("\t\t\t|   欢迎使用物品竞拍管理系统   |\n");
        printf("\t\t\t+------------------------------+\n");
        printf("\t    提示：为保证您的操作得到保存，请按正常顺序退出系统^_^\n");
        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+            主菜单            |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----用户登录           \n");
        printf("\t\t\t+    [2]----用户注册           \n");
        printf("\t\t\t+    [3]----管理员登录         \n");
        printf("\t\t\t+    [4]----帮助               \n");
        printf("\t\t\t+    [0]----退出系统           \n");
        printf("\t\t\t--------------------------------\n");
        printf("\n\t    请输入您的选择：");
        scanf("%d", &z);
        switch (z) {
            case 0 :
                break;
            case 1 :
                password_yonghu();
                break;
            case 2 :
                user_register();
                break;
            case 3 :
                password_adm();
                break;
            case 4 :
                help();
                break;
            default:
                printf("\n客官，没有这个选项噻!\n两秒后返回");
                sleep(2);

        }
    } while (z != 0);
    file_close();  // to free all the pointer and input data to file
}
