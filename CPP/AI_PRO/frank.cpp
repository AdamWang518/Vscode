#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct Node
{
	int m;
	int c;
	int b;		 // A船狀態，1:左，0:右
	int B;		 // B船狀態，1:左，0:右
	int b_count; // A船次數
	int B_count; // B船次數
	int step;
	double f_loss;
	Node *parent;
	Node(int m_num, int c_num, int A_boat_state, int B_boat_state, int bcount, int Bcount, int step_count, Node *ptr) : m{m_num}, c{c_num}, b{A_boat_state}, B{B_boat_state}, b_count{bcount}, B_count{Bcount}, step{step_count}, parent{ptr}
	{
		f_loss = (m + c) / 2;
	}
	Node(): m{0}, c{0}, b{0}, B{0}, b_count{0}, B_count{0}, step{0}, f_loss{0}, parent{nullptr} {};
};

deque<Node> opened_list;
vector<Node> closed_list;
int m_num=3, c_num=3;

bool is_safe(Node *n)
{ // 判斷兩岸的傳教士是否安全
	int m_left = n->m;
	int c_left = n->c;
	int m_right = m_num - m_left;
	int c_right = c_num - c_left;

	if (m_left < c_left && m_left != 0) // 左岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if (m_right < c_right && m_right != 0) // 右岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if (m_left < 0 || c_left < 0)
		return false;
	if (m_left >= c_left || m_right >= c_right)
		return true;
}

bool in_closed_list(Node *node)
{ // 確認是否已經在closed_list內
	for (auto x : closed_list)
	{
		if (x.m == node->m && x.c == node->c && x.b == node->b && x.B ==node->B)
		{
			return true;
		}
	}
	return false;
}

void check_openlist()
{
	cout<<"openlist:";
 	for(deque<Node>::iterator it=opened_list.begin(); it!=opened_list.end();it++){
  		cout << it->m << " " << it->c << " " << it->b << " "<<it->B<<" " << it->step<<"/";
 	}
 	cout << endl;
}
void check_closelist()
{
	cout<<"closelist:";
    if(closed_list.size()==0)
    {
        cout << endl;
        return;
    }
    for (int i = 0; i < closed_list.size();i++)
    {
        cout << closed_list[i].m << " " << closed_list[i].c << " " << closed_list[i].b << " "<<closed_list[i].B << " " << closed_list[i].step<<"/";
    }
    cout << endl;
}

template<typename T>
void sort_bubble(deque<T>& dq)
{
	int exchg=0;
	int capa=dq.size();
	T arrtmp[capa];
	for(int i=0;i<capa;++i){
		T dqcur=dq.back();
		dq.pop_back();
		arrtmp[i]=dqcur;
	}
	T tmp;
	for(int i=0;i<capa;++i){
		exchg=0;
		for(int j=capa-1;j>=i;--j){
			if(arrtmp[j].f_loss<arrtmp[j-1].f_loss){
				tmp=arrtmp[j];
				arrtmp[j]=arrtmp[j-1];
				arrtmp[j-1]=tmp;
				exchg=1;
			}
		}
		if(exchg!=1)
			break;
	}
	for(int i=0;i<capa;++i){
		dq.push_back(arrtmp[i]);	
	}
    check_openlist();
}
void sort_by_floss()
{ 
	// 將opened_list內的點按照分數大小排序
	for(auto i=opened_list.begin(); i!=opened_list.end(); i++){
		for(auto j=i+1; j!=opened_list.end(); j++){
			if(j->step>=i->f_loss){
				continue;
			}
			else
				swap(i, j);
		}
	}
    check_openlist();
}

void refresh_opened(Node *n)
{ // 如果節點有較低的分數或步數，更新opened_list
	for (auto x : opened_list)
	{
		if (x.m == n->m && x.c == n->c && x.b == n->b)
		{
			if (n->f_loss < x.f_loss || n->step < x.step)
			{
				memcpy(&x, n, sizeof(Node));
			}
			return;
		}
	}
	opened_list.push_back(Node(n->m, n->c, n->b,n->B,n->b_count,n->B_count, n->step, n->parent));
}
void a_star_algorithm()
{
	while (opened_list.size() != 0)
	{
        sort_bubble(opened_list);
        // 從opened_list中取出分數最小的
		Node node;
		node = opened_list.front();
		opened_list.pop_front();
		// cout<<node.step<<" ";
        check_closelist();
        closed_list.push_back(Node(node.m, node.c, node.b, node.B,node.b_count,node.B_count, node.step, node.parent)); // 將取出的點加入closed_list中
		// 判斷取出的點是否為目標點
		if (node.m == 0 && node.c == 0 && node.b == 0)
			break;

		for (int i = 0; i <= m_num; i++)
		{ // i代表上船的傳教士數量
			for (int j = 0; j <= c_num; j++)
			{ // j代表上船的食人魔數量
				// 判斷船上的合理情況
				if (i + j == 0 || i + j > 3 || (i != 0 && i < j))
				{
					continue;
				}
				//需要判斷4種情況，每次只能選一艘船，bB:00-> 01/10，01-> 00/11，10-> 00/11，11-> 01/10
				if (node.b == 1 && node.B == 1) // A船在左側，B船在左側
				{
					if (i > node.m || j > node.c)
					{
						continue;
					}
					//走A船
					if(i + j < 3)
					{
						Node *childA_node = new Node(node.m - i, node.c - j, 0,1,node.b_count+1,node.B_count, node.step + 1, &closed_list.back());
						if (!in_closed_list(childA_node))
						{
							if (is_safe(childA_node))
							{
								refresh_opened(childA_node);
							}
						}
					}
					//走B船
					Node *childB_node = new Node(node.m - i, node.c - j, 1,0,node.b_count,node.B_count+1, node.step + 1, &closed_list.back());
					if (!in_closed_list(childB_node))
					{
						if (is_safe(childB_node))
						{
							refresh_opened(childB_node);
						}
					}

				}
				else if(node.b == 1 && node.B == 0)// A船在左側，B船在右側
				{ 
					if (i > m_num - node.m || j > c_num - node.c || i > node.m || j > node.c)
					{
						continue;
					}
					//走A船
					if(i + j < 3)
					{
						Node *childA_node = new Node(node.m - i, node.c - j, 0,0,node.b_count+1,node.B_count, node.step + 1, &closed_list.back());
						if (!in_closed_list(childA_node))
						{
							if (is_safe(childA_node))
							{
								refresh_opened(childA_node);
							}
						}
					}
					//走B船
					Node *childB_node = new Node(node.m + i, node.c + j, 1,1,node.b_count,node.B_count+1, node.step + 1, &closed_list.back());
					if (!in_closed_list(childB_node))
					{
						if (is_safe(childB_node))
						{
							refresh_opened(childB_node);
						}
					}
				}
				else if(node.b == 0 && node.B == 1)// A船在右側，B船在左側
				{ 
					if (i > m_num - node.m || j > c_num - node.c || i > node.m || j > node.c)
					{
						continue;
					}
					//走A船
					if(i + j < 3)
					{
						Node *childA_node = new Node(node.m + i, node.c + j, 1,1,node.b_count+1,node.B_count, node.step + 1, &closed_list.back());
						if (!in_closed_list(childA_node))
						{
							if (is_safe(childA_node))
							{
								refresh_opened(childA_node);
							}
						}
					}
					//走B船
					Node *childB_node = new Node(node.m - i, node.c - j, 0,0,node.b_count,node.B_count+1, node.step + 1, &closed_list.back());
					if (!in_closed_list(childB_node))
					{
						if (is_safe(childB_node))
						{
							refresh_opened(childB_node);
						}
					}
				}
				else if(node.b == 0 && node.B == 0)// A船在右側，B船在右側
				{ 
					if (i > m_num - node.m || j > c_num - node.c)
					{
						continue;
					}
					//走A船
					if(i+j<3)
					{
						Node *childA_node = new Node(node.m + i, node.c + j, 1,0,node.b_count+1,node.B_count, node.step + 1, &closed_list.back());
						if (!in_closed_list(childA_node))
						{
							if (is_safe(childA_node))
							{
								refresh_opened(childA_node);
							}
						}
					}
					//走B船
					Node *childB_node = new Node(node.m + i, node.c + j, 0,1,node.b_count,node.B_count+1, node.step + 1, &closed_list.back());
					if (!in_closed_list(childB_node))
					{
						if (is_safe(childB_node))
						{
							refresh_opened(childB_node);
						}
					}
				}
			}
		}
		
	}
}
void output()
{
	Node *current = &closed_list.back();
	while (current != NULL)
	{
		cout << "步數:" << current->step << "左岸傳教士:" << current->m << "左岸食人族:" << current->c << "右岸傳教士:" <<m_num - current->m  << "右岸食人族:" << c_num - current->c << "\tA船位:";
		if (current->b == 1)
			cout << "右" ;
		else
			cout << "左" ;
		cout<<"\tB船位:";
		if (current->B == 1)
			cout << "右";
		else
			cout << "左";
		cout<<"\tA船次數:"<<current->b_count<<"\tB船次數:"<<current->B_count<<"共花費:"<<3*current->b_count+25*current->B_count<<"元"<<endl;
		current = current->parent;
	}
}
int main(int argc, char const *argv[])
{
	SetConsoleOutputCP(CP_UTF8);

	// cout << "請輸入初始傳教士人數：";
	// cin >> m_num;
	// cout << "請輸入初始野人人數：";
	// cin >> c_num;
    int x;
    cout << "\n1.最短步數\n2.最少花費\n\n請選擇模式: ";
	cin >> x;
	while(x!=1 && x!=2){
		cout << "輸入的數字錯誤，請重試。\n";
		cout << "\n1.最短步數\n2.最少花費\n\n請選擇模式: ";
		cin >> x;
	}
	closed_list.reserve((m_num + 1) * (c_num + 1) * 2 + 1); //解決記憶體位址跳掉的問題
	Node start_node(m_num, c_num, 1, 1, 0, 0, 0, nullptr);	//初始化
	opened_list.push_back(start_node);

	a_star_algorithm();
	output();

	return 0;
}