/* ENGINE.H

º = 186
» = 187
¼ = 188
É = 200
È = 201
Í = 205

/*=============================================================== */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>

#include "keys.h"
#include "sfx.h"

#define YSIZ 40
#define XSIZ 10

void clearPartOfScreen(unsigned char x1, unsigned char y1, unsigned char x2,unsigned char y2);
void box(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char type);
int secondElapsed();
void adjust(unsigned char [4][4]);
void moveleft(unsigned char [4][4]);
void movedown(unsigned char ymove);
void printBoard();
void drawBox();

unsigned char key;
unsigned long secondbuffer;
unsigned char tetris[YSIZ][XSIZ];

/* ====================================================================== */

/* Draws the tetris board outer box */
void drawBox(){
	box(0,0,2+(XSIZ*2),(YSIZ/2)+2,3);
}

void box(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char type){
	int i, j;
	char set[4][6] = {{218,191,192,217,179,196},
			{213,184,212,190,179,205},
			{214,183,211,189,186,196},
			{201,187,200,188,186,205}};
	x++; y++;
	for(j=0;j<y2;j++){
		for(i=0;i<x2;i++){
			gotoxy(x+i,y+j);
			if((i==0) && (j==0)){
				putch(set[type][0]);
			}else{
				if((i==x2-1) && (j==0)){
					putch(set[type][1]);
				}else{
					if((i==0) && (j==y2-1)){
						putch(set[type][2]);
					}else{
						if((i==x2-1) && (j==y2-1)){
							putch(set[type][3]);
						}else{
							if(((i==0) || (i==x2-1)) && ((j>0) && (j<y2))){
								putch(set[type][4]);
							}else{
								if(((j==0) || (j==y2-1)) && ((x>0) && (x<x2))){
									putch(set[type][5]);
								}
							}
						}
					}

				}
			}
		}

	}
}


/* Returns true if a second has elapsed */
int secondElapsed(){
	unsigned long sbuffer=0;
	unsigned int lotick;

	if(!key){

		asm{
			mov ah,0
			int 0x1a
			mov lotick,dx
		}

		sbuffer = lotick;

		if((sbuffer - secondbuffer) >= 19){
			secondbuffer = sbuffer;
			return 1;
		}
		return 0;
	}else{
		key=0;
		return 1;
	}
}

/* Adjust the piece to start on 0,0 */
void adjust(unsigned char buffer[4][4]){
	int jblock=0,j;

	do{
		if(jblock == 4){
			moveleft(buffer);
		}

		for(j=0,jblock=0;j<4;j++){
			if(buffer[j][0] == 0){
				jblock++;
			}
		}
	}while(jblock==4);
}

/* Moves to the left the piece internally by one step */
void moveleft(unsigned char buffer[4][4]){

	unsigned char buffer2[4][4];
	int i,j;

	memset(buffer2[0],0,4);
	memset(buffer2[1],0,4);
	memset(buffer2[2],0,4);
	memset(buffer2[3],0,4);


	for(i=0;i<4;i++){
		for(j=1;j<4;j++){
			buffer2[i][j-1] = buffer[i][j];
		}
	}

	memcpy(buffer[0],buffer2[0],4);
	memcpy(buffer[1],buffer2[1],4);
	memcpy(buffer[2],buffer2[2],4);
	memcpy(buffer[3],buffer2[3],4);
}


/* Moves down the piece on the board by one step */
void movedown(unsigned char ymove){

	int i;

	for(i=ymove;i>=0;i--){
		memcpy(tetris[i],tetris[i-1], XSIZ);
	}
}

void clearPartOfScreen(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2){

	asm{
		mov al,0
		mov bh,0
		mov ch,y1
		mov cl,x1
		mov dh,y2
		mov dl,x2
		mov ah,6h
		int 10h
	};
}

/* Draws the tetris board */
void printBoard(){
	int i,j;

	for(i=0;i<YSIZ;i++){
		for(j=0;j<XSIZ;j++){

			if(i >= (YSIZ / 2)){
			/* This probably it'll need masking too*/
				gotoxy((2*j)+2, (i+2)-(YSIZ/2));
				putch(tetris[i][j]);
				putch(tetris[i][j]);
			}

		}
	}

}
