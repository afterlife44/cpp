#include <iostream>
#include <vector>

using namespace std;

// Класс, представляющий игру в крестики-нолики
class TicTacToe {
private:
    int size;             // Размер игрового поля
    int winLength;        // Длина выигрышной последовательности
    vector<vector<char>> board;  // Игровое поле в виде двумерного вектора символов

public:
    // Конструктор для инициализации игрового поля с указанным размером и длиной выигрышной последовательности
    TicTacToe(int _size, int _winLength) : size(_size), winLength(_winLength) {
        // Инициализация игрового поля пустыми ячейками
        board = vector<vector<char>>(size, vector<char>(size, ' '));
    }

    // Отображение текущего состояния игрового поля
    void displayBoard() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // Выполнение хода игрока
    bool makeMove(int row, int col, char symbol) {
        // Адаптация координат от 1 до size
        --row;
        --col;

        // Проверка корректности хода и установка символа в ячейку
        if (row < 0 || row >= size || col < 0 || col >= size || board[row][col] != ' ') {
            return false;
        }

        board[row][col] = symbol;
        return true;
    }

    // Проверка, является ли текущий символ победителем
    bool isWinner(char symbol) const {
        // Проверка по строкам и столбцам
        for (int i = 0; i < size; ++i) {
            int countRow = 0;
            int countCol = 0;
            for (int j = 0; j < size; ++j) {
                if (board[i][j] == symbol) {
                    ++countRow;
                }
                if (board[j][i] == symbol) {
                    ++countCol;
                }
            }
            if (countRow == winLength || countCol == winLength) {
                return true;
            }
        }

        // Проверка по диагоналям
        for (int i = 0; i <= size - winLength; ++i) {
            for (int j = 0; j <= size - winLength; ++j) {
                int countDiagonal1 = 0;
                int countDiagonal2 = 0;
                for (int k = 0; k < winLength; ++k) {
                    if (board[i + k][j + k] == symbol) {
                        ++countDiagonal1;
                    }
                    if (board[i + k][j + winLength - 1 - k] == symbol) {
                        ++countDiagonal2;
                    }
                }
                if (countDiagonal1 == winLength || countDiagonal2 == winLength) {
                    return true;
                }
            }
        }

        return false;
    }

    // Проверка, заполнено ли игровое поле
    bool isBoardFull() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }
};

// Абстрактный класс, представляющий игрока
class Player {
protected:
    char symbol;  // Символ игрока (X или O)

public:
    // Конструктор для установки символа игрока
    Player(char _symbol) : symbol(_symbol) {}

    // Виртуальная функция для выполнения хода игрока
    virtual void makeMove(TicTacToe& game) = 0;
};

// Класс, представляющий человеческого игрока
class HumanPlayer : public Player {
public:
    // Конструктор для установки символа человеческого игрока
    HumanPlayer(char _symbol) : Player(_symbol) {}

    // Реализация функции для выполнения хода человеческого игрока
    void makeMove(TicTacToe& game) override {
        int row, col;
        cout << "Введите ваш ход (строка и столбец): ";
        cin >> row >> col;

        // Повторный ввод, пока ход некорректен
        while (!game.makeMove(row, col, symbol)) {
            cout << "Некорректный ход. Попробуйте снова: ";
            cin >> row >> col;
        }
    }
};

// Основная функция программы
int main() {
    const int boardSize = 3;   // Размер игрового поля
    const int winLength = 3;   // Длина выигрышной последовательности

    // Создание объекта игры и двух человеческих игроков
    TicTacToe game(boardSize, winLength);
    HumanPlayer playerX('X');
    HumanPlayer playerO('O');

    // Игровой цикл до завершения игры (победа, ничья)
    while (!game.isBoardFull() && !game.isWinner('X') && !game.isWinner('O')) {
        // Ход игрока X
        playerX.makeMove(game);
        game.displayBoard();

        // Проверка победы или ничьей после хода игрока X
        if (game.isWinner('X')) {
            cout << "Игрок X побеждает!" << endl;
            break;
        }
        else if (game.isBoardFull()) {
            cout << "Ничья!" << endl;
            break;
        }

        // Ход игрока O
        playerO.makeMove(game);
        game.displayBoard();

        // Проверка победы или ничьей после хода игрока O
        if (game.isWinner('O')) {
            cout << "Игрок O побеждает!" << endl;
            break;
        }
        else if (game.isBoardFull()) {
            cout << "Ничья!" << endl;
            break;
        }
    }

    return 0;
}

