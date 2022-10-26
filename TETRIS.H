/* Simple Text-Mode TETRIS game by Vincebus Riveruptum, this
   was made just for learning purposes :) so be free to use it too
   made with Borland C/C++ 3.1, Nov 2019.

   Pieces :	   Char codes:

	O = 0		² = 178
	I = 1		± = 177
	S = 2  		° = 176
	Z = 3		Û = 219
	L = 4		Ü = 220
	J = 5		ß = 223
	T = 6		ÿ = 255
			  = 254
*/

#include "engine.h"

#define OPIECE 0
#define IPIECE 1
#define SPIECE 2
#define ZPIECE 3
#define LPIECE 4
#define JPIECE 5
#define TPIECE 6

/* Globals ================================================================*/

unsigned char currentPositionY;
unsigned char currentPositionX;
int score;
int numpieces;

unsigned char currentPiece[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
unsigned char currentPieceNumber;
unsigned char pieceIncoming;

/* ========Functions===================================================== */

void mainLoop();
void newGame();
void startTetris();
void clearBoard();

int getMatchingRow();
unsigned char getOneMatchingRow();

/* Game Rules Methods ================================================== */

int isTouchingDown();
int isTouchingRight();
int isTouchingLeft();

void copyPieceToBoard();
void rotateClockwise();
void setCurrentPiece();
void setMap();
void printPiece();
void drawBox();

void movePieceLeft();
void movePieceRight();

/* ====================================================================== */
/* Clears the tetris board*/

void clearBoard(){
	int i;

	for(i=0;i<YSIZ;i++){
		memset(tetris[i],0,XSIZ);
	}
}

/* Verify if the current piece is touching down with another piece or the
bottom in the board */
int isTouchingDown(){
	int i,j;

	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			if(currentPiece[i][j] != 0){
				if((currentPositionY + i) < YSIZ-1){
					if((i<4)){
						if(currentPiece[i + 1][j] == 0){
							if(tetris[currentPositionY + i + 1][currentPositionX + j] != 0){
								return 1;
							}
						}
					}else{
							if(tetris[currentPositionY + i + 1][currentPositionX + j] != 0){
								return 1;
							}
					}
				}else{
					return 1;
				}
			}
		}
	}
	return 0;

}

/* Verify if the piece is not touching another piece to the right or the board X limit*/

int isTouchingRight(){
	int i,j;

	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			if(currentPiece[i][j] != 0){
				if((currentPositionX + j) < XSIZ-1){
					if((j<4)){
						if(currentPiece[i][j+1] == 0){
							if(tetris[i+currentPositionY][currentPositionX + j + 1] != 0){
								return 1;
							}
						}
					}
				}else{
					return 1;
				}
			}
		}
	}
	return 0;
}

/* Verify if the piece is not touching another piece on the left or the position is over 0*/

int isTouchingLeft(){
	int i,j;

	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			if(currentPiece[i][j] != 0){
				if((currentPositionX + j) > 0){
					if((j>0)){
						if(currentPiece[i][j-1] == 0){
							if(tetris[i+currentPositionY][currentPositionX + j - 1] != 0){
								return 1;
							}
						}
					}else{
						if(tetris[i+currentPositionY][currentPositionX + j - 1] != 0){
								return 1;
						}

					}
				}else{
					return 1;
				}
			}
		}
	}
	return 0;
}

/*========================================================================*/

/* Prints the current piece into the screen on its current position */
void printPiece(){
	int i,j;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(currentPiece[i][j] != 0){
				if(i+currentPositionY > (YSIZ / 2)){
					gotoxy(((currentPositionX*2)+(2*j)+2), (currentPositionY+i+1)-(YSIZ/2));
					putch(currentPiece[i][j]);
					putch(currentPiece[i][j]);
				}
			}
		}
	}
}

/* Copies the current piece on its position to the tetris board */
void copyPieceToBoard(){
	int i,j,x,y;
	for(y=currentPositionY,i=0;y<YSIZ,i<4;i++,y++){
		for(x=currentPositionX,j=0;x<10,j<4;j++,x++){
			/* Copy piece but not overwriting the rest */
			if(currentPiece[i][j] !=0){
				tetris[y][x] = currentPiece[i][j];
			}
		}
	}
}
/* Rotates the incoming piece */
void rotateClockwise(){
	int i,j,x;
	unsigned char buffer[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

	if(!isTouchingLeft()){
		if(!isTouchingRight()){
			if(!isTouchingDown()){

				for(i=0,x=3;i<4,x>=0;i++,x--){
					for(j=0;j<4;j++){
						buffer[j][x] = currentPiece[i][j];
					}
				}

				adjust(buffer);

				memcpy(currentPiece[0], buffer[0],4);
				memcpy(currentPiece[1], buffer[1],4);
				memcpy(currentPiece[2], buffer[2],4);
				memcpy(currentPiece[3], buffer[3],4);


			}
		}
	}

}

/* Sets the incoming piece */
void setCurrentPiece(){
	int pnum = (rand() % 7);

	currentPositionY = 15;
	currentPositionX = rand() % 7;

	currentPieceNumber = pnum;
	/* ²²
	   ²²
	*/
	if(pnum == OPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][0] = 'Û';
		currentPiece[0][1] = 'Û';
		currentPiece[1][0] = 'Û';
		currentPiece[1][1] = 'Û';
	}

	/*
	    ²
	    ²
	    ²
	    ²
	*/
	if(pnum == IPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][0] = '±';
		currentPiece[1][0] = '±';
		currentPiece[2][0] = '±';
	}

	/*
	    ²²
	   ²²
	*/
	if(pnum == SPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][1] = '°';
		currentPiece[0][2] = '°';
		currentPiece[1][0] = '°';
		currentPiece[1][1] = '°';
	}

	/*
	   ²²
	    ²²
	*/

	if(pnum == ZPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][0] = '²';
		currentPiece[0][1] = '²';
		currentPiece[1][1] = '²';
		currentPiece[1][2] = '²';
	}

	/*
	    ²
	    ²
	    ²²
	*/
	if(pnum == LPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][0] = '±';
		currentPiece[1][0] = '±';
		currentPiece[2][0] = '±';
		currentPiece[2][1] = '±';
	}
	/*
	   ²
	   ²
	  ²²
	*/
	if(pnum == JPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][1] = 'Û';
		currentPiece[1][1] = 'Û';
		currentPiece[2][1] = 'Û';
		currentPiece[2][0] = 'Û';
	}
	/*
	 ²²²
	  ²
	*/

	if(pnum == TPIECE){
		memset(currentPiece[0],0,4);
		memset(currentPiece[1],0,4);
		memset(currentPiece[2],0,4);
		memset(currentPiece[3],0,4);
		currentPiece[0][0] = '°';
		currentPiece[0][1] = '°';
		currentPiece[0][2] = '°';
		currentPiece[1][1] = '°';
	}
}
/* Key binds */
void setMap(){

	switch(key){
		case KEY_SPACE:
			/* Flip the piece */
			rotateClockwise();
			key=0;
			break;
		case KEY_1:
			/* Resets the game*/
			newGame();
			key=0;
			break;
		case KEY_A:
			/* Moves piece to left */
			movePieceLeft();
			break;
		case KEY_D:
			/* Moves piece to the right*/
			movePieceRight();
			break;
		case KEY_S:
			/* Moves piece to the bottom */
			//key = 0;
			break;
	}
}

/* Piece Movement ========================================================*/

void movePieceRight(){
	if(!isTouchingRight()){
		currentPositionX++;
	}
}

void movePieceLeft(){
	if(!isTouchingLeft()){
		currentPositionX--;
	}
}

/**=======================================================================*/
/* Find a matching row in the board and sums score */
int getMatchingRow(){
   unsigned char ymove;

   if((ymove = getOneMatchingRow()) != 0){
	/* We get the Y position in the board to clear and move the
	rest of blocks below this position */
	movedown(ymove);
   }else{
	return 0;
   }

}
/* Finds the first matching row found in the board*/
unsigned char getOneMatchingRow(){
	unsigned char ymove;
	int i,j, nextcount;

	for(i=0;i<YSIZ;i++){
		/* the loops follows if there is no 0 in the row */
		nextcount = 0;
		for(j=0 ; (j<XSIZ) && (tetris[i][j] !=0) ; j++){
			nextcount++;

			if(nextcount==(XSIZ)){
				return i;
			}
		}
	}
	return 0;

}
/**======================================================================*/

void mainLoop(){
	/* Drawing and main game loop */
	clrscr();
	drawBox();
	do{
		/* Permanent stuff on screen */
		clearPartOfScreen(2,2,(XSIZ*2)-2,(YSIZ/2));

//		drawBox();
		printBoard();
		printPiece();

		do{
			if(kbhit()){
				key = getch();
				/* Defines the action */
				setMap();
			}

		}while(!(getMatchingRow()) && (!secondElapsed()));

		if(!isTouchingDown()){
			currentPositionY++;
		}else{
			/* The piece gets fixed on the position*/
			copyPieceToBoard();
			setCurrentPiece();
		}

	}while(key!=KEY_ESC);
}

void newGame(){
	int i;

	/* Tetris board cleaning */

	currentPositionX = 0;
	currentPositionY = 0;
	score = 0;
	numpieces = 0;
	pieceIncoming = rand() % 6;

	clearBoard();
	setCurrentPiece();

	mainLoop();

}

void startTetris(){

	newGame();


}