#include<iostream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include<fstream>
#define FIFO_PATH "/myfifo"
using namespace std;
class ListNode{
    private:
        string Name;
        int ID;
        int Balance;
        ListNode *Next;
    friend class LinkedList;
};
class LinkedList{
    private:
        ListNode *first;            // list的header，裡面不放東西，
    public:
        void PrintList();      // 印出list的所有資料
        void Insert(string Name, int ID, int Balance);         // 在list新增一個node
        void Search(int ID);         //找出list中特定ID的Node
        void Delete(int ID);         // 刪除list中特定ID的Node
        void Clear();           //將整份list刪除(非必要)
        void Sort();             // 將整份list依照ID排好(非必要)
};
int main(){
    int ret;
	int fd;
	char buffer;
	int nread;
	int i;
	/*建立FIFO*/
	ret = mkfifo(FIFO_PATH, 0777);
	/*打開FIFO*/
	fd = open(FIFO_PATH, O_RDONLY);
	if(-1 == fd)
	{
		printf("error/n");
		return -1;
	}
	while(1)
	{
		nread = read(fd, &buffer, 1);
		if(nread > 0) 
		{
			buffer = toupper(buffer);
			printf("%c", buffer);
		}
	}
    return 0;
}