#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include<unistd.h>
#define FIFO_PATH "myfifo"
using namespace std;
int main(){
    int fd;
	int ret;
	char c;
	
    int option;
    bool n = 1;
    string Name;
    int ID;
    int Balance;
    while(n)
    {
        fd = open(FIFO_PATH, O_WRONLY);
	    if(-1 == fd)
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
        // std::string tmp = std::to_string(option);
        // char const *option_char = tmp.c_str();
        write(fd, &option, sizeof(option));
        switch(option){
            case 1:
                cout << "Enter the name:" << endl;
                cin >> Name;
                //write(fd, &Name, sizeof(Name));
                cout << "Enter the ID:" << endl;
                cin >> ID;
                //write(fd, &ID, sizeof(ID));
                cout << "Enter the balance" << endl;
                cin >> Balance;
                //write(fd, &Balance, sizeof(Balance));
                break;
            case 2:
                cout << "Enter the ID to search" << endl;
                cin >> ID;
                //write(fd, &ID, sizeof(ID));
                break;
            case 3:
                cout << "Enter the ID to Delete" << endl;
                cin >> ID;
                //write(fd, &ID, sizeof(ID));
                break;
            case 4:
                break;
            case 5:
                n = 0;
                break;
            default:
                cout << "Please Enter again" << endl;
                break;
        }
        close(fd);
    }
    return 0;
}