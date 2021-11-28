#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
int main(){
    int option;
    bool n = 1;
    string Name;
    int ID;
    int Balance;
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
        switch(option){
            case 1:
                cout << "Enter the name:" << endl;
                cin >> Name;
                cout << "Enter the ID:" << endl;
                cin >> ID;
                cout << "Enter the balance" << endl;
                cin >> Balance;
                break;
            case 2:
                cout << "Enter the ID to search" << endl;
                break;
            case 3:
                cout << "Enter the ID to Delete" << endl;
                break;
            case 4:
                break;
            case 5:
                n = 0;
                break;
        }
    }
    return 0;
}