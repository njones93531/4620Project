#include <iostream>
#include <list>
#include <cstring>
using namespace std;

class HashTable {
    private:
        static const int hashGroups = 10;
        // List 1, index 0, list 2, index 1, ...
        list<pair<int, string> > table[hashGroups];

    public:
        bool isEmpty() const;
        int hashFunction(int key);
        void insertItem(int key, string value);
        void removeItem(int key);
        string searchTable(int key);
        void printTable();
};

// Check for empty
bool HashTable::isEmpty() const {
    int sum{};
    for (int i{}; i < hashGroups; i++) {
        sum += table[i].size();
    }
    if(!sum) {
        return true;
    }
    return false;
}

// hash function
int HashTable::hashFunction(int key) {
    // if key is 905, this return 5
    return key % hashGroups;
}

// inserting item 
void HashTable::insertItem(int key, string value) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;

    for (; bItr != end(cell); bItr++) {
        if (bItr->first == key) {
            keyExists = true;
            bItr->second = value;
            break;
        }
    }
    if (!keyExists) {
        cell.emplace_back(key, value);
    }
    return;
}

// removing item 
void HashTable::removeItem(int key) {
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExists = false;

    for (; bItr != end(cell); bItr++) {
        if (bItr->first == key) {
            keyExists = true;
            bItr = cell.erase(bItr);
            break;
        }
    }
    if (!keyExists) {
        // Not Found;
    }
    return;
}

// printing hash table
void HashTable::printTable() {
    for (int i{}; i < hashGroups; i++) {
        if (table[i].size() == 0) continue;

        auto bItr = table[i].begin();
        for(; bItr != table[i].end(); bItr++) {
            cout << "Key: " << bItr->first << " Value: " << bItr->second << endl;
        }
    }
    return;
}