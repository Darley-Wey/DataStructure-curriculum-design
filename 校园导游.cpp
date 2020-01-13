#include "malloc.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAXVEX 50        //最大顶点个数
#define MAXWEIGHT 5000   //最大权值
typedef struct{    
    int no;         //景点编号
    char name[20];  //景点名称
    char desc[100]; //景点简介
} DataType;         //景点结构体
typedef int weight; //权值
typedef struct {
    weight arcs[MAXVEX][MAXVEX]; //邻接矩阵
    DataType data[MAXVEX];       //顶点信息
    int vexs;                    //顶点数
} MGraph, *AdjMetrix;

/*创建邻接矩阵*/
void CreateGraph(AdjMetrix g, int m[][MAXVEX], DataType d[], int n) {
    int i, j;
    g->vexs = n;
    for (i = 0; i < n; i++) {
        g->data[i] = d[i];
        for (j = 0; j < n; j++)
            g->arcs[i][j] = m[i][j];
    }
}

/*给顶点赋值*/
void PutVex(AdjMetrix g, int k, DataType x) {
    if (k < 0 || k > g->vexs) {
        printf("参数k超出范围！\n");
        return;
    }
    g->data[k] = x;
}

/*迪杰斯特拉算法求最短路径*/
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

/*显示最短路径*/
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
    printf("从[%s]到[%s]的最佳路径为：", g->data[start].name,g->data[end].name);
    while (top != -1) {
        x = base[top--];
        printf("%s->", x.name);
    }
    printf("%s", g->data[end].name);
    printf("\n此路径长为%d米,大约需要走%d分钟\n\n", dis[end], dis[end] / 60);
}

/*根据景点名称查询景点序号*/
int Locate(AdjMetrix g, char name[]) {
    int i;
    for (i = 0; i < g->vexs; i++)
        if (!strcmp(name, g->data[i].name))
            return i;
    return -1;
}

/*根据景点名称查询景点信息*/
void QueryVex(AdjMetrix g, char name[]) {
    int i;
    for (i = 0; i < g->vexs; i++){
        if (!strcmp(name, g->data[i].name)) {
            system("cls");    //清屏
            printf("景点编号：[%d],景点名称：%s\n简介：\n------\n%s\n\n",g->data[i].no, g->data[i].name, g->data[i].desc);
            return;
        }
    }
    printf("输入错误，请检查后重新操作\n\n");     
}

/*欢迎页*/
void welcome(AdjMetrix g){
    int i;
    printf("欢迎来到河北科技大学，景点列表如下：\n");
    for (i = 0; i < g->vexs; i++)
        printf("景点编号：[%d],景点名称：%s\n", g->data[i].no, g->data[i].name);
}

/*再见页*/
void goodbye(){
    system("cls");
    printf("谢谢使用，下次再见\n\n\n");
    system("pause");
    exit(0);
}

/*主函数*/
int main(int argc, char *argv[]) {
    MGraph gg;
    AdjMetrix g = &gg;

    /*景点信息数组，默认使用外部文件获取景点信息
    DataType d[] = {
        {1, "校名石", "正对学校东大门的巨石，上刻有校名，气势宏伟。"},
        {2,"银杏树林","学校东大门北侧的银杏树林，秋季落叶时节必去的美丽景点。"}, 
        {3,"主体育场", "拥有数千人看台的专业体育场，足球比赛举办地。"}, 
        {4, "孔雀园","学校最名贵的动物孔雀的家，拥有多个品种数只孔雀。"}, 
        {5, "风雨操场","室内体育场所在地，设有篮球、排球、羽毛球等体育设施。"}, 
        {6, "时间广场","图书馆前的巨大下沉式广场，不定时举办音乐喷泉表演。"}, 
        {7,"图书馆","学校标志性建筑，信息资源中心，学习圣地。"}, 
        {8, "牧星湖","学校人工湖，最大人文景观所在地，湖水清澈，绿柳成荫。"}, 
        {9, "二月兰园","巨大的二月兰花园，盛开时一片紫色花海，好不壮观。"},
        {10,"大学商城","学校南门的商业中心，美食服务一条街，吃喝玩乐必去。"}
    };
    */
   /*使用外部文件获取景点信息，找不到时请注释以下部分的代码，并使用上方代码使用内部景点信息*/
    DataType d[10];
    int i = 0;
    FILE *fp;
    if ((fp = fopen("DataType.txt", "r")) == NULL) // 打开文件并且让fp指向文件
    {
        printf("景点信息文件不存在，请检查\n\n");
        system("pause");
        exit(1);
    }
    while (!feof(fp)) //读取一组数据后指针指向下一组数据，并且判断是否指向最后一行
    {
        fscanf(fp, "%d %s %s\n", &d[i].no, &d[i].name, &d[i].desc);
        i++;
    }
    fclose(fp); //关闭文件；

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
    CreateGraph(g, m, d, 10);    //创建邻接矩阵

    do {
        printf("*************************************************\n");
        printf("*	河北科技大学校园导游系统		*\n");
        printf("*-----------------------------------------------*\n");
        printf("*	1.查询景点信息 				*\n");
        printf("*	2.查询景点间的最短路线                  *\n");
        printf("*	0.退出系统				*\n");
        printf("*************************************************\n");
        printf("\n请选择你要使用的功能(1,2,0):");     //主程序菜单
        int choice = -1;
        fflush(stdin);    //清空输入缓存区
        scanf("%d", &choice);
        
        switch (choice) {
        case 1:
            system("cls");
            welcome(g);
            printf("\n请输入景点名字:");
            scanf("%s", vname);
            QueryVex(g, vname);
            break;
        case 2:
            system("cls");
            welcome(g);
            printf("\n请输入你的出发景点名称:");
            scanf("%s", vname);
            start = Locate(g, vname);
            printf("请输入你的目的景点名称:");
            scanf("%s", vname);
            end = Locate(g, vname);
            if (start == end||start == -1 || end == -1)
                printf("输入错误，请检查后重新操作\n\n");
            else if (start != -1 && end != -1) {
                system("cls");
                Dijstra(g, start, dis, path);
                DispPath(g, path, dis, start, end);
            }
            break;
        case 0:
            goodbye( );
        default :
            printf("输入错误，请检查后重新操作\n\n");
            break;
        }
    } while (1);
    return 0;
}
