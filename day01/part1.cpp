#include <iostream>
#include <fstream>
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
    // The amount of calories held by the Elf we're currently looking at.
    int current_elf_cals = 0;
    // The amount of calories held by the Elf with the most of them.
    int highest_cals_amount = 0;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        if(line.length()) {
            // If the line isn't empty, it means we're still processing the same Elf. Try to parse the line as an
            // integer, and add it to the amount for the current Elf.
            try {
                int line_i = stoi(line);
                current_elf_cals += line_i;
            } catch(const invalid_argument&) {
                // In our context, this is a fatal error - there's nothing much to do apart exiting the program.
                cerr << "Failed to read number at line " << line_n << "\n";
                return 1;
            }
        } else {
            // If we've reached an empty line, we check if this Elf has the highest amount of calories recorded,
            // and if so record them as such.
            if(current_elf_cals > highest_cals_amount) {
                highest_cals_amount = current_elf_cals;
            }
            // Reset the current count so we can move on to the next Elf.
            current_elf_cals = 0;
        }
    }

    // Print out the solution.
    cout << "The Elf with the highest number of calories is carrying " << highest_cals_amount << " calories. \n";

    return 0;
}