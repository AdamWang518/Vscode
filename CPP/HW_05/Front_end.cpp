#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define FIFO_PATH "/myfifo"
using namespace std;
int main(){
    int option;
    bool n = 1;
    string Name;
    int ID;
    int Balance;
    int fd;
    fd = open(FIFO_PATH, O_WRONLY);
    if(-1 == fd)
	{
        cout << "FIFO ERROR" << endl;
        return -1;
	}
    while(n)
    {
        cout << "--------Options--------" << endl;
        cout << "(1) insert data record" << endl;
        cout << "(2) Search data record" << endl;
        cout << "(3) Delete data record" << endl;
        cout << "(4) List all data record" << endl;
        cout << "(5) Close the front end" << endl;
        cout << "-----------------------" << endl;
        cout << "Please select an option" << endl;
        cin >> option;
        std::string tmp = std::to_string(option);
        char const *optionChar = tmp.c_str();
        write(fd, &optionChar, 1);
        if(option==1)
        {
            cout << "Enter the name:" << endl;
            cin >> Name;
            cout << "Enter the ID:" << endl;
            cin >> ID;
            cout << "Enter the balance" << endl;
            cin >> Balance;
        }
        else if(option==2)
        {
            cout << "Enter the ID to search" << endl;
            cin >> ID;
            string tmp = to_string(option);
            char const *IDChar = tmp.c_str();
            write(fd, &IDChar, 1);
        }
        else if(option==3)
        {
            cout << "Enter the ID to Delete" << endl;
            cin >> ID;
            break;
        }
        else if(option==4)
        {

        }
        else if(option==5)
        {
            n = 0;
        }
        else
        {
            cout << "Please Enter again" << endl;
        }
        // switch(option){
        //     case 1:
        //         cout << "Enter the name:" << endl;
        //         cin >> Name;
        //         cout << "Enter the ID:" << endl;
        //         cin >> ID;
        //         cout << "Enter the balance" << endl;
        //         cin >> Balance;
        //         break;
        //     case 2:
        //         cout << "Enter the ID to search" << endl;
        //         cin >> ID;
        //         string tmp = to_string(option);
        //         char const *IDChar = tmp.c_str();
        //         write(fd, &IDChar, 1);
        //         break;
        //     case 3:
        //         cout << "Enter the ID to Delete" << endl;
        //         cin >> ID;
        //         break;
        //     case 4:
        //         break;
        //     case 5:
        //         n = 0;
        //         break;
        // }
    }
    return 0;
}