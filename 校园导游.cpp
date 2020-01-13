#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXVEX 50        //��󶥵����
#define MAXWEIGHT 5000   //���Ȩֵ
typedef struct{    
    int no;         //������
    char name[20];  //��������
    char desc[100]; //������
} DataType;         //����ṹ��
typedef int weight; //Ȩֵ
typedef struct {
    weight arcs[MAXVEX][MAXVEX]; //�ڽӾ���
    DataType data[MAXVEX];       //������Ϣ
    int vexs;                    //������
} MGraph, *AdjMetrix;

/*�����ڽӾ���*/
void CreateGraph(AdjMetrix g, int m[][MAXVEX], DataType d[], int n) {
    int i, j;
    g->vexs = n;
    for (i = 0; i < n; i++) {
        g->data[i] = d[i];
        for (j = 0; j < n; j++)
            g->arcs[i][j] = m[i][j];
    }
}

/*�����㸳ֵ*/
void PutVex(AdjMetrix g, int k, DataType x) {
    if (k < 0 || k > g->vexs) {
        printf("����k������Χ��\n");
        return;
    }
    g->data[k] = x;
}

/*�Ͻ�˹�����㷨�����·��*/
void Dijstra(AdjMetrix g, int v, int dis[], int path[]) {
    int vset[MAXVEX];
    int MinDis, i, j, w;
    for (i = 0; i < g->vexs; i++) {
        vset[i] = 1;
        dis[i] = g->arcs[v][i];
        if (g->arcs[v][i] < MAXWEIGHT)
            path[i] = v;
        else
            path[i] = -1;
    }
    vset[v] = 0;
    path[v] = 0;
    for (i = 1; i < g->vexs; i++) {
        MinDis = MAXWEIGHT;
        for (j = 0; j < g->vexs; j++)
            if (vset[j] && dis[j] < MinDis) {
                w = j;
                MinDis = dis[j];
            }
        if (MinDis == MAXWEIGHT)
            return;
        vset[w] = 0;
        for (j = 0; j < g->vexs; j++) {
            if (vset[j] && g->arcs[w][j] < MAXWEIGHT &&dis[w] + g->arcs[w][j] < dis[j]) {
                dis[j] = dis[w] + g->arcs[w][j];
                path[j] = w;
            }
        }
    }
}

/*��ʾ���·��*/
void DispPath(AdjMetrix g, int path[], int dis[], int start, int end) {
    int top = -1;
    DataType base[MAXVEX];
    int pos;
    DataType x;
    pos = path[end];
    while (pos != start) {
        base[++top] = g->data[pos];
        pos = path[pos];
    }
    base[++top] = g->data[start];
    printf("��[%s]��[%s]�����·��Ϊ��", g->data[start].name,g->data[end].name);
    while (top != -1) {
        x = base[top--];
        printf("%s->", x.name);
    }
    printf("%s", g->data[end].name);
    printf("\n��·����Ϊ%d��,��Լ��Ҫ��%d����\n\n", dis[end], dis[end] / 60);
}

/*���ݾ������Ʋ�ѯ�������*/
int Locate(AdjMetrix g, char name[]) {
    int i;
    for (i = 0; i < g->vexs; i++)
        if (!strcmp(name, g->data[i].name))
            return i;
    return -1;
}

/*���ݾ������Ʋ�ѯ������Ϣ*/
void QueryVex(AdjMetrix g, char name[]) {
    int i;
    for (i = 0; i < g->vexs; i++){
        if (!strcmp(name, g->data[i].name)) {
            system("cls");    //����
            printf("�����ţ�[%d],�������ƣ�%s\n��飺\n------\n%s\n\n",g->data[i].no, g->data[i].name, g->data[i].desc);
            return;
        }
    }
    printf("���������������²���\n\n");     
}

/*��ӭҳ*/
void welcome(AdjMetrix g){
    int i;
    printf("��ӭ�����ӱ��Ƽ���ѧ�������б����£�\n");
    for (i = 0; i < g->vexs; i++)
        printf("�����ţ�[%d],�������ƣ�%s\n", g->data[i].no, g->data[i].name);
}

/*�ټ�ҳ*/
void goodbye(){
    system("cls");
    printf("ллʹ�ã��´��ټ�\n\n\n");
    system("pause");
    exit(0);
}

/*������*/
int main(int argc, char *argv[]) {
    MGraph gg;
    AdjMetrix g = &gg;

    /*������Ϣ���飬Ĭ��ʹ���ⲿ�ļ���ȡ������Ϣ
    DataType d[] = {
        {1, "У��ʯ", "����ѧУ�����ŵľ�ʯ���Ͽ���У�������ƺ�ΰ��"},
        {2,"��������","ѧУ�����ű�����������֣��＾��Ҷʱ�ڱ�ȥ���������㡣"}, 
        {3,"��������", "ӵ����ǧ�˿�̨��רҵ����������������ٰ�ء�"}, 
        {4, "��ȸ԰","ѧУ������Ķ����ȸ�ļң�ӵ�ж��Ʒ����ֻ��ȸ��"}, 
        {5, "����ٳ�","�������������ڵأ���������������ë���������ʩ��"}, 
        {6, "ʱ��㳡","ͼ���ǰ�ľ޴��³�ʽ�㳡������ʱ�ٰ�������Ȫ���ݡ�"}, 
        {7,"ͼ���","ѧУ��־�Խ�������Ϣ��Դ���ģ�ѧϰʥ�ء�"}, 
        {8, "���Ǻ�","ѧУ�˹�����������ľ������ڵأ���ˮ�峺����������"}, 
        {9, "������԰","�޴�Ķ�������԰��ʢ��ʱһƬ��ɫ�������ò�׳�ۡ�"},
        {10,"��ѧ�̳�","ѧУ���ŵ���ҵ���ģ���ʳ����һ���֣��Ժ����ֱ�ȥ��"}
    };
    */
   /*ʹ���ⲿ�ļ���ȡ������Ϣ���Ҳ���ʱ��ע�����²��ֵĴ��룬��ʹ���Ϸ�����ʹ���ڲ�������Ϣ*/
    DataType d[10];
    int i = 0;
    FILE *fp;
    if ((fp = fopen("DataType.txt", "r")) == NULL) // ���ļ�������fpָ���ļ�
    {
        printf("������Ϣ�ļ������ڣ�����\n\n");
        system("pause");
        exit(1);
    }
    while (!feof(fp)) //��ȡһ�����ݺ�ָ��ָ����һ�����ݣ������ж��Ƿ�ָ�����һ��
    {
        fscanf(fp, "%d %s %s\n", &d[i].no, &d[i].name, &d[i].desc);
        i++;
    }
    fclose(fp); //�ر��ļ���

    int m[][MAXVEX] = {
        {MAXWEIGHT, 60, 400, MAXWEIGHT, MAXWEIGHT, 400,MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT},
        {60, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT,MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT},
        {400, MAXWEIGHT, MAXWEIGHT, 200, 300, MAXWEIGHT,MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT},
        {MAXWEIGHT, MAXWEIGHT, 200, MAXWEIGHT, 200, MAXWEIGHT,MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT},
        {MAXWEIGHT, MAXWEIGHT, 300, 200, MAXWEIGHT, 400,MAXWEIGHT, 400, MAXWEIGHT, MAXWEIGHT},
        {400, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, 400, MAXWEIGHT,200, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT},
        {MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT,200, MAXWEIGHT, 300, 400, 450},
        {MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, 400,MAXWEIGHT, 300, MAXWEIGHT, 350, 650},
        {MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT,MAXWEIGHT, 400, 350, MAXWEIGHT, 450},
        {MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT, MAXWEIGHT,MAXWEIGHT, 450, 650, 450, MAXWEIGHT}
    };

    int dis[5], path[MAXVEX];
    int  start, end;
    char vname[20];
    DataType x;
    CreateGraph(g, m, d, 10);    //�����ڽӾ���

    do {
        printf("*************************************************\n");
        printf("*	�ӱ��Ƽ���ѧУ԰����ϵͳ		*\n");
        printf("*-----------------------------------------------*\n");
        printf("*	1.��ѯ������Ϣ 				*\n");
        printf("*	2.��ѯ���������·��                  *\n");
        printf("*	0.�˳�ϵͳ				*\n");
        printf("*************************************************\n");
        printf("\n��ѡ����Ҫʹ�õĹ���(1,2,0):");     //������˵�
        int choice = -1;
        fflush(stdin);    //������뻺����
        scanf("%d", &choice);
        
        switch (choice) {
        case 1:
            system("cls");
            welcome(g);
            printf("\n�����뾰������:");
            scanf("%s", vname);
            QueryVex(g, vname);
            break;
        case 2:
            system("cls");
            welcome(g);
            printf("\n��������ĳ�����������:");
            scanf("%s", vname);
            start = Locate(g, vname);
            printf("���������Ŀ�ľ�������:");
            scanf("%s", vname);
            end = Locate(g, vname);
            if (start == end||start == -1 || end == -1)
                printf("���������������²���\n\n");
            else if (start != -1 && end != -1) {
                system("cls");
                Dijstra(g, start, dis, path);
                DispPath(g, path, dis, start, end);
            }
            break;
        case 0:
            goodbye( );
        default :
            printf("���������������²���\n\n");
            break;
        }
    } while (1);
    return 0;
}
