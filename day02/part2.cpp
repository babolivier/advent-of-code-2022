#include <iostream>
#include <fstream>
#include <map>
using namespace std;

/*
Shapes:
    A = Rock
    B = Paper
    C = Scissors

Outcomes:
    X = Loss
    Y = Draw
    Z = Win
*/


int main() {
    // Amount of points given for each outcome.
    const int WIN_SCORE = 6;
    const int DRAW_SCORE = 3;
    const int LOSS_SCORE = 0;

    // Amount of points given for each shape.
    map<char, int> score_per_shape;
    score_per_shape['A'] = 1;
    score_per_shape['B'] = 2;
    score_per_shape['C'] = 3;

    // For each play by the opponent, shape to play to win the round.
    map<char, char> win_outcomes;
    // Paper defeats rock.
    win_outcomes['A'] = 'B';
    // Scissors defeats paper.
    win_outcomes['B'] = 'C';
    // Rock defeats scissors.
    win_outcomes['C'] = 'A';

    // For each play by the opponent, shape to play to lose the round.
    // This is an invert map to win_outcomes. We could use pointers to
    // optimise both maps' size, but given we're working with a small
    // static set of key/values, it doesn't feel worth the effort.
    map<char, char> loss_outcomes;
    loss_outcomes['A'] = 'C';
    loss_outcomes['B'] = 'A';
    loss_outcomes['C'] = 'B';

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
    // The shapes played by the opponent and us, as well as the desired outcome.
    char opponent_play, my_play, outcome;
    // The total score of every round so far.
    int total_score = 0;
    while(getline(input, line)) {
        // Line numbers start at 1 so we can align with text editors to look at where the file is badly formatted.
        line_n++;

        // Make sure the lines follow the correct format. If they don't this is a fatal error - there's nothing more
        // for us to do apart exiting.
        if(line.length() != 3) {
            cerr << "Bad format at line " << line_n << "\n";
            return 1;
        }

        // Read the shapes played from the line.
        opponent_play = line[0];
        outcome = line[2];

        // Set the outcome score and the shape to play depending on the desired outcome.
        int outcome_score;
        switch (outcome) {
            case 'X':
                outcome_score = LOSS_SCORE;
                my_play = loss_outcomes[opponent_play];
                break;
            case 'Y':
                outcome_score = DRAW_SCORE;
                my_play = opponent_play;
                break;
            case 'Z':
                outcome_score = WIN_SCORE;
                my_play = win_outcomes[opponent_play];
        }

        // Figure out the shape score.
        int shape_score = score_per_shape[my_play];

        // Calculate the new total score.
        total_score += shape_score + outcome_score;
    }

    // Print the result.
    cout << "Total score: " << total_score << "\n";
    return 0;
}