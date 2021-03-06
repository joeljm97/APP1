#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <direct.h>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <ctime>
#include <iomanip>
#include "logmanager.h"

using namespace std;

class testApp1
{
private:
    typedef vector<string> stringvec;
    typedef unordered_set<string> stringset;
    stringset::iterator itr;

    

    char* getCurDir() //returns the current working directory
    {
        writetolog(__LINE__,"Inside :"+ funcName(__FUNCTION__));

        char* currentDir;
        if ((currentDir = _getcwd(NULL, 0)) == (NULL)) {
            perror("_getcwd error");
            writetolog(__LINE__, "_getcwd error"+funcName(__FUNCTION__));
            return(NULL);
        }
        else
        {
            string templog;
            templog.assign(currentDir);
            writetolog(__LINE__, "returning value:"+templog+" From "+__FUNCTION__);
            return(currentDir);
        }
    }

    void read_directory(const string& name, stringset& s) //gets directory contents one by one and adds it to a SET of strings
    {
        writetolog(__LINE__, "Entered:" + funcName(__FUNCTION__));
        string pattern(name);
        pattern.append("\\*");
        WIN32_FIND_DATA data;
        HANDLE hFind;
        if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) 
        {
            do {
                s.insert(data.cFileName);
            } while (FindNextFile(hFind, &data) != 0);
            FindClose(hFind);
        }
    }

    void copyFile(string sourcefile, string destinationfile,string f) //Copies contents of source file to destination file
    {
        writetolog(__LINE__, "Entered :" + funcName(__FUNCTION__));
        ifstream fs;
        ofstream ft;
        string str;

        fs.open(sourcefile);

        if (!fs)
        {
            writetolog(__LINE__, "Cannot Open File:Root\\" + f);
            cout << "Error in Opening File : " << sourcefile;
        }
        else
        {
            writetolog(__LINE__, "Successfully Opened File:Root\\" + f);
        }
        ft.open(destinationfile);

        if (!ft)
        {
            writetolog(__LINE__, "Cannot Open file :" + f + " :in Current Directory");
            cout << "Error in Opening Destination File" << destinationfile << endl;
            fs.close();

        }
        else
        {
            writetolog(__LINE__, "Successfully opened file :" + f + " :in Current Directory");
        }

        if (fs && ft)
        {
            while (getline(fs, str))
            {
                ft << str << "\n";
            }
            writetolog(__LINE__, "Copied File:" + f + " :in Current Directory");

        }
        else
        {
            writetolog(__LINE__, "Cannot open file");
            cout << "File Cannot Open...!!!";
        }

        fs.close(); writetolog(__LINE__, "Closed File:Root\\"+f);
        ft.close(); writetolog(__LINE__, "Closed File:" + f +" :in Current Directory");
    }

    void displaySet(stringset s) //prints the contents of a String Set to console window
    {
        writetolog(__LINE__, "Inside :" + funcName(__FUNCTION__));
        cout << "\n\n";
        for (const string& i : s)
        {
            std::size_t index = i.find(".");
            if (index < i.length() && i != "." && i != "..")
                cout << endl << i;
        }
        cout << "\n\n";
        writetolog(__LINE__, "Exited :" + funcName(__FUNCTION__));
    }

    string::size_type indexOf(string s, string s1) //get the index position of a character or substring in a String
    {
        writetolog(__LINE__, "Entered :" + funcName(__FUNCTION__));
        string::size_type loc = s.find(s1);
        if ((int)loc < s.length())
        {
            cout << loc;
            writetolog(__LINE__, "Returned Value: 0 from" + funcName(__FUNCTION__));
            return loc;
        }
        else
        {
            writetolog(__LINE__, "Returned Value: 0 from" + funcName(__FUNCTION__));
            return 0;
        }
    }


    public:void assignment() 
    {
        writetolog(__LINE__, "Entered :" + funcName(__FUNCTION__));
    
        string rootDir, filepath, curfilepath;
        char* currentDir = getCurDir();
        bool ipcheck = false;
        stringset currentFiles;
        stringvec missingFiles, line;
        stringset::iterator itr;
        stringvec::iterator ptr;

        stringvec filesNeeded{ "ipt_config.xml","ipt_config_DMI1.xml","ipt_config_DMI2.xml",
            "OteCfgPc_Dmi_Iptcom_forRealDMI.cfg","tdchosts.txt" }; // a string vector containing filenames of the files needed


        read_directory(currentDir, currentFiles);
        cout << "\nFiles in current directory:";
        for (itr = currentFiles.begin(); itr != currentFiles.end(); itr++)
        {
            string str = *itr;
            std::size_t index = str.find(".");
            if (index > str.length())
                currentFiles.erase(itr--);
        }

        writetolog(__LINE__, "called function:displaySet()");
        displaySet(currentFiles);
        writetolog(__LINE__, "Displayed list Current Files to console");

        int count = 0;

        for (ptr = filesNeeded.begin(); ptr < filesNeeded.end(); ptr++) //checks for missing files and adds it to string Vector : missingfiles
        {
            if (!currentFiles.count(*ptr))
            {
                missingFiles.push_back(*ptr);
                count++;
            }
        }

        if (count > 0) //if there are any missing files
        {
            cout << "\nThe missing files are : ";
            for (ptr = missingFiles.begin(); ptr < missingFiles.end(); ptr++) //list the missing files
                cout << endl << *ptr;
            cout << "\n\nEnter Root Directory location:";

            getline(cin, rootDir); //get root directory from user

            int i = 0;
            for (ptr = missingFiles.begin(); ptr < missingFiles.end(); ptr++) //Append missing filenames to Root Directory and Current Directory and copy file to Current directory
            {
                filepath = rootDir + "\\" + *ptr;
                string curTemp;
                curTemp.assign(currentDir);
                curfilepath = curTemp.append("\\") + *ptr;

                copyFile(filepath, curfilepath,*ptr);
            }
            cout << "\nNecessary Files have been Copied";
        }
        else //if all files are present
        {
            cout << "\n**************************\n";
            cout << "All files are present";
            cout << "\n**************************\n";
        }

        cout << endl;
        read_directory(currentDir, currentFiles);
        cout << "\nFiles in current Directory:";

        writetolog(__LINE__, "called function:displaySet()" );
        displaySet(currentFiles);
        writetolog(__LINE__, "Displayed list Current Files to console");
    
        ifstream fs;
        ofstream ft;
        string str;
        string checkfile,checkfile1;
        checkfile.assign(currentDir);
        checkfile = checkfile + "\\tdchosts.txt"; //location of tdchosts.txt
        fs.open(checkfile);

        if (!fs)
        {
            cout << "Error in Opening File : " << checkfile1;
        }
        else
        {
            cout << "========================================";
            cout << "\nContents of file : tdchosts.txt \n\n";
            while(getline(fs, str)) // printing contents of tdchosts and at the same time checking if the ip address for DMI2.lCst is 192.168.1.16
            {
                string str_temp, key = "", value = "";
                int pos = str.find(" ");
                value = str.substr(0, pos);
                key = str.substr(++pos);
                if (key == "DMI2.lCst")
                {
                
                    if (value != "192.168.1.16") //checking and correcting ip values for ecah entry and stores correct values in a String Vector: line
                    {
                        writetolog(__LINE__, "IP for DMI2.lCst is : "+value);
                        ipcheck = true;
                        line.push_back("192.168.1.16");
                        line.push_back(key);
                    }   
                    else
                    {
                        line.push_back(value);
                        line.push_back(key);
                    }
                }
                else
                {
                    line.push_back(value);
                    line.push_back(key);
                }
                cout << str <<endl;
            }
        
        }
        fs.close();

        ft.open(checkfile,ios::trunc); //opening tdchost.txt and after clearing all contents
        int i = 1;

        for (ptr = line.begin(); ptr < line.end(); ptr++) //copy contents of string Vector : line to file :tdchost.txt
        {
            if (i % 2 != 0) 
            {
                ft << *ptr;
            }
            else 
            {
                ft << " " << *ptr << "\n";
            }
            i++;
        }
        ft.close();

        fs.open(checkfile);
        if (ipcheck == true) //if content of tdchost was changed show appropriate message and display its new content
        {
            writetolog(__LINE__, "IP for DMI2.lCst is incorrect.IP for DMI2.lCst has been changed");
            cout << "\nIP for DMI2.lCst is incorrect.\nIP for DMI2.lCst has been changed\n\n";
            while (getline(fs, str))
            {
                cout << str << "\n";
            }
            cout << endl;
        }
        else //if content of tdchost was not changed show appropriate message .
        {
            writetolog(__LINE__, "IP for DMI2.lCst is correct .No change required");
            cout << "\nIP for DMI2.lCst is correct . \nNo change required\n\n";
        }
        fs.close();
        writetolog(__LINE__, "Exited :" + funcName(__FUNCTION__));
    }
};

int main() // main driver function
{
    initializeLog();
    testApp1 A1;
    A1.assignment();
    endlog();
}