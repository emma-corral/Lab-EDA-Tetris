#include "game.h"

/**** LAB 1 - given functions ****/
void print_game_state(GameState *gs){
    printf("*** Game State ***\n");
    printf("Score=%d\n", gs->score);
    print_board(gs);
}

void print_options(){
    printf("Options:\n");
    printf("\t%d. Move left\n", MOVE_LEFT);
    printf("\t%d. Move right\n", MOVE_RIGHT);
    printf("\t%d. Rotate clockwise\n", ROTATE_CW);
    printf("\t%d. Rotate counter clockwise\n", ROTATE_CCW);
    printf("\t%d. None\n", NONE);
    printf("\t%d. Show best move\n", SHOW_BEST_MOVE);
    printf("\t%d. Quit game\n", QUIT_GAME);
}

bool is_valid_option(int option){
    return ((MOVE_LEFT <= option) && (option<=QUIT_GAME));
}


void print_line(){
    for(int c=-1; c<MAX_COLUMNS+1; ++c) 
    	printf("-");    
    printf("\n");
}

void print_board(GameState *game_state){
    Piece *piece = &game_state->current_piece.p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int current_row = game_state->current_piece.at_row;
    int current_col = game_state->current_piece.at_col;
    print_line();
    for(int r = 0; r < MAX_ROWS; ++r){
        if(r == 4) print_line();
        printf("|");
        for(int c=0; c < MAX_COLUMNS; ++c){
            if((game_state->board[r][c] == '.') &&
               (current_row <= r) && (r < current_row + p_row_size) && 
               (current_col <= c) && (c < current_col + p_col_size)){
                printf("%c", piece->board[r-current_row][c-current_col]);
            }
            else printf("%c", game_state->board[r][c]);
        }
        printf("|\n");
    }
    print_line();
    printf("\n");
}

void get_new_random_piece(GameState *game_state){
    // Random piece
    switch (rand()%MAX_NUM_PIECES){
    case 0:
        game_state->current_piece.p = make_I();
        break;    
    case 1:
        game_state->current_piece.p = make_J();
        break;
    case 2:
        game_state->current_piece.p = make_L();
        break;
    case 3:
        game_state->current_piece.p = make_O();
        break;
    case 4:
        game_state->current_piece.p = make_S();
        break;
    case 5:
        game_state->current_piece.p = make_T();
        break;
    case 6:
        game_state->current_piece.p = make_Z();
        break;
    default:
        break;
    }

    // Random orientation
    int rotations = rand()%3;
    for(int r = 0; r < rotations; ++r)
        rotate_clockwise(&(game_state->current_piece.p));

    // Random location
    game_state->current_piece.at_row = 4 - game_state->current_piece.p.rows;
    game_state->current_piece.at_col = rand() % (MAX_COLUMNS + 1 - game_state->current_piece.p.cols);
}

void block_current_piece(GameState *game_state){
    Piece *piece = &game_state->current_piece.p;
    int row = game_state->current_piece.at_row;
    int col = game_state->current_piece.at_col;
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if(piece->board[i][j] == '#')
                game_state->board[row+i][col+j] = 'X';
}

bool is_collision(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info){
    Piece *piece = &piece_info->p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int row = piece_info->at_row;
    int col = piece_info->at_col;

    if((row < 0) || (col < 0) || (row+p_row_size-1 >= MAX_ROWS) || (col+p_col_size-1 >= MAX_COLUMNS))
    	return true; // piece is out of the grid bounds
    
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if((piece->board[i][j] == '#') && (board[row+i][col+j] == 'X'))
            	return true; // piece collides with another structure in the board
    
    return false;
}

int remove_completed_lines(char board[MAX_ROWS][MAX_COLUMNS]){
    int lines = 0;
    bool completed_line;
    for(int r=4; r<MAX_ROWS; ++r){
        completed_line = true;
        for(int c=0; c<MAX_COLUMNS; ++c){
            if(board[r][c] != 'X'){
                completed_line = false; 
                break;
            }
        }
        if(completed_line){
            ++lines;
            // Move all rows above, once down
            for(int r2=r; r2>3; --r2){
                for(int c=0; c<MAX_COLUMNS; ++c){
                    board[r2][c] = board[r2-1][c];
                }
            }
        }
    }
    return lines;
}

/********************************************************/
/******* LAB 1 - functions to program (start here) ******/
/********************************************************/

void init_game_state(GameState *game_state){
    game_state->score = 0;
	for(int r=0; r<MAX_ROWS; ++r)
		for(int c=0; c<MAX_COLUMNS; ++c)
			game_state->board[r][c] = '.';
    get_new_random_piece(game_state);        
    // to do in lab 1        
}



bool is_terminal(char board[MAX_ROWS][MAX_COLUMNS]){
    for(int r = 0; r < 4; ++r) {  // Check only the top 4 rows
        for(int c = 0; c < MAX_COLUMNS; ++c) {
            if(board[r][c] == '#'){ 
                return true; // Game over
            }    
        }
    }
    
    // ToDo in LAB 1
    return false;  // Game can continue
}


void move_piece(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info, int option){
    int dir = 0;
    if (option == MOVE_LEFT) dir = -1;
    else if (option == MOVE_RIGHT) dir = 1;
    else { printf("[ERROR] Invalid move option %d.\n", option); return; }

    // Check if there are no collisions...
    piece_info->at_col += dir;
    if (is_collision(board, piece_info)) {
        piece_info->at_col -= dir;
        return; // Exit if the piece cannot move left/right
    }
     // Move the piece down by one row
    piece_info->at_row++;

    // Check for collisions after moving down
    if (is_collision(board, piece_info)) {
        // Revert the downward movement if there is a collision
        piece_info->at_row--;
    }
    // ToDo in LAB 1
}

void rotate_piece(char board[MAX_ROWS][MAX_COLUMNS], PieceInfo *piece_info, int option){
    if(option == ROTATE_CW) rotate_clockwise(&(piece_info->p));
    else if(option == ROTATE_CCW) rotate_counter_clockwise(&(piece_info->p));
    else{ printf("[ERROR] Invalid rotation %d.\n", option); }
    //Check for collisions... (if there's a collision, move back to its original position)
    if (is_collision(board, piece_info)) {
        // Move back to its original position if there is a collision
        if (option == ROTATE_CW) rotate_counter_clockwise(&(piece_info->p));
        else if (option == ROTATE_CCW) rotate_clockwise(&(piece_info->p));
    }
    // ToDo in LAB 1
}
/********************************************************/
/******* LAB 1 - functions to program (end here) ********/
/********************************************************/





void run_turn(GameState *game_state, int option){
	PieceInfo *p_inf = &(game_state->current_piece);
	if(option == MOVE_LEFT || option == MOVE_RIGHT) 
		move_piece(game_state->board, p_inf, option);
	else if(option == ROTATE_CW || option == ROTATE_CCW)
		rotate_piece(game_state->board, p_inf, option);
    else if(option == NONE){} // do nothing
    else{ printf("[ERROR] Invalid option %d.\n", option); exit(-3); }

	// Move down if possible, otherwise block the piece and remove
    // the completed lines, aggregating them to the current score.
    // If it is not in a terminal state, add a new random piece to the board.
	p_inf->at_row++;
	if(is_collision(game_state->board, p_inf)){
		p_inf->at_row--;
		block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state->board);
        if(!is_terminal(game_state->board))
            get_new_random_piece(game_state);
	}
}

