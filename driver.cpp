#include "MiniGit.hpp"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <filesystem>

//menu function
void menu(){cout<<"\n1.Add files to the current commit\n2.Remove files from the current commit\n3.Commit the changes\n4.Check out a different version in a specific commit\n5.Quit out and go explore the outdoors" << endl;}
//function for checking if a string is a  number
bool isNumber(string num){return (num.length()>0)?(((int)num[0] >= '0')&&((int)num[0] <='5') && isNumber(num.substr(1))):true;}

int main() {

    //ask user if they want to create a repository
    cout << "Would you like to create a new repository: (enter y or n) " << endl;
    string entry;
    getline(cin, entry);

    //if the user does not enter y or n ask again
    while (entry != "n") {
        if (entry == "y") {

            //create the directory and start git going
            MiniGit Git;
            string cc;
            bool end = true;
            cout << "\nOkay bud you can do 5 things" << endl;
            //loop through menu while user is executing git commands
            while(end){
                menu();
                
                //convert inputs to an int if they are formated correctly
                getline(cin,cc);
                while(!isNumber(cc) || cc.length()>1){
                    cout << "try again fivehead" << endl;
                    getline(cin,cc);
                }
                int c = stoi(cc);
                
                //switch statement for the rubric and i guess users entries
                switch (c){
                    case 1:
                    {
                        //If you want to add files
                        std::string fName;
                        cout << "Enter the file name:" << endl;
                        getline(cin, fName);
                        //loop until the user entyers a valid name
                        while (!Git.add(fName)) {
                            getline(cin, fName);
                        }
                        cout <<"\n";
                        break;
                    }
                    case 2:
                    {
                        //If you want to delete files
                        std::string fName;
                        cout << "Enter the file name:" << endl;
                        getline(cin, fName);
                        //try to delete file of filename
                        if (!Git.del(fName)) {
                            //if  deleteion fails release the user
                            cout << "Deletion failed - there is no file of such name in the current commit" << endl;
                        }
                        cout <<"\n";
                        break;
                    }
                    case 3:
                        //if the user wants to commit - all functionality inside commit function
                        Git.commit();
                        break;
                    case 4:
                        //if the user wants to checkout
                        if(Git.checkout()) {
                            //if checkout was sucessful dont let the user do anything else with the program
                            cout << "Branching is not allowed, exiting program once you press any key." << endl;
                            getline(cin, entry);
                            cout << "\ngg's\n" << endl;
                            return 0;
                        }
                        break;
                    case 5:
                    //exit function
                        cout << "\ngg's\n" << endl;
                        end = false;
                        //quit and go
                        break;
                    default:
                        break;
                }
                //styatement before the menu
                if(c != 5)cout << "What do you want to do now?" << endl;
            }
            break;
        } else {
            //if user didnt enter y or n
            cout << "Invalid input, try again: " << endl;
            getline(cin, entry);
        }
    }
}