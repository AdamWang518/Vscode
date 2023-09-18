#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    ifstream inputFile("input.txt");
    if (!inputFile)
    {
        cout << "無法開啟檔案！" << endl;
        return 1;
    }

    string number;
    while (getline(inputFile, number))
    {
        if (number == "0")
        {
            break;
        }
        if (stoi(number)%11==0)
        {
            cout << number << " is a multiple of 11." << endl;
        }
        else
        {
            cout << number << " is not a multiple of 11." << endl;
        }
    }
    inputFile.close();
    return 0;
}
