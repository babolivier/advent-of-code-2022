#include <iostream>
#include <fstream>
#include <map>
using namespace std;

/*
Opponent shapes:
    A = Rock
    B = Paper
    C = Scissors

Our shapes:
    X = Rock
    Y = Paper
    Z = Scissors
*/


int main() {
    // Amount of points given for each outcome.
    const int WIN_SCORE = 6;
    const int DRAW_SCORE = 3;
    const int LOSS_SCORE = 0;

    // Amount of points given for each shape.
    map<char, int> score_per_shape;
    score_per_shape['X'] = 1;
    score_per_shape['Y'] = 2;
    score_per_shape['Z'] = 3;

    // For each play by the opponent, shape to play to win the round.
    map<char, char> win_outcomes;
    // Paper defeats rock.
    win_outcomes['A'] = 'Y';
    // Scissors defeats paper.
    win_outcomes['B'] = 'Z';
    // Rock defeats scissors.
    win_outcomes['C'] = 'X';

    // For each play by the opponent, shape to play to draw the round.
    map<char, char> draw_outcomes;
    // Rock.
    draw_outcomes['A'] = 'X';
    // Paper.
    draw_outcomes['B'] = 'Y';
    // Scissors.
    draw_outcomes['C'] = 'Z';

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
    // The shapes played by the opponent and us.
    char opponent_play, my_play;
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
        my_play = line[2];

        // Figure out the outcome of the round.
        int outcome_score = LOSS_SCORE;
        if(win_outcomes[opponent_play] == my_play) {
            outcome_score = WIN_SCORE;
        } else if(draw_outcomes[opponent_play] == my_play) {
            outcome_score = DRAW_SCORE;
        }

        // Figure out the score for the shape.
        int shape_score = score_per_shape[my_play];

        // Calculate the new total score.
        total_score += outcome_score + shape_score;
    }

    // Print the result.
    cout << "Total score: " << total_score << "\n";
    return 0;
}