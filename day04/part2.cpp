#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
using namespace std;

// Take a string and a delimiter, and split that string in two at the first occurrence
// of the delimiter.
// Throws a runtime error if the delimiter can't be found.
tuple<string, string> split_string_in_two(string s, char delimiter) {
    // Find the delimiter in the string.
    size_t pos_delimiter = s.find(delimiter);
    if(pos_delimiter == string::npos) {
        throw runtime_error("Failed to find delimiter");
    }

    // Split the string in two parts.
    string part1(s.begin(), s.begin() + pos_delimiter);
    string part2(s.begin() + pos_delimiter + 1, s.end());

    // Return the parts as a tuple.
    return make_tuple(part1, part2);
}

// The assignment for one elf. Represents the range of section the elf is in charge of.
class Assignment {
    public:
        Assignment(string line_section) {
            // Split the assignment into a start and end, and store them as integers.
            auto [start_s, end_s] = split_string_in_two(line_section, '-');

            start = stoi(start_s);
            end = stoi(end_s);
        }

        // Returns true if the current assignment is a superset of the given one.
        bool is_superset_of(Assignment* a) {
            return (start <= a->start && a->end <= end);
        }

        // Returns true if the given assignment overlaps with the current one, either fully
        // (i.e. one is a superset of the other), or partly (the start or end of one is within
        // the range of the other).
        bool overlaps_with(Assignment *a) {
            return (
                this->is_superset_of(a)
                || a->is_superset_of(this)
                || (start <= a->end && a->end <= end)
                || (start <= a->start && a->start <= end)
            );
        }
        
    private:
        // The start and end of the assignment.
        int start, end;
};

int main() {
    // Amount of elfs in a pair. This isn't super clean as we can't really change the value of this
    // constant without also changing the code to not always expect 2, but it's still nicer for readability
    // than using the literal 2 everywhere.
    static constexpr size_t ELFS_IN_PAIR = 2;

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
    // The amount of pairs which assigments ares overlapping.
    size_t pairs_overlapping = 0;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        // Split the line in two, each part is the assignment of one elf in the pair.
        string parts[ELFS_IN_PAIR];
        try {
            tie(parts[0], parts[1]) = split_string_in_two(line, ',');
        } catch(const runtime_error&) {
            // We've failed to find a comma, which must mean the input is incorrectly formatted.
            cerr << "No comma at line " << line_n << endl;
            return 1;
        }

        // Parse each assignment.
        Assignment* assignments[ELFS_IN_PAIR];
        for(size_t i = 0; i < ELFS_IN_PAIR; i++) {
            try {
                assignments[i] = new Assignment(parts[i]);
            } catch(const runtime_error&) {
                // We've failed to find a delimiter, which must mean the input is incorrectly formatted.
                cerr << "No delimiter in assigment #" << i << " of line " << line_n << endl;
                return 1;
            } catch(const invalid_argument&) {
                // We've encountered an unexpected non-numerical character when parsing the range, which
                // can mean the input is incorrectly formatted, or that we're not splitting the string
                // correctly.
                cerr << "Failed to read assigment #" << i << " of line " << line_n << endl;
                return 1;

            }
        }

        // Check if the assignments overlap. If so, increment the count.
        if(assignments[0]->overlaps_with(assignments[1])) {
            pairs_overlapping++;
        }
    }

    // Print result.
    cout << "Total pairs overlapping: " << pairs_overlapping << endl;

    return 0;
}