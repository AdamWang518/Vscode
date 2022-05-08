#include <Windows.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Node
{
	int m;
	int c;
	int b;
	int step;
	int parent_count;
	int node_count;
	int node_cost;
	double f_loss;
	Node *parent;
	Node(int m_num, int c_num, int boat_state, int step_count, Node *ptr,int parent_count,int node_count,int cost) : m{m_num}, c{c_num}, b{boat_state}, step{step_count}, parent{ptr},parent_count{parent_count},node_count{node_count},node_cost{node_cost}
	{
		f_loss = (m + c) / 2;
	}
	Node() : m{0}, c{0}, b{0}, step{0}, f_loss{0},node_cost{0}, parent{nullptr} {};
};

deque<Node> opened_list;
vector<int> closed_list;
deque<Node> output_closed_list;
Node *step_list = NULL;
Node *more_step_list = NULL;
int step_list_count = 0;
int m_num, c_num;

bool is_safe(Node n)
{ // 判斷兩岸的傳教士是否安全
	int m_left = n.m;
	int c_left = n.c;
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

int cal_value(Node node)
{
	return node.m * 100 + node.c * 10 + node.b;
}

bool in_closed_list(Node node)
{ // 確認是否已經在closed_list內
	int value = cal_value(node);
	if (find(closed_list.begin(), closed_list.end(), value) != closed_list.end())
		return true;
	return false;
}

void sort_by_floss()
{ // 將opened_list內的點按照分數大小排序
	for (deque<Node>::iterator i = opened_list.begin(); i != opened_list.end(); i++)
	{
		for (deque<Node>::iterator j = i + 1; j != opened_list.end(); j++)
		{
			if (i->f_loss > j->f_loss)
			{
				swap(i, j);
			}
			if (i->f_loss==j->f_loss&&i->step > j->step)
			{
				swap(i, j);
			}
		}
	}
}

void swap(Node &a, Node &b)
{
	Node tmp;
	tmp = a;
	a = b;
	b = tmp;
}

int countt = 1;

void check_openlist()//node的parent有問題
{
	cout <<"open list "<< countt << ":";
	for (deque<Node>::iterator it = opened_list.begin(); it != opened_list.end(); it++)
	{
        
		cout << it->m << " " << it->c << " " << it->b << " " << it->step << "/";
	}
	cout << endl;
	countt++;
}
void check_closelist(){
    cout <<"close list "<< countt << ":";
    for (deque<Node>::iterator it = output_closed_list.begin(); it != output_closed_list.end(); it++)
	{
		cout << it->m << " " << it->c << " " << it->b << " " << it->step << "/";
	}
    cout << endl;
}
void refresh_opened(Node n)
{ // 如果有新的節點，更新opened_list
	opened_list.push_back(Node(n.m, n.c, n.b, n.step, n.parent,n.parent_count,n.node_count));
}

void a_star_algorithm(Node &result)
{
	int shore_m; //岸上傳教士人數
	int shore_c;
	int counter = 1;
	while (opened_list.size() != 0)
	{
		// 從opened_list中取出分數最小的
		Node node;
		// int tmp_list_count=step_list_count-1;
		node = opened_list.front();
		opened_list.pop_front();
		cout << endl;
		cout << "choose node state:" << node.m << " " << node.c << " " << node.b << " " << node.step << endl;
		// cout << "s0address:" << &step_list[0] << endl;
		if (counter >= 2)
        {
            cout << "node parent state:" << step_list[node.parent_count].m << " " << step_list[node.parent_count].c << " " << step_list[node.parent_count].b << " " << step_list[node.parent_count].step << endl;
        }
		if (counter >= 2)
        {
            check_closelist();
        }	
		counter++;
        
		// 判斷取出的點是否為目標點
		if (node.m == 0 && node.c == 0)
		{
			result = node;
			cout << "result parent:" << node.parent->step << endl;
			break;
		}
		if (node.b == 1) //船在左岸
		{
			shore_m = node.m;
			shore_c = node.c;
		}
		else
		{
			shore_m = m_num - node.m;
			shore_c = c_num - node.c;
		}

		// 將取出的點加入closed_list中
		closed_list.push_back(cal_value(node));
        output_closed_list.push_back(node);
        for (int i = 0; i <= shore_m; i++)
		{ // i代表上船的傳教士數量
			for (int j = 0; j <= shore_c; j++)
			{ // j代表上船的食人魔數量
				// 判斷船上的合理情況
				if (i + j == 0 || i + j > 2 || (i != 0 && i < j))
				{
					continue;
				}
				if (node.b == 1)
				{ // 船在左側，下一狀態船在右側
					// Node node2 = (node.m - i, node.c - j, 0, node.step + 1, &step_list[step_list_count - 1]);
					
					Node child_node(node.m - i, node.c - j, 0, node.step + 1, &step_list[node.node_count],node.node_count,step_list_count - 1,node.cost+3);
					// Node child_node(node.m - i, node.c - j, 0, node.step + 1, &step_list[0]);

					// cout <<"step_list[step_list_count - 1].m:"<< step_list[step_list_count - 1].m << endl;
					// cout << "steplistcount" << step_list_count - 1 << endl;
					// cout <<"s0:"<< step_list[0].m << step_list[0].c << step_list[0].b << endl;
					// Node child_node = (node.m - i, node.c - j, 0, node.step + 1, nullptr);

					step_list_count++;
					more_step_list = (Node *)realloc(step_list, step_list_count * sizeof(Node));
					step_list = more_step_list;
					step_list[step_list_count - 1] = child_node;
					// Node* child_node = new Node(node.m - i, node.c - j, 0, node.step + 1, &node);

					if (!in_closed_list(child_node))
					{
						if (is_safe(child_node))
						{
							refresh_opened(child_node);
						}
					}
				}
				else
				{ // 船在右側，下一狀態船在左側
					
					Node child_node(node.m + i, node.c + j, 1, node.step + 1, &step_list[node.node_count],node.node_count,step_list_count - 1);
					// Node child_node(node.m + i, node.c + j, 1, node.step + 1, &step_list[0]);

					// cout <<"s0:"<< step_list[0].m << step_list[0].c << step_list[0].b << endl;

					// cout <<"step_list[step_list_count - 1].m:"<< step_list[step_list_count - 1].m << endl;
					// cout << "steplistcount" << step_list_count - 1 << endl;
					step_list_count++;
					more_step_list = (Node *)realloc(step_list, step_list_count * sizeof(Node));
					step_list = more_step_list;
					step_list[step_list_count - 1] = child_node;
					// Node *child_node = new Node(node.m + i, node.c + j, 1, node.step + 1, &node);
					if (!in_closed_list(child_node))
					{
						if (is_safe(child_node))
						{
							refresh_opened(child_node);
						}
					}
				}
			}
		}
		sort_by_floss();
		check_openlist();
        
	}
}
void output_result(Node n)
{
	Node *node;
	cout << "傳教士\t食人族\t船\tstep\n";
	node = &n;
	// cout<<node->m<< "\t" << node->c << "\t" << node->b << "\t" << node->step << endl;
	// cout << node->step << endl;
	// cout << node->parent->step;
	for (int i = 0; i < n.step;i++)
	{
		cout << step_list[node->node_count].m << "\t" << step_list[node->node_count].c << "\t" << step_list[node->node_count].b << "\t" << step_list[node->node_count].step << endl;
		// cout<<node->m<< "\t" << node->c << "\t" << node->b << "\t" << node->step << endl;
		node = node->parent;
	}

		// while (1)
		// {
		// 	// step_list[node.parent_count].m
		// 	cout << step_list[node->parent_count].m << "\t" << step_list[node->parent_count].c << "\t" << step_list[node->parent_count].b << "\t" << step_list[node->parent_count].step << endl;

		// 	if (step_list[node->parent_count].m == m_num && step_list[node->parent_count].c == c_num && step_list[node->parent_count].b == 1)
		// 	{
		// 		break;
		// 	}
		// 	node = node->parent;
		// }
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	// cout << "請輸入初始傳教士人數：";
	// cin >> m_num;
	// cout << "請輸入初始野人人數：";
	// cin >> c_num;
	m_num = 3;
	c_num = 3;
	Node start_node(m_num, c_num, 1, 0, nullptr,-1,0,0);
	
	opened_list.push_front(start_node);
	//宣告起始節點，放入steplist
	step_list_count++;
	more_step_list = (Node *)realloc(step_list, step_list_count * sizeof(Node));
	step_list = more_step_list;
	step_list[step_list_count - 1] = start_node;

	cout <<"s0:"<< step_list[0].m << step_list[0].c << step_list[0].b <<" " <<&step_list[0]<< endl;
	// Node start_node2(start_node.m+2, c_num, 1, 0, &step_list[step_list_count-1]);
	// step_list_count++;
	// more_step_list = (Node *)realloc(step_list, step_list_count * sizeof(Node));
	// step_list = more_step_list;
	// step_list[step_list_count - 1] = start_node2;

	// cout << "step list:" << endl;
	// for (int i = 0; i < step_list_count;i++)
	// {
	// 	cout << step_list[i].m << endl;
	// }
	// cout << start_node2.parent->m<<endl;
	
	Node result;
	Node *ptrresult;
	ptrresult = &result;
	a_star_algorithm(result);

	cout <<"step_list_count:" <<step_list_count<<endl;
	cout << "result:" << result.m <<" "<< result.c <<" "<<result.b<<" "<< result.step<<" "<<result.parent_count<<" "result.node_cost<<endl;
	cout << step_list[result.parent_count].step;
	
	// for (int i = 0; i < step_list_count;i++)
	// {
	// 	cout << step_list[i].m <<" "<<step_list[i].c<<" "<<step_list[i].b<<" "<<step_list[i].step<< endl;
	// }
	// output_result(&result);
	// cout <<"s0:"<< step_list[0].m << step_list[0].c << step_list[0].b << endl;
		return 0;
}