#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int MAX_PROC = 0; //will hold the number of processes we can have at once
string USERNAME = "Bob"; //"" for root

unordered_map <string, int> HT;
vector <string> pwds;

int main(){
	pwds.push_back("1234");

	//Ideally, print an error message if user fails to run program correctly 
	cout << "Usage: ./a.out < passwords.txt <optional: password>" << endl;

	//Figure out how many procs we can have at once
	//this gets the total number of processes we can have as the user
	//do we care about the total number of processes or procs?
	system("nproc >> process.txt");
	ifstream process_file;
	process_file.open("process.txt");
	process_file >> MAX_PROC;
	printf("%d is the max proc\n", MAX_PROC);


	//PASSWORD ANALYSIS MODE:
	//Store each password as a hash in order to give user password feedback (if contained)
	//If password is in dictionary, do math to tell user how long it would take 
	//If not, do math to tell user how long it would take to dial up to their password

	//PASSWORD CRACKING MODE:
	//Spawn procs 
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
	if (HT.empty()) {
		cout << "Empty Hash!" << endl;
	} else {
		cout << "Not Empty!" << endl;
	}
	string pwd;
	int num = 1;
	while (cin >> pwd) {
		HT[pwd] = num;        //use HT[pwd] to get index
		pwds.push_back(pwd);  //use pwds[index] to get pwd
		num++;
    }
	// Print Entire Dictionary
	// HT.printTable();

	system("install -m 666 /dev/null su.txt"); //writeable file

	// PROC_ID; //Starts at 0
	// NUM_PROCS; 
	for(int i = PROC_ID; i < DICT_SIZE; i+=NUM_PROCS){ //Keeps procs from trying the same words
		const string cmd = "bash suprobe.sh " + pwds[0] + USERNAME + '&';
		system(cmd.c_str());
	 	//if(cracked) stop;
	}
	//regather all procs probably, just in case one of them succeeded 
	//If we make it this far, password was not in dict; respawn procs
	// string password = increment_password("", PROC_ID); //Don't let procs try the same words
	// while(!cracked){
	// 	//if password not in dict, try password
	// 	increment_password(password, NUM_PROCS);
	// }

	//When a proc is successful, it will print the correct password into passwords.txt



	return 0;
}
