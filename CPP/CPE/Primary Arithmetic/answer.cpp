#include <iostream>
using namespace std;
int main()
{
    int a, b, temp = 0, carry = 0;
    cin >> a >> b;
    while (a != 0 || b != 0)
    {
        while (a > 0 || b > 0)
        {
            if (a % 10 + b % 10 + temp > 9)
            {
                carry++;
                temp = 1;
            }
            else
            {
                temp = 0;
            }
            a /= 10;
            b /= 10;
        }
        if (carry <= 1)
        {
            if (carry == 0)
            {
                cout << "No carry operation." << endl;
            }
            else
            {
                cout << carry << " carry operation." << endl;
            }
        }
        else
        {
            cout << carry << " carry operations." << endl;
        }
        cin >> a >> b;
        carry = 0;
    }
    return 0;
}
