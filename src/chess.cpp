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

        char getPieceNotation(int pieceValue){
            char pieceNotation = ' ';
            bool isWhite = true;

            if(pieceValue > black){
                isWhite = false;
                pieceValue -= black;

            } else {pieceValue -= white;}

            switch(pieceValue){
                case king:
                    pieceNotation = 'k';
                    break;

                case queen:
                    pieceNotation = 'q';
                    break;

                case bishop:
                    pieceNotation = 'b';
                    break;

                case knight:
                    pieceNotation = 'n';
                    break;

                case rook:
                    pieceNotation = 'r';
                    break;

                case pawn:
                    pieceNotation = 'p';
                    break;

                default:
                    pieceNotation = 'x';
                    break;
            }
            if(isWhite){
                return toupper(pieceNotation);

            } else {return pieceNotation;}
        }
};


class Board {
    private:
        std::vector<int> square;
        bool whiteToMove = false;
        bool whiteCanCastleKing = true;
        bool whiteCanCastleQueen = true;
        bool blackCanCastleKing = true;
        bool blackCanCastleQueen = true;


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
        int file = 0;
        int rank = 7;
        char lastChar = 'z';
        Piece pieceValues;

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
                file = 0;
                rank--;
            // elif above is a bit iffy, need to fix

            } else if(isdigit(currentChar)) {
                int digitToAdd = currentChar - '0';
                file += digitToAdd;

            } else if(currentChar == 'w' && rank == 0 && lastChar == ' '){
                currentPositionInFEN = i;
                whiteToMove = true;
                break;

            } else if(currentChar == 'b' && rank == 0 && lastChar == ' '){
                currentPositionInFEN = i + 1;
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
                file ++;
            } 
        }

        for(int i = currentPositionInFEN; i < FENLength; i++){
            char currentChar = FEN[i];
            
            if(currentChar == ' '){
                continue;
            } else if(currentChar == 'K'){
                whiteCanCastleKing = true;
            } else if(currentChar == 'Q'){
                whiteCanCastleQueen = true;
            } else if(currentChar == 'k'){
                blackCanCastleKing = true;
            } else if(currentChar == 'q'){
                blackCanCastleQueen = true;
            } else {
                currentPositionInFEN = i;
                break;
            }

        }

    }


    void printBoard(){
        Piece pieceValues;

        for(int rank = 7; rank >= 0; rank--){
            if(rank != 7){std::cout << "|";}

            std::cout << std::endl;
            std::cout << "_________________________________" << std::endl;

            for(int file = 0; file <= 7; file++){
                int squareValue = showValue(rank * 8 + file);

                if(squareValue == 0){
                    std::cout << "|   ";

                } else { std::cout << "| " << pieceValues.getPieceNotation(squareValue) << " ";}
            }
        }

        std::cout << "|" << std::endl;
        std::cout << "_________________________________" << std::endl;
    }

};


int main(){
    Board board1;
    std::string FEN;
    std::cout << "Insert FEN: ";
    std::cin >> FEN;
    board1.loadPositionFromFEN(FEN);
    board1.printBoard();
    return 0;
}
