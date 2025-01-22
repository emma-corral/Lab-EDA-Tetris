#include "pieces.h"

void init_piece(Piece *p){
	// ToDo in LAB 1
}

void print_piece(Piece p){
    printf("Name: %c; Size: %dx%d\n", p.name, p.rows, p.cols);
    for(int i=0; i<PIECE_SIZE; ++i){
        for(int j=0; j<PIECE_SIZE; ++j){
            printf("%c", p.board[i][j]);
        }
        printf("\n");
    }
}

void rotate_clockwise(Piece *p){
    // ToDo in LAB 1
}

void rotate_counter_clockwise(Piece *p){
    // ToDo in LAB 1
}

Piece make_O(){
	Piece piece;
	init_piece(&piece);
	piece.name= 'O'; piece.rows = 2; piece.cols= 2;
	piece.boarde[0][0] = '#';
	piece.board[0][1]= '#';
	piece.board[1][0] ='#';
	piece.board[1][1] = '#';
	return piece;
}

Piece make_I(){
    // ToDo in LAB 1
}


Piece make_S(){
	Piece piece;
	init_piece(&piece);
	piece.name = 'S'; piece.rows = 2; piece.cols= 3;
	piece.board[0][1]= '#';
	piece.board[0][2] = '#';
	piece.board[1][0] = '#';
	piece.board[1][1] = '#';
	return piece
}


Piece make_Z(){
    // ToDo in LAB 1
}

Piece make_L(){
    // ToDo in LAB 1
}

Piece make_J(){
    // ToDo in LAB 1
}

Piece make_T(){
    	Piecce piece;
	init_piece(&piece);
	piece.name = 'T'; piece.rows = 2; piece.cols= 3;
	piece.board[0][0]= '#';
	piece.board[0][1]= '#';
	piece.board[0][2]= '#';
	piece.board[1][1]= '#';
	return piece
}

