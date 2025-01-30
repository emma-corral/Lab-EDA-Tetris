#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "utils.h"
#include "pieces.h"

/// ToDo in LAB 1 - define macros MAX_ROWS and MAX_COLUMNS
#define MAX_ROWS 15         //define dimentions of the board (15x12)
#define MAX_COLUMNS 12

typedef struct{
    // ToDo in LAB 1 - define data structure
    Piece p;    
    int at_row; 
    int at_col; 
} PieceInfo;          // Define the data structure of the position of the piece

typedef struct{
    // ToDo in LAB 1 - define data structure
    char board[MAX_ROWS][MAX_COLUMNS]; 
    int score; 
    PieceInfo current_piece;
} GameState;          // Define the data structure of the position of all the pieces in the board and the score


/// Implemented functions
void print_game_state(GameState *gs);
void print_options();
bool is_valid_option(int option);
void print_line();
void print_board(GameState *game_state);
void get_new_random_piece(GameState *game_state);
void block_current_piece(GameState *game_state);
bool is_collision(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info);

/**** LAB 1 - functions to program (start here) ****/
void init_game_state(GameState *game_state);
bool is_terminal(char board[MAX_ROWS][MAX_COLUMNS]); // True if contains X values in TOP-4 rows
void move(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info, int option);
void rotate(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info, int option);
/**** LAB 1 - functions to program (end here) ****/

/// Implemented functions
void run_turn(GameState *game_state, int option);


#endif

