#include <iostream>
#include <string>

using namespace std;

string toStr(int n, int base) {
    string convertString = "abcdefghijklmnopqrstuvwxyz";
    if (n < base) {
        return string(1, convertString[n]);
    } else {
        cout << "Mod" << n % base << endl;
        string x  = "0";
        return toStr(n/base, base) + convertString[n%base];
    }
}

int main() {
    string pwd0, pwd1, pwd2, pwd3, pwd4, pwd5, pwd;
    for(long long i = 0; i < 26; i++){ //If we try long enough, give up
        pwd0=toStr(i, 26);
        for(long long j = 0; j < 26; j++){
            pwd1=toStr(j, 26);
            for(long long k = 0; k < 26; k++){
                pwd2=toStr(k, 26);
                for(long long l = 0; l < 26; l++){
                    pwd3=toStr(l, 26);
                    for(long long m = 0; m < 26; m++){
                        pwd4=toStr(m, 26);
                        for(long long n = 0; n < 26; n++){
                            pwd5=toStr(n, 26);
                        }
                    }
                }
            }
        }
        pwd = pwd0 + pwd1 + pwd2 + pwd3 + pwd4 + pwd5;
    }
    cout << pwd << endl;
}
