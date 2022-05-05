#include <iostream>
#include <queue>
using namespace std;
int M;
int C;
int K;

class Node{
    public:
        Node(int mInt,int cInt,bool bBool,Node *parentNode,int stepInt){
            step = stepInt;
            m = mInt;
            c = cInt;
            b = bBool;
            parent = parentNode;
            f_loos = stepInt + mInt + cInt - K * bBool;
        }
        int step;//深度
        int m;//傳教士在右岸人數
        int c;//食人族在右岸人數
        int b;//A船位置
        int f_loos;//f(n)
        Node *parent;//父節點
};
class Search{
    public:
        void init();
        void refreshOpen();
    private:
        queue<Node> opened;
        queue<Node> closed;
};
void Search::init(){
    Node startNode(M,C,1,NULL,0);
    opened.push(startNode);
};
void Search::refreshOpen(){
    queue<Node> tmpOpen = opened;
    for (int i = 0; i < tmpOpen.size();i++)
    {

    }
};
int main(){
    cout << "請輸入傳教士人數:\n";
    cin >> M;
    cout << "請輸入食人族人數:\n";
    cin >> C;
    cout << "請輸入A船人數上限:\n";
    cin >> K;
    Search search;
    search.init();
}