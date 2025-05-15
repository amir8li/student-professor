#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(){
    fstream file;
    file.open("text.txt", ios::in | ios::out | ios::app);
    string line;
    while(getline(file, line)){
        cout << line << endl;
    }
    for(int i = 0; i < 5; i++)
        cout << i << endl;
    // while(true){
    //     getline(cin, line);
    //     if(line == "stop")
    //         break;
    //     file << line << endl;
    // }
    file.close();
    return 0;
}