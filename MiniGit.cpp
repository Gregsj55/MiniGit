#include "MiniGit.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

MiniGit::MiniGit() {
    //constructor
    //ask if we have permision to initialize before executing
    //create first node of DLL
    //system("mkdir .minigit");
    fs::create_directory(".minigit");

    //create first node
    head = new dNode();
    head -> commitNum = 0;
    head -> next = NULL;
    head -> prev = NULL;
    head -> head = NULL;
    tail = head;
}

MiniGit::~MiniGit() {
    //destructor
    //clear files and delete folder
    fs::remove_all(".minigit");
}

bool MiniGit::add(string fName) {
    //add a file to the singly linked list that is a part of the current node
    ifstream file;
    file.open(fName);
    if(!file.is_open()){
        //No File of this name yet
        cout << "Dummy put in a real name" << endl;
        return false;
    }else{
        // File does exist
        if (tail -> head == NULL) {
            //Makes a head for the linked list
            tail -> head = new sNode;
            tail -> head -> fileName = fName;
            tail -> head -> fileVersion = "";
            tail -> head -> next = NULL;
        } else {
            sNode* temp = tail -> head;
            if (tail -> head -> fileName == fName) {
                //Can't add the same file to the list
                cout << "File already exists, not added" << endl;
                return true;
            }
            while (temp -> next != NULL) {
                // Loops through the linked list, if it exists, return, goes until you find the end.
                if (temp -> fileName == fName) {
                    cout << "File already exists, not added" << endl;
                    return true;
                }
                temp = temp -> next;
            }
            //Temp is the end of the linked list. Temp->next is where the new file gets put.
            temp -> next = new sNode;
            temp -> next -> fileName = fName;
            temp -> next -> fileVersion = "";
            temp -> next -> next = NULL;
        }
        file.close();
    }
    cout << "Added!" << endl;
    sNode* temp = tail -> head;
    while (temp -> next != NULL) { // This loop returns the file name of the node just added.
        temp = temp -> next;
    }
    cout << temp -> fileName << endl; 
    return true;
}
bool MiniGit::del(string fName) {
    //create a SLL node for the head and a previous node
    sNode* temp = tail -> head;
    sNode* prev;
    while (temp != NULL) {
        //if the file of the same name is found
        if (temp -> fileName == fName) {
            //remove file
            cout << "removing: " << fName << endl;
            if (temp == tail -> head) { //case for if file is  the had of the SLL
                sNode* temp2 = tail -> head -> next;
                delete temp;
                tail -> head = temp2;
            } else {
                //delete node as one normally would from a SLL
                prev -> next = temp -> next;
                delete temp;
            }
            //return true if it was removed
            return true;
        }
        //incriment
        prev = temp;
        temp = temp -> next;
    }
    return false;
}
bool MiniGit::fileEquality(string normFile, string minigitfile){
    // returns false if the files don't exist
    //open files
    ifstream file1;
    ifstream file2;
    file1.open(normFile);
    file2.open(".minigit/"+minigitfile);
    
    //if they didnt open return false as one doesnt exist
    if(!(file1.is_open() && file2.is_open()))return false;

    //copy both files to a string
    string temp1 = "";
    string temp2 = "";
    string crawler;
    while(getline(file1,crawler)){
        temp1+=crawler;
    }while(getline(file2,crawler)){
        temp2+=crawler;
    }
    file1.close();
    file2.close();
    //if strings are equal return true else  return false
    return temp1 == temp2;
}

string addNums(string t) {
    //remove the .txt or cpp or hpp
    string filetype = t.substr(t.length()-4);
    t = t.substr(0,t.length()-4);

    //get the numbers
    int x = stoi(t.substr(t.length()-2,2));

    //add to the numbers for the file tobe saved in .minigit
    t= t.substr(0,t.length()-2);
    if(x < 9){
        t+="0" + to_string(x+1);
    }else if(x < 99){
        t+="" + to_string(x+1);
    }else {
        //if theres more than 99 commits for one file
        cout << "too big forehead" << endl;
        return "";
    }

    //return updated file  name
    return t+filetype;
}

bool MiniGit::commit() {
    //read through SLL and copy files that have been changed
    //then create new DLL node and incriment commit number
    
    //check if fileversion file exists
    sNode* temp = tail -> head;//Crawler for the while loop.
    cout << tail -> head -> fileName << endl; //Prints the name of the current commit.
    while (temp != NULL) {
        // This loop checks file versions to copy.
        if (temp -> fileVersion != "") {
            // If the current file is on it's original version
            cout << "filename not empty" << endl;
            string filename = ".minigit/"+temp -> fileVersion; // ".minigit/ puts the filename in the right directory."
            bool tf = fileEquality(temp -> fileName, temp -> fileVersion);// if the files exist in both and are the same runs true
            if (!tf) {
                //if the files aren't the same add
                temp->fileVersion = addNums(temp->fileVersion); //makes the file version the right number.
                filename = ".minigit/"+temp -> fileVersion;
                ofstream copy; // output to file
                copy.open(filename);
                ifstream original;
                original.open(temp->fileName);// input from the original
                string line;
                while (getline(original, line))
                {
                    copy << line << endl;// Writes line for line from the original to the copy with correct version #
                }
                copy.close();
                original.close();
                cout << "commited: " << temp->fileName << " as: " << temp->fileVersion << endl;
           } //dont do anything otherwise
        } else {
            // Sets the name of the new file
            temp -> fileVersion = (temp -> fileName).substr(0,(temp -> fileName).length()-4) + "00" + (temp -> fileName).substr((temp -> fileName).length() -4 , (temp -> fileName).length()-1);
            string filename = ".minigit/"+temp -> fileVersion;
            ofstream newFile(filename);
            ifstream oldFile(temp -> fileName);
            string crunch;
            while (getline(oldFile,crunch)){
                newFile << crunch << endl; // writes from the old file to the new file.
            }
            cout << "commited: " << temp -> fileName << " as: " << temp -> fileVersion << endl;
            newFile.close();
            oldFile.close();
        }
        temp = temp ->next; 
    }

    //deep copy to next dll node
    dNode* prevNode = tail; // keep a copy of the previous node before adding a new tail
    if(tail == head) {
        //case for if this is the second commit
        tail = new dNode;
        tail -> prev = head;
        head -> next = tail;
        tail ->  commitNum = head -> commitNum +1;
        tail -> head = NULL;
    } else {
        //case for if it is not the second commit
        tail = new dNode;
        tail -> prev = prevNode;
        prevNode -> next = tail;
        tail -> commitNum = prevNode -> commitNum +1;
        tail -> head = NULL;
    }

    //copy linked list over
    //create 2 SLL nodes, 1 to track the previous commit's SLL, and one so that the new LL has next's
    sNode* temp2 = prevNode -> head;
    sNode* prevSNode;
    sNode* newLL =  tail -> head;
    while (temp2 != NULL) {
        //while the previous commits LL still has nodes
        if (tail -> head == NULL) {
            //if this is the first node in the new commit
            tail -> head = new sNode;
            tail -> head -> fileName = temp2 -> fileName;
            tail -> head -> fileVersion = temp2 -> fileVersion;
            prevSNode = tail -> head;
        } else {
            //if this is not the first node
            newLL = new sNode;
            newLL -> fileName = temp2 -> fileName;
            newLL -> fileVersion = temp2 -> fileVersion;
            prevSNode -> next = newLL;
            prevSNode = newLL;
        }
        //incriment to move through list
        temp2 = temp2 -> next; 
    }
    return true;
}

//check if a string is a number
bool isNumber2(string num){return (num.length()>0)?(((int)num[0] >= '0')&&((int)num[0] <='9') && isNumber2(num.substr(1))):true;}

bool MiniGit::checkout() {
    //first confirm with user
    cout << "Are you sure you want to checkout? This will lose all non-commited changes" << endl;
    cout << "Type y to continue or any other key to exit" << endl;
    string entry;
    getline(cin,entry);
    if (entry == "y") {
        //then lose the current progress since last commit and go back to a previous version
        cout << "What version do you want to go to (enter an integer): " << endl;
        getline(cin,entry);
        if(isNumber2(entry)) {
            //if the user entered a valid number try to find the DLL node with that commit number
            int num = stoi(entry);
            if (num <= tail -> commitNum) {
                dNode*  temp = head;
                while (temp != NULL) {
                    if (temp -> commitNum == num) { // if DLL node is found
                        sNode* sTemp = temp ->  head;
                        //go through SLL in that node
                        while (sTemp != NULL) {
                            //copy from .minigit file to base directory file
                            ofstream wFile(sTemp -> fileName);
                            ifstream rFile(".minigit/" + sTemp -> fileVersion);
                            string line;
                            while (getline(rFile,line)) {
                                wFile << line << endl;
                            }
                            wFile.close();
                            rFile.close();
                            //go to next node of SLL
                            sTemp = sTemp -> next;
                        }
                        //this is the only case where everything worked perfectly
                        return true;
                    }
                    temp = temp ->next;
                }
            } else {
                //number was higher than the number of commits
                cout << "Too high of a number, nothing has been changed." << endl;
                return false;;
            }
        } else {
            //integer not entered
            cout << "Please enter an integer, nothing has been changed." <<  endl;
            return false;
        }
    } else {
        //user didnt enter y
        cout << "Exiting, nothing has been changed." << endl;
        return false;
    }
    return false;
}