#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

// A file in the file system.
class File {
    public:
        string name;
        size_t size;

        File(string _name, size_t _size) {
            name = _name;
            size = _size;
        }
};

// A directory containing a combination of files and subdirectories.
class Dir {
    public:
        string name;
        // The subdirectories and files in the directory.
        map<string, Dir*> subdirs;
        map<string, File*> files;
        // The directory's parent. Null if the directory is the root of
        // the file system.
        Dir* parent;
        // Whether this directory is the file system's root.
        bool root;

        Dir(string _name, Dir* _parent) {
            // Mark the root directory as such.
            if(_name == "/") {
                root = true;
            } else {
                // If this directory isn't the root, it needs a parent.
                if(_parent == nullptr) {
                    throw invalid_argument("only root dir cannot have a parent");
                }

                root = false;
            }

            name = _name;
            parent = _parent;
        }
};


// Calculate the size of the given directory, including its subdirectories. If
// the total size is at most 100000 (as per instructions), also add it to the given
// vector.
int calculate_dir_size(Dir* dir, vector<tuple<size_t, Dir*>>* small_dirs) {
    size_t total_size = 0;

    // Iterate over the files in the directory.
    for(auto [name, file] : dir->files) {
        total_size += file->size;
    }

    // Iterate over the subdirectories. For each of them, recursively call this
    // function.
    for(auto [name, subdir] : dir->subdirs) {
        int subdir_size = calculate_dir_size(subdir, small_dirs);
        total_size += subdir_size;
    }

    // If the total size of the directory is at most 100000, store it in the vector.
    if(total_size <= 100000) {
        small_dirs->push_back(make_tuple(total_size, dir));
    }

    return total_size;
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
    // The file system's root.
    Dir* root = new Dir("/", nullptr);
    // The current directory.
    Dir* current_dir = nullptr;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        stringstream sline(line);
        string word;

        // Read the first word in the sentence.
        sline >> word;

        // If the line starts with '$', this is a command. Otherwise, it's describing an element
        // in the current directory.
        if(word == "$") {
            // Read the command.
            sline >> word;
            
            // If the command is "cd", change the current directory to the correct value.
            if(word == "cd") {
                sline >> word;

                if(word == "..") {
                    // If the directory to move to is "..", then we move to the parent of the
                    // current directory.
                    current_dir = current_dir->parent;
                } else if(word == "/") {
                    // If the directory to move to is "/", then we move to the root of the file
                    // system.
                    current_dir = root;
                } else {
                    // Otherwise, we move to any directory with that name.
                    if(current_dir->subdirs.find(word) == current_dir->subdirs.end()) {
                        // The input file happens to only move into directories we've discovered
                        // with "ls", but the instructions don't state this fact as part of the
                        // rules of the exercise. Therefore we consider it undefined behaviour and
                        // exit.
                        cerr    << "Trying to move into directory "
                                << word
                                << " without checking if it exists at line "
                                << line_n
                                << endl;
                        return 1;
                    }

                    current_dir = current_dir->subdirs[word];
                }
            } else if(word != "ls") {
                // If we're not changing directories, then only "ls" (which in our case does not take)
                // arguments is allowed.
                cerr << "Unexpected " << word << " command in line " << line_n << endl;
                return 1;
            }
        } else {
            if(word == "dir") {
                // This element is a subdirectory, get its name and store it as such.
                sline >> word;
                try {
                    current_dir->subdirs[word] = new Dir(word, current_dir);
                } catch(const invalid_argument&) {
                    cerr    << "Tried to build a non-root directory with no parent at line"
                            << line_n
                            << ". This should not happen."
                            << endl;
                    return 1;
                }
            } else {
                // The element is a file, try to read its size and name and store it as such.
                size_t size;
                try{
                    size = stoi(word);
                } catch(const invalid_argument&) {
                    cerr    << "Invalid value for size "
                            << word
                            << " at line "
                            << line_n
                            << endl;
                    return 1;
                }
                sline >> word;
                current_dir->files[word] = new File(word, size);
            }
        }
    }

    // Calculate the size of all directories and subdirectories, and store the ones under 100000
    // (as per instructions).
    vector<tuple<size_t, Dir*>> small_dirs;
    calculate_dir_size(root, &small_dirs);

    // Calculate and print the sum of all directories which size is under 100000.
    int total_size = 0;
    for(auto el : small_dirs) {
        total_size += get<0>(el);
    }
    cout << "Total size of small directories: " << total_size << endl;
}