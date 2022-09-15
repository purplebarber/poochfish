#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

class Piece {
    public:
        static const int none = 0;
        static const int king = 1;
        static const int queen = 2;
        static const int bishop = 3;
        static const int knight = 4;
        static const int rook = 5;
        static const int pawn = 6;

        static const int white = 8;
        static const int black = 16;

        static int getPieceValue(char pieceLetter){
            switch(pieceLetter){
                case 'k':
                    return king;

                case 'q':
                    return queen;

                case 'b':
                    return bishop;

                case 'n':
                    return knight;

                case 'r':
                    return rook;

                case 'p':
                    return pawn;

                case 'w':
                    return white;

                case 'l':
                    return black;

            default:
                return 0;
            }
        }

        static bool isWhite(int pieceValue){
            if(pieceValue < black){return true;} else {return false;}
        }

        static char getPieceNotation(int pieceValue){
            char pieceNotation = ' ';

            if(!isWhite(pieceValue)){
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
            if(isWhite(pieceValue)){
                return toupper(pieceNotation);
            } else {return pieceNotation;}
        }

};

static const std::vector<int> edgesNorth = {56, 57, 58, 59, 60, 61, 62, 63};
static const std::vector<int> edgesEast = {7, 15, 23, 31, 39, 47, 55, 63};
static const std::vector<int> edgesSouth = {0, 1, 2, 3, 4, 5, 6, 7};
static const std::vector<int> edgesWest = {0, 8, 16, 24, 32, 40, 48, 56};

class Moves{
    private:
    static const int north = 8, south = -8, east = 1, west = -1, ne = 9, se = -7, sw = -9, nw = 7;
    constexpr static const int directionValues[8] = {north, east, south, west, ne, se, sw, nw};

    public:
    static std::vector<int> squaresToEdgeOfBoard(int pieceLocation){
        int squaresToNorth = 0;
        int squaresToEast = 0;
        int squaresToSouth = 0;
        int squaresToWest = 0;
        int squareIterator = pieceLocation;

        // North
        while(!(std::find(edgesNorth.begin(), edgesNorth.end(), squareIterator) != edgesNorth.end())){
            squareIterator += north;
            squaresToNorth ++;
        } squareIterator = pieceLocation;

        // South
        while(!(std::find(edgesSouth.begin(), edgesSouth.end(), squareIterator) != edgesSouth.end())){
            squareIterator += south;
            squaresToSouth ++;
        } squareIterator = pieceLocation;

        // East
        while(!(std::find(edgesEast.begin(), edgesEast.end(), squareIterator) != edgesEast.end())){
            squareIterator += east;
            squaresToEast ++;
        } squareIterator = pieceLocation;

        // West
        while(!(std::find(edgesWest.begin(), edgesWest.end(), squareIterator) != edgesWest.end())){
            squareIterator += west;
            squaresToWest ++;
        }

        // Diagonals
        int squaresNorthEast = std::min(squaresToNorth, squaresToEast);
        int squaresSouthEast = std::min(squaresToSouth, squaresToEast);
        int squaresSouthWest = std::min(squaresToSouth, squaresToWest);
        int squaresNorthWest = std::min(squaresToNorth, squaresToWest);

        std::vector<int> movesToEdgesAllDirections = {squaresToNorth, squaresToEast, squaresToSouth, squaresToWest,
                                                      squaresNorthEast, squaresSouthEast, squaresSouthWest, squaresNorthWest};

        return movesToEdgesAllDirections;
    }

    static bool isEnemyPiece(bool ourWeWhite, int enemyValue) {
        if ((ourWeWhite && enemyValue < Piece::black) || (!ourWeWhite && enemyValue > Piece::black)) {
            return false;
        } else { return true; }
    }

    static std::vector<int> getPseudoPieceMoves(int pieceLocation, std::vector<int> &boardVector){
        // north = +8, south = -8, east = +1, west = -1, ne = +9, se = -7, sw = -9, nw = +7
        // sliding pieces: king, queen, bishop, rook
        int pieceValue = boardVector.at(pieceLocation);
        bool isWhite = pieceValue < Piece::black;
        if(isWhite){pieceValue -= Piece::white;} else {pieceValue -= Piece::black;}

        std::vector<int> squaresToEdge = squaresToEdgeOfBoard(pieceLocation);
        std::vector<int> possibleSquares = {};
        int testingSquare;
        int testingSquareValue;

        if(pieceValue == Piece::bishop || pieceValue == Piece::queen || pieceValue == Piece::king){
            for(int i = 4; i < 8; i++){
                for(int j = 0; j < squaresToEdge.at(i); j++){
                    testingSquare = pieceLocation + (j + 1) * directionValues[i];
                    testingSquareValue = boardVector.at(testingSquare);

                    if(testingSquareValue != 0){
                        if(isEnemyPiece(isWhite, testingSquareValue)){
                            possibleSquares.push_back(testingSquare); j += 16;
                        } else {j += 16;}
                    } else {
                        possibleSquares.push_back(testingSquare);
                    }

                    if(pieceValue == Piece::king){j += 16;}
                }
            }
        }

        if(pieceValue == Piece::rook || pieceValue == Piece::queen || pieceValue == Piece::king){
            for(int i = 0; i < 4; i++){
                for(int j = 0; j < squaresToEdge.at(i); j++){
                    testingSquare = pieceLocation + (j + 1) * directionValues[i];
                    testingSquareValue = boardVector.at(testingSquare);

                    if(testingSquareValue != 0){
                        if(isEnemyPiece(isWhite, testingSquareValue)){
                            possibleSquares.push_back(testingSquare); j += 16;
                        } else {j += 16;}
                    } else {
                        possibleSquares.push_back(testingSquare);
                    }

                    if(pieceValue == Piece::king){j += 16;}
                }
            }
        }

        if(pieceValue == Piece::knight){
            for(int i = 0; i < 4; i++){
                if(squaresToEdge.at(i) >= 2){
                    if(i == 0 || i == 2){
                        if(squaresToEdge.at(1) > 0){
                            testingSquare = pieceLocation + east + (2 * directionValues[i]);
                            testingSquareValue = boardVector.at(testingSquare);
                            if(testingSquareValue == 0 || isEnemyPiece(isWhite, testingSquareValue)){
                                possibleSquares.push_back(testingSquare);
                            }
                        }
                        if(squaresToEdge.at(3) > 0){
                            testingSquare = pieceLocation + west + 2 * directionValues[i];
                            testingSquareValue = boardVector.at(testingSquare);
                            if(testingSquareValue == 0 || isEnemyPiece(isWhite, testingSquareValue)){
                                possibleSquares.push_back(testingSquare);
                            }
                        }
                    } else {
                        if(squaresToEdge.at(0) > 0){
                            testingSquare = pieceLocation + north + 2 * directionValues[i];
                            testingSquareValue = boardVector.at(testingSquare);
                            if(testingSquareValue == 0 || isEnemyPiece(isWhite, testingSquareValue)){
                                possibleSquares.push_back(testingSquare);
                            }
                        }

                        if(squaresToEdge.at(2) > 0){
                            testingSquare = pieceLocation + south + 2 * directionValues[i];
                            testingSquareValue = boardVector.at(testingSquare);
                            if(testingSquareValue == 0 || isEnemyPiece(isWhite, testingSquareValue)){
                                possibleSquares.push_back(testingSquare);
                            }
                        }
                    }
                }
            }
        }

        if(pieceValue == Piece::pawn){
            bool unMovedPawn = false;
            if((pieceLocation >= 8 && pieceLocation <= 15) || (pieceLocation >= 48 && pieceLocation <=55)){
                unMovedPawn = true;
            }
            if(isWhite && squaresToEdge.at(0) > 0){
                testingSquare = pieceLocation +  north;
                testingSquareValue = boardVector.at(testingSquare);
                if(testingSquareValue == 0){
                    possibleSquares.push_back(testingSquare);
                }

                if(unMovedPawn){
                    testingSquare = pieceLocation + 2 * north;
                    testingSquareValue = boardVector.at(testingSquare);
                    if(testingSquareValue == 0){
                        possibleSquares.push_back(testingSquare);
                    }
                }

                for(int i = 4; i < 8; i+=3){
                    if(squaresToEdge.at(i) >= 0){
                        testingSquare = pieceLocation + directionValues[i];
                        testingSquareValue = boardVector.at(testingSquare);
                        if(isEnemyPiece(isWhite, testingSquareValue)){
                            possibleSquares.push_back(testingSquare);
                        }
                    }
                }
            } else if(!isWhite && squaresToEdge.at(2) > 0){
                testingSquare = pieceLocation + south;
                testingSquareValue = boardVector.at(testingSquare);
                if(testingSquareValue == 0){
                    possibleSquares.push_back(testingSquare);
                }

                if(unMovedPawn){
                    testingSquare = pieceLocation + 2 * south;
                    testingSquareValue = boardVector.at(testingSquare);
                    if(testingSquareValue == 0){
                        possibleSquares.push_back(testingSquare);
                    }
                }

                for(int i = 5; i < 7; i++){
                    if(squaresToEdge.at(i) >= 0){
                        testingSquare = pieceLocation + directionValues[i];
                        testingSquareValue = boardVector.at(testingSquare);
                        if(isEnemyPiece(isWhite, testingSquareValue)){
                            possibleSquares.push_back(testingSquare);
                        }
                    }
                }
            }
        }
        return possibleSquares;
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

        int FENLength = FEN.length();
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
                    currentPieceValue += Piece::getPieceValue('w');

                } else {
                    currentPieceValue += Piece::getPieceValue('l');
                }
                currentPieceValue += Piece::getPieceValue(char(tolower(currentChar)));

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
        for(int rank = 7; rank >= 0; rank--){
            if(rank != 7){std::cout << "|";}

            std::cout << std::endl;
            std::cout << "_________________________________" << std::endl;

            for(int file = 0; file <= 7; file++){
                int squareValue = showValue(rank * 8 + file);

                if(squareValue == 0){
                    std::cout << "|   ";

                } else { std::cout << "| " << Piece::getPieceNotation(squareValue) << " ";}
            }
        }

        std::cout << "|" << std::endl;
        std::cout << "_________________________________" << std::endl;
    }

    void playChess() {
        std::string FEN;
        int pieceLocation;
        int squareToMoveTo;

        std::cout << "Now playing chess!" << std::endl << "Insert FEN: ";
        std::cin >> FEN;
        loadPositionFromFEN(FEN);


        for(int i = 0; i < 50; i++) {
            printBoard();
            std::cout << "Enter piece to move: " << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> pieceLocation;

            int pieceToMove = showValue(pieceLocation);

            std::cout << "Enter a square to move to: " << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> squareToMoveTo;

            std::cout << std::endl << "Gotten inputs: " << std::endl;
            std::vector<int> pseudoLegalMoves = {};
            std::cout << pieceLocation << " ---> " << squareToMoveTo << std::endl;
            pseudoLegalMoves = Moves::getPseudoPieceMoves(pieceLocation, square);

            if (std::find(pseudoLegalMoves.begin(), pseudoLegalMoves.end(), squareToMoveTo) != pseudoLegalMoves.end()) {
                updateSquare(squareToMoveTo, pieceToMove);
                updateSquare(pieceLocation, 0);
                std::cout << "Moved piece to square!" << std::endl;

            } else {
                std::cout << "That piece cannot move there." << std::endl;
            }
        }
    }

};

int main(){
    Board board1;
    board1.playChess();
    return 0;
}
