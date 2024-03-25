#include <algorithm>
#include <iostream>
using namespace std;
int calculate(int number)
{
    int count = 1;
    while (number != 1)
    {
        count++;
        if (number % 2 == 1)
        {
            number = 3 * number + 1;
        }
        else
        {
            number = number / 2;
        }
    }
    return count;
}
int main()
{
    int i, j, max;
    int number1, number2, temp;
    while (cin >> i >> j)
    {
        max = 0;
        number1 = i;
        number2 = j;
        if (number1 > number2)
        {
            temp = number1;
            number1 = number2;
            number2 = temp;
        }
        for (int a = number1; a <= number2; a++)
        {
            int b = calculate(a);
            if (b > max)
            {
                max = b;
            }
        }
        cout << i << " " << j << " " << max << endl;
    }
}