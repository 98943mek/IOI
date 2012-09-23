//Fun Coloring
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 30
#define MAX_M 100
#define MAX_CASES 100

using namespace std;

int assign[MAX_N]; // Bit 0,1 for all variables
int number[MAX_CASES][MAX_M][3]; // Keep variables for a clause
int n[MAX_CASES],m[MAX_CASES];
char answer[MAX_CASES]; // Keep answer for each case

void initAssign() {
	for (int i=0;i<MAX_N;i++) {
		assign[i] = 0;
	}
}

int bin_incr(int x) { // Binary increment
    if (assign[n[x]-1] == 0) assign[n[x]-1] = 1;
    else
        for (int i=n[x]-1;i>=0;i--)
            if (assign[i] == 1) assign[i] = 0;
            else { assign[i] = 1; break; }
}

int valid_clause(int cl,int x) {
    int sum=0;
    if (number[x][cl][2] > 0) { // 3 Variables
        sum = assign[number[x][cl][0]-1]+assign[number[x][cl][1]-1]+assign[number[x][cl][2]-1];
        if (sum>0 && sum <3) return 1; // Return 1 iff a clause contains both "1" and "0"
    }else if (number[x][cl][1] > 0) { // 2 Variables
        sum = assign[number[x][cl][0]-1]+assign[number[x][cl][1]-1];
        if (sum==1) return 1; // Return 1 iff a clause contains both "1" and "0", So the sum must be exactly "1"
    }
    return 0;
}


string arrstr[3];
void split(string s) {  // Split string "s" by a space
    stringstream in (s);
    string token;
    int i=0;
    arrstr[0] = "0";arrstr[1] = "0";arrstr[2] = "0";
    while ( std::getline ( in, token, ' ' ) ) {
        arrstr[i] = token;
        i++;
    }
}

void solve(int x) { // Brute force to find an answer for
    // Brute force
    initAssign();
    for (int i=0;i<pow(2,n[x])-1;i++) {
        int logic=1;
        for (int cl=0;cl<m[x];cl++) {
            logic = (logic && valid_clause(cl,x));

        }
        if (!logic) // Not satisfied
            bin_incr(x); // Try next
        else break;
    }

    int unsolve=1;
    for (int i=0;i<n[x];i++) {
        unsolve = (unsolve && assign[i]);
    }

    if (unsolve) answer[x] = 'N';else answer[x] = 'Y'; // If a case is unsolvable the array "assign" contains all "1" (after finished brute force)
}

int main()
{
    string ncase;
    getline(cin, ncase);
    int num_case = atoi(ncase.c_str());

    // Input each case
    for (int x=0;x<num_case;x++) {
        string inbuff;
        getline(cin, inbuff);split(inbuff); // Input data as a line and split data by space
        n[x] = atoi(arrstr[0].c_str()); // String to int casting
        m[x] = atoi(arrstr[1].c_str()); // String to int casting

        // Input cluases data
        for (int i=0;i<m[x];i++) {
            getline (cin, inbuff);split(inbuff);
            number[x][i][0] = atoi(arrstr[0].c_str());
            number[x][i][1] = atoi(arrstr[1].c_str());
            number[x][i][2] = atoi(arrstr[2].c_str());
        }
        if (x!=num_case-1) getline(cin, inbuff); // Blank line (dummy input for separating each case)
    }
    // Solve each test case and keep the answer in answer[x]
    for (int x=0;x<num_case;x++) {
        solve(x);
    }

    // Display answer
    for (int x=0;x<num_case;x++) {
        cout<<answer[x];
    }
    return 0;
}

