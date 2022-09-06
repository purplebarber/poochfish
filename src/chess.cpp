#include <iostream>
#include <algorithm>
#include <vector>

class Piece {
    private:
        static const int none = 0;
        static const int king = 1;
        static const int queen = 2;
        static const int bishop = 3;
        static const int knight = 4;
        static const int rook = 5;
        static const int pawn = 6;

        static const int white = 8;
        static const int black = 16;

    public:
        int getPieceValue(char pieceLetter){
            switch(pieceLetter){
                case 'k':
                    return king;
                    break;

                case 'q':
                    return queen;
                    break;

                case 'b':
                    return bishop;
                    break;

                case 'n':
                    return knight;
                    break;

                case 'r':
                    return rook;
                    break;

                case 'p':
                    return pawn;
                    break;

                case 'w':
                    return white;
                    break;

                case 'l':
                    return black;
                    break;

            default:
                return 0;
            }
        }
};


class Board {
    private:
        std::vector<int> square;

    public:
    Board(){
        square.resize(64, 0); 
    }

    int showValue(int squareID){
        return square[squareID];
    }

    void updateSquare(int squareToUpdate, int pieceValue){
        square[squareToUpdate] = pieceValue;
    }

    void loadPositionFromFEN(std::string FEN){
        // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        int file = 7;
        int rank = 7;
        char lastChar = 'z';
        Piece pieceValues;

        bool whiteToMove = false;
        bool whiteCanCastle = false;
        bool blackCanCastle = false;

        int FENLength = FEN.length();
        std::cout << FENLength << std::endl;
        std::cout << FEN << std::endl;
        int currentPositionInFEN = 0;

        for(int i = 0; i < FENLength; i++){
            char currentChar = FEN[i];
            int currentPieceValue = 0;

            if(currentChar == ' '){
                lastChar = ' ';
                currentPieceValue = 0;

            } else if(currentChar == '/'){
                file = 7;
                rank--;
            // elif above is a bit iffy, need to fix

            } else if(isdigit(currentChar)) {
                int digitToMinus = currentChar - '0';
                file -= digitToMinus;

            } else if(currentChar == 'w' && rank == 0 && file <= 0 && lastChar == ' '){
                currentPositionInFEN = i;
                whiteToMove = true;
                break;

            } else if(currentChar == 'b' && rank == 0 && file <= 0 && lastChar == ' '){
                currentPositionInFEN = i;
                whiteToMove = false;
                break;

            } else {
                if(std::isupper(currentChar)){
                    currentPieceValue += pieceValues.getPieceValue('w');

                } else {
                    currentPieceValue += pieceValues.getPieceValue('l');
                }
                currentPieceValue += pieceValues.getPieceValue(char(tolower(currentChar)));

                updateSquare(rank * 8 + file, currentPieceValue);
                file --;
            } 
        }
    }

    void printBoard(){
        std::cout << square[6] << std::endl;
        for(int i = 63; i >= 0; i--){
            if(i != 63 && (i +1) % 8 == 0){
                std::cout << std::endl;
            }
            std::cout << "|" << showValue(i);
        }
    }

};


int main(){
    Board board1;
    board1.loadPositionFromFEN("rnbq1rk1/2p1bppp/p2p1n2/1p2p3/4P3/1BP2N1P/PP1P1PP1/RNBQR1K1 w - - 1 10");
    board1.printBoard();
    return 0;
}
