#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Абстрактный класс для игрока
class Player {
public:
    virtual void makeMove(vector<vector<char>>& board, char symbol) = 0;
};

// Класс для управления ходами человека
class HumanPlayer : public Player {
public:
    void makeMove(vector<vector<char>>& board, char symbol) override {
        int x, y;
        while (true) {
            cout << "Enter your move (row and column, starting from 1): "; // Запрос хода у пользователя
            cin >> x >> y;
            x--; y--; // Преобразование в индексы массива
            if (x >= 0 && x < board.size() && y >= 0 && y < board.size() && board[x][y] == ' ') {
                board[x][y] = symbol;
                break;
            } else {
                cout << "Invalid move. Try again.\n"; // Сообщение об ошибке
            }
        }
    }
};

// Класс для управления ходами бота
class Bot : public Player {
public:
    void makeMove(vector<vector<char>>& board, char symbol) override {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1, bestCol = -1;

        // Проходим по всем возможным ходам и выбираем лучший
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = symbol;
                    // Оценка хода с использованием минимакса и альфа-бета отсечений
                    int score = minimax(board, false, symbol, getOpponent(symbol), numeric_limits<int>::min(), numeric_limits<int>::max());
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
        cout << "Bot made a move at (" << bestRow + 1 << ", " << bestCol + 1 << ")\n"; // Сообщение о ходе бота
    }

    // Реализация алгоритма минимакса с альфа-бета отсечениями
    int minimax(vector<vector<char>>& board, bool isMaximizing, char botSymbol, char playerSymbol, int alpha, int beta) {
        // Эвристика: проверка на победу
        if (checkWin(board, botSymbol)) return 1;
        if (checkWin(board, playerSymbol)) return -1;
        if (isBoardFull(board)) return 0;

        int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();

        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = isMaximizing ? botSymbol : playerSymbol;
                    // Рекурсивная оценка хода
                    int score = minimax(board, !isMaximizing, botSymbol, playerSymbol, alpha, beta);
                    board[i][j] = ' ';

                    if (isMaximizing) {
                        bestScore = max(score, bestScore);
                        alpha = max(alpha, bestScore);
                    } else {
                        bestScore = min(score, bestScore);
                        beta = min(beta, bestScore);
                    }

                    // Условие для отсечения
                    if (beta <= alpha) break;
                }
            }
            if (beta <= alpha) break;
        }
        return bestScore;
    }

    // Проверка на выигрыш
    bool checkWin(const vector<vector<char>>& board, char symbol) {
        int size = board.size();

        // Проверка по строкам
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

        // Проверка по столбцам
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

    // Проверка на заполненность поля
    bool isBoardFull(const vector<vector<char>>& board) {
        for (const auto& row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }

    // Получение символа противника
    char getOpponent(char symbol) {
        return (symbol == 'X') ? 'O' : 'X';
    }
};

// Класс игры "Крестики-нолики"
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

    // Отображение игрового поля
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

    // Проверка на заполненность поля
    bool isBoardFull() {
        for (const auto &row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }

    // Проверка на победу
    bool checkWin(char symbol) {
        return bot.checkWin(board, symbol);
    }

    // Основной цикл игры
    void playGame() {
        while (true) {
            displayBoard();
            cout << "Your turn:\n";
            player.makeMove(board, PLAYER_SYMBOL);
            if (checkWin(PLAYER_SYMBOL)) {
                displayBoard();
                cout << "Congratulations! You win!\n"; // Сообщение о победе игрока
                break;
            }
            if (isBoardFull()) {
                displayBoard();
                cout << "It's a draw!\n"; // Сообщение о ничьей
                break;
            }

            cout << "Bot's turn:\n";
            bot.makeMove(board, BOT_SYMBOL);
            if (checkWin(BOT_SYMBOL)) {
                displayBoard();
                cout << "Bot wins! Try again!\n"; // Сообщение о победе бота
                break;
            }
            if (isBoardFull()) {
                displayBoard();
                cout << "It's a draw!\n"; // Сообщение о ничьей
                break;
            }
        }
    }
};

// Главная функция
int main() {
    int boardSize;
    cout << "Enter the board size: ";
    cin >> boardSize;

    if (boardSize < 3) {
        cout << "The board size must be at least 3.\n";
        return 1;
    }

    TicTacToe game(boardSize);
    game.playGame();
    return 0;
}
