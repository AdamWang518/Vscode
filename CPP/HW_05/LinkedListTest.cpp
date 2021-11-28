#include<iostream>
#include<string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
class ListNode{
    private:
        string Name;
        int ID;
        int Balance;
        ListNode *Next=NULL;
    friend class LinkedList;
};
class LinkedList{
    private:
        ListNode *first;            // list的header，裡面不放東西，
    public:
        void Start();
        void PrintList();      // 印出list的所有資料
        void Insert(string Name, int ID, int Balance);         // 在list新增一個node
        void Search(int ID);         //找出list中特定ID的Node
        void Delete(int ID);         // 刪除list中特定ID的Node
        void Clear();           //將整份list刪除(非必要)
        void Sort();             // 將整份list依照ID排好(非必要)
};
void LinkedList::Start(){
    first = new ListNode();
    first->Next = NULL;
}
void LinkedList::PrintList(){
    ListNode *current = first->Next; 
    if (current == NULL) {                      // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return;
    }
    // 用pointer *current在list中移動
    while (current != NULL) {                 // Traversal
        cout << "-----------------------" << endl;
        cout << "Name:" << current->Name << endl;
        cout << "ID:" << current->ID << endl;
        cout << "Balance:" << current->Balance << endl;
        cout << "-----------------------" << endl;
        current = current->Next;
    }
    cout << endl;
}
void LinkedList::Insert(string Name, int ID, int Balance){
    ListNode *current=first;
    while (current->Next != NULL) {                 // Traversal
        current = current->Next;//讓current走到list的尾端，過程中順便檢查ID是否重複
        if(current->ID==ID)
        {
            cout << "The ID already exist" << endl;
            return;
        }//如果已經存在則讓使用者再次輸入        
    }
    ListNode *node = new ListNode; //創造新node
    node->Name = Name;
    node->ID = ID;
    node->Balance = Balance;
    current->Next = node; //將新的node接上去
    current = current->Next;
    current->Next = NULL;
    return;
}
void LinkedList::Search(int ID)
{
    bool Flag = false;
    ListNode *current = first->Next;
    if (current == NULL) {                      // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return;
    }
    while (current != NULL) {                 // Traversal
        if(current->ID==ID)
        {
            cout << "-----------------------" << endl;
            cout << "Name:" << current->Name << endl;
            cout << "ID:" << current->ID << endl;
            cout << "Balance:" << current->Balance << endl;
            cout << "-----------------------" << endl;
            Flag = true;
        }
        current = current->Next;
    }
    if(Flag==false)
    {
        cout << "Thist ID does Not Exist" << endl;
    }
    cout << endl;
}
void LinkedList::Delete(int ID)
{
    bool Flag = false;
    ListNode *forward = first;
    ListNode *current = first->Next;
    ListNode *backward = current->Next;
    if (current == NULL) {                      // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return;
    }
    while (current != NULL) {                 // Traversal
        if(current->ID==ID)
        {
            forward->Next = backward;
            delete current;
            cout << "delete success" << endl;
            Flag = true;
            return;
        } //將該node刪除並銜接前後的node
        forward = current;
        current = current->Next;
        backward = current->Next;
    }
    if(Flag==false)
    {
        cout << "Thist ID does Not Exist" << endl;
    }
    cout << endl;
}
int main(){
    LinkedList list;
    list.Start();
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
                list.Insert(Name, ID, Balance);
                break;
            case 2:
                cout << "Enter the ID to search" << endl;
                cin >> ID;
                list.Search(ID);
                break;
            case 3:
                cout << "Enter the ID to Delete" << endl;
                cin >> ID;
                list.Delete(ID);
                break;
            case 4:
                list.PrintList();
                break;
            case 5:
                n = 0;
                break;
            default:
                cout << "Please Enter again" << endl;
                break;
        }
    }
    return 0;
}