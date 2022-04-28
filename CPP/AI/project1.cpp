#include <stdio.h>
#include <iostream>
#define MAX 100
using namespace std;
//狀態
struct zt
{
	int right_m;
	int left_m;
	int right_c;
	int left_c;
	int boat_location;
    int cost;
};
struct zt ztarr[MAX];
struct zt costList[MAX];
struct zt stepList[MAX];
int maxcost = INT_MAX;
int maxstep = INT_MAX;
int Index=0;
int count;

int numpass=0;
int start_c,start_y;


int handle(zt t)
{
	//是否達到目標轉態
	if(	t.left_m == start_c && t.left_c == start_y)
	{
		numpass++;
		printf("\n找到第%d條路徑！\n",numpass);
		printf("左傳\t左野\t右傳\t右野\t船\t錢\n");
        int i;
        for(i = 0; i <= Index ; i++)
		{
			
			printf("%2d\t",ztarr[i].left_m);
			printf("%2d\t",ztarr[i].left_c);
            printf("%2d\t",ztarr[i].right_m);
			printf("%2d\t",ztarr[i].right_c);
			
            if(ztarr[i].boat_location==1)
            {
                printf("右\t");
            }
            else if(ztarr[i].boat_location==-1)
            {
                printf("左\t");
            }
            printf("%2d\t",ztarr[i].cost);
			printf("\n");
		}
        if(ztarr[Index].cost<maxcost)
        {
            count = Index;
            maxcost = ztarr[Index].cost;
            for(i = 0; i <= count ; i++)
            {
                costList[i].right_m = ztarr[i].right_m;
                costList[i].right_c=ztarr[i].right_c;
                costList[i].left_m=ztarr[i].left_m;
                costList[i].left_c=ztarr[i].left_c;
                costList[i].boat_location=ztarr[i].boat_location;
                costList[i].cost=ztarr[i].cost;
            }
        }
		if(Index<maxstep)
		{
			maxstep = Index;
			for(i = 0; i <= maxstep ; i++)
            {
                stepList[i].right_m = ztarr[i].right_m;
                stepList[i].right_c=ztarr[i].right_c;
                stepList[i].left_m=ztarr[i].left_m;
                stepList[i].left_c=ztarr[i].left_c;
                stepList[i].boat_location=ztarr[i].boat_location;
                stepList[i].cost=ztarr[i].cost;
            }
		}
		//找到多條路徑的關鍵一
		return 0;
	}
	//是否重複操作
	for(int i = 0; i < Index; i++)
	{
		if(t.right_m == ztarr[i].right_m && t.right_c == ztarr[i].right_c)
		{
			if(t.boat_location == ztarr[i].boat_location)
			{
				return 0;
			}
			
		}
	}
	//人數是否合理嗎
	if(t.right_m < 0 || t.right_c < 0 || t.left_m < 0 || t.left_c < 0  )
	{
		return 0;
	}
	//傳教士是否被吃
	if((t.right_m < t.right_c && t.right_m != 0) || (t.left_m < t.left_c && t.left_m != 0) )
	{
		return 0;
	}

	//定義一個臨時節點
	struct zt tt;

	//兩個傳教士過河
	tt.right_m = t.right_m - 2 * t.boat_location;
	tt.right_c = t.right_c;
	tt.left_m = t.left_m + 2 * t.boat_location;
	tt.left_c = t.left_c;
	tt.boat_location = ( -t.boat_location);
    tt.cost = t.cost + 3;
    Index = Index + 1;
	ztarr[Index] = tt;
	handle(ztarr[Index]);
	Index = Index - 1;
	
	//兩個野人過河
	tt.right_m = t.right_m;
	tt.right_c = t.right_c - 2 * t.boat_location;
	tt.left_m = t.left_m ;
	tt.left_c = t.left_c + 2 * t.boat_location;
	tt.boat_location = ( -t.boat_location);
    tt.cost = t.cost + 3;
	Index = Index + 1;
	ztarr[Index] = tt;
	handle(ztarr[Index]);
	Index = Index-1;	

	//一個野人，一個傳教士過河
	tt.right_m = t.right_m - 1 * t.boat_location;
	tt.right_c = t.right_c - 1 * t.boat_location;
	tt.left_m = t.left_m + 1 * t.boat_location;
	tt.left_c = t.left_c + 1 * t.boat_location;
	tt.boat_location = ( -t.boat_location);
    tt.cost = t.cost + 3;
	Index = Index + 1;
	ztarr[Index] = tt;
	handle(ztarr[Index]);
	Index = Index-1;

	//一個傳教士過河
	tt.right_m = t.right_m - 1 * t.boat_location;
	tt.right_c = t.right_c;
	tt.left_m = t.left_m + 1 * t.boat_location;
	tt.left_c = t.left_c;
	tt.boat_location = ( -t.boat_location);
    tt.cost = t.cost + 3;
	Index = Index + 1;
	ztarr[Index] = tt;
	handle(ztarr[Index]);
	Index = Index-1;

	//一個野人過河
	tt.right_m = t.right_m;
	tt.right_c = t.right_c - 1 * t.boat_location;
	tt.left_m = t.left_m;
	tt.left_c = t.left_c + 1 * t.boat_location;
	tt.boat_location = ( -t.boat_location);
    tt.cost = t.cost + 3;
	Index = Index + 1;
	ztarr[Index] = tt;
	handle(ztarr[Index]);
	Index = Index-1;
	//找到多條路徑的關鍵二
	return 0;
}


int main()
{
	printf("請輸入初始傳教士人數：");
	scanf("%d",&start_c);
	printf("請輸入初始野人人數：");
	scanf("%d",&start_y);
	ztarr[Index].right_m = start_c;
	ztarr[Index].right_c = start_y;
	ztarr[Index].left_m = 0;
	ztarr[Index].left_c = 0;
	ztarr[Index].boat_location = 1;
    ztarr[Index].cost = 0;
	handle(ztarr[Index]);
	printf("已為您找到%d條過河路徑！並且已全部載入完畢！\n",numpass);
    printf("\n找到最低成本路徑！\n");
    printf("左傳\t左野\t右傳\t右野\t船\t錢\n");
    
    for (int j = 0; j <= count ; j++)
    {
        
		printf("%2d\t",costList[j].left_m);
		printf("%2d\t",costList[j].left_c);
        printf("%2d\t",costList[j].right_m);
		printf("%2d\t",costList[j].right_c);
        if(costList[j].boat_location==1)
        {
            printf("右\t");
        }
        else if(costList[j].boat_location==-1)
        {
            printf("左\t");
        }

        printf("%2d\t",costList[j].cost);
		printf("\n"); 
    }
    cout <<"花費:"<< maxcost <<"元"<< endl;
	printf("\n找到最少次數路徑！\n");
    printf("左傳\t左野\t右傳\t右野\t船\t錢\n");
	for (int j = 0; j <= maxstep ; j++)
    {
        
		printf("%2d\t",stepList[j].left_m);
		printf("%2d\t",stepList[j].left_c);
        printf("%2d\t",stepList[j].right_m);
		printf("%2d\t",stepList[j].right_c);
        if(stepList[j].boat_location==1)
        {
            printf("右\t");
        }
        else if(stepList[j].boat_location==-1)
        {
            printf("左\t");
        }

        printf("%2d\t",stepList[j].cost);
		printf("\n"); 
    }
	printf("共%d步",maxstep+1);
    return 0;
}