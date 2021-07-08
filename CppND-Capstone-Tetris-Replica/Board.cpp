#include "Board.h"

// Defining the methods from Board.h
// Init 

Board::Board(Pieces* pPieces, int pScreenHeight) {

	// Get the screen height
	mScreenHeight = pScreenHeight;

	// Get the pointer to the pieces class
	mPieces = pPieces;

	// Init the board with free positions
	InitBoard();
}

// Initialize the board blocks with free positions

void Board::InitBoard() {
	for (int i = 0; i < BOARD_WIDTH; i++)
		for (int j = 0; j < BOARD_HEIGHT; j++)
			mBoard[i][j] = POS_FREE;
}

/*
Store a piece in the board by filling the blocks

Parameters :
1. pX :			Horizontal positions in blocks
2. pY :			Vertical positions in blocks
3. pPiece :		Piece to draw
4. pRotation :	1 to 4 of the possible rotations
*/

void Board::StorePiece(int pX, int pY, int pPiece, int pRotation) {
	// Store each block of the piece in the board
	for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++) {
		for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++) {
			// Store only the blocks of the pieces that are not holes
			if (mPieces->GetBlockType(pPiece, pRotation, j2, i2) != 0)
				mBoard[i1][j1] = POS_FILLED;
		}
	}
}

/*
Check if the game is over if a piece has acheived the upper position
Return true or false
*/

bool Board::IsGameOver() {
	// If the first line of the board has blocks then game over
	for (int i = 0; i < BOARD_WIDTH; i++) {
		if (mBoard[i][0] == POS_FILLED) return true;
	}

	return false;
}

/*
Delete a line of the board by moving all above lines down

Parameters :
1. pY : Vertical position in blocks of the line to delete
*/

void Board::DeleteLine(int pY) {

	// Moves all the upper lines one row down
	for (int j = pY; j > 0; j--) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			mBoard[i][j] = mBoard[i][j - 1];
		}
	}
}

/*
Delete all the lines that should be removed
*/

void Board::DeletePossibleLines() {
	for (int j = 0; j < BOARD_HEIGHT; j++) {
		int i = 0;
		while (i < BOARD_WIDTH) {
			if (mBoard[i][j] != POS_FILLED) break;
			i++;
		}
		if (i == BOARD_WIDTH) DeleteLine(j);
	}
}

/*
Returns 1 if this block of the board is empty,
0 if it is filled.

Parameters :
1. pX : Horizontal position of the blocks
2. pY : Vertical position of the blocks
*/

bool Board::IsBlockFree(int pX, int pY) {
	if (mBoard[pX][pY] == POS_FREE) return true;
	else return false;
}

/*
Returns the horizontal position (in pixels) of the block given

Parameters ;
1. pPos : Horizontal position of the block in the board
*/

int Board::GetXPositionInPixels(int pPos) {
	return ((BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2))) + (pPos * BLOCK_SIZE));
}


/*
Returns the vertical position (in pixels) of the block given

Parameters ;
1. pPos : Vertical position of the block in the board
*/

int Board::GetYPositionInPixels(int pPos) {
	return ((mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT)) + (pPos * BLOCK_SIZE));
}

/*
Check if the piece can be stored at this position without any collision
Returns true if the movement is possible, false if it is not possible

Parameters :
1. pX :			Horizontal position in blocks
2. pY :			Vertical position in blocks
3. pPiece :		Piece to draw
4. pRotation :	1 of the 4 possible rotations
*/

bool Board::IsMovementPossible(int pX, int pY, int pPiece, int pRotation) {

	// check collision with pieces already stored in the board or the board limits
	// This is just to check the 5 X 5 blocks of a piece with the appropriate area in the board
	for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++) {
		for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++) {
			// check if the piece is outside the limits of the board
			if (i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1) {
				if (mPieces->GetBlockType(pPiece, pRotation, j2, i2) != 0)
					return 0;
			}
			// Check if the piece have collisioned with a block already stored in the map
			if (j1 >= 0) {
				if ((mPieces->GetBlockType(pPiece, pRotation, j2, i2) != 0) &&
					(!IsBlockFree(i1, j1)))
					return false;
			}
		}
	}

	// No collision
	return true;
}