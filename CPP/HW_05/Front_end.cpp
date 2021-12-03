#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#define FIFO_PATH "fifo"
#define MY_FIFO_PATH "myfifo"

using namespace std;
int main()
{
    int back;
    int front;
    int ret;
    char c;
    int flag;
    int option;
    bool n = 1;
    char FIFO[] = "fifo";
    char MYFIFO[] = "myfifo";
    int ID;
    int Balance;
    back = open("back", O_WRONLY);   //stuck here
    front = open("front", O_RDONLY); //or here
    while (n)
    {
        char Namechar[50];

        if (-1 == back)
        {
            printf("ERROR/n");
            return -1;
        }
        cout << "--------Options--------" << endl;
        cout << "(1) insert data record" << endl;
        cout << "(2) Search data record" << endl;
        cout << "(3) Delete data record" << endl;
        cout << "(4) List all data record" << endl;
        cout << "(5) Close the front end" << endl;
        cout << "-----------------------" << endl;
        cout << "Please select an option" << endl;
        cin >> option;
        if (option <= 0 || option > 5)
        {
            cout << "wrong number" << endl;
            continue;
        }
        write(back, &option, sizeof(option));
        if(option==1)
        {
            cout << "Enter the name:" << endl;
            cin >> Namechar;
            write(back, &Namechar, sizeof(Namechar));
            cout << "Enter the ID:" << endl;
            cin >> ID;
            write(back, &ID, sizeof(ID));
            cout << "Enter the balance" << endl;
            cin >> Balance;
            write(back, &Balance, sizeof(Balance));
            read(front, &flag, sizeof(flag));
            if (flag == 0)
            {
                cout << "ID already exist" << endl;
            }
            else if (flag == 1)
            {
                cout << "Insert scuccess" << endl;
            }
        }
        else if(option==2)
        {
            cout << "Enter the ID to search" << endl;
            cin >> ID;
            write(back, &ID, sizeof(ID));
            //read(front, &flag, sizeof(flag));
            int length;
            char Pack[100] = "";
            //收一包char陣列就好
            // 先Read他的length
            // 再依照length去收char陣列
            read(front, &length, sizeof(int));
            read(front, &Pack, length);
            cout << "-----------------------" << endl;
            cout << Pack << endl;
            cout << "-----------------------" << endl;
        }
        else if(option==3)
        {
            cout << "Enter the ID to Delete" << endl;
            cin >> ID;
            write(back, &ID, sizeof(ID));
            read(front, &flag, sizeof(flag));
            if (flag == 0)
            {
                cout << "List is empty.\n";
            }
            else if (flag == 1)
            {
                cout << "delete success" << endl;
            }
            else if (flag == 2)
            {
                cout << "Thist ID does Not Exist" << endl;
            }
        }
        else if(option==4)
        {
            int number;
            read(front, &number, sizeof(int));
            char **Array;
            Array = new char*[number];
            for (int i = 0; i < number; i++)
            {
                Array[i] = new char[100];
            }
            if (number == 0)
            {
                cout << "-----------------------" << endl;
                cout << "List is empty" << endl;
                cout << "-----------------------" << endl;
            }
            for (int i = 0; i < number;i++)
            {
                cout << "-----------------------" << endl;
                cout << number << endl;
                cout << "-----------------------" << endl;
            }
            for (int i = 0; i < number;i++)
            {
                delete[] Array[i];
            }
            delete[] Array;
        }
        else if(option==5)
        {
            n = 0;
        }
    }
    close(back);
    close(front);
    return 0;
}