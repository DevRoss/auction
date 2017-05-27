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

struct things   //��Ʒ�ṹ�� ��š����֡���������̽��ܡ��۸�
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

struct yonghu  //  �û��ṹ�� ��š����������롢�绰���ʼ�������
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

struct adm    //  ����Ա�ṹ�� ��š����������롢�绰���ʼ�������
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

struct yhthings //�û���Ʒ�ṹ�� ��Ʒ��š���Ʒ���ơ���Ʒԭ�۸���Ʒ���ĺ�۸�
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

struct order  // Ϊ���û���������
{
    struct yonghu *pointer;
    struct order *next;
};

struct info //��Ϣ�ṹ��
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

void copy_things(struct things *p1, struct things thing1);// ���ڸ�����Ʒ��Ϣ
void copy_paimaipin(struct paimaipin *p1, struct paimaipin wupin1);// ���ڸ���������Ʒ��Ϣ ���ٴ�����
void buy(struct yonghu *p);//�����Լ���Ҫ����Ʒ
void checkpai(struct yonghu *p);//�鿴���Ծ�������Ʒ�������Ϣ
void del_things();//ɾ���û���Ϣ
void del_yhthings();//ɾ���û���Ʒ��Ϣ
void del_yonghu();//ɾ���û���Ϣ
void copy_yonghu(struct yonghu *p1, struct yonghu yonghu1);// ���ڸ����û���Ϣ
void insert_thing(struct things thing1); //��Ʒ�ṹ�����
void file_open();  //�������ļ��͹�������
void file_close();  //�ر��ļ�
void input_thing(); //��Ʒ��Ϣ¼��
void input_yonghu();//�û�¼��
void input_yhthings();//�û���Ʒ��Ϣ¼��
void insert_adm(struct adm adm1); //����Ա�ṹ�����
void insert_yhthings1(struct yhthings yhthings1); //����Ʒ��Ϣ�����û�
void insert_yhthings2(struct yhthings yhthings1); //����Ʒ��Ϣ�������Ա
void insert_paimaipin1(struct paimaipin wupin1); //��������Ʒ�ṹ�����
void jingpai(struct yonghu *p);//���Ĺ���
void jinggou(struct yonghu *p);//������Ʒ���ܽ���
void password_input(char *password);//���Ǻ���ʽ�������벢�ŵ���ַpassword��
void password_yonghu();//�û�ID��������֤
void password_adm();//����ԱID��������֤
int idc(char *id);  //idװ�� char ��int
void modify_thing_adm();// ����Ա�޸���Ʒ��Ϣ
void modify_yonghu(struct yonghu *p1);// �û��޸��Լ���Ϣ
void modify_yonghu_adm();//����Ա����ĳһ�û�ϵͳ �޸��û���Ϣ ������
void yonghu_ui(struct yonghu *p);//ѧ������
void input_adm();  //����Ա¼��
void adm_ui_1();//����Ա����1
void adm_ui_2();//����Ա����2
void adm_password();//����Ա�����޸�
void st_password_yonghu(struct yonghu *p);//�û������޸�
void paimai(struct yonghu *p);//�û�������Ʒ
void pairesult(struct yonghu *p);//�û���������Ʒ�Ľ������ɾ���û���Ʒ��Ϣ
void zuigaojia(struct paimaipin *p, struct yonghu *ppp);//��þ�������Ʒ�����������û�����Ʒ����
void show_thing_with_kind(); // �������鿴��Ʒ ����Ա����
void skim_yhthings(struct yonghu *p);//�鿴�����û���Ʒ��Ϣ
void pfree_yhthings(struct yonghu *p_yonghu, struct things *p_things, struct yhthings yhthings1);//�������ͷ��û���Ʒ�ڴ�
void skim_thing_all();//�鿴������Ʒ��Ϣ
void insert_yonghu(struct yonghu yonghu1); //�û��ṹ�����
void help();//����
void develop();//����
void password_encryption(char *password); //�������
void logging(int id, char *action); //��־��¼
void show_log();  //����Ա��ʾ��־
void user_register(); //�û�ע��

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
    strcpy(log_message, log_message_clear);   //�����־ʵ����Ϣ
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
    printf("��������˳�");
    getch();
    system("cls");
}


void password_input(char *password) {
    char c;
    int i = 0;
    printf("���������룺\n");
    while ((c = getch()) != 13 || i == 0) {
        system("cls");
        printf("���������룺\n");
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

void file_open()  //�������ļ��͹�������
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
            printf("\n�ļ���ʧ��");
        exit(0);
    } else  // open file successfully
    {
        fread(&thing_info, sizeof(struct info), 1, thing_file); // things
        fread(&yhthings_info, sizeof(struct info), 1, yhthings_file); // yhthings
        fread(&yonghu_info, sizeof(struct info), 1, yonghu_file); // yonghu
        fread(&adm_info, sizeof(struct info), 1, adm_file); // adm
        fread(&paimaipin_info, sizeof(struct info), 1, paimaipin_file); // paimaipin

        if (thing_info.num <= 0)//û��Ʒ���ݻ���û�г�ʼ��0

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
        // �������Ա�ṹ
        if (adm_info.num <= 0)// û����Ա���ݻ�û�г�ʼ��
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

        // �����û��ṹ
        if (yonghu_info.num <= 0)// û�û����ݻ�û�г�ʼ��
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
        // �����û���Ʒ�ṹ
        if (yhthings_info.num <= 0)// û�û���Ʒ���ݻ���û�г�ʼ��
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
        // ����������Ʒ�ṹ
        if (paimaipin_info.num <= 0 || head_pai == NULL)// û������Ʒ���ݻ���û�г�ʼ��
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


void file_close()  //�ر��ļ�,�����汣�����ļ�
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


void copy_things(struct things *p1, struct things thing1)// ���ڸ�����Ʒ��Ϣ ���ٴ�����
{
    p1->id = thing1.id;
    p1->type = thing1.type;
    p1->price = thing1.price;
    strcpy(p1->name, thing1.name);
    strcpy(p1->time, thing1.time);
    strcpy(p1->introduce, thing1.introduce);
}


void copy_paimaipin(struct paimaipin *p1, struct paimaipin wupin1)// ���ڸ���������Ʒ��Ϣ ���ٴ�����
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


void insert_thing(struct things thing1) //��Ʒ�ṹ�����
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
        } else printf("\n����Ʒ�Ѵ��ڡ�");

    }
}


void insert_yhthings2(struct yhthings yhthings1)  //���û���Ʒ��Ϣ�����û���Ʒ������
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
        } else printf("\n���û���Ʒ�Ѵ��ڡ�");

    }

}


void input_thing()  //��Ʒ��Ϣ¼��
{
    system("cls");
    struct things thing1;
    printf("\n��������Ʒ����,����0��������.");
    printf("\n��������ƷID(8λ����):");
    scanf("%d", &thing1.id);
    while (thing1.id != 0) {
        //initialize and create
        printf("\n��������Ʒ����:");
        scanf("%s", thing1.name);
        printf("\n��������Ʒ����(���ƴ�):");
        scanf("%s", thing1.time);
        printf("\n��������Ʒ�۸�");
        scanf("%lf", &thing1.price);
        printf("��������Ʒ���ͣ�������ƷΪ0��ʵ����ƷΪ1��");
        scanf("%d", &thing1.type);
        printf("\n��������Ʒ����(140������):");
        scanf("%s", thing1.introduce);
        thing1.next = NULL;
        thing1.yonghu = NULL;
        insert_thing(thing1);
        //select_n(p1);
        sprintf(log_message, "thing_input\nname:%s, time:%s, type:%d,", thing1.name, thing1.time,
                thing1.type); //��ʽ����־���
        logging(thing1.id, log_message);
        printf("\n��������ƷID(8λ����):");
        scanf("%d", &thing1.id);
    }
}


void input_yonghu()  //�û�¼��
{
    system("cls");
    struct yonghu yonghu1;
    char password[password_max_length] = {'\0'};
    char password2[password_max_length] = {'\0'};
    printf("\n�������û�����,����0��������.");
    printf("\n�������û�ID(8λ����):");
    scanf("%d", &yonghu1.id);
    while (yonghu1.id != 0) {
        printf("\n�������û�����:");
        scanf("%s", yonghu1.name);
        printf("\n�������û��绰����(11λ�ֻ�����):");
        scanf("%s", yonghu1.phone);
        printf("\n�������û��������ַ:");
        scanf("%s", yonghu1.email);
        while (1) {
            printf("\n�������û�����:");
            password_input(password);
            system("cls");
            password_input(password2);
            if ((strcmp(password, password2)) == 0) {
                password_encryption(password);
                strcpy(yonghu1.password, password);
                break;
            }
            printf("\n�������벻һ�������������롣����");
            strcpy(password, password_clear);
            strcpy(password2, password_clear);
        }
        printf("\n�������û�����:");
        scanf("%d", &yonghu1.score);
        yonghu1.next = NULL;
        yonghu1.yhthings = NULL;
        insert_yonghu(yonghu1);
        sprintf(log_message, "user_input\nname:%s, phone:%s, email:%s, score:%d", yonghu1.name, yonghu1.phone,
                yonghu1.email,
                yonghu1.score);
        logging(yonghu1.id, log_message);
        printf("\n�������û�ID(8λ����):");
        scanf("%d", &yonghu1.id);
    }
}

void user_register()  //�û�ע��
{
    system("cls");
    struct yonghu yonghu1;
    char password[password_max_length] = {'\0'};
    char password2[password_max_length] = {'\0'};
    printf("\n�������û�����,����0��������.");
    printf("\n�������û�ID(8λ����):");
    scanf("%d", &yonghu1.id);
    while (yonghu1.id != 0) {
        printf("\n�������û�����:");
        scanf("%s", yonghu1.name);
        printf("\n�������û��绰����(11λ�ֻ�����):");
        scanf("%s", yonghu1.phone);
        printf("\n�������û��������ַ:");
        scanf("%s", yonghu1.email);
        while (1) {
            printf("\n�������û�����:");
            password_input(password);
            printf("\n��ȷ��\n:");
            password_input(password2);
            if ((strcmp(password, password2)) == 0) {
                password_encryption(password);
                strcpy(yonghu1.password, password);
                break;
            }
            printf("\n�������벻һ�������������롣����");
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
        printf("\n�������û�ID(8λ����):");
        scanf("%d", &yonghu1.id);
    }
    system("cls");
}


void input_yhthings()  //�û���Ʒ��Ϣ¼��
{
    struct yhthings yhthings1;
    struct things *thing_p;
    int flag;
    //struct yhthings *cp;
    printf("\n�������û���Ʒ��Ϣ����,����0��������.");
    printf("\n������Ŀ����ƷID(8λ����):");
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
        if (flag) printf("\n����Ʒ��δ�Ǽǣ�������Ʒ��¼����ϸ��Ϣ���ٽ����û���Ʒ��¼��~");
        else {
            printf("\n������Ŀ���û�ID��");
            scanf("%d", &yhthings1.id1);
            printf("\n�������û���Ʒԭ�۸�:");
            scanf("%lf", &yhthings1.f_price);
            printf("\n�������û���Ʒ���ĺ�۸�:");
            scanf("%lf", &yhthings1.n_price);
            yhthings1.next = NULL;
            yhthings1.follow = NULL;
            insert_yhthings1(yhthings1);
            insert_yhthings2(yhthings1);
            sprintf(log_message, "user_ownership\nname:%s, price_before:%lf, price_now:%lf", thing_p->name,
                    yhthings1.f_price, yhthings1.n_price);
            logging(yhthings1.id, log_message);
        }

        printf("\n�������û���ƷID(8λ����):");
        scanf("%d", &yhthings1.id);
    }
}


void print_yonghu(struct yonghu *p1)  //����û���Ϣ
{
    struct yhthings *p2;
    printf("\n------------------------------------------------------------");
    printf("\nID>>>>>>>>����>>>>>>>>>����>>>>�ֻ�����>>>>>>>>�ʼ�>>>>>>>>>>>>>");
    printf("\n------------------------------------------------------------");
    printf("\n%-8d  %-10s   %-5d    %-11s    %s", p1->id, p1->name, p1->score, p1->phone, p1->email);
    if (p1->yhthings == NULL) {
        printf("\n����������Ʒ��Ϣ!");
    } else {
        p2 = (struct yhthings *) p1->yhthings;
        printf("\n------------------------------------------------------------");
        printf("\n��ƷID>>>>��Ʒ����>>>��Ʒԭ�۸�>>��Ʒ�ּ۸�>>>>>");
        printf("\n------------------------------------------------------------");
        while (p2 != NULL) {
            printf("\n%d   %12s   %.2lf     %.2lf", p2->id, p2->name, p2->f_price, p2->n_price);
            p2 = p2->next;
        }
    }
    getchar();
    getchar();
}


void print_things(struct things *p1)  //�����Ʒ
{
    printf("\n------------------------------------------------------------");
    printf("\nID>>>>>>>>����>>>>>>>>>����>>>>>�۸�>>>>>>>>>");
    printf("\n------------------------------------------------------------");
    printf("\n%-8d  %-10s  %-7s   %lf", p1->id, p1->name, p1->time, p1->price);
    printf("\n��Ʒ��̽���>>>>>>>>>>>>>>>>>>>");
    printf("\n%s", p1->introduce);
    getchar();
    getchar();
}


void skim_things(struct things *head)   //��ID�����ֲ鿴��Ʒ��Ϣ
{
    char id[60] = {'\0'};
    int id1;
    bool is_num = 1;
    struct things *p1;
    system("cls");
    if (head == NULL) {
        printf("\nĿǰ����Ʒ����,�뷵��¼��.\n");
    } else {
        printf("\n������Ҫ��ѯ����ƷID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
        scanf("%s", id);
        while (id[0] != '0') {
            p1 = head;
            for (int i = 0; i < strlen(id); i++) {
                if (!('0' <= id[i] && id[i] <= '9')) {
                    is_num = 0;
                    break;
                }
            }
            if (is_num) {    //�����id
                id1 = idc(id);
                while (p1->id != id1 && p1->next != NULL && p1->id < id1) {
                    p1 = p1->next;
                }
                if ((p1->next == NULL && p1->id != id1) || p1->id > id1) {
                    printf("\nû��������ƷŶ~������������ƷID(8λ����,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                } else {
                    print_things(p1);
                    printf("\n������ѯ���ٴ�������ƷID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                }
            } else {
                while (p1 != NULL && strcmp(id, p1->name) != 0)    //���ж��Ƿ�������β�����߲�ѯ�����ǻ������
                    p1 = p1->next;
                if (p1 == NULL) {
                    printf("\nû��������ƷŶ!������������ƷID(8λ����,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                } else {
                    print_things(p1);
                    printf("\n������ѯ���ٴ�������ƷID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
                    fflush(stdin);
                    scanf("%s", id);
                }

            }
        }
    }
}


int idc(char *id)  //idת�� char ��int
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


void password_yonghu()//�û�ID��������֤
{
    int z = 0;
    char password1[password_max_length] = {'\0'};
    int id;
    struct yonghu *p;
    system("cls");
    do {
        printf("\n\t\t\t|&|*************�û���¼**************|&|");
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

        printf("\n�������û�ID(���8λ���֣�0Ϊ�˳�):");
        scanf("%d", &id);
        fflush(stdin);
        if (id == 0)
            return;
        printf("\n����������(24λ����):");
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
                printf("\n�˺Ż�������������������(�㻹��%d�λ���)��", 5 - z);
                sprintf(log_message, "user_login\nlogin failed");
                logging(p->id, log_message);
                getchar();
                fflush(stdin);
            }
        } else {
            printf("���û������ڡ�");
            break;
        }
    } while (z != 5);
}


void password_adm()//����ԱID��������֤
{
    int z = 0;
    char password1[password_max_length] = {'\0'};
    int id;
    struct adm *p;
    system("cls");
    do {
        printf("\n\t\t\t|&|*************�û���¼**************|&|");
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
        printf("\n���������ԱID(8λ���֣�0Ϊ�˳�):");
        scanf("%d", &id);
        fflush(stdin);
        if (id == 0)
            return;
        printf("\n����������(24λ����):");
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
                printf("\n�˺Ż�������������������(�㻹��%d�λ���)��", 5 - z);
                getchar();
                fflush(stdin);
            }
        } else {
            printf("\n�Բ��𣬸�ID�����ڣ����֤�������롣");
            break;
        }
    } while (z != 5);

}


void modify_yonghu(struct yonghu *p1)// �û��޸��Լ���Ϣ
{
    int z;
    system("cls");
    printf("\n\t\t\t**************************");
    printf("\n\t\t\t**\t�޸���Ŀ\t**");
    printf("\n\t\t\t**************************");
    printf("\n\t\t\t**\t(1)�ʼ�    \t**");
    printf("\n\t\t\t**\t(2)�绰����\t**");
    printf("\n\t\t\t**************************");
    printf("\n����������(����0ֹͣ�޸�):");
    scanf("%d", &z);
    switch (z) {
        case 0:
            break;
        case 1:
            printf("�������ʼ�:");
            scanf("%s", p1->email);
            sprintf(log_message, "modify_user\nchange email to%s", p1->email);
            logging(p1->id, log_message);
            fflush(stdin);
            break;
        case 2:
            printf("������绰����:");
            scanf("%s", p1->phone);
            sprintf(log_message, "modify_user\nchange phone to%s", p1->phone);
            logging(p1->id, log_message);
            fflush(stdin);
            break;
        default :
            printf("\n����ȷ������Ӧ���֡�");
            break;
    }
    printf("\n������޸ġ�����������ء�");
    getchar();
    getchar();
    system("cls");
}


void modify_yonghu_adm()//����Ա����ĳһ�û�ϵͳ �޸��û���Ϣ ������
{
    struct yonghu *p1;
    int id;
    p1 = head_yonghu;
    system("cls");
    printf("\n������Ҫ�޸��û���ID(8λid,����0ֹͣ����):");
    scanf("%d", &id);
    fflush(stdin);
    while (p1 != NULL) {
        if (p1->id >= id) break; //���ټ�������
        p1 = p1->next;
    }
    if (p1 != NULL) {
        if (p1->id == id) yonghu_ui(p1);
        sprintf(log_message, "admin_modify_user_info\n");
        logging(p1->id, log_message);
    } else
        printf("\n��λ�û���û���ֹ���~");
}


void del_yonghu()//ɾ���û���Ϣ
{
    struct yonghu *p1, *p11, *p31;
    struct yhthings *p2, *p22;
    struct things *p3;
    int id;
    system("cls");
    printf("\n������Ҫɾ���û���ID(8λID,����0Ϊֹͣɾ��):");
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
            printf("\n�ɹ�ɾ�����û���Ϣ.");

        }

        p1 = p11 = head_yonghu;
        while (p1 != NULL) //�ͷ��û��ڴ�
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
        printf("\n������Ҫɾ���û���ID(8λID,����0Ϊֹͣɾ��):");
        scanf("%d", &id);
    }
}


void del_yhthings()//ɾ���û���Ʒ��Ϣ
{
    struct yhthings yhthings1;
    struct yonghu *p_yonghu;
    struct things *p, *p_things;
    system("cls");
    printf("\n�������û���Ʒ������:");
    scanf("%s", yhthings1.name);
    fflush(stdin);
    printf("\n�������û���Ʒ��ID:");
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
        printf("\n�������û���Ʒ������:");
        scanf("%s", yhthings1.name);
        printf("\n�������û���Ʒ��id:");
        scanf("%d", &yhthings1.id);
    }
}


void pfree_yhthings(struct yonghu *p_yonghu, struct things *p_things, struct yhthings yhthings1)//�������ͷ��û���Ʒ�ڴ�
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


void skim_yonghu(struct yonghu *head)   //��ID�����ֲ鿴�û���Ϣ
{
    char id[60];
    bool is_num = 1;
    int id1;
    struct yonghu *p1;
    system("cls");
    if (head == NULL) {
        printf("\nĿǰ���û�����,�뷵��¼��.\n");
    } else {
        printf("\n������Ҫ��ѯ���û�ID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
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
                    printf("\n���޴��û�!�����������û�ID(8λ����,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                } else {
                    print_yonghu(p1);
                    printf("\n������ѯ���ٴ������û�ID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                }
            } else {
                while (p1 != NULL && strcmp(id, p1->name) != 0) {
                    p1 = p1->next;
                }
                if (p1 != NULL) {
                    print_yonghu(p1);
                    printf("\n������ѯ���ٴ������û�ID������(8λ���ֻ��ߺ���,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                } else {
                    printf("\n���޴��û�!�����������û�ID(8λ����,����0ֹͣ��ѯ):");
                    scanf("%s", id);
                }
            }

        }
    }

}


void skim_yonghu_all()//�鿴�����û����ּ���ӵ����Ʒ
{
    struct yonghu *p1;
    struct yhthings *p2;
    system("cls");
    p1 = head_yonghu;
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>�û�����>>>>>>>����>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n%d  %s  \t  %d", p1->id, p1->name, p1->score);
        p2 = p1->yhthings;
        if (p2 == NULL) printf("\n���û�û�и�����Ʒ��");
        else {
            printf("\n���û�����Ʒ���£�");
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


void adm_password()//����Ա�����޸�
{
    int z = 0;
    struct adm *p;
    int ID;
    char password0[password_max_length] = {'\0'};
    char password2[password_max_length]{'\0'};
    system("cls");
    do {
        printf("\n���������ԱID��8λ���֣���");
        scanf("%d", &ID);
        printf("\n������ԭ���루10λ���ڣ�:");
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
                printf("\n�����������������(�㻹��%d�λ���).", 5 - z);
            }
        }

    } while (z != 5);
    if (z != 5) {
        do {
            printf("\n������������:");
            password_input(password0);
            fflush(stdin);
            printf("\n���ٴ�����������:");
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
                printf("\n�������벻һ�£�����������,�����޸�������0,�����޸�������1.");
                scanf("%d", &z);
            }
        } while (z != 0);
        system("cls");
    } else exit(0);
}


struct order *list(struct yonghu *head)// �û��������� ��������ͷ   Ϊ�˽����û�������
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


void yonghu_ui(struct yonghu *p)//�û�����
{
    int z;
    system("cls");
    do {

        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+        %s,��ӭ��          |\n", p->name);
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----�޸ĸ�����Ϣ       |\n");
        printf("\t\t\t+    [2]----���뾺��           |\n");
        printf("\t\t\t+    [3]----��ѯ�����û���Ϣ   |\n");
        printf("\t\t\t+    [4]----�޸�����           |\n");
        printf("\t\t\t+    [5]----�鿴������Ʒ��Ϣ   |\n");
        printf("\t\t\t+    [0]----����������         |\n");
        printf("\t\t\t--------------------------------\n");
        printf("����������ѡ��");
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
                printf("\n��Чѡ��!");
        }
    } while (z != 0);
    system("cls");
}


void st_password_yonghu(struct yonghu *p)//�û������޸�
{
    char password1[11] = {'\0'};
    char password2[11] = {'\0'};
    int z = 1;
    system("cls");
    do {
        printf("\n������������:");
        scanf("%s", password1);
        fflush(stdin);
        printf("\n���ٴ�����������:");
        scanf("%s", password2);
        fflush(stdin);
        if (strcmp(password1, password2) == 0) {
            password_encryption(password1);
            strcpy(p->password, password1);
            sprintf(log_message, "user_password_changing\nsuccessfully");
            logging(p->id, log_message);
            printf("\n�����޸ĳɹ�!");
            break;
        } else {
            printf("\n�������벻һ�£�����������,�����޸�������0,�����޸��������κ�����ֵ.");
            sprintf(log_message,
                    "user_password_changing\nfailed  reason:new passwords ard not the same in confirmation");
            logging(p->id, log_message);
            scanf("%d", &z);
            fflush(stdin);
        }
    } while (z != 0);
    system("cls");
}


void skim_thing_all()//�鿴������Ʒ��Ϣ
{
    struct things *p1;
    system("cls");
    p1 = head;
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>��Ʒ��Ϣ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n��ƷID��  %d", p1->id);
        printf("\n��Ʒ���ƣ�%s", p1->name);
        printf("\n��Ʒ������%s", p1->time);
        printf("\n��Ʒ�۸�%lf", p1->price);
        printf("\n��Ʒ����: %s\n", p1->introduce);
        if (p1->type == 1)
            printf("\n��Ʒ����: ʵ��");
        else printf("\n��Ʒ����: ����");
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void show_thing_with_kind()//�鿴������Ʒ��Ϣ
{
    struct things *p1;
    system("cls");
    p1 = head;
    int type;
    printf("��������Ʒ���ͣ�0Ϊ������Ʒ��1Ϊʵ����Ʒ");
    scanf("%d", &type);
    printf("\n-------------------------------------------------------------------");
    printf("\nID>>>>>��Ʒ��Ϣ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        if (type == p1->type) {
            printf("\n��ƷID��  %d", p1->id);
            printf("\n��Ʒ���ƣ�%s", p1->name);
            printf("\n��Ʒ������%s", p1->time);
            printf("\n��Ʒ�۸�%lf", p1->price);
            printf("\n��Ʒ����: %s\n", p1->introduce);
            if (p1->type == 1)
                printf("\n��Ʒ����: ʵ��");
            else printf("\n��Ʒ����: ����");
        }
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}


void skim_yhthings(struct yonghu *p)//�鿴�����û���Ʒ��Ϣ
{
    struct yhthings *p1;
    system("cls");
    p1 = p->yhthings;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>>>>>>>>>>>>>>>>>�û���Ʒ��Ϣ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n\t\t��ƷID��  %d", p1->id);
        printf("\n\t\t��Ʒ���ƣ�%s", p1->name);
        printf("\n\t\t��Ʒ������%s", p1->time);
        printf("\n\t\t��Ʒ�۸�%lf\n", p1->n_price);
        p1 = p1->next;
    }
    getchar();
    getchar();
    system("cls");
}

void skim_yhthings1()//�鿴�����û���Ʒ��Ϣ
{
    struct yhthings *p1;
    system("cls");
    p1 = head_yhthings;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>>>>>>>>>>>>>>>>>�û���Ʒ��Ϣ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p1 != NULL) {
        printf("\n\t\t��ƷID��  %d", p1->id);
        printf("\n\t\t������ID��  %d", p1->id1);
        printf("\n\t\t���������֣�  %s", p1->yonghu_name);
        printf("\n\t\t��Ʒ���ƣ�%s", p1->name);
        printf("\n\t\t��Ʒ������%s", p1->time);
        printf("\n\t\t��Ʒ�۸�%lf\n", p1->n_price);
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
                printf("\n\tϵͳ��ǰʱ��Ϊ��");
                t = time(NULL);
                p7 = localtime(&t);
                printf(ctime(&t));
                printf("\n��ƷID��  %d", p3->id);
                printf("\n��Ʒ���ƣ�%s", p3->name);
                printf("\n��Ʒ������%s", p3->time);
                printf("\n��Ʒ�۸�%lf", p3->f_price);
                printf("\n��Ʒ����: %s\n", p3->introduce);
                printf("\n����Ʒ��ʼ����ʱ�䣺%d��%d��%d�� %dʱ%d��%d��", p3->tb.year, p3->tb.mon, p3->tb.mday, p3->tb.hour, p3->tb.min,
                       p3->tb.sec);
                printf("\n����Ʒ��������ʱ�䣺%d��%d��%d�� %dʱ%d��%d��", p3->te.year, p3->te.mon, p3->te.mday, p3->te.hour, p3->te.min,
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
                    printf("\n���Ļ�δ��������ȴ����Ľ�����鿴�����");
                } else {
                    zuigaojia(p3, p);
                }
            }
            p4 = p4->next;
        }
        p3 = p3->next;
    }
    if (!flag) printf("\nû������뾺������ƷŶ~");
    return;

}

void modify_thing_adm()// ����Ա�޸���Ʒ��Ϣ
{
    struct things *p1;
    int z;
    int id1 = 0;
    system("cls");
    printf("\n\t\t\t������Ҫ�޸ĵ���Ʒid��");
    scanf("%d", &id1);
    p1 = head;
    while (p1 != NULL) {
        if (p1->id >= id1) break;
        p1 = p1->next;
    }
    if (p1 != NULL) {
        if (p1->id == id1) {
            printf("\n\t\t\t**************************");
            printf("\n\t\t\t**\t�޸���Ŀ\t**");
            printf("\n\t\t\t**************************");
            printf("\n\t\t\t**\t(1)����    \t**");
            printf("\n\t\t\t**\t(2)����    \t**");
            printf("\n\t\t\t**\t(3)�۸�    \t**");
            printf("\n\t\t\t**\t(4)��̽���\t**");
            printf("\n\t\t\t**\t(5)��Ʒ����\t**");
            printf("\n\t\t\t**************************");
            printf("\n����������(����0ֹͣ�޸�):");
            scanf("%d", &z);
            switch (z) {
                case 0:
                    break;
                case 1:
                    printf("������������:");
                    scanf("%s", p1->name);
                    sprintf(log_message, "thing_name_changing\n new_name:%s", p1->name);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 2:
                    printf("�����볯����");
                    scanf("%s", p1->time);
                    sprintf(log_message, "thing_time_changing\n new_time:%s", p1->time);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 3:
                    printf("������۸�");
                    scanf("%lf", &p1->price);
                    sprintf(log_message, "thing_price_changing\n new_price:%lf", p1->price);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 4:
                    printf("�������½��ܣ�");
                    scanf("%s", p1->introduce);
                    sprintf(log_message, "thing_introduction_changing\n successfully");
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                case 5:
                    printf("����������Ʒ����,������ƷΪ0��ʵ����ƷΪ1��");
                    scanf("%d", &p1->type);
                    sprintf(log_message, "thing_type_changing\n new_price:%d", p1->type);
                    logging(p1->id, log_message);
                    fflush(stdin);
                    break;
                default :
                    printf("\n����ȷ������Ӧ���֡�");
                    break;
            }
            printf("\n������޸ġ�����������ء�");
            getchar();
            getchar();
            system("cls");
        } else
            printf("����Ʒ�����ڡ�");
    }
}


void skim_thing_kind() // �������鿴��Ʒ ����Ա����
{
    struct things *p1, *p;
    int id1, id2;
    int choice;
    int n = 0;
    p1 = head;
    do {
        system("cls");
        printf("\n��ѡ������Ҫ��ѯ�����");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("��������Ʒ������");
                scanf("%s", p->time);
                printf("\n-------------------------------------------------------------------");
                printf("\nID>>>��Ʒ����>>>��Ʒ����>>>��Ʒ�۸�>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                printf("\n-------------------------------------------------------------------");
                p1 = head;
                n = 0;
                while (p1 != NULL) {
                    if (strcmp(p1->time, p->time) == 0) //Ѱ��ͬһ��������Ʒ
                    {
                        printf("\n%s\t%s\t%lf\t", p1->name, p1->time, p1->price);
                        n = 1;  // �а༶���� n��Ϊ�ж���
                    }
                    p1 = p1->next;
                }
                if (n == 0)
                    printf("\n�޸ó�������Ʒ");
                getchar();
                getchar();
                break;
            case 2:
                printf("��������Ʒ�۸����䣺");
                scanf("%d%d", &id1, &id2);
                printf("\n-------------------------------------------------------------------");
                printf("\nID>>>��Ʒ����>>>��Ʒ����>>>��Ʒ�۸�>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                printf("\n-------------------------------------------------------------------");
                p1 = head;
                n = 0;
                while (p1 != NULL) {
                    if (p1->price < id2 && p1->price > id1) //Ѱ��ͬһ��������Ʒ
                    {
                        printf("\n%s\t%s\t%lf\t", p1->name, p1->time, p1->price);
                        n = 1;  // ����Ʒ���� n��Ϊ�ж���
                    }
                    p1 = p1->next;
                }
                if (n == 0)
                    printf("\n�޸ó�������Ʒ");
                getchar();
                getchar();
                break;
            case 0:
                break;
        }
    } while (choice != 0);


}


void del_things()//ɾ���û���Ϣ
{
    struct things *p1, *p11;
    struct yonghu *p2, *p3;
    struct yhthings *p21, *p22;
    int id;
    int flag = 0;
    system("cls");
    printf("\n������Ҫɾ����Ʒ��ID(8λ,����0Ϊֹͣɾ��):");
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
                printf("\n�ɹ�ɾ������Ʒ��Ϣ.");
            }
            while (p1 != NULL) //�ͷ�things�ڴ�
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
            printf("\n��Ǹû�и���Ʒ~");
        }
        printf("\n������Ҫɾ����Ʒ��ID(8λѧ��,����0Ϊֹͣɾ��):");
        scanf("%d", &id);
    }
}


void input_adm()  //����Ա¼��
{
    struct adm adm1;
    char password[password_max_length] = {'\0'};
    printf("\n���������Ա����,����0��������.");
    printf("\n���������ԱID(8λ����):");
    scanf("%d", &adm1.id);
    while (adm1.id != 0) {
        //initialize and create
        printf("\n���������Ա����:");
        scanf("%s", adm1.name);
        printf("\n���������Ա�绰����(11λ�ֻ�����):");
        scanf("%s", adm1.phone);
        printf("\n���������Ա�����ַ��");
        scanf("%s", adm1.email);
        printf("\n���������Ա���֣�");
        scanf("%d", &adm1.score);
        printf("\n���������Ա����:");
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
        printf("\n���������ԱID(8λ����):");
        scanf("%d", &adm1.id);
    }
    system("cls");
}


void adm_ui_1()//����Ա����1
{
    int z;

    system("cls");
    do {
        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+            ����Ա            |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----¼����Ʒ��Ϣ       \n");
        printf("\t\t\t+    [2]----¼���û���Ϣ       \n");
        printf("\t\t\t+    [3]----¼�����Ա��Ϣ     \n");
        printf("\t\t\t+    [4]----��ѯ��Ʒ��Ϣ       \n");
        printf("\t\t\t+    [5]----��ѯ�û���Ϣ       \n");
        printf("\t\t\t+    [6]----ɾ����Ʒ��Ϣ       \n");
        printf("\t\t\t+    [7]----ɾ���û���Ϣ       \n");
        printf("\t\t\t+    [8]----¼���û���Ʒ��Ϣ   \n");
        printf("\t\t\t+    [9]----��һҳ             \n");
        printf("\t\t\t+    [0]----�˳�ϵͳ           \n");
        printf("\t\t\t--------------------------------\n");
        printf("����������ѡ��");
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
                printf("\n�͹٣�û�����ѡ����!");
        }
    } while (z != 0);
    system("cls");
}


void adm_ui_2()//����Ա����2
{
    int z;
    system("cls");
    do {
        system("cls");
        printf("\n\t\t\t-----------------------------------\n");
        printf("\t\t\t+            ����Ա                |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+  [1]----�޸���Ʒ��Ϣ              \n");
        printf("\t\t\t+  [2]----�޸��û���Ϣ              \n");
        printf("\t\t\t+  [3]----ͳ��������Ʒ��Ϣ           \n");
        printf("\t\t\t+  [4]----��������ͳ��������Ʒ��Ϣ    \n");
        printf("\t\t\t+  [5]----ͳ�������û���Ϣ           \n");
        printf("\t\t\t+  [6]----ͳ�������û���Ʒ��Ϣ       \n");
        printf("\t\t\t+  [7]----����Ա�����޸�            \n");
        printf("\t\t\t+  [8]----��ʾ��־                 \n");
        printf("\t\t\t+  [0]---- ��һҳ                  \n");
        printf("\t\t\t-----------------------------------\n");
        printf("����������ѡ��");
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
                printf("\n�Բ���û�����ѡ��!");
        }
    } while (z != 0);
    system("cls");
}


void skim_yonghu_score(struct yonghu *p)//�����ֲ鿴�û�
{
    struct yonghu *p3;
    struct order *p11, *p1, *head11;
    p1 = list(p);
    head11 = p1;
    system("cls");
    while (p1 != NULL) {
        p3 = p1->pointer;
        printf("\n---------------------------------------------------------------------");
        printf("\n�û�ID>>�û�����>>�û�����>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
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


void buy(struct yonghu *p)//�����Լ���Ҫ����Ʒ
{
    int id;
    int dd1, dd2, tt1, tt2;
    int flag;
    system("cls");
    time_t t;
    struct tm *p7;
    struct paimairen *p6, *p8, *p9;
    struct paimaipin *p3;
    printf("��������Ҫ��������Ʒid(����0�򷵻���һ��˵�����");
    scanf("%d", &id);
    do {
        system("cls");
        flag = 0;
        p3 = head_pai;
        while (p3 != NULL && p3->id < id) {
            p3 = p3->next;
        }
        if (p3 != NULL) {
            printf("\n��ƷID��  %d", p3->id);
            printf("\n��Ʒ���ƣ�%s", p3->name);
            printf("\n��Ʒ������%s", p3->time);
            printf("\n��Ʒ�۸�%lf", p3->f_price);
            printf("\n��Ʒ����: %s\n", p3->introduce);
            printf("\n����Ʒ��ʼ����ʱ�䣺%d��%d��%d�� %dʱ%d��%d��", p3->tb.year, p3->tb.mon, p3->tb.mday, p3->tb.hour, p3->tb.min,
                   p3->tb.sec);
            printf("\n����Ʒ��������ʱ�䣺%d��%d��%d�� %dʱ%d��%d��", p3->te.year, p3->te.mon, p3->te.mday, p3->te.hour, p3->te.min,
                   p3->te.sec);
            printf("\n\tϵͳ��ǰʱ��Ϊ��");
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
                printf("�����ܾ��Ĵ���Ʒ����~");
            } else if (flag == 1) {
                sprintf(log_message, "buying\nfailed reason:time up");
                logging(p3->id, log_message);
                printf("�Ѿ����ܾ��Ĵ���Ʒ����~");
            } else {
                p8 = (struct paimairen *) malloc(sizeof(struct paimairen));

                if (p3->ren == NULL) {
                    p9 = (struct paimairen *) malloc(sizeof(struct paimairen));
                    p9->id = p->id;
                    printf("���������ĳ��ۣ�");
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
                        printf("���������ĳ��ۣ�");
                        scanf("%lf", &p9->price);
                        sprintf(log_message, "buying\nsuccessfully,thing_id:%d , pre_price:%lf, now_price:%lf", p9->id,
                                p3->n_price, p9->price);
                        logging(p9->id, log_message);

                        p8->next = p9;
                        p6 = p9;
                        p6->next = NULL;
                    } else if (p6->id == p->id) {
                        printf("���������ĳ��ۣ�");
                        scanf("%lf", &p6->price);
                        sprintf(log_message, "buying\nsuccessfully,thing_id:%d , pre_price:%lf, now_price:%lf", p9->id,
                                p3->n_price, p9->price);
                        logging(p9->id, log_message);
                    }
                }
                printf("���ľ��������Ѿ��ύ�����ھ��Ľ�����鿴�����");
            }
        }
        printf("��������Ҫ��������Ʒid(����0�򷵻���һ��˵�����");
        scanf("%d", &id);
    } while (id != 0);
    system("cls");
    return;

}


void copy_yonghu(struct yonghu *p1, struct yonghu yonghu1)// ���ڸ����û���Ϣ ���ٴ�����
{
    strcpy(p1->email, yonghu1.email);
    strcpy(p1->name, yonghu1.name);
    strcpy(p1->phone, yonghu1.phone);
    p1->score = yonghu1.score;
    strcpy(p1->password, yonghu1.password);
}


void checkpai(struct yonghu *p)//�鿴���Ծ�������Ʒ�������Ϣ
{
    struct paimaipin *p3;
    system("cls");
    p3 = head_pai;
    printf("\n-------------------------------------------------------------------");
    printf("\n>>>>>������Ʒ��Ϣ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    printf("\n-------------------------------------------------------------------");
    while (p3 != NULL && p3->id1 != p->id) {
        printf("\n��ƷID��  %d", p3->id);
        printf("\n��Ʒ���ƣ�%s", p3->name);
        printf("\n��Ʒ������%s", p3->time);
        printf("\n��Ʒ�۸�%lf", p3->f_price);
        printf("\n��Ʒ����: %s\n", p3->introduce);
        p3 = p3->next;
    }
    getchar();
    getchar();
    system("cls");
}


void insert_yonghu(struct yonghu yonghu1) //�û��ṹ�����
{
    struct yonghu *p1, *p2, *p3;
    if (head_yonghu == NULL)  // �ձ�ʱ
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
            if (p2->id > yonghu1.id)  //�ڱ�ͷ����
            {
                p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
                p1->id = yonghu1.id;
                copy_yonghu(p1, yonghu1);
                p1->next = head_yonghu;
                p1->yhthings = NULL;
                yonghu_info.num++;
                head_yonghu = p1;
            } else //�ڱ��л��β����
            {
                while (p2->id < yonghu1.id && p2->next != NULL) // �ҵ�Ҫ�����λ��
                {
                    p3 = p2;
                    p2 = p2->next;
                }

                if (p2->next == NULL && p2->id < yonghu1.id) // �ڱ�β���в���
                {
                    p1 = (struct yonghu *) malloc(sizeof(struct yonghu));
                    p1->id = yonghu1.id;
                    copy_yonghu(p1, yonghu1);
                    p1->next = NULL;
                    p1->yhthings = NULL;
                    yonghu_info.num++;
                    p2->next = p1;
                } else //�ڱ��н��в���
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
        } else printf("\n���û��Ѵ��ڡ�");

    }
}


void copy_admin(struct adm *p1, struct adm adm1)// ���ڸ��ƹ���Ա��Ϣ ���ٴ�����
{
    strcpy(p1->email, adm1.email);
    strcpy(p1->name, adm1.name);
    strcpy(p1->phone, adm1.phone);
    p1->score = adm1.score;
    strcpy(p1->password, adm1.password);
}


void insert_adm(struct adm adm1) //����Ա�ṹ�����
{
    struct adm *p1, *p2, *p3;
    if (head_adm == NULL)  // �ձ�ʱ
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
            if (p2->id > yonghu1.id)  //�ڱ�ͷ����
            {
                p1 = (struct adm *) malloc(sizeof(struct adm));
                p1->id = adm1.id;
                copy_admin(p1, adm1);
                p1->next = head_adm;
                p1->yhthings = NULL;
                adm_info.num++;
                head_adm = p1;
            } else //�ڱ��л��β����
            {
                while (p2->id < adm1.id && p2->next != NULL) // �ҵ�Ҫ�����λ��
                {
                    p3 = p2;
                    p2 = p2->next;
                }

                if (p2->next == NULL && p2->id < adm1.id) // �ڱ�β���в���
                {
                    p1 = (struct adm *) malloc(sizeof(struct adm));
                    p1->id = adm1.id;
                    copy_admin(p1, adm1);
                    p1->next = NULL;
                    p1->yhthings = NULL;
                    adm_info.num++;
                    p2->next = p1;
                } else //�ڱ��н��в���
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
        } else printf("\n�ù���Ա�Ѵ��ڡ�");

    }
}


void insert_yhthings1(struct yhthings yhthings1)  //����Ʒ��Ϣ�����û�
{
    struct yhthings *p1, *p2, *p3;
    struct yonghu *p8;
    p8 = head_yonghu;
    while (p8->id != yhthings1.id1 && p8->next != NULL)  //Ѱ����Ʒ�����û�
    {
        p8 = p8->next;
    }
    if (p8->next == NULL && p8->id != yhthings1.id1) {
        printf("\nû������û���!��Ϣ���벻��ȥ�磡");
    } else {
        p1 = (struct yhthings *) p8->yhthings;
        p3 = (struct yhthings *) p8->yhthings;
        while (p1 != NULL) //p3Ϊǰһ��ָ�� p1Ϊ��ǰ��ַ
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
        } else printf("\n��������Ʒ�Ѵ��ڡ�");

    }
}


void jingpai(struct yonghu *p)//���Ĺ���
{
    int z = 1;
    time_t t;
    do {
        t = time(NULL);
        system("cls");
        printf("\n\tϵͳ��ǰʱ��Ϊ��");
        printf(ctime(&t));
        printf("\n\t\t     ****************************** ");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t         ���Ĺ���        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     ------------------------------  ");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (1)������Ʒ        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (2)������Ʒ        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (3)�������        |*");
        printf("\n\t\t     *|                          |* ");
        printf("\n\t\t     *|\t      (0)������һ��      |*");
        printf("\n\t\t     ****************************** \n");
        printf("\n\t\t     ���������ѡ��");

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
                printf("\n��Чѡ��Ŷ~����������~");

        }
    } while (z != 0);
    system("cls");
    return;


}


void paimai(struct yonghu *p)//�û�������Ʒ
{
    system("cls");
    struct yhthings *p1, *p2;
    struct paimaipin wupin1;
    int id;
    time_t t;
    t = time(NULL);
    printf("\n\tϵͳ��ǰʱ��Ϊ��");
    printf(ctime(&t));
    printf("\n������Ҫ��������Ʒ��ţ�");
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
        printf("\n��������������ʼʱ�䣺");
        printf("\n��� �·� ����:");
        scanf("%d%d%d", &wupin1.tb.year, &wupin1.tb.mon, &wupin1.tb.mday);
        printf("\nСʱ ���� �룺");
        scanf("%d%d%d", &wupin1.tb.hour, &wupin1.tb.min, &wupin1.tb.sec);
        printf("\n�����������Ľ���ʱ�䣺");
        printf("\n��� �·� ����:");
        scanf("%d%d%d", &wupin1.te.year, &wupin1.te.mon, &wupin1.te.mday);
        printf("\nСʱ ���� �룺");
        scanf("%d%d%d", &wupin1.te.hour, &wupin1.te.min, &wupin1.te.sec);
        wupin1.situ = 0;
        printf("\n��������Ʒ�ļ򵥽��ܣ�");
        scanf("%s", wupin1.introduce);
        insert_paimaipin1(wupin1);
        sprintf(log_message, "auction_add\nname:%s", wupin1.name);
        logging(wupin1.id, log_message);
        printf("\n������Ʒ�Ѿ�����������Ʒ�б��У�������Ʒ����������鿴��Ʒ�����");

    } else {
        printf("\n��û��Ȩ����������Ʒ�����Ʒ�����ڣ�");
        sprintf(log_message, "auction_add\nfailed  reason: unauthenticated or thing does not exist");
        printf("\n������Ҫ������ص��û���������û���Ʒ��");
    }
    system("cls");
    return;


}



//void pairesult2(struct yonghu *p){
//
//}



void pairesult(struct yonghu *p)//�û���������Ʒ�Ľ������ɾ���û���Ʒ��Ϣ��
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
            printf("\n\tϵͳ��ǰʱ��Ϊ��");
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
                printf("\n����Ʒ�ľ�����δ�����������δ֪����");
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
                printf("\n%d��Ʒ�ѱ�IDΪ%d�û���%lfԪ�ļ�Ǯ����~", p3->id, winner, max);
            }
        }
        p5 = p3;
        p3 = p3->next;
    }
    sleep(3);
    return;
}

void jinggou(struct yonghu *p)//������Ʒ���ܽ���
{
    int z;
    system("cls");
    time_t t;
    do {
        t = time(NULL);
        printf("\n\tϵͳ��ǰʱ��Ϊ��");
        printf(ctime(&t));
        printf("\n\t\t     ************************************ ");
        printf("\n\t\t     *|                                |* ");
        printf("\n\t\t     *|\t         ������ʱ�䵽��        |*");
        printf("\n\t\t     *|                                |* ");
        printf("\n\t\t     -------------------------------------");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (1)�鿴���Ծ�������Ʒ   ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (2)������Ҫ����Ʒ       ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (3)�鿴�����Ľ��       ||*");
        printf("\n\t\t     *||                              ||* ");
        printf("\n\t\t     *||      (0)������һ�����       ||*");
        printf("\n\t\t     ************************************ \n");
        printf("\n\t\t���������ѡ��");

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
                printf("��Чѡ��Ŷ~����������~");
                break;
        }
    } while (z != 0);
    system("cls");
    return;


}

void zuigaojia(struct paimaipin *p, struct yonghu *ppp)//��þ�������Ʒ�����������û�����Ʒ����
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
        printf("\n��Ǹ�����ڴ˴ξ�����ʧ���ˡ�����Ʒ��ӵ������%d����%lfԪ���á�", winner, max);
        p6 = head_yonghu;
        while (p6 != NULL) {
            if (p6->id == winner) break;
            p6 = p6->next;
        }
        if (p6 != NULL) {
            p9 = p6->yhthings;
            while (p9 != NULL) //p3Ϊǰһ��ָ�� p1Ϊ��ǰ��ַ
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
            while (p9 != NULL) //p3Ϊǰһ��ָ�� p1Ϊ��ǰ��ַ
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
            printf("\n����Ʒ�ѱ������ã����õļ�Ǯ�ǣ�%lf\n", max);
        }
    }
    printf("5����Զ��˳�������");
    sleep(5);
    system("cls");
    return;
}


void help()//����
{
    printf("\n\t\t***************************************");
    printf("\n\t\t*+*\t\t��  �� \t\t*+*");
    printf("\n\t\t***************************************");
    printf("\n\t\t*+*(n(*�R���Q*)n)");
    printf("\n\t\t*+*�û������������������0�˳���ǰ����");
    printf("\n\t\t*+*��¼����ܽ��о���");
    printf("\n\t\t*+*δ��¼���û���ע�������ϵ����Ա���������Ϣ�Ǽ�");
    printf("\n\t\t*+*���Ҫ���Լ�����Ʒ���Ͼ���ƽ̨������ϵ����Ա");
    printf("\n\t\t****************************************");
    develop();
    getchar();
    getchar();
    system("cls");
}


void develop()//����
{
    printf("\n\t\t       Ross");
    printf("\n\n2������˳�");
    sleep(2);
    system("cls");
}


int main() {
    int z;
    char ch;
    file_open();    //���ļ�
    if (head_adm == NULL) {
        printf("Ŀǰ�����ݣ��Ƿ�¼��?(y/n)\n");
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
        printf("\t\t\t|   ��ӭʹ����Ʒ���Ĺ���ϵͳ   |\n");
        printf("\t\t\t+------------------------------+\n");
        printf("\t    ��ʾ��Ϊ��֤���Ĳ����õ����棬�밴����˳���˳�ϵͳ^_^\n");
        printf("\n\t\t\t--------------------------------\n");
        printf("\t\t\t+            ���˵�            |\n");
        printf("\t\t\t--------------------------------\n");
        printf("\t\t\t+    [1]----�û���¼           \n");
        printf("\t\t\t+    [2]----�û�ע��           \n");
        printf("\t\t\t+    [3]----����Ա��¼         \n");
        printf("\t\t\t+    [4]----����               \n");
        printf("\t\t\t+    [0]----�˳�ϵͳ           \n");
        printf("\t\t\t--------------------------------\n");
        printf("\n\t    ����������ѡ��");
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
                printf("\n�͹٣�û�����ѡ����!\n����󷵻�");
                sleep(2);

        }
    } while (z != 0);
    file_close();  // to free all the pointer and input data to file
}
