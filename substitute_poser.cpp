#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

using namespace std;

unordered_map <string, int> dict;

int main(){

	//Ideally, print an error message if user fails to run program correctly 
	cout << "Usage: ./a.out < passwords.txt <optional: password>";

	//Figure out how many threads we can have at once

	//PASSWORD ANALYSIS MODE:
	//Store each password as a hash in order to give user password feedback (if contained)
	//If password is in dictionary, do math to tell user how long it would take 
	//If not, do math to tell user how long it would take to dial up to their password

	//PASSWORD CRACKING MODE:
	//Spawn threads 
	//load entire dictionary in an array and seperately in a hash table
	THREAD_ID; //Starts at 0
	NUM_THREADS; 
	for(int i = THREAD_ID; i < DICT_SIZE; i+=NUM_THREADS){ //Keeps threads from trying the same words
		//Try password i 
		//if(cracked) stop;
	}
	//regather all threads probably, just in case one of them succeeded 
	//If we make it this far, password was not in dict; respawn threads
	string password = increment_password("", THREAD_ID); //Don't let threads try the same words
	while(!cracked){
		//if password not in dict, try password
		increment_password(password, NUM_THREADS);
	}

	//Hopefully when a thread is successful, we can get it to spawn a root shell or something



	return 0;
}
