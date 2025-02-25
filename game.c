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


void print_line(int columns){   // we refactored it
    for(int c=-1; c<columns+1; ++c) 
    	printf("-");    
    printf("\n");
}

void print_board(GameState *game_state){
    Piece *piece = &game_state->current_piece.p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int current_row = game_state->current_piece.at_row;
    int current_col = game_state->current_piece.at_col;
    print_line(game_state->columns);
    for(int r = 0; r < game_state->rows; ++r){
        if(r == 4) print_line(game_state->columns);
        printf("|");
        for(int c=0; c < game_state->columns; ++c){
            if((game_state->board[r][c] == '.') &&
               (current_row <= r) && (r < current_row + p_row_size) && 
               (current_col <= c) && (c < current_col + p_col_size)){
                printf("%c", piece->board[r-current_row][c-current_col]);
            }
            else printf("%c", game_state->board[r][c]);
        }
        printf("|\n");
    }
    print_line(game_state->columns);
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
    game_state->current_piece.at_col = rand() % (MIN_COLUMNS + 1 - game_state->current_piece.p.cols);
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

bool is_collision(GameState *gs){  // we refactored it
    Piece *piece =&gs->current_piece.p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int row = gs->current_piece.at_row;
    int col = gs->current_piece.at_col;

    if((row < 0) || (col < 0) || (row+p_row_size-1 >= gs->rows) || (col+p_col_size-1 >= gs->columns))
    	return true; // piece is out of the grid bounds
    
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if((piece->board[i][j] == '#') && (gs->board[row+i][col+j] == 'X'))
            	return true; // piece collides with another structure in the board
    
    return false;
}

/*int remove_completed_lines(char **board, int rows, int columns){
    int lines = 0;
    bool completed_line;
    for(int r=4; r<rows; ++r){
        completed_line = true;
        for(int c=0; c<columns; ++c){
            if(board[r][c] != 'X'){
                completed_line = false; 
                break;
            }
        }
        if(completed_line){
            ++lines;
            // Move all rows above, once down
            for(int r2=r; r2>3; --r2){
                for(int c=0; c<columns; ++c){
                    board[r2][c] = board[r2-1][c];
                }
            }
        }
    }
    return lines;
}*/

int remove_completed_lines(GameState *game_state){
    int lines = 0;
    bool completed_line;
    for(int r=4; r<game_state->rows; ++r){
        completed_line = true;
        for(int c=0; c<game_state->columns; ++c){
            if(game_state->board[r][c] != 'X'){
                completed_line = false; 
                break;
            }
        }
        if(completed_line){
            ++lines;
            // Move all rows above, once down
            for(int r2=r; r2>3; --r2){
                for(int c=0; c<game_state->columns; ++c){
                    game_state->board[r2][c] = game_state->board[r2-1][c];
                }
            }
        }
    }
    return lines;
}

/********************************************************/
/******* LAB 1 - functions to program (start here) ******/
/********************************************************/


void init_game_state(GameState *game_state){   //We refactored it
    game_state->board = NULL;
    game_state->rows = MIN_ROWS;
    game_state->columns = MIN_COLUMNS;
    make_board(game_state);
    set_default_game_state(game_state);        
    // to do in lab 1        
}      // It creates an empty board with '.', strart the game and put random piece



bool is_terminal(GameState *gs){   // we refactored it
    for(int r = 0; r < 4; ++r) {  // Check only the top 4 rows
        for(int c = 0; c < gs->columns; ++c) {
            if(gs->board[r][c] == 'X'){ 
                return true; // Game over
            }    
        }
    }
    
    // ToDo in LAB 1
    return false;  // Game can continue
}    // It check the first rows and sees if there is a piece to determinate if is game over


void move_piece(GameState *gs, int option){    // we refactored it
    int dir = 0;
    if (option == MOVE_LEFT) dir = -1;
    else if (option == MOVE_RIGHT) dir = 1;
    else { printf("[ERROR] Invalid move option %d.\n", option); return; }
    gs->current_piece.at_col += dir;
    if (is_collision(gs)) {       // Check if there are no collisions
        gs->current_piece.at_col -= dir;
        return; // Exit if the piece cannot move left or right
    }
    gs->current_piece.at_row++;   // The piece goes down

    if (is_collision(gs)) {    // Check if there are collisions after moving down
        gs->current_piece.at_row--;  // If detects collision move back
    }
    // ToDo in LAB 1
}     // This is for move the piece to the right or left

void rotate_piece(GameState *gs, int option){   // we refactored it
    if(option == ROTATE_CW) rotate_clockwise(&(gs->current_piece.p));
    else if(option == ROTATE_CCW) rotate_counter_clockwise(&(gs->current_piece.p));
    else{ printf("[ERROR] Invalid rotation %d.\n", option); }
    if (is_collision(gs)) {       //Check for collisions
        if (option == ROTATE_CW) rotate_counter_clockwise(&(gs->current_piece.p));    // Move back to its original position if there is a collision
        else if (option == ROTATE_CCW) rotate_clockwise(&(gs->current_piece.p));
    }   //This function rotate the piece depending of the users choice
    // ToDo in LAB 1
}
/********************************************************/
/******* LAB 1 - functions to program (end here) ********/
/********************************************************/





/*bool run_turn(GameState *game_state, int option){
	PieceInfo *p_inf = &(game_state->current_piece);
	if(option == MOVE_LEFT || option == MOVE_RIGHT) 
		move_piece(game_state, option);
	else if(option == ROTATE_CW || option == ROTATE_CCW)
		rotate_piece(game_state, option);
    else if(option == NONE){} // do nothing
    else{ printf("[ERROR] Invalid option %d.\n", option); exit(-3); }

	// Move down if possible, otherwise block the piece and remove
    // the completed lines, aggregating them to the current score.
    // If it is not in a terminal state, add a new random piece to the board.
	p_inf->at_row++;
	if(is_collision(game_state)){
		p_inf->at_row--;
		block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state->board, game_state->rows, game_state->columns);{
        if(!is_terminal(game_state))
            get_new_random_piece(game_state);
        return true;
        }
	}
    return false;
}*/
/*bool run_turn(GameState *game_state, int option){
	PieceInfo *p_inf = &(game_state->current_piece);
	if(option == MOVE_LEFT || option == MOVE_RIGHT) 
		move_piece(game_state, option);
	else if(option == ROTATE_CW || option == ROTATE_CCW)
		rotate_piece(game_state, option);
    else if(option == NONE){} // do nothing
    else{ printf("[ERROR] Invalid option %d.\n", option); exit(-3); }

	// Move down if possible, otherwise block the piece and remove
    // the completed lines, aggregating them to the current score.
    // If it is not in a terminal state, add a new random piece to the board.
	p_inf->at_row++;
	if(is_collision(game_state)){
		p_inf->at_row--;
		block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state);
        if(!is_terminal(game_state))
            get_new_random_piece(game_state);
        return true;
	}
    return false;
}*/
bool run_turn(GameState *game_state, int option) {
    PieceInfo *p_inf = &(game_state->current_piece);
    if (option == MOVE_LEFT || option == MOVE_RIGHT) {
        move_piece(game_state, option);
    } else if (option == ROTATE_CW || option == ROTATE_CCW) {
        rotate_piece(game_state, option);
    } else if (option == NONE) {
        // Do nothing
    } else {
        printf("[ERROR] Invalid option %d.\n", option);
        return false;
    }

    p_inf->at_row++;
    if (is_collision(game_state)) {
        p_inf->at_row--;
        block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state);
        if (!is_terminal(game_state)) {
            get_new_random_piece(game_state);
        }
        return true;
    }
    return false;
}


void make_board(GameState *gs){
    gs->board = (char**)malloc(gs->rows * sizeof(char*));   //keeps space for the array of pointers of the board in game state (for the memory)
    for(int i = 0; i < gs->rows; ++i)   //goes though the rows to allocate their memory
        gs->board[i] = (char*)malloc(gs->columns * sizeof(char));
}

void free_game_state(GameState *gs){   //check if the board has been allocated before trying to free it
    if(gs->board != NULL){
        for(int i = 0; i < gs->rows; ++i) //free memory for all the rows
            free(gs->board[i]);
        free(gs->board);  //free the memory for the array of row pointers
    }
}

/*void free_game_state(GameState *game_state) {
    if (game_state->board != NULL) {
        for (int i = 0; i < game_state->rows; i++) {
            free(game_state->board[i]);
        }
        free(game_state->board);
    }
}*/

void set_default_game_state(GameState *gs){  
    gs->score = 0;    //initialize the game score to 0
    for(int r=0; r<gs->rows; ++r)  //goes throug every position in the board to put '.'
        for(int c=0; c<gs->columns; ++c)
            gs->board[r][c] = '.';
    get_new_random_piece(gs);  //it gets a random piece
}

void restart_game_state(GameState *gs){   //it asks again the dimensions of the board and it creates a new board with the imputed dimensions
    printf("Enter number of rows (minimum of 10): ");
    gs->rows = read_int();
    printf("Enter number of columns (minimum of 6): ");
    gs->columns = read_int();
    make_board(gs);   //creates the board
    set_default_game_state(gs);
}



/*GameState copy_game_state(GameState *game_state) {
    GameState copy;
    copy.rows = game_state->rows;
    copy.columns = game_state->columns;
    copy.score = game_state->score;
    copy.current_piece = game_state->current_piece;
    
    copy.board = (char**)malloc(copy.rows * sizeof(char*));
    for(int i = 0; i < copy.rows; ++i) {
        copy.board[i] = (char*)malloc(copy.columns * sizeof(char));
        for(int j = 0; j < copy.columns; ++j) {
            copy.board[i][j] = game_state->board[i][j];
        }
    }
    
    return copy;
}*/
GameState copy_game_state(GameState *game_state) {
    GameState copy;
    copy.rows = game_state->rows;
    copy.columns = game_state->columns;
    copy.score = game_state->score;
    copy.current_piece = game_state->current_piece;
    
    // Deep copy the board
    copy.board = (char**)malloc(copy.rows * sizeof(char*));
    for (int i = 0; i < copy.rows; i++) {
        copy.board[i] = (char*)malloc(copy.columns * sizeof(char));
        for (int j = 0; j < copy.columns; j++) {
            copy.board[i][j] = game_state->board[i][j];
        }
    }
    
    return copy;
}

/*int recursive_best_score(GameState *game_state, int depth) {
    if (depth >= MAX_DEPTH) return game_state->score;
    
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    
    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        if (run_turn(&copy, options[i])) { // Run a move and check if blocked
            best_score = (copy.score > best_score) ? copy.score : best_score;
        }
        free_game_state(&copy);
    }
    
    return best_score;
}*/


/*int recursive_best_score(GameState *game_state, int depth) {
    for(int i = depth; i<=MAX_DEPTH; i++){
        int best_score = game_state->score;
        int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
        
        for (int i = 0; i < 5; i++) {
            GameState copy = copy_game_state(game_state);
            if (run_turn(&copy, options[i])) { // Apply move if valid
                int score = recursive_best_score(&copy,i+1); // Recursively continue until landing
                if (score > best_score){ 
                    best_score = score; // Keep the highest score
                }
            free_game_state(&copy);
            }
        };
    return best_score;
    }
}*/

/*int recursive_best_score(GameState *game_state, int depth) {
    int best_score=game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    for(int i=depth;depth<=MAX_DEPTH;i++){
        for (int i = 0; i < 5; i++) {
            GameState copy = copy_game_state(game_state);
            if (run_turn(&copy, options[i])) { // Apply move if valid
                if(depth==MAX_DEPTH){
                    int score= game_state->score;
                    if(score>best_score){
                        best_score = score;
                    }
                }
                else{recursive_best_score(&copy,i+1);}
            free_game_state(&copy);
            }
        };
    return best_score;
    }
}*/
/*int recursive_best_score(GameState *game_state, int depth) {
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    
    if (depth == MAX_DEPTH) {
        return game_state->score; // Base case: return the score if the depth is maxed out.
    }

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        
        if (run_turn(&copy, options[i])) { // Apply move if valid
            int score = recursive_best_score(&copy, depth + 1); // Recursive call with updated depth
            if (score > best_score) {
                best_score = score; // Update the best score if needed
            }
        }
        
        free_game_state(&copy);
    }

    return best_score; // Return the best score after all options are explored
}*/
/*int recursive_best_score(GameState *game_state, int depth) {
    if (depth == MAX_DEPTH || is_terminal(game_state)) {
        return game_state->score; // Base case: return the current score
    }

    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy, depth + 1);
            if (score > best_score) {
                best_score = score;
            }
        }
        free_game_state(&copy);
    }

    return best_score;
}*/
/*int recursive_best_score(GameState *game_state, int depth) {
    printf("Depth: %d, Current Score: %d\n", depth, game_state->score);
    if (depth == MAX_DEPTH || is_terminal(game_state)) {
        printf("Base case reached. Returning score: %d\n", game_state->score);
        return game_state->score;
    }

    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Trying option: %d\n", options[i]);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy, depth + 1);
            printf("Option: %d, Score: %d\n", options[i], score);
            if (score > best_score) {
                best_score = score;
            }
        } else {
            printf("Option: %d is invalid or doesn't block the piece.\n", options[i]);
        }
        free_game_state(&copy);
    }

    printf("Best score at depth %d: %d\n", depth, best_score);
    return best_score;
}*/
/*int recursive_best_score(GameState *game_state, int depth) {
    printf("Depth: %d, Current Score: %d\n", depth, game_state->score);
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    if (depth == MAX_DEPTH || is_terminal(game_state)) {
        printf("Base case reached. Returning score: %d\n", game_state->score);
        return game_state->score;
    }
    else{
        for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Trying option: %d\n", options[i]);
        int score = recursive_best_score(&copy, depth + 1);
            if (run_turn(&copy, options[i])){
            printf("Option: %d, Score: %d\n", options[i], score);
                if (score > best_score) {
                best_score = score;
                }
            
            } 
        free_game_state(&copy);
        }
    }

    printf("Best score at depth %d: %d\n", depth, best_score);
    return best_score;
}*/

/*int recursive_best_score(GameState *game_state, int depth) {
    printf("Depth: %d, Current Score: %d\n", depth, game_state->score);

    // Base case
    if (depth == MAX_DEPTH || is_terminal(game_state)) {
        printf("Base case reached. Returning score: %d\n", game_state->score);
        return game_state->score;
    }

    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Trying option: %d\n", options[i]);

        // Apply the move
        if (run_turn(&copy, options[i])) {
            printf("Move applied. Piece is blocked.\n");
        } else {
            printf("Move applied. Piece is not blocked.\n");
        }

        // Recursively evaluate the move
        int score = recursive_best_score(&copy, depth + 1);
        printf("Option: %d, Score: %d\n", options[i], score);

        if (score > best_score) {
            best_score = score;
            printf("New best score: %d\n", best_score);
        }

        free_game_state(&copy);
    }

    printf("Best score at depth %d: %d\n", depth, best_score);
    return best_score;
}*/




/*int show_best_move(GameState *game_state) {
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Evaluating move: %d\n", options[i]);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy, 0);
            printf("Move: %d, Score: %d\n", options[i], score);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        } else {
            printf("Move: %d is invalid or doesn't block the piece.\n", options[i]);
        }
        free_game_state(&copy);
    }

    printf("Best move: %d, Best score: %d\n", best_move, best_score);
    return best_move;
}*/
/*int show_best_move(GameState *game_state) {
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Evaluating move: %d\n", options[i]);
        // Apply the move (even if it doesn't block the piece)
        if (run_turn(&copy, options[i])) {
            // Recursively evaluate the move
            int score = recursive_best_score(&copy, 0);
            printf("Move: %d, Score: %d\n", options[i], score);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        } else {
            // If the move doesn't block the piece, still evaluate it recursively
            int score = recursive_best_score(&copy, 0);
            printf("Move: %d is invalid or doesn't block the piece.Doing recurseve inside the else.\n", options[i]);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        }

        free_game_state(&copy);
    }

    return best_move;
}*/
/*int show_best_move(GameState *game_state) {
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy, 0);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        }
        free_game_state(&copy);
    }

    return best_move;
}*/

/*int show_best_move(GameState *game_state){
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    
    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy,0);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        }
        free_game_state(&copy);
    }
    
    return best_move;
}*/
        
/*int show_best_move(GameState *game_state){
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    
    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        if (run_turn(&copy, options[i])) {
            int score = recursive_best_score(&copy,1);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        }
        free_game_state(&copy);
    }
    
    return best_move;
}*/



int recursive_best_score(GameState *game_state, int depth, GameState *visited_states[], int *visited_count) {
    printf("Depth: %d, Current Score: %d\n", depth, game_state->score);

    // Base case
    if (depth == MAX_DEPTH || is_terminal(game_state)) {
        printf("Base case reached. Returning score: %d\n", game_state->score);
        return game_state->score;
    }

    // Check if this state has been visited to avoid infinite recursion
    for (int i = 0; i < *visited_count; i++) {
        if (compare_game_states(game_state, visited_states[i])) {
            printf("State already visited, skipping recursion.\n");
            return game_state->score;
        }
    }

    // Mark this state as visited
    *visited_states[*visited_count] = copy_game_state(game_state);  // Store the pointer
    (*visited_count)++;

    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Trying option: %d\n", options[i]);

        // Apply the move
        if (run_turn(&copy, options[i])) {
            printf("Move applied. Piece is blocked.\n");
        } else {
            printf("Move applied. Piece is not blocked.\n");
        }

        // Recursively evaluate the move
        int score = recursive_best_score(&copy, depth + 1, visited_states, visited_count);
        printf("Option: %d, Score: %d\n", options[i], score);

        if (score > best_score) {
            best_score = score;
            printf("New best score: %d\n", best_score);
        }

        free_game_state(&copy);
    }

    printf("Best score at depth %d: %d\n", depth, best_score);
    return best_score;
}

int show_best_move(GameState *game_state) {
    int best_move = NONE;
    int best_score = game_state->score;
    int options[] = {MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};

    // Create a visited states array to keep track of previously evaluated states
    GameState *visited_states[MAX_DEPTH];  // Adjust the size as needed.
    int visited_count = 0;

    for (int i = 0; i < 5; i++) {
        GameState copy = copy_game_state(game_state);
        printf("Evaluating move: %d\n", options[i]);

        // Apply the move (even if it doesn't block the piece)
        if (run_turn(&copy, options[i])) {
            // Recursively evaluate the move
            int score = recursive_best_score(&copy, 0, visited_states, &visited_count);
            printf("Move: %d, Score: %d\n", options[i], score);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        } else {
            // If the move doesn't block the piece, still evaluate it recursively
            int score = recursive_best_score(&copy, 0, visited_states, &visited_count);
            printf("Move: %d is invalid or doesn't block the piece. Doing recursion inside the else.\n", options[i]);
            if (score > best_score) {
                best_score = score;
                best_move = options[i];
            }
        }

        free_game_state(&copy);
    }

    // Free visited game states after use
    for (int i = 0; i < visited_count; i++) {
        free_game_state(visited_states[i]);
    }

    return best_move;
}

int compare_game_states(GameState *state1, GameState *state2) {
    // Compare two game states for equality (you can adjust this as needed)
    return state1->score == state2->score;
}