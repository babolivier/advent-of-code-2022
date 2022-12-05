#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // Number of members in a group.
    static constexpr size_t NB_MEMBERS_IN_GROUP = 3;
    // The amount of members we need to keep in a buffer (which in this case is an array of the given size)
    // in order to be able to process the whole group when we reach its last member. This is 1 less than
    // the number of members in the group, since we look at the array when we process the last member.
    static constexpr size_t NB_MEMBERS_IN_BUFFER = NB_MEMBERS_IN_GROUP - 1;

    // Open input and make sure that succeeded.
    fstream input("input", ios::in);
    if(!input.is_open()) {
        cerr << "Failed to open file :(";
        return 1;
    }

    // The current line's content.
    string line;
    // The line number. We're keeping track of this in order to log it if something goes wrong and figuring out
    // how many members of a group we've already processed.
    int line_n = 0;
    // The total priority of all common items.
    int total_priority = 0;
    // The lines for the first 2 members of the group.
    string items_in_group[NB_MEMBERS_IN_BUFFER];
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        if(line_n % NB_MEMBERS_IN_GROUP != 0) {
            // If the line number isn't a multiple of 3, we store the line and continue. We need to substract
            // 1 to get the index because we count line numbers starting at 1.
            items_in_group[line_n % NB_MEMBERS_IN_GROUP - 1] = line;
        } else {
            // We're processing the final member of the group.
            optional<char> maybe_common;
            // In the current line, try to find a character that's common with the last two lines we've stored.
            for(auto c : line) {
                // For each character, test if it can be found in the lines we've stored.
                // Setting the boolean to true before we start iterating is slightly risky here because it can
                // lead to false results if we've missed something in the logic, but given the simplicity of the
                // logic and the context of the exercise it should be fine in this instance.
                bool found = true;
                for(size_t i = 0; i < NB_MEMBERS_IN_BUFFER; i++) {
                    if(items_in_group[i].find(c) == string::npos) {
                        found = false;
                        break;
                    }
                }

                // If no match was found, continue to the next character, otherwise break out of the loop.
                if(found) {
                    maybe_common.emplace(c);
                    break;
                }
            }

            if(!maybe_common) {
                // If there's no common item, there's an issue with the code or the input, so going any further will
                // only produce a wrong result.
                cerr << "No common item at line " << line_n << '\n';
                return 1;
            }

            // Now we know for sure there's a common item.
            char common_item = maybe_common.value();
            // Figure out the priority of the common item.
            int priority;
            if('a' <= common_item && common_item <= 'z') {
                priority = common_item - 'a' + 1;
            } else if('A' <= common_item && common_item <= 'Z') {
                priority = common_item - 'A' + 27;
            } else {
                // If the common item isn't part of the character set provided by the instructions, then there is
                // something wrong and continuing further will only lead to wrong results.
                cerr << "Common item is not a letter at line " << line_n << '\n';
                return 1;
            }

            // Calculate the new total priority.
            total_priority += priority;
        }
    }

    // Print the result.
    cout << "Total priority sum: " << total_priority << '\n';
    return 0;
}