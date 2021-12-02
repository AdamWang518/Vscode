#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<fstream>
#include<unistd.h>
#include<cstring>
#include <errno.h>
#define FIFO_PATH "fifo"
#define MY_FIFO_PATH "myfifo"
using namespace std;
class ListNode{
    private:
        char Name[50];
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
        int Insert(char Name[50], int ID, int Balance);         // 在list新增一個node
        int Search(int ID);         //找出list中特定ID的Node
        int Delete(int ID);         // 刪除list中特定ID的Node
        void BackUp(int signum);              //在被SIGINT時備份所有內容
        void ReBuild();         //以備份重建List
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
int LinkedList::Insert(char Name[50], int ID, int Balance){
    ListNode *current=first;
    while (current->Next != NULL) {                 // Traversal
        current = current->Next;//讓current走到list的尾端，過程中順便檢查ID是否重複
        if(current->ID==ID)
        {
            cout << "The ID already exist" << endl;
            return 0;
        }//如果已經存在則讓使用者再次輸入        
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
int LinkedList::Search(int ID)
{
    bool Flag = false;
    ListNode *current = first->Next;
    if (current == NULL) {                      // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return 0;
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
        return 2;
    }
    else{
        return 1;
    }
    cout << endl;
}
int LinkedList::Delete(int ID)
{
    bool Flag = false;
    ListNode *forward = first;
    ListNode *current = first->Next;
    ListNode *backward = current->Next;
    if (current == NULL) {                      // 如果current指向NULL, 表示list沒有資料
        cout << "List is empty.\n";
        return 0;
    }
    while (current != NULL) {                 // Traversal
        if(current->ID==ID)
        {
            forward->Next = backward;
            delete current;
            cout << "delete success" << endl;
            Flag = true;
            return 1;
        } //將該node刪除並銜接前後的node
        forward = current;
        current = current->Next;
        backward = current->Next;
    }
    if(Flag==false)
    {
        cout << "Thist ID does Not Exist" << endl;
        return 2;
    }
    cout << endl;
}

int main(){
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
    int fd;
    int wfd;
    int flag;
    /*建立FIFO*/
     if(mkfifo(FIFO_PATH, 0777)==-1||mkfifo(MY_FIFO_PATH, 0777)==-1){
        if(errno!=EEXIST){
            printf("建立FIFO檔發生錯誤，程式中止。\n");
            return 2;
        }
    }
    wfd = open(MY_FIFO_PATH, O_WRONLY);
    fd = open(FIFO_PATH, O_RDONLY);
	/*打開FIFO*/
	while(1)
	{
        
	    if(-1 == fd)
	    {
            cout << "Error" << endl;
            return -1;
	    }
        if(-1==wfd)
        {
            cout << "Error" << endl;
            return -1;
        }
        read(fd, &option, sizeof(option));
        cout << option << endl;
        switch(option){
            case 1:
                cout << "Enter the name:" << endl;
                //cin >> Name;
                read(fd, &NameChar, sizeof(NameChar));
                //cout << NameChar << endl;
                cout << "Enter the ID:" << endl;
                //cin >> ID;
                read(fd, &ID, sizeof(ID));
                cout << "Enter the balance" << endl;
                //cout << ID << endl;
                //cin >> Balance;
                read(fd, &Balance, sizeof(Balance));
                //cout << Balance << endl;
                flag=list.Insert(NameChar, ID, Balance);
                //write(wfd, &flag, sizeof(flag));
                break;
            case 2:
                cout << "Enter the ID to search" << endl;
                //cin >> ID;
                read(fd, &ID, sizeof(ID));
                flag=list.Search(ID);
                //write(wfd, &flag, sizeof(flag));
                break;
            case 3:
                cout << "Enter the ID to Delete" << endl;
                //cin >> ID;
                read(fd, &ID, sizeof(ID));
                flag=list.Delete(ID);
                //write(wfd, &flag, sizeof(flag));
                break;
            case 4:
                list.PrintList();
                break;
            case 5:
                cout << "Front end closed" << endl;
                break;
            default:
                cout << "Please Enter again" << endl;
                break;
        }
        close(fd);
        close(wfd);
    }
    return 0;
}