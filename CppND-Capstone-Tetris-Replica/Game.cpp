#include <windows.h>
#include "Game.h"

// Constructor
Game::Game(Board* pBoard, Pieces* pPieces, IO* pIO, int pScreenHeight):mBoard(pBoard),mPieces(pPieces)
, mIO(pIO), mScreenHeight(pScreenHeight) {
	InitGame();
}

/*
Get a random integer between two integers

Parameters:
1. pA : First number
2. pB : Second number
*/

int Game::GetRand(int pA, int pB) {
	return rand() % (pB - pA + 1) + pA;
}

// Initial parameters of the game

void Game::InitGame() {

	// Init random numbers
	srand((unsigned int)time(NULL));

	// First piece
	mPiece = GetRand(0, 6);
	mRotation = GetRand(0, 3);
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	// Next Piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
	mNextPosX = BOARD_WIDTH + 5;
	mNextPosY = 5;
}

// Create a random piece

void Game::CreateNewPiece() {
	// The new piece
	mPiece = mNextPiece;
	mRotation = mNextRotation;
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	// Random next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);

}

/*
Draw piece

Parameters :
1. pX :			Horizontal positions in blocks
2. pY :			Vertical positions in blocks
3. pPiece :		Piece to draw
4. pRotation :	1 to 4 of the possible rotations
*/

void Game::DrawPiece(int pX, int pY, int pPiece, int pRotation) {
	
	color mColor;		// color of the block

	// Obtain the position in pixel in the screen of the block we want to draw
	int mPixelsX = mBoard->GetXPositionInPixels(pX);
	int mPixelsY = mBoard->GetYPositionInPixels(pY);

	// Travel the matrix of blocks of the piece and draw the blocks that are filled
	for (int i = 0; i < PIECE_BLOCKS; i++) {
		for (int j = 0; j < PIECE_BLOCKS; j++) {

			// Get the type of the block and draw it with the correct color
			switch (mPieces->GetBlockType(pPiece, pRotation, j, i)) {
			case 1: mColor = GREEN; break;		// For each block of the piece except the pivot
			case 2: mColor = BLUE; break;		// For the pivot
			}

			if (mPieces->GetBlockType(pPiece, pRotation, j, i) != 0)
				mIO->DrawRectangle(mPixelsX + i * BLOCK_SIZE,
								   mPixelsY + j * BLOCK_SIZE,
								   (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
								   (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
								    mColor);
		}
	}
}

/*
Draw Board

Draw the two lines delimiting the board
*/

void Game::DrawBoard() {

	// Calculate the limits of the board in pixels
	int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
	int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
	int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);

	// Check that the vertical margin is not to small
	// assert (mY > MIN_VERTICAL_MARGIN)

	// Rectangles that dilimit the board
	mIO->DrawRectangle(mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLUE);
	mIO->DrawRectangle(mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLUE);

	// Drawing the blocks that are already stored in the board
	mX1 += 1;
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {

			// Check if the block is filled, if so, draw it
			if(!mBoard->IsBlockFree(i,j))
				mIO->DrawRectangle(mX1 + i * BLOCK_SIZE,
								   mX1 + j * BLOCK_SIZE,
					               (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
					               (mX1 + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
					               RED);
		}
	}
}

/*
Draw Scene

Draw all the objects of the scene
*/

void Game::DrawScene() {

	DrawBoard();													// Draw the delimiter lines and blocks stored on the board
	DrawPiece(mPosX, mPosY, mPiece, mRotation);						// Draw the playing piece
	DrawPiece(mNextPosX, mNextPosY, mNextPiece, mNextRotation);		// Draw the next piece
}