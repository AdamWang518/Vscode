#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "myfstream.h"

using namespace std;

int main(void)
{
	myfstream f;
	char heading[] = "myfstream\n";
	//char heading2[] = "hello\n";
	char fname[80];
	char strValue[80];
	int intValue; 
	float floatValue;
	double doubleValue;

	//
	// Write 
	//	
	cout << "Enter the output filename: ";
	cin >> fname;

	cout << "Enter an integer value: ";
	cin >> intValue;

	cout << "Enter a float value: ";
	cin >> floatValue;

	cout << "Enter a double value: ";
	cin >> doubleValue;
	// 5 points
	f.Open(fname, ios::out);

	// 5 points
	f.Write(heading, strlen(heading));
	// 10 points
	// string num_str1(std::to_string(intValue));
    // char *cstr =  const_cast<char*>(num_str1.c_str());
	// f.Write(cstr, strlen(cstr));
	// f.Write("\n", 1);
	f << intValue;	
	f.Write("\n", 1);

	// 10 points
	f << floatValue;
	f.Write("\n", 1);

	// // 10 points
	f << doubleValue; 
	f.Write("\n", 1);

	// // 10 points
    f << intValue << ", "<< floatValue << ", " << doubleValue << "\n";

	f.Close();

	// //
	// //	Read
	// //
	cout << endl << "Enter the input filename: ";
	cin >> fname;

	// 5 points
	f.Open(fname, ios::in);
	
	// 5 points
	f.Read(strValue,9);
	
	strValue[9] = '\0';

	// // 10 points
	f >> intValue;
	
	// // 10 points
	 f >> floatValue;

	// // 10 points
	 f >> doubleValue;
	
	cout << strValue << endl;
	cout <<intValue<<", "<< floatValue << ", " << doubleValue << endl;

	// // 10 points
	f >>intValue>> floatValue >> doubleValue;
	
	cout  <<intValue<< ", " << floatValue << ", " << doubleValue << endl;
	 
	f.Close();
	
	return 0;
}
