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
            cout << "Enter your move (row and column, starting from 1): ";
            cin >> x >> y;
            x--; y--; // Convert to array indices
            if (x >= 0 && x < board.size() && y >= 0 && y < board.size() && board[x][y] == ' ') {
                board[x][y] = symbol;
                break;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        }
    }
};

// Класс для управления ходами бота с эвристической оценкой
class Bot : public Player {
public:
    void makeMove(vector<vector<char>>& board, char symbol) override {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1, bestCol = -1;

        // Проходим по всем возможным ходам и выбираем лучший на основе эвристической оценки
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = symbol;
                    int score = evaluateBoard(board, symbol);
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

    // Эвристическая функция оценки игрового поля
    int evaluateBoard(const vector<vector<char>>& board, char symbol) {
        int score = 0;

        // Оценка строк, столбцов и диагоналей
        score += evaluateLines(board, symbol);
        score -= evaluateLines(board, getOpponent(symbol));

        return score;
    }

    // Оценка строк, столбцов и диагоналей для данного символа
    int evaluateLines(const vector<vector<char>>& board, char symbol) {
        int score = 0;
        int size = board.size();

        // Оценка строк и столбцов
        for (int i = 0; i < size; i++) {
            score += evaluateLine(board, i, 0, 0, 1, symbol); // Строка
            score += evaluateLine(board, 0, i, 1, 0, symbol); // Столбец
        }

        // Оценка диагоналей
        score += evaluateLine(board, 0, 0, 1, 1, symbol); // Главная диагональ
        score += evaluateLine(board, 0, size - 1, 1, -1, symbol); // Побочная диагональ

        return score;
    }

    // Оценка отдельной линии (строка, столбец или диагональ)
    int evaluateLine(const vector<vector<char>>& board, int startX, int startY, int dx, int dy, char symbol) {
        int count = 0;
        int size = board.size();

        // Проходим по линии и считаем количество совпадений
        for (int i = 0; i < size; i++) {
            int x = startX + i * dx;
            int y = startY + i * dy;
            if (x >= 0 && x < size && y >= 0 && y < size) {
                if (board[x][y] == symbol) {
                    count++;
                } else if (board[x][y] != ' ') {
                    return 0; // Прерываем, если линия заблокирована
                }
            }
        }

        // Если вся линия состоит из одного символа, возвращаем высокий балл
        if (count == size) {
            return 10;
        } else if (count == size - 1) {
            return 5;
        }
        return 0;
    }

    // Получение символа противника
    char getOpponent(char symbol) {
        return (symbol == 'X') ? 'O' : 'X';
    }

    // Проверка на выигрыш для данного символа (строки, столбцы, диагонали)
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
};

// Класс игры
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
        for (const auto& row : board)
            for (char cell : row)
                if (cell == ' ') return false;
        return true;
    }

    // Проверка на выигрыш
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
                cout << "Bot wins! Try again!\n";
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

// Точка входа в программу
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
