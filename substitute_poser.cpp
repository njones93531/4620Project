#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <unordered_map>
#include "hash_table.cpp"

using namespace std;

unordered_map <string, int> dict;

int main(){

	//Ideally, print an error message if user fails to run program correctly 
	cout << "Usage: ./a.out < passwords.txt <optional: password>" << endl;

	//Figure out how many threads we can have at once
	//this gets the total number of processes we can have as the user
		//do we care about the total number of processes or threads?
	int max_proc;//will hold the number of processes we can have at once
	system("nproc >> process.txt");
	ifstream process_file;
	process_file.open("process.txt");
	process_file >> max_proc;
	printf("%d is the max proc\n", max_proc);
	//PASSWORD ANALYSIS MODE:
	//Store each password as a hash in order to give user password feedback (if contained)
	//If password is in dictionary, do math to tell user how long it would take 
	//If not, do math to tell user how long it would take to dial up to their password

	//PASSWORD CRACKING MODE:
	//Spawn threads 
	//load entire dictionary in an array and seperately in a hash table
	HashTable HT;
	if (HT.isEmpty()) {
		cout << "Empty Hash!" << endl;
	} else {
		cout << "Not Empty!" << endl;
	}
	string pwd;
	int num = 0;
	while (cin >> pwd) {
		HT.insertItem(num, pwd);
		num++;
    }
	// Print Entire Dictionary
	// HT.printTable();

	// THREAD_ID; //Starts at 0
	// NUM_THREADS; 
	// for(int i = THREAD_ID; i < DICT_SIZE; i+=NUM_THREADS){ //Keeps threads from trying the same words
	// 	//Try password i 
	// 	//if(cracked) stop;
	// }
	//regather all threads probably, just in case one of them succeeded 
	//If we make it this far, password was not in dict; respawn threads
	// string password = increment_password("", THREAD_ID); //Don't let threads try the same words
	// while(!cracked){
	// 	//if password not in dict, try password
	// 	increment_password(password, NUM_THREADS);
	// }

	//Hopefully when a thread is successful, we can get it to spawn a root shell or something



	return 0;
}
