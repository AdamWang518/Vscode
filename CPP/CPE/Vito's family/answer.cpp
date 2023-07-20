
#include <algorithm>
#include <iostream>
using namespace std;
int main()
{
    int testcase; // how many testcase?
    cin >> testcase;
    for (int i = 0; i < testcase; i++)
    {
        int distance = 0;
        int r;
        cin >> r;
        int s[500]; // 每個親戚的門牌號碼，上限500人
        int mid;
        for (int j = 0; j < r; j++)
        {
            cin >> s[j];
        }
        sort(s, s + r);//排列
        mid = s[(int)r / 2]; // 取中位數
        for (int j = 0; j < r; j++)
        {
            distance += abs(s[j] - mid);
        }
        cout << distance << endl;
    }
    return 0;
}