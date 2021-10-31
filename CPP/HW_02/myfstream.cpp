#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "myfstream.h"

void myfstream::Read(char* str, int size){

    read(opfile, str,size);
};
void myfstream::Open(char *filename, int io){
    if(io==ios::out)
    {
        opfile = open(filename, O_WRONLY,S_IREAD|S_IWRITE);
    }
    else if(io==ios::in)
    {
        opfile = open(filename, O_RDONLY,S_IREAD|S_IWRITE);
    }
};
void myfstream::Close(){
    close(opfile);
};
void myfstream::Write(char* str,int size){
    write(opfile, str, size);
};
myfstream myfstream::operator<<(int& intInput)
{
    string num_str1(std::to_string(intInput));
    char *cstr =  const_cast<char*>(num_str1.c_str());
    // char c = intInput + '0';
    // char *str = ;
    
    write(opfile, cstr, sizeof(cstr));
    return *this;
};

myfstream myfstream::operator<<(float& floatInput)
{
    string num_str1(std::to_string(floatInput));
    const char *cstr = num_str1.c_str();
    // char c = intInput + '0';
    // char *str = ;
    write(opfile, cstr, sizeof(cstr));
    return *this;
};
myfstream myfstream::operator<<(double& doubleInput)
{
    string num_str1(std::to_string(doubleInput));
    const char *cstr = num_str1.c_str();
    // char c = intInput + '0';
    // char *str = ;
    write(opfile, cstr, sizeof(cstr));
    return *this;
};
myfstream myfstream::operator<<(char* charInput)
{
    // char c = intInput + '0';
    char *str = charInput;
    if(charInput=="\n")
    {
        Write("\n", 1);
    }
    else
    {
        Write(str, sizeof(str));
    }
    return *this;
};
myfstream myfstream::operator>>(int& intInput)
{
    string num_str1(std::to_string(intInput));
    const char *cstr = const_cast<char*>(num_str1.c_str());
    // char c = intInput + '0';
    // char *str = ;
    
    write(opfile, cstr, sizeof(cstr));
    return *this;
};

myfstream myfstream::operator>>(float& floatInput)
{
    string num_str1(std::to_string(floatInput));
    const char *cstr = const_cast<char*>(num_str1.c_str());
    // char c = intInput + '0';
    // char *str = ;
    write(opfile, cstr, sizeof(cstr));
    return *this;
};
myfstream myfstream::operator>>(double& doubleInput)
{
    string num_str1(std::to_string(doubleInput));
    char *cstr =  const_cast<char*>(num_str1.c_str());

    // char c = intInput + '0';
    // char *str = ;
    read(opfile, cstr, sizeof(cstr));
    return *this;
};
myfstream myfstream::operator>>(char* charInput)
{
    // char c = intInput + '0';
    char *str = charInput;
    read(opfile, str, sizeof(str));
    return *this;
};