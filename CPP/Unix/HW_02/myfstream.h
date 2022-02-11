#include <string>
using namespace std;
class myfstream{
    
    public:
        void Open(char *filename, int out);
        void Read(char *str,int size);
        void Write(char *str, int size);
        void Test();
        void Close();
        myfstream operator<<(char* charInput);
        myfstream operator<<(int& intInput);
        myfstream operator<<(float& floatInput);
        myfstream operator<<(double& doubleInput);
        myfstream operator>>(char* charInput);
        myfstream operator>>(int& intInput);
        myfstream operator>>(float& floatInput);
        myfstream operator>>(double& doubleInput);
        int opfile;
};