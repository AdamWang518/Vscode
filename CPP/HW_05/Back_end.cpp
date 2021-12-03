#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#define FIFO_PATH "fifo"
#define MY_FIFO_PATH "myfifo"
using namespace std;
class ListNode
{
private:
    char Name[50];
    int ID;
    int Balance;
    ListNode *Next = NULL;
    friend class LinkedList;
};
class LinkedList
{
private:
    ListNode *first; // list的header，裡面不放東西，
    int number = 0;  //計有幾筆資料

public:
    void Start();
    void PrintList(int max_len);                               // 印出list的所有資料
    int Insert(char Name[50], int ID, int Balance); // 在list新增一個node
    int Search(int ID, char *Array,int max_len);                //找出list中特定ID的Node
    int Delete(int ID);                             // 刪除list中特定ID的Node
    void BackUp(int signum);                        //在被SIGINT時備份所有內容
    void ReBuild();                                 //以備份重建List
    void Clear();                                   //將整份list刪除(非必要)
    void Sort();                                    // 將整份list依照ID排好(非必要)
    int getNumber();
};
int LinkedList::getNumber()
{
    return number;
}
void LinkedList::Start()
{
    first = new ListNode();
    first->Next = NULL;
}
void LinkedList::PrintList(int max_len)
{
    ListNode *current = first->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return;
    }
    // 用pointer *current在list中移動
    while (current != NULL)
    { // Traversal
        cout << "-----------------------" << endl;
        cout << "Name:" << current->Name << endl;
        cout << "ID:" << current->ID << endl;
        cout << "Balance:" << current->Balance << endl;
        cout << "-----------------------" << endl;
        current = current->Next;
    }
    cout << endl;
}
int LinkedList::Insert(char Name[50], int ID, int Balance)
{
    ListNode *current = first;
    number = 0;
    while (current->Next != NULL)
    {                            // Traversal
        current = current->Next; //讓current走到list的尾端
        number++;
    }
    current = first;
    while (current->Next != NULL)
    {                            // Traversal
        current = current->Next; //讓current走到list的尾端，過程中順便檢查ID是否重複
        if (current->ID == ID)
        {
            cout << "The ID already exist" << endl;
            return 0;
        } //如果已經存在則讓使用者再次輸入
    }
    ListNode *node = new ListNode; //創造新node
    strcpy(node->Name, Name);
    node->ID = ID;
    node->Balance = Balance;
    current->Next = node; //將新的node接上去
    current = current->Next;
    current->Next = NULL;
    cout << "Insert Success" << endl;
    return 1;
}
int LinkedList::Search(int ID,char *Array,int max_len)
{//不要分類直接回傳char 陣列
    bool Flag = false;
    ListNode *current = first->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        int length = snprintf(Array,max_len,  "List is empty.\n");
        return length;
    }
    while (current != NULL)
    { // Traversal
        if (current->ID == ID)
        {
            int length = snprintf(Array,max_len, "Name:%s\nID:%d\nBalnce:%d", current->Name, current->ID, current->Balance);
            // cout << "-----------------------" << endl;
            // cout << "Name:" << current->Name << endl;
            // cout << "ID:" << current->ID << endl;
            // cout << "Balance:" << current->Balance << endl;
            // cout << "-----------------------" << endl;
            return length;
        }
        current = current->Next;
    }
    if (Flag == false)
    {
        int length = snprintf(Array, max_len, "Thist ID does Not Exist");
        cout << "Thist ID does Not Exist" << endl;
        return length;
    }
}
int LinkedList::Delete(int ID)
{
    bool Flag = false;
    ListNode *forward = first;
    ListNode *current = first->Next;
    ListNode *backward = current->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return 0;
    }
    while (current != NULL)
    { // Traversal
        if (current->ID == ID)
        {
            forward->Next = backward;
            delete current;
            cout << "delete success" << endl;
            number--;
            Flag = true;
            return 1;
        } //將該node刪除並銜接前後的node
        forward = current;
        current = current->Next;
        backward = current->Next;
    }
    if (Flag == false)
    {
        cout << "Thist ID does Not Exist" << endl;
        return 2;
    }
    cout << endl;
}

int main()
{
    LinkedList list;
    list.Start();
    int option;
    bool n = 1;
    //string Name;
    char NameChar[50];
    int ID;
    int Balance;
    int ret;
    int wet;
    int back;
    int front;
    int flag;
    /*建立FIFO*/
    if (mkfifo("back", 0777) == -1 || mkfifo("front", 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("建立FIFO檔發生錯誤，程式中止。\n");
            return 2;
        }
    }
    back = open("back", O_RDONLY);
    front = open("front", O_WRONLY);
    if (back == -1 || front == -1)
    {
        cout << "開檔錯誤。\n";
    }
    /*打開FIFO*/
    while (1)
    {

        if (back == -1)
        {
            cout << "Error" << endl;
            return -1;
        }
        if (front == -1)
        {
            cout << "Error" << endl;
            return -1;
        }
        read(back, &option, sizeof(int));
        if(option==1)
        {
            // cout << "Enter the name:" << endl;
            //cin >> Name;
            read(back, &NameChar, sizeof(NameChar));
            //cout << NameChar << endl;
            //cout << "Enter the ID:" << endl;
            //cin >> ID;
            read(back, &ID, sizeof(ID));
            //cout << "Enter the balance" << endl;
            //cout << ID << endl;
            //cin >> Balance;
            read(back, &Balance, sizeof(Balance));
            //cout << Balance << endl;
            flag = list.Insert(NameChar, ID, Balance);
            write(front, &flag, sizeof(flag));
        }
        else if(option==2)
        {
           // cout << "Enter the ID to search" << endl;
            read(back, &ID, sizeof(ID));
            char output[100]="";
            int length = list.Search(ID, output, 100);
            write(front, &length, sizeof(int));
            write(front, &output, length);
        }
        else if(option==3)
        {
            //cout << "Enter the ID to Delete" << endl;
            //cin >> ID;
            read(back, &ID, sizeof(ID));
            flag = list.Delete(ID);
            write(front, &flag, sizeof(flag));
        }
        else if(option==4)
        {
            list.PrintList(100);
        }
        else if(option==5)
        {
            cout << "Front end closed" << endl;
            break;
        }
        
        option = 0;
    }
    close(back);
    close(front);

    return 0;
}