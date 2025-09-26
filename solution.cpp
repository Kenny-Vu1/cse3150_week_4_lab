#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <iterator>
#include <vector>
#include <chrono>
#include <utility>
#include <iostream>

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
    for(auto& row: board){
        for(auto& cell: row){
            cout << cell << " ";
        }
            cout << endl;
    }
    write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly
// void spawn_tile(vector<vector<int>>& board) {
//     for (int r=0;r<4;r++)
//         for (int c=0;c<4;c++)
//             if (board[r][c]==0) { board[r][c]=2; return; }
//     // TODO: Feed this into chat GPT and have it correct the function for you
//     // with proper prompting
// }

// void spawn_tile(std::vector<std::vector<int>>& board) {
//     // 1) collect all coords
//     std::vector<std::pair<int,int>> coords; coords.reserve(16);
//     for (int r = 0; r < 4; ++r)
//         for (int c = 0; c < 4; ++c)
//             coords.emplace_back(r, c);

//     // 2) filter empties
//     std::vector<std::pair<int,int>> empties; empties.reserve(16);
//     std::copy_if(coords.begin(), coords.end(), std::back_inserter(empties),
//                  [&](const std::pair<int,int>& p){ return board[p.first][p.second] == 0; });

//     if (empties.empty()) return;

//     // 3) RNG: std::mt19937 + uniform_int_distribution
//     static thread_local std::mt19937 rng{std::random_device{}()};
//     std::uniform_int_distribution<std::size_t> pick(0, empties.size() - 1);
//     std::bernoulli_distribution is_four(0.10); // 10% for a 4

//     auto [r, c] = empties[pick(rng)];
//     board[r][c] = is_four(rng) ? 4 : 2;        // 90% 2, 10% 4
// }
// Deterministic under srand(): pick a random empty, 90% 2 / 10% 4

void spawn_tile(std::vector<std::vector<int>>& board) {
    std::vector<std::pair<int,int>> empties;
    empties.reserve(16);
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            if (board[r][c] == 0) empties.emplace_back(r, c);

    if (empties.empty()) return;

    int idx = std::rand() % static_cast<int>(empties.size());
    auto [r, c] = empties[idx];

    // 90% chance 2, 10% chance 4
    int roll = std::rand() % 10;   // 0..9
    board[r][c] = (roll == 0) ? 4 : 2;
}



// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end
std::vector<int> compress_row(const std::vector<int>& row) {
    std::vector<int> compressed;
    copy_if(row.begin(), row.end(), back_inserter(compressed), [](int x){return x != 0;});
    compressed.resize(4,0);
    return compressed;
}

// TODO: Merge a row (assumes the row is already compressed)
std::vector<int> merge_row(std::vector<int> row) {
    row = compress_row(row);
    for(int i =0; i< 3; i++){
        if(row[i] == row[i+1] && row[i] != 0){
            row[i] = row[i]*2;
            row[i+1] = 0;
            i ++;
        }
    }
    row = compress_row(row);
    return row;
}

// TODO: use copy_if and iterators
bool move_left(vector<vector<int>>& board){
    const auto old_board = board;
    for(auto& row:board){
        row=merge_row(row);
    }
    return board != old_board;
}

// TODO: use reverse iterators
bool move_right(vector<vector<int>>& board){
    const auto old_board = board;
    for(auto& row:board){
        vector<int> reversed(row.rbegin(), row.rend());
        reversed=merge_row(reversed);
        row.assign(reversed.rbegin(),reversed.rend());
    }
    return board != old_board;
}
// TODO: use column traversal
bool move_up(vector<vector<int>>& board){
    const auto old_board = board;
    
    for(int col=0; col<4;col++){
        vector<int> column(4);

        for(int row=0; row<4; row++){
            column[row] = old_board[row][col];
        }
        column = merge_row(column);

        for(int row=0; row<4; row++){
            board[row][col] = column[row];
        }
    }
    return board != old_board;
}


// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){
    const auto old_board = board;
    
    for(int col=0; col<4;col++){
        vector<int> column(4);

        for(int row=0; row<4; row++){
            column[row] = old_board[row][col];
        }

        vector<int> rev(column.rbegin(), column.rend());
        rev = merge_row(rev);
        vector<int> output(rev.rbegin(), rev.rend());

        for(int row=0; row<4; row++){
            board[row][col] = output[row];
        }
    }
    return board != old_board;
}


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

            if(history.empty()){
                cout << "Nothing to undo." << endl;
                continue;
            }
            board = history.top();
            cout << "\n";
            history.pop();
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            history.push(prev);
            spawn_tile(board);
        }
    }
    return 0;
}
