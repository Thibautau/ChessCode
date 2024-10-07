//
// Created by Peter on 02/10/2024.
//

#ifndef PIECE_H
#define PIECE_H

#define NO_ERROR            0x0000
#define WRONG_PARAMETER     0x0001
#define ERROR               0x0002

#include "Type.h"

class Piece {
private:
    TypePieces m_tpTypePiece;
    Color m_colColorPiece;
    std::vector<int> m_possibleMoves;

public:
    Piece(TypePieces type, Color color);
    TypePieces getTypePiece() const;
    Color getColor() const;
    Color getEnemyColor() const;
    char getColorAsChar() const;
    char getDisplayChar() const;
    static int getColumnOfRookAfterRock(int in_iColumn);
    TypeOfPieceAttack typeOfAttack() const;
    bool attackStraight() const;
    bool attackDiagonal() const;
    bool attackKnight() const;
    const std::vector<int>& getPossibleMoves() const;

    void movePiece(Piece**, int newPosition, int oldPosition = 0);
    static bool isValidPosition(int position);

    /**
     *  Fill the tabVector with 4 movements of the rook
     *  The difference between in_iIndicesStart and in_iIndicesEnd must be 4, either it returns WRONG_PARAMETER
     * @param out_tabvectRookDisplacement
     * @param in_iIndicesStart
     * @param in_iLengthToAdjust length max of the vector
     */
    static int getRookVectorWithAdjustableLength(Vector* out_tabvectRookDisplacement, int in_iIndicesStart, int in_iLengthToAdjust);

    static int getBishopVectorWithAdjustableLength(Vector* out_tabvectBishopDisplacement, int in_iIndicesStart, int in_iLengthToAdjust);

    static int getKnightVectorWithAdjustableLength(Vector* out_tabvectBishopDisplacement, int in_iIndicesStart, int in_iLengthToAdjust);
};

#endif //PIECE_H
