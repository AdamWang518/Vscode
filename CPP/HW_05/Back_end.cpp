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
    int number;  //計有幾筆資料

public:
    void Start();
    void PrintList(char** data,int max_len,int *length);                               // 印出list的所有資料
    int Insert(char Name[50], int ID, int Balance); // 在list新增一個node
    int Search(int ID, char *Array,int max_len);                //找出list中特定ID的Node
    int Delete(int ID, char *Array,int max_len);                             // 刪除list中特定ID的Node
    void BackUp();                        //備份所有內容到backupt.txt
    void calNumber();
    int getNumber();
    void setNumber(int a);
};
void LinkedList::BackUp()
{
    FILE *fp = fopen("backup.txt", "w");    //每次改動(Inser,Delete)後都重新寫入全部的資料。
    if(fp==0){
        cout << "無法開啟backup.txt" << endl;
        return;
    }
    ListNode *current = first->Next;
    while(current!=NULL){
	    char buf[100];
	    int length = snprintf(buf, 100, "Name:%s\nID:%d\nBalance:%d\n", current->Name, current->ID, current->Balance);
	    fwrite(buf, 1, length, fp);
	    current = current->Next;
    }
    // 關檔
    fclose(fp);
}
void LinkedList::calNumber()
{
    ListNode *current = first->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        number=0;
        return;
    }
    while (current != NULL)
    { // Traversal
        number++;
        current = current->Next;
    }
}
void LinkedList::setNumber(int a)
{
    number = a;
    return ;
}
int LinkedList::getNumber()
{
    return number;
}
void LinkedList::Start()
{
    first = new ListNode();
    first->Next = NULL;
}
void LinkedList::PrintList(char** Array,int max_len,int *length)
{
    bool Flag = false;
    ListNode *current = first->Next;
    int i = 0;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        length[0] = snprintf(Array[0], max_len, "List is empty.");
        return;
    }
    while(current!=NULL)
    {
        length[i] = snprintf(Array[i], max_len, "Name:%s\nID:%d\nBalance:%d", current->Name, current->ID, current->Balance);
        i++;
        current = current->Next;
    }
    return;
}
int LinkedList::Insert(char Name[50], int ID, int Balance)
{
    ListNode *current = first->Next;
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
    number++;
    return 1;
}
int LinkedList::Search(int ID,char *Array,int max_len)
{//不要分類直接回傳char 陣列
    bool Flag = false;
    ListNode *current = first->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        int length = snprintf(Array,max_len,  "List is empty.");
        return length;
    }
    while (current != NULL)
    { // Traversal
        if (current->ID == ID)
        {
            int length = snprintf(Array,max_len, "Name:%s\nID:%d\nBalance:%d", current->Name, current->ID, current->Balance);
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
        int length = snprintf(Array, max_len, "This ID does Not Exist");
        cout << "Thist ID does Not Exist" << endl;
        return length;
    }
}
int LinkedList::Delete(int ID, char *Array,int max_len)
{
    bool Flag = false;
    ListNode *forward = first;
    ListNode *current = first->Next;
    if (current == NULL)
    { // 如果current指向NULL, 表示list沒有資料
        int length = snprintf(Array, max_len, "List is empty.");
        return length;
    }
    while (current != NULL)
    { // Traversal
        if (current->ID == ID)
        {
            forward->Next = current->Next;
            delete current;
            int length = snprintf(Array,max_len,  "Delete success");
            number--;
            Flag = true;
            return length;
        } //將該node刪除並銜接前後的node
        forward = current;
        current = current->Next;
    }
    if (Flag == false)
    {
        int length = snprintf(Array,max_len,  "This ID does Not Exist");
        return length;
    }
}

int main()
{
    LinkedList list;
    list.Start();
    list.calNumber();
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
            read(back, &NameChar, sizeof(NameChar));
            read(back, &ID, sizeof(ID));
            read(back, &Balance, sizeof(Balance));
            flag = list.Insert(NameChar, ID, Balance);
            write(front, &flag, sizeof(flag));
            list.BackUp();//新增後備份
        }
        else if(option==2)
        {
            char output[100]="";
            read(back, &ID, sizeof(ID));
            int length = list.Search(ID, output, 100);
            write(front, &length, sizeof(int));
            write(front, &output, sizeof(char) * length);
        }
        else if(option==3)
        {
            char output[100]="";
            read(back, &ID, sizeof(int));
            int length = list.Delete(ID, output, 100);
            write(front, &length, sizeof(int));
            write(front, &output, sizeof(char) * length);
            list.BackUp();//刪除後備份
        }
        else if(option==4)
        {
            int number = list.getNumber();
            write(front, &number, sizeof(int));
            if(number==0)
            {
                number++;
            }//由於在number==0時依舊會回傳資料為空，因此還是要宣告陣列
            char **Pack = new char *[number];
            int *Length = new int[number];
            for (int i = 0; i < number;i++)
            {
                Pack[i] = new char[100];
            }
            list.PrintList(Pack, 100, Length);
            for (int i=0; i < number;i++)
            {
                write(front, &Length[i], sizeof(int));
                write(front, Pack[i], sizeof(char) * Length[i]);
            }
            for (int i = 0; i < number; i++)
            {
                delete[] Pack[i];
            }
            delete[] Pack;
            delete[] Length;
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