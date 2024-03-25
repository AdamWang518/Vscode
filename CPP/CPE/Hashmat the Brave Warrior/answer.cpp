#include<iostream>
#include<algorithm>
using namespace std;
int main(){
    long hashmat, enemy;
    while(true){
        cin>>hashmat>>enemy;
        cout << abs(hashmat - enemy) << endl;
    }
}
