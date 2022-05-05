#include <iostream>
#include <queue>

using namespace std;

struct Node{
		int m; 
		int c;
		int b;
		int step;
		double f_loss;
		Node* parent;
		Node(int M, int C, int B, int S, Node* ptr):
		m{M}, c{C}, b{B}, step{S}, parent{ptr} {
		f_loss = (m + c) / 3;
	}
};

queue<Node> opened_list;
queue<int>  closed_list;
int M, C, B, S;

bool is_safe(Node* n){
 
}
bool in_closed_list(Node* node){

}
Node* a_star_algorithm(){
 while(opened_list.size() != 0){
  // 從opened_list中取出分數最小的
  Node* node = opened_list.front();
  opened_list.pop();
  // 判斷取出的點是否為目標點
  if(node->m == 0 && node->c == 0 && node->b == 0){
   return node;
  }
  for(int i=0; i<=M; i++){  // i代表上船的傳教士數量
   for(int j=0; j<=C; j++){ // j代表上船的食人魔數量
    // 判斷船上的合理情況
    if(i+j==0 || i+j>3 || (i!=0 && i<j)){
     continue;
    }
    if(node->b == 1){ // 船在左側，下一狀態船在右側
     Node* child_node = new Node(
      node->m - i,
      node->c - j,
      0,
      node,
      node->step + 1);
     if(!in_closed_list(child_node)){
      if(is_safe(child_node))
     }
    }
    else{ // 船在右側，下一狀態船在左側

    }
   }
  }
 }
 return nullptr;
}
void output_result(){
 // pass
}

int main(int argc, char const *argv[])
{
 result = a_star_algorithm();
 output_result(result);
 return 0;
}