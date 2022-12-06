#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    // Make sure the program is being called correctly.
    if(argc != 2) {
        cerr << "Usage: " << argv[0] << " MARKER_LENGTH" << endl;
        return 1;
    }
    
    // Make sure the marker length is a number and parse it.
    int marker_length;
    try {
        marker_length = stoi(argv[1]);
    } catch(const invalid_argument&) {
        cerr << "Marker length must be a number" << endl;
        return 1;
    }

    // Open input and make sure that succeeded.
    fstream input("input", ios::in);
    if(!input.is_open()) {
        cerr << "Failed to open file :(";
        return 1;
    }

    // The current line's content.
    string line;
    // The input is on a single line, we can call getline outside of a loop.
    getline(input, line);
    // Declare the index outside of the loop so we can use it to print out the result.
    size_t i;
    for(i = 0; i < line.length(); i++) {
        // Isolate the section we want to look at.
        string section(line.begin() + i, line.begin() + i + marker_length);

        // Whether the current section is a marker.
        bool is_marker;
        // For each character in the section, we check if the character only appears once.
        for(size_t j = 0; j < section.length(); j++) {
            if(count(section.begin(), section.end(), section[j]) != 1) {
                // We have determined this section is not a packet marker, so we set the bool
                // to false and break out of the loop (since we've found what we were looking for).
                is_marker = false;
                break;
            }

            is_marker = true;
        }

        // The marker has been found, let's stop here.
        if(is_marker) {
            break;
        }
    }

    // Print out the result. We need to add the marker length to correctly represent all the characters we've processed.
    cout << "Processed " << i + marker_length << " characters." << endl;
}