#include <iostream>
#include <cstring>
#include <deque>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

#define MAX_CAPACITY 2

struct Node{
	int m;			// 左岸傳教士數量
	int c;			// 左岸食人魔數量
	int b;			// 船的位置，1左/0右
	int step;		// 目前的總步數
	double f_loss;	// 啟發函式的分數
	Node* parent;	// 上一步
	Node(int m_num, int c_num, int boat_state, int step_count, Node* ptr): m{m_num}, c{c_num}, b{boat_state}, step{step_count}, parent{ptr} {
		f_loss = (m + c) / 2.0;
		// cout << f_loss << endl;
	}
	Node():m{0}, c{0}, b{0}, step{0}, f_loss{0}, parent{nullptr} {};
};

void print_node(Node* node);
deque<Node> opened_list;
// vector<int> closed_list;
vector<Node> closed_list;
int m_num, c_num;	// 全部的傳教士、食人魔數量

bool is_safe(Node* n){	// 判斷兩岸的傳教士是否安全
	int m_left = n->m;
	int c_left = n->c;
	int m_right =  m_num - m_left;
	int c_right = c_num - c_left;

	if(m_left<c_left && m_left!=0)		// 左岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_right<c_right && m_right!=0)	// 右岸食人魔數量不可超過傳教士數量，除非傳教士數量為0
		return false;
	if(m_left<0 || c_left<0)
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

void sort_by_floss(){	// 將opened_list內的點按照分數大小排序
	for(auto i=opened_list.begin(); i!=opened_list.end(); i++){
		for(auto j=i+1; j!=opened_list.end(); j++){
			if(j->f_loss>=i->f_loss){
				continue;
			}
			else{
				swap(i, j);
			}
		}
	}
}
void refresh_opened(Node* n){	// 如果節點有較低的分數或步數，更新opened_list
	for(auto x: opened_list){
		if(x.m==n->m && x.c==n->c && x.b==n->b){
			if(n->f_loss<x.f_loss || n->step<x.step){
				memcpy(&x, n, sizeof(Node));
			}
			return;
		}
	}
	opened_list.push_back(Node(n->m, n->c, n->b, n->step, n->parent));
}
void check_openlist()
{
 	for(deque<Node>::iterator it=opened_list.begin(); it!=opened_list.end();it++){
  		cout << it->m << " " << it->c << " " << it->b << " " << it->step<<"/";
 	}
 	cout << endl;
}
void check_closedlist()
{	
	cout << "CLOSED LIST:\n";
	cout << "STEP\tM_LEFT\tC_LEFT\tBOAT\tM_RIGHT\tC_RIGHT\tPARENT ADDRESS\n";
 	for(auto it:closed_list){
  		print_node(&it);
 	}
}

/* 主要演算法 */
void a_star_algorithm(){
	while(opened_list.size() != 0){
		// 從opened_list中取出分數最小的
		Node node;
		node = opened_list.front();
		opened_list.pop_front();

		// 將取出的點加入closed_list中
		// closed_list.push_back(cal_value(&node));
		closed_list.push_back(Node(node.m, node.c, node.b, node.step, node.parent));

		// 判斷取出的點是否為目標點
		if(node.m == 0 && node.c == 0 && node.b == 0){
			break;
		}

		for(int i=0; i<=MAX_CAPACITY; i++){		// i代表上船的傳教士數量
			for(int j=0; j<=MAX_CAPACITY; j++){	// j代表上船的食人魔數量
				// 判斷船上的合理情況
				if((i==0 && j==0)||(i+j>MAX_CAPACITY)||(i<j && i!=0)){
					continue;
				}
				if(node.b == 1){	// 船在左側，下一狀態船在右側
					if(i>node.m||j>node.c){
						continue;
					}
					Node* child_node = new Node(node.m-i, node.c-j, 0, node.step+1, &closed_list.back());
					if(!in_closed_list(child_node)){
						if(is_safe(child_node)){
							refresh_opened(child_node);
						}
					}
				}
				else{	// 船在右側，下一狀態船在左側
					if(i>m_num-node.m||j>c_num-node.c){
						continue;
					}
					Node* child_node = new Node(node.m+i, node.c+j, 1, node.step+1, &closed_list.back());
					if(!in_closed_list(child_node)){
						if(is_safe(child_node)){
							refresh_opened(child_node);
						}
					}
				}
			}
		}
		sort_by_floss();
		// check_closedlist();
	}

}

/* 輸出類函式 */
void print_node(Node* node){	// 印出節點資訊
	printf("%d\t%d\t%d\t%s\t%d\t%d\t%p\n", node->step, node->m, node->c, node->b?"左":"右", m_num-node->m, c_num-node->c, node->parent);
}
void output_result(){
	vector<Node> path;
	Node* ptr = &closed_list.back();
	while(ptr!=nullptr){
		print_node(ptr);
		ptr = ptr->parent;
	}
}

/* Main function */
int main(int argc, char const *argv[]){
	SetConsoleOutputCP(CP_UTF8);

	cout << "請輸入初始傳教士人數：";
	cin >> m_num;
	cout << "請輸入初始野人人數：";
	cin >> c_num;

	Node start_node(m_num, c_num, 1, 0, nullptr);
	opened_list.push_back(start_node);

	a_star_algorithm();

	check_closedlist();
	// output_result();

	// m_num = 2;
	// c_num = 0;
	// Node a(2,0,0,0,nullptr);
	// Node b(0,0,1,1,&a);
	// output_result(b);
	return 0;
}