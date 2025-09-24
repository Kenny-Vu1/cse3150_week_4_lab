#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    // TODO: implement print_board here
    write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly
void spawn_tile(vector<vector<int>>& board) {
    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++)
            if (board[r][c]==0) { board[r][c]=2; return; }
    // TODO: Feed this into chat GPT and have it correct the function for you
    // with proper prompting
}


// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end
std::vector<int> compress_row(const std::vector<int>& row) {return row;}
// TODO: Merge a row (assumes the row is already compressed)
std::vector<int> merge_row(std::vector<int> row) {return row;}
// TODO: use copy_if and iterators
bool move_left(vector<vector<int>>& board){return false;}
// TODO: use reverse iterators
bool move_right(vector<vector<int>>& board){return false;}
// TODO: use column traversal
bool move_up(vector<vector<int>>& board){return false;}
// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){return false;}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: get the history and print the board and continue
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            spawn_tile(board);
        }
    }
    return 0;
}
