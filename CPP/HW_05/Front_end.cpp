#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include<unistd.h>
#define FIFO_PATH "fifo"
#define MY_FIFO_PATH "myfifo"
using namespace std;
int main(){
    int fd;
    int rfd;
    int ret;
	char c;
    int flag;
    int option;
    bool n = 1;
    
    int ID;
    int Balance;
    while(n)
    {
        char Namechar[50];
        fd = open(FIFO_PATH, O_WRONLY);
        rfd = open(MY_FIFO_PATH, O_RDONLY);
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
        write(fd, &option, sizeof(option));
        switch(option){
            case 1:
                cout << "Enter the name:" << endl;
                cin >> Namechar;
                //char *NameChar = Name.c_str();
                write(fd, &Namechar, sizeof(Namechar));
                cout << "Enter the ID:" << endl;
                cin >> ID;
                write(fd, &ID, sizeof(ID));
                cout << "Enter the balance" << endl;
                cin >> Balance;
                write(fd, &Balance, sizeof(Balance));
                read(rfd, &flag, sizeof(flag));
                if(flag==0)
                {
                    cout << "ID already exist" << endl;
                }
                else if(flag==1)
                {
                    cout << "Inser scuccess" << endl;
                }
                break;
            case 2:
                cout << "Enter the ID to search" << endl;
                cin >> ID;
                write(fd, &ID, sizeof(ID));
                read(rfd, &flag, sizeof(flag));
                if(flag==0)
                {
                    cout << "List is empty" << endl;
                }
                else if(flag==1)
                {
                    read(rfd, &Namechar, sizeof(Namechar));
                    read(rfd, &ID, sizeof(ID));
                    read(rfd, &Balance, sizeof(Balance));
                    cout << "-----------------------" << endl;
                    cout << "Name:" << Namechar << endl;
                    cout << "ID:" << ID << endl;
                    cout << "Balance:" << Balance << endl;
                    cout << "-----------------------" << endl;
                }
                else if(flag==2)
                {
                    cout << "Thist ID does Not Exist" << endl;
                }
                break;
            case 3:
                cout << "Enter the ID to Delete" << endl;
                cin >> ID;
                write(fd, &ID, sizeof(ID));
                read(rfd, &flag, sizeof(flag));
                if(flag==0)
                {
                    cout << "List is empty.\n";
                }
                else if(flag==1)
                {
                     cout << "delete success" << endl;
                }
                else if(flag==2)
                {
                    cout << "Thist ID does Not Exist" << endl;
                }
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
        close(rfd);
    }
    return 0;
}