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


class Moves{
    private:
    std::vector<int> edgesNorth = {56, 57, 58, 59, 60, 61, 62, 63};
    std::vector<int> edgesEast = {7, 15, 23, 31, 39, 47, 55, 63};
    std::vector<int> edgesSouth = {0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<int> edgesWest = {0, 8, 16, 24, 32, 40, 48, 56};

    public:
    std::vector<int> squaresToEdgeOfBoard(int pieceLocation){
        int squaresToNorth = 0;
        int squaresToEast = 0;
        int squaresToSouth = 0;
        int squaresToWest = 0;
        int squaresNorthEast = 0;
        int squaresSouthEast = 0;
        int squaresSouthWest = 0;
        int squaresNorthWest = 0;

        int squareIterator = pieceLocation;

        // North
        while(!std::binary_search(edgesNorth.begin(), edgesNorth.end(), squareIterator)){
            squareIterator += 8;
            squaresToNorth ++;
        } squareIterator = pieceLocation;

        // South
        while(!std::binary_search(edgesSouth.begin(), edgesSouth.end(), squareIterator)){
            squareIterator -= 8;
            squaresToSouth ++;
        } squareIterator = pieceLocation;

        // East
        while(!(std::find(edgesEast.begin(), edgesEast.end(), squareIterator) != edgesEast.end())){
            squareIterator ++;
            squaresToEast ++;
        } squareIterator = pieceLocation;

        // West
        while(!(std::find(edgesWest.begin(), edgesWest.end(), squareIterator) != edgesWest.end())){
            squareIterator --;
            squaresToWest ++;
        } squareIterator = pieceLocation;

        // Diagonals
        squaresNorthEast = std::min(squaresToNorth, squaresToEast);
        squaresSouthEast = std::min(squaresToSouth, squaresToEast);
        squaresSouthWest = std::min(squaresToSouth, squaresToWest);
        squaresNorthWest = std::min(squaresToNorth, squaresToWest);

        std::vector<int> movesToEdgesAllDirections = {squaresToNorth, squaresToEast, squaresToSouth, squaresToWest,
                                                    squaresNorthEast, squaresSouthEast, squaresSouthWest, squaresNorthWest};

        return movesToEdgesAllDirections;
    }


    void getSlidingMoves(int pieceLocation){
        // north = +8, south = -8, east = +1, west = -1, ne = +9, se = -7, sw = -9, nw = +7
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
        Moves moves;


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
