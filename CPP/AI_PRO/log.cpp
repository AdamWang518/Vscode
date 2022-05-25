#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <deque>
#include <vector>
#include <Windows.h>

using namespace std;

#define A_MAX_CAPACITY 2
#define B_MAX_CAPACITY 3
#define OPENED_LIST_PATH "opened_list.txt"
#define CLOSED_LIST_PATH "closed_list.txt"

int m_num, c_num;	// 全部的傳教士、食人魔數量
bool mode;			// true:最短步數 false:最少花費

struct Node{
	int m;			// 左岸傳教士數量
	int c;			// 左岸食人魔數量
	int a;			// a船的位置，1左/-1右
	int b;			// b船的位置，1左/-1右
	int step;		// 目前的總步數
	int cost;		// 目前的花費
	double f_loss;	// 啟發函式的分數
	Node* parent;	// 上一步
	Node(int m_num, int c_num, int a_state, int b_state, int step_count, int cost_num, Node* ptr): 
	m{m_num}, c{c_num}, a{a_state}, b{b_state}, step{step_count}, cost{cost_num}, parent{ptr} {
		int score = mode ? step : cost;
		f_loss = score + ((m + c) / 2.0) * (mode ? 1:30);
	}
	Node():m{0}, c{0}, a{0}, b{0}, cost{0}, step{0}, f_loss{0}, parent{nullptr} {};
};

deque<Node> opened_list;
vector<Node> closed_list;

bool is_safe(Node* n){	// 判斷兩岸的傳教士是否安全
	int m_left = n->m;
	int c_left = n->c;
	int m_right =  m_num - m_left;
	int c_right = c_num - c_left;

	if(m_left<c_left && m_left!=0)		// 左岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_right<c_right && m_right!=0)	// 右岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_left<0 || c_left<0 || m_right<0 || c_right<0)
		return false;
	if(m_left>=c_left || m_right>=c_right)
		return true;
}

int cal_value(Node* node){
	return node->m*100 + node->c*10 + node->b;
}

bool in_closed_list(Node* node){	// 確認是否已經在closed_list內
	// int value = cal_value(node);
	// if(find(closed_list.begin(), closed_list.end(), value)!=closed_list.end())
	// 	return true;
	// return false;
	for(auto x: closed_list){
		if(x.m==node->m && x.c==node->c && x.b==node->b){
			return true;
		}
	}
	return false;
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
 
}
void refresh_opened(Node* n){	// 如果節點有較低的分數或步數，更新opened_list
	for(auto x: opened_list){
		if(x.m==n->m && x.c==n->c && x.a==n->a && x.b==n->b){
			if(n->f_loss<x.f_loss){
				n->f_loss = x.f_loss;
			}
			return;
		}
	}
	opened_list.push_back(Node(n->m, n->c, n->a, n->b, n->step, n->cost, n->parent));
}

/* 輸出類函式 */
void print_node(Node* node){	// 印出節點資訊
	printf("%d\t%d\t%d\t%s\t%s\t%d\t%d\t\n", 
		mode ? node->step:node->cost,
		node->m, 
		node->c, 
		(node->a==1) ? "左":"右",
		(node->b==1) ? "左":"右",
		m_num-node->m,
		c_num-node->c
	);
}
void output_result(){
	vector<Node> path;
	Node* ptr = &closed_list.back();
	while(ptr!=nullptr){
		print_node(ptr);
		ptr = ptr->parent;
	}
}
void fprint_node(FILE* fp, Node& node){
	fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n", 
		mode ? node.step:node.cost,
		node.m, 
		node.c, 
		node.a,
		node.b,
		m_num-node.m,
		c_num-node.c
		);
}
void log_opened_list(FILE* fp){
	fprintf(fp, "Choosed Node:\n");
	fprint_node(fp, closed_list.back());
	
    fprintf(fp, "Opened List:\n");
    for(auto node: opened_list){
    	fprint_node(fp, node);
    }
    fprintf(fp, "\n");
}
void log_closed_list(FILE* fp){
    fprintf(fp, "Closed List:\n");
    for(auto node: closed_list){
    	fprint_node(fp, node);
    }
    fprintf(fp, "\n");
}
/* 主要演算法 */
void a_star_algorithm(){
	FILE* f_opened = fopen(OPENED_LIST_PATH, "a+");
	FILE* f_closed = fopen(CLOSED_LIST_PATH, "a+");
	fseek(f_opened, 0, SEEK_SET);
	fseek(f_closed, 0, SEEK_SET);

	while(opened_list.size() != 0){
		log_opened_list(f_opened);
		// 從opened_list中取出分數最小的
		Node node;
		node = opened_list.front();
		opened_list.pop_front();

		// 將取出的點加入closed_list中
		// closed_list.push_back(cal_value(&node));
		closed_list.push_back(Node(node.m, node.c, node.a, node.b, node.step, node.cost, node.parent));
		// 判斷取出的點是否為目標點
		if(node.m == 0 && node.c == 0){
			//output_result();
			return;
		}

		int carry_max_number = (A_MAX_CAPACITY>B_MAX_CAPACITY) ? A_MAX_CAPACITY : B_MAX_CAPACITY;
		for(int i=0; i<=carry_max_number; i++){		// i代表上船的傳教士數量
			for(int j=0; j<=carry_max_number; j++){	// j代表上船的食人魔數量
				// 判斷船上的合理情況
				if((i==0 && j==0)||(i+j>carry_max_number)||(i<j && i!=0)){
					continue;
				}
				// a船
				if(i+j<=A_MAX_CAPACITY){
					Node* child_node = new Node(node.m-node.a*i, node.c-node.a*j, -node.a, node.b, node.step+1, node.cost+3, &closed_list.back());
					if(!in_closed_list(child_node)){
						if(is_safe(child_node)){
							refresh_opened(child_node);
						}
					}
				}
				// b船
				Node* child_node = new Node(node.m-node.b*i, node.c-node.b*j, node.a, -node.b, node.step+1, node.cost+25, &closed_list.back());
				if(!in_closed_list(child_node)){
					if(is_safe(child_node)){
						refresh_opened(child_node);
					}
				}
			}
		}
		sort_bubble(opened_list);
		log_closed_list(f_closed);
	}

}

/* Main function */
int main(int argc, char const *argv[]){
	SetConsoleOutputCP(CP_UTF8);
	// 選擇模式
	int x;
	cout << "\n1.最短步數\n2.最少花費\n\n請選擇模式: ";
	cin >> x;
	while(x!=1 && x!=2){
		cout << "輸入的數字錯誤，請重試。\n";
		cout << "\n1.最短步數\n2.最少花費\n\n請選擇模式: ";
		cin >> x;
	}
	mode = (x==1) ? true : false;	// true:最短步數 false:最少花費
	// input parameter
	cout << "請輸入初始傳教士人數：";
	cin >> m_num;
	cout << "請輸入初始野人人數：";
	cin >> c_num;

	closed_list.reserve((m_num+1) * (c_num+1) * 2 * 2 + 1);
	Node start_node(m_num, c_num, 1, 1, 0, 0, nullptr);
	opened_list.push_back(start_node);

	a_star_algorithm();

	// check_closedlist();
	// cout << endl;
	output_result();

	// m_num = 2;
	// c_num = 0;
	// Node a(2,0,0,0,nullptr);
	// Node b(0,0,1,1,&a);
	// output_result();
	return 0;
}