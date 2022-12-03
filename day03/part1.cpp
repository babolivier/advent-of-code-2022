#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // Open input and make sure that succeeded.
    fstream input("input", ios::in);
    if(!input.is_open()) {
        cerr << "Failed to open file :(";
        return 1;
    }

    // The current line's content.
    string line;
    // The line number. We're keeping track of this in order to log it if something goes wrong.
    int line_n = 0;
    // The total priority of all duplicate items.
    int total_priority = 0;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        // Split the line in two.
        int middle = line.length() / 2;
        string compartment1(line.begin(), line.begin() + middle);
        string compartment2(line.begin() + middle, line.end());

        // Find the common character in each compartment.
        char duplicate = -1;
        for(auto c : compartment1) {
            size_t found = compartment2.find(c);
            if(found != string::npos) {
                duplicate = c;
            }
        }

        if(duplicate == -1) {
            // If there's no duplicate item, there's an issue with the code or the input, so going any further will
            // only produce a wrong result.
            cerr << "No duplicate item at line " << line_n << '\n';
            return 1;
        }

        // Figure out the priority of the duplicate item.
        int priority;
        if('a' <= duplicate && duplicate <= 'z') {
            priority = duplicate - 'a' + 1;
        } else if('A' <= duplicate && duplicate <= 'Z') {
            priority = duplicate - 'A' + 27;
        } else {
            // If the duplicate item isn't part of the character set provided by the instructions, then there is
            // something wrong and continuing further will only lead to wrong results.
            cerr << "Duplicate item is not a letter at line " << line_n << '\n';
            return 1;
        }

        // Calculate the new total priority.
        total_priority += priority;
    }

    // Print the result.
    cout << "Total priority sum: " << total_priority << '\n';
    return 0;
}