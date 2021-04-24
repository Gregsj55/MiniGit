#pragma once

#include <iostream>
#include <vector>

using namespace std;

//singly linked list node
struct sNode{
    string fileName; //name of local file
    string fileVersion; //name of file in .minigit folder
    sNode* next;
};

//doubly linked list node
struct dNode{
    int commitNum;
    sNode* head;
    dNode* prev;
    dNode* next;
};

//class
class MiniGit {
    public:
        //constructor / destructor
        MiniGit();
        ~MiniGit();

        //functionality of program
        bool add(string fName);
        bool del(string fName);
        bool commit();
        bool checkout();
        bool fileEquality(string a, string b);

    private:
        //variables for head and tail of DLL
        dNode* head;
        dNode* tail;

};