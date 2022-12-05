#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

// Parse the given line into the quantity, start and end io arguments.
// Returns a boolean indicating whether the line follows the correct format.
bool parse_instruction_line(string line, int* quantity, int* move_start, int* move_end) {
    // The format to follow in an instruction line. X, Y and Z are arbitrary placeholders for
    // the quantity to move, the stack to move from and the stack to move to, respectively.
    // If we had a more complex format to deal with, we could have used e.g. an enum or a map
    // to avoid repeating them (in the format definition and in the validation) like we do here.
    static const string FORMAT = "move X from Y to Z";

    // Define two stringstreams, which we'll read at the same time: one for the current line,
    // and one for the format it should follow.
    stringstream sline(line);
    string word_in_line;
    stringstream sformat(FORMAT);
    string word_in_format;

    // Parse the line word by word, validating its format at the same time.
    while(sline >> word_in_line) {
        sformat >> word_in_format;

        bool format_ok = false;

        // Check if the current word in the format is either the current word in the line,
        // or is one of the placeholders.
        if(word_in_format == word_in_line) {
            format_ok = true;
        } else if(word_in_format == "X") {
            try {
                *quantity = stoi(word_in_line);
                format_ok = true;
            } catch(const invalid_argument&) {}
        } else if(word_in_format == "Y") {
            try {
                *move_start = stoi(word_in_line);
                format_ok = true;
            } catch(const invalid_argument&) {}
        } else if(word_in_format == "Z") {
            try {
                *move_end = stoi(word_in_line);
                format_ok = true;
            } catch(const invalid_argument&) {}
        }

        if(!format_ok) {
            return false;
        }
    }

    return true;
}


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
    // A temporary nested vector into which to store the content of each stack while we don't
    // know their names.
    vector<vector<string>> crates{};
    // The final map of stack name to content.
    map<int, vector<string>> crates_per_stack{};
    // Whether we've finished reading the initial state of the crates and are ready to start
    // moving them around.
    bool read_crates = false;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        // If the line is empty, then it's the delimiter between the initial state of the columns
        // the move instructions. In this case, we record that we've finished reading said initial
        // state, and we skip to the next iteration.
        if(line.length() == 0) {
            read_crates = true;
            continue;
        }

        // TODO: Refactor into a function that returns a value for read_crates.
        if(!read_crates) {
            // Initialise the vector if its size isn't the expected one (which is one element for each 4-char
            // section representing a stack, and a final one for the last stack in the line since it doesn't
            // end in an extra space).
            while(crates.size() < line.length() / 4 + 1) {
                vector<string> col_v{};
                crates.push_back(col_v);
            }

            // We iterate over chunks of 4 characters: 3 characters for the crates, plus a space delimiter.
            for(size_t i = 0; i < line.length(); i += 4) {
                // Read the section of the line with the crates, omitting the space delimiter.
                string section(line.begin() + i, line.begin() + i + 3);

                int stack_nb = i / 4;

                if(section[0] == '[') {
                    crates[stack_nb].push_back(section);
                } else if('0' <= section[1] && section[1] <= '9') {
                    auto crates_in_stack = crates[stack_nb];
                    // Reverse the vector, so we can more easily push and pop crates in and out of it.
                    reverse(crates_in_stack.begin(), crates_in_stack.end());
                    crates_per_stack[section[1] - '0'] = crates_in_stack;
                }
            }
        } else {
            int quantity, start, end;

            // Parse the instruction in the line, and validate that its format is correct.
            if(!parse_instruction_line(line, &quantity, &start, &end)) {
                cerr << "Bad format for line " << line_n << endl;
                return 1;
            }

            // Move the crates.
            for(size_t i = 0; i < quantity; i++) {
                string el = crates_per_stack[start].back();
                crates_per_stack[start].pop_back();
                crates_per_stack[end].push_back(el);
            }
        }
    }

    // Print the final result.
    cout << "Final result: ";
    for(auto [key, value] : crates_per_stack) {
        cout << value.back()[1];
    }
    cout << endl;
}