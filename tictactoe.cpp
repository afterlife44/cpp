#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Player {
public:
    virtual void makeMove(vector<vector<char>>& board, char symbol) = 0;
};

class HumanPlayer : public Player {
public:
    void makeMove(vector<vector<char>>& board, char symbol) override {
        int x, y;
        while (true) {
            cout << "Enter your move (row and column starting from 1): ";
            cin >> x >> y;
            x--; y--;
            if (x >= 0 && x < board.size() && y >= 0 && y < board.size() && board[x][y] == ' ') {
                board[x][y] = symbol;
                break;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        }
    }
};

class Bot : public Player {
public:
    void makeMove(vector<vector<char>>& board, char symbol) override {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1, bestCol = -1;

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = symbol;
                    int score = minimax(board, false, symbol, getOpponent(symbol));
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        board[bestRow][bestCol] = symbol;
        cout << "Bot made a move at (" << bestRow + 1 << ", " << bestCol + 1 << ")\n";
    }

private:
    int minimax(vector<vector<char>>& board, bool isMaximizing, char botSymbol, char playerSymbol) {
        if (checkWin(board, botSymbol)) return 1;
        if (checkWin(board, playerSymbol)) return -1;
        if (isBoardFull(board)) return 0;

        int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = isMaximizing ? botSymbol : playerSymbol;
                    int score = minimax(board, !isMaximizing, botSymbol, playerSymbol);
                    board[i][j] = ' ';
                    bestScore = isMaximizing ? max(score, bestScore) : min(score, bestScore);
                }
            }
        }
        return bestScore;
    }

    bool checkWin(const vector<vector<char>>& board, char symbol) {
        int size = board.size();

        // Проверка строк
        for (int i = 0; i < size; i++) {
            bool rowWin = true;
            for (int j = 0; j < size; j++) {
                if (board[i][j] != symbol) {
                    rowWin = false;
                    break;
                }
            }
            if (rowWin) return true;
        }

        // Проверка столбцов
        for (int i = 0; i < size; i++) {
            bool colWin = true;
            for (int j = 0; j < size; j++) {
                if (board[j][i] != symbol) {
                    colWin = false;
                    break;
                }
            }
            if (colWin) return true;
        }

        // Проверка главной диагонали
        bool mainDiagonalWin = true;
        for (int i = 0; i < size; i++) {
            if (board[i][i] != symbol) {
                mainDiagonalWin = false;
                break;
            }
        }
        if (mainDiagonalWin) return true;

        // Проверка побочной диагонали
        bool secondaryDiagonalWin = true;
        for (int i = 0; i < size; i++) {
            if (board[i][size - i - 1] != symbol) {
                secondaryDiagonalWin = false;
                break;
            }
        }
        return secondaryDiagonalWin;
    }

    bool isBoardFull(const vector<vector<char>>& board) {
        for (const auto& row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }

    char getOpponent(char symbol) {
        return (symbol == 'X') ? 'O' : 'X';
    }
};

class TicTacToe {
private:
    vector<vector<char>> board;
    HumanPlayer player;
    Bot bot;
    const char PLAYER_SYMBOL = 'X';
    const char BOT_SYMBOL = 'O';
    int size;

public:
    TicTacToe(int boardSize) : size(boardSize), board(boardSize, vector<char>(boardSize, ' ')) {}

    void displayBoard() {
        cout << "  ";
        for (int i = 1; i <= size; ++i) cout << i << " ";
        cout << "\n";

        for (int i = 0; i < size; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < size; j++) {
                cout << board[i][j];
                if (j < size - 1) cout << "|";
            }
            cout << "\n";
            if (i < size - 1) {
                cout << "  ";
                for (int k = 0; k < size; ++k) cout << "--";
                cout << "-\n";
            }
        }
    }

    bool isBoardFull() {
        for (const auto &row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }

    bool checkWin(char symbol) {
        int size = board.size();

        // Проверка строк
        for (int i = 0; i < size; i++) {
            bool rowWin = true;
            for (int j = 0; j < size; j++) {
                if (board[i][j] != symbol) {
                    rowWin = false;
                    break;
                }
            }
            if (rowWin) return true;
        }

        // Проверка столбцов
        for (int i = 0; i < size; i++) {
            bool colWin = true;
            for (int j = 0; j < size; j++) {
                if (board[j][i] != symbol) {
                    colWin = false;
                    break;
                }
            }
            if (colWin) return true;
        }

        // Проверка главной диагонали
        bool mainDiagonalWin = true;
        for (int i = 0; i < size; i++) {
            if (board[i][i] != symbol) {
                mainDiagonalWin = false;
                break;
            }
        }
        if (mainDiagonalWin) return true;

        // Проверка побочной диагонали
        bool secondaryDiagonalWin = true;
        for (int i = 0; i < size; i++) {
            if (board[i][size - i - 1] != symbol) {
                secondaryDiagonalWin = false;
                break;
            }
        }
        return secondaryDiagonalWin;
    }

    void playGame() {
        while (true) {
            displayBoard();
            cout << "Your turn:\n";
            player.makeMove(board, PLAYER_SYMBOL);
            if (checkWin(PLAYER_SYMBOL)) {
                displayBoard();
                cout << "Congratulations! You win!\n";
                break;
            }
            if (isBoardFull()) {
                displayBoard();
                cout << "It's a draw!\n";
                break;
            }

            cout << "Bot's turn:\n";
            bot.makeMove(board, BOT_SYMBOL);
            if (checkWin(BOT_SYMBOL)) {
                displayBoard();
                cout << "Bot wins! Better luck next time!\n";
                break;
            }
            if (isBoardFull()) {
                displayBoard();
                cout << "It's a draw!\n";
                break;
            }
        }
    }
};

int main() {
    int boardSize;
    cout << "Enter the size of the board: ";
    cin >> boardSize;

    if (boardSize < 3) {
        cout << "The board size must be at least 3.\n";
        return 1;
    }

    TicTacToe game(boardSize);
    game.playGame();
    return 0;
}
