#include <iostream>
using namespace std;
// 注意，此題的對稱矩陣跟線性代數裡的定義不一樣
int main()
{
    long long a[101][101], T;
    cin >> T;
    bool ifsym = true;
    for (int i = 0; i < T; i++)
    {
        int n;
        char ch;
        cin >> ch >> ch >> n;
        ifsym = true;
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                int buff;
                cin >> buff;
                a[j][k] = buff;
                if (a[j][k] < 0)
                {
                    ifsym = false;
                }
            }
        }

        for (int j = 0; j <= n / 2; j++)
        {
            if (ifsym == false)
            {
                break;
            }
            for (int k = 0; k < n - j; k++)
            {
                // 在最大的row要檢查最小的col，反之亦然
                if (a[j][k] != a[n - 1 - j][n - 1 - k])
                {
                    ifsym = false;
                    break;
                }
            }
        }
        if (ifsym == true)
        {
            printf("Test #%d: Symmetric", i + 1);
        }
        else
        {
            printf("Test #%d: Non-symmetric", i + 1);
        }
    }
    return 0;
}