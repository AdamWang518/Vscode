#include <stdio.h>
#define MAX 100
//狀態
struct zt
{
	int right_m;
	int left_m;
	int right_c;
	int left_c;
	int boat_location;
};
struct zt ztarr[MAX];
int Index=0;

int numpass=0;
int start_c,start_y;


int handle(zt t)
{
	//是否達到目標轉態
	if(	t.left_m == start_c && t.left_c == start_y)
	{
		numpass++;
		printf("\n找到第%d條路徑！\n",numpass);
		printf("左傳\t左野\t右傳\t右野\t船\n");
		for(int i = 0; i <= Index ; i++)
		{
			printf("%2d\t",ztarr[i].right_m);
			printf("%2d\t",ztarr[i].right_c);
			printf("%2d\t",ztarr[i].left_m);
			printf("%2d\t",ztarr[i].left_c);
			printf("%2d\t",ztarr[i].boat_location);
			printf("\n");
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
	handle(ztarr[Index]);
	printf("已為您找到%d條過河路徑！並且已全部載入完畢！\n",numpass);
    return 0;
}