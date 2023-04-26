using namespace sf;
using namespace std;

#include <vector>

class Board
{
    struct Chip
    {
        enum State : int
        {
            BOARD = 0,
            RED = 1,
            YELLOW = -1,
            BACKGROUND = 2
        };

        Vector2f pos;
        State state;

        Chip()
        {
        }
        Chip(Vector2f p)
        {
            pos = p;
        }
    };

    int startCol = 4;
    int endCol = 12;
    int startRow = 3;
    int endRow = 9;
    vector<vector<Chip>> board;

public:
    Board()
    {
    }
    void copy(Board rhs)
    {
        this->board = rhs.board;
    }

    void setBoard()
    {
        // set up background
        for (int i = 0 ; i < 13 ; i++)
        {
            vector<Chip> tempVec;
            for (int j = 0; j < 16; j++)
            {
                float x = (float) (j * 64);
                float y = (float) (i * 64);
                Chip temp = Chip(Vector2f(x, y));
                temp.state = Chip::BACKGROUND;

                tempVec.push_back(temp);
            }
            board.push_back(tempVec);
        }

        // set up game board
        // Board is 6x7 but there are 2 extra cols and 1 extra row for border
        for (int i = startRow ; i <= endRow ; i++)
            for (int j = endCol ; j >= startCol ; j--)
            {
                if (i == 9 || j == 4 || j == 12) board[i][j].state = Chip::BOARD;
            }

    }
    void resetBoard()
    {
        for (int i = startRow ; i <= endRow ; i++)
            for (int j = startCol + 1; j <= endCol - 1; j++)
                board[i][j].state = Chip::BACKGROUND;
    }
    void drawBoard(RenderWindow &window)
    {
        RectangleShape tile(Vector2f(64,64));
        tile.setOutlineColor(Color::Black);
        tile.setOutlineThickness(4);

        for (int i = 0 ; i < 12 ; i++)
            for (int j = 0 ; j < 16 ; j++)
            {
                tile.setPosition(board[i][j].pos);

                if (board[i][j].state == Chip::BOARD)
                    tile.setFillColor(Color::Blue);
                else if (board[i][j].state == Chip::RED)
                    tile.setFillColor(Color::Red);
                else if (board[i][j].state == Chip::YELLOW)
                    tile.setFillColor(Color::Yellow);
                else if (board[i][j].state == Chip::BACKGROUND)
                    tile.setFillColor(Color::White);

                window.draw(tile);
            }
    }

    int selectCol(int mouseX)
    {
        // we have to check between cols 5 and 11 inclusive
        for (int i = startCol ; i <= endCol-1 ; i++)
            if (mouseX <= (i+1) * 64 && mouseX >= i * 64)
                return i;
    }
    bool colFull(int col)
    {
        return (board[startRow][col].state != Chip::BACKGROUND);
    }
    vector<pair<int,int>> validMoves()
    {
        vector<pair<int,int>> moves;
        for (int i = startCol+1 ; i <= endCol-1 ; i++)
            if (!colFull(i))
                moves.emplace_back(i, 0);

        return moves;
    }
    void makeMove(int player, int col)
    {
        if (this->isTerminalState())
            return;
        if (col < startCol + 1 || col > endCol + 1)
            return;
        for (int i = endRow - 1; i >= startRow ; i--)
            if (board[i][col].state == Chip::BACKGROUND)
            {
                board[i][col].state = (Chip::State) player;
                break;
            }
    }
    bool checkWin(int player)
    {
        for (int i = endRow - 1 ; i >= startRow ; i--)
            for (int j = startCol + 1 ; j <= endCol + 1 ; j++)
            {
                // check row
                if (board[i][j].state == player)
                    if (board[i-1][j].state == player)
                        if (board[i-2][j].state == player)
                            if (board[i-3][j].state == player)
                                return true;

                // check column
                if (board[i][j].state == player)
                    if (board[i][j+1].state == player)
                        if (board[i][j+2].state == player)
                            if (board[i][j+3].state == player)
                                return true;

                // check right diagonal
                if (board[i][j].state == player)
                    if (board[i-1][j+1].state == player)
                        if (board[i-2][j+2].state == player)
                            if (board[i-3][j+3].state == player)
                                return true;

                // check left diagonal
                if (board[i][j].state == player)
                    if (board[i-1][j-1].state == player)
                        if (board[i-2][j-2].state == player)
                            if (board[i-3][j-3].state == player)
                                return true;
            }
        return false;
    }
    bool isTerminalState()
    {
        return (checkWin(-1) || checkWin(1) || validMoves().empty());
    }

    int score(vector<Chip> chips, int player)
    {
        int score = 0;
        int pChips = 0;
        int oChips = 0;
        int empty = 0;

        for (auto chip : chips)
        {
            if (chip.state == player)
                pChips++;
            else if (chip.state == -player)
                oChips++;
            else
                empty++;
        }

        if (pChips == 4)
            score += 1001;
        else if (pChips == 3 && empty == 1)
            score += 100;
        else if (pChips == 2 && empty == 2)
            score += 10;
        else if (oChips == 2 && empty == 2)
            score -= 20;
        else if (oChips == 3 && empty == 1)
            score -= 1000;

        return score;
    }
    int scoreBoard(Board b, int player)
    {
        // score center column
        int sum = 0;
        for (int i = endRow - 1 ; i >= startRow ; i--)
            if (b.board[i][8].state == player)
                sum += 3;

        // score horizontal
        for (int i = endRow - 1 ; i >= startRow ; i--)
            for (int j = startCol + 1 ; j <= endCol - 1 ; j++)
            {
                vector<Chip> chips;
                for (int k = 0 ; k < 4 ; k++)
                    chips.push_back(b.board[i][j+k]);

                sum += score(chips, player);
            }

        // score vertical
        for (int i = endRow - 1 ; i >= startRow ; i--)
            for (int j = startCol + 1 ; j <= endCol - 1 ; j++)
            {
                vector<Chip> chips;
                for (int k = 0 ; k < 4 ; k++)
                    chips.push_back(b.board[i-k][j]);

                sum += score(chips, player);
            }

        // score diagonal
        for (int i = endRow - 1 ; i >= startRow ; i--)
            for (int j = startCol + 1 ; j <= endCol - 1 ; j++)
            {
                vector<Chip> chips;
                for (int k = 0 ; k < 4 ; k++)
                    chips.push_back(b.board[i-k][j+k]);

                sum += score(chips, player);
            }

        // score diagonal
        for (int i = endRow - 1 ; i >= startRow ; i--)
            for (int j = startCol + 1 ; j <= endCol - 1 ; j++)
            {
                vector<Chip> chips;
                for (int k = 0 ; k < 4 ; k++)
                    chips.push_back(b.board[i-k][j-k]);

                sum += score(chips, player);
            }

        return sum;
    }
    pair<int,int> miniMax(int player, Board _board, int depth, int alpha, int beta)
    {
        Board temp = Board();
        pair<int,int> bestMove = make_pair(-1,NULL);

        for (auto move : validMoves())
        {
            temp.copy(_board);
            temp.makeMove(player, move.first);

            if (temp.isTerminalState() || depth == 0)
                move.second = scoreBoard(temp, -1);

            else
                move.second = miniMax(-player, temp, depth - 1, alpha, beta).second;

            if ((bestMove.first == -1 || bestMove.second < move.second) && player == -1)
            {
                bestMove = make_pair(move.first, move.second);
                alpha = max(alpha, bestMove.second);
                if (alpha >= beta)
                    break;
            }
            else if ((bestMove.first == -1 || bestMove.second > move.second) && player == 1)
            {
                bestMove = make_pair(move.first, move.second);
                beta = min(beta, bestMove.second);
                if (alpha >= beta)
                    break;
            }
        }
        return bestMove;
    }
};