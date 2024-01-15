#include <windows.h>
#include <iostream>
using namespace std;

const int BOARD_SIZE = 8;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void init(char board[8][8])
{
    char initialPieces[8] = { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R' };
    for (int i = 0; i < 8; ++i)
    {
        board[0][i] = initialPieces[i];
        board[1][i] = 'P';
        board[6][i] = 'p';
        board[7][i] = tolower(initialPieces[i]);
        for (int j = 2; j < 6; ++j)
        {
            board[j][i] = ' ';
        }
    }
}

void displayBoard(const char board[8][8])
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

bool isValidCoordinate(int x, int y)
{
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

bool isHorizontalMove(int startX, int startY, int endX, int endY)
{
    return (startX == endX);
}

bool isVerticalMove(int startX, int startY, int endX, int endY)
{
    return (startY == endY);
}

bool isDiagonalMainMove(int startX, int startY, int endX, int endY)
{
    return (abs(startX - endX) == abs(startY - endY)) && (startX != endX);
}

bool isDiagonalSecMove(int startX, int startY, int endX, int endY)
{
    return (startX + startY == endX + endY) && (startX != endX);
}

bool isLegalPawn(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char pawn = board[startX][startY];
    int direction = (turn == 0) ? 1 : -1;
    if (startY == endY)
    {
        if (startX + direction == endX && board[endX][endY] == ' ')
        {
            return true;
        }
        if (startX + 2 * direction == endX && startX == (turn == 0 ? 1 : 6) && board[startX + direction][endY] == ' ' && board[endX][endY] == ' ')
        {
            return true;
        }
    }

    if (abs(startY - endY) == 1 && startX + direction == endX)
    {
        if (islower(board[endX][endY]) && turn == 0)
        {
            return true;
        }
        if (isupper(board[endX][endY]) && turn == 1)
        {
            return true;
        }
    }

    return false;
}


bool isLegalKing(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char king = board[startX][startY];

    if (abs(startX - endX) <= 1 && abs(startY - endY) <= 1)
    {
        if ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY])))
        {
            return true;
        }
    }

    return false;
}


bool isLegalQueen(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char queen = board[startX][startY];

    if ((startX == endX || startY == endY || abs(startX - endX) == abs(startY - endY)) && (startX != endX || startY != endY))
    {
        int dx = (endX - startX > 0) ? 1 : ((endX - startX < 0) ? -1 : 0);
        int dy = (endY - startY > 0) ? 1 : ((endY - startY < 0) ? -1 : 0);

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX || y != endY)
        {
            if (board[x][y] != ' ')
            {
                return false;
            }
            x += dx;
            y += dy;
        }

        if ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY])))
        {
            return true;
        }
    }

    return false;
}


bool isLegalBishop(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char bishop = board[startX][startY];

    if (abs(startX - endX) == abs(startY - endY) && (startX != endX || startY != endY))
    {
        int dx = (endX - startX > 0) ? 1 : -1;
        int dy = (endY - startY > 0) ? 1 : -1;

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX || y != endY)
        {
            if (board[x][y] != ' ')
            {
                return false;
            }
            x += dx;
            y += dy;
        }

        if ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY])))
        {
            return true;
        }
    }

    return false;
}


bool isLegalRook(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char rook = board[startX][startY];

    if ((startX == endX || startY == endY) && (startX != endX || startY != endY))
    {
        if (startX == endX)
        {
            int dy = (endY - startY > 0) ? 1 : -1;

            int y = startY + dy;

            while (y != endY)
            {
                if (board[startX][y] != ' ')
                {
                    return false;
                }
                y += dy;
            }
        }
        else {
            int dx = (endX - startX > 0) ? 1 : -1;

            int x = startX + dx;

            while (x != endX)
            {
                if (board[x][startY] != ' ')
                {
                    return false;
                }
                x += dx;
            }
        }

        if ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY])))
        {
            return true;
        }
    }

    return false;
}


bool isLegalKnight(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char knight = board[startX][startY];

    if ((abs(startX - endX) == 2 && abs(startY - endY) == 1) || (abs(startX - endX) == 1 && abs(startY - endY) == 2))
    {
        if ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY])))
        {
            return true;
        }
    }

    return false;
}
bool isValidMove(int startX, int startY, int endX, int endY)
{
    return true;
}
bool isKingCaptured(const char board[8][8])
{
    bool whiteKing = false;
    bool blackKing = false;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == 'k')
            {
                blackKing = true;
            }
            else if (board[i][j] == 'K')
            {
                whiteKing = true;
            }
        }
    }

    return !blackKing || !whiteKing;
}

int main()
{
    char board[BOARD_SIZE][BOARD_SIZE];
    init(board);

    bool gameOver = false;
    int turn = 0;
    while (!gameOver)
    {
        displayBoard(board);

        int startX, startY, endX, endY;

        cout << "Player " << (turn + 1) << ", click the piece you want to move." << endl;
        getRowColbyLeftClick(startY, startX); 
        startX /= 2;
        startY /= 2;

        cout << "Click the destination to move the piece." << endl;
        getRowColbyLeftClick(endY, endX); 
        endX /= 2; 
        endY /= 2;

        if (!isValidCoordinate(startX, startY) || !isValidCoordinate(endX, endY))
        {
            cout << "Invalid coordinates! Try again." << endl;
            continue;
        }

        char selectedPiece = board[startX][startY];
        if ((turn == 0 && islower(selectedPiece)) || (turn == 1 && isupper(selectedPiece)))
        {
            cout << "It's not your piece! Try again." << endl;
            continue;
        }

        if (board[endX][endY] != ' ' && ((turn == 0 && !islower(board[endX][endY])) || (turn == 1 && !isupper(board[endX][endY]))))
        {
            cout << "Illegal move! Try again." << endl;
            continue;
        }

        bool legalMove = false;
        switch (tolower(selectedPiece))
        {
        case 'p':
            legalMove = isLegalPawn(startX, startY, endX, endY, board, turn);
            break;
        case 'r':
            legalMove = isLegalRook(startX, startY, endX, endY, board, turn);
            break;
        case 'n':
            legalMove = isLegalKnight(startX, startY, endX, endY, board, turn);
            break;
        case 'b':
            legalMove = isLegalBishop(startX, startY, endX, endY, board, turn);
            break;
        case 'q':
            legalMove = isLegalQueen(startX, startY, endX, endY, board, turn);
            break;
        case 'k':
            legalMove = isLegalKing(startX, startY, endX, endY, board, turn);
            break;
        default:
            break;
        }

        if (!legalMove)
        {
            cout << "Illegal move! Try again." << endl;
            continue;
        }

        board[endX][endY] = selectedPiece;
        board[startX][startY] = ' ';
        gameOver = isKingCaptured(board);

        if (gameOver)
        {
            cout << "Game Over!" << endl;
            break;
        }

        turn = (turn + 1) % 2;
    }

    return 0;
}