#include "main.h"

void run_game(Session *session){
    GameState *game_state = &session->current_game_state;

    bool finish_game; 

    do{
        // 1. Print data
        printf("Best score: %d\n", session->best_score);
        print_game_state(game_state);

        // 2. Select a move
        int game_option;
        do{
            print_options();
            printf("Enter a game option: ");
            game_option = read_int();
        }while(!is_valid_option(game_option));
        
        // 3.a Quit game
        if(game_option == QUIT_GAME)
            return;
        // 3.b Show best move
        if(game_option == SHOW_BEST_MOVE){
            // ToDo in Lab 3
            continue;
        }

        // 3.c Run one turn with the given option
        run_turn(game_state, game_option);

        // 4. Update scores
        session->best_score = myMax(session->best_score, game_state->score);
        
        // 5. Add new number
        finish_game = is_terminal(game_state);
    }while(!finish_game);

    printf("*** GAME OVER ***\n");
    print_game_state(game_state);
}

void new_game(Session *session){
    restart_session_game_state(session);
    ///state(session);
    run_game(session);
}

void save_game(Session *session){
    // ToDo in Lab 2
    char filename[MAX_STR_LENGTH];
    printf("Enter filename to save: ");
    read_filename(filename);
    FILE* file = fopen(filename, "w");
    if(file == NULL){
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "Score: %d\n\n", session->current_game_state.score);
    PieceInfo *p = &session->current_game_state.current_piece;
    fprintf(file, "Piece at: %d %d\n", p->at_row, p->at_col);
    fprintf(file, "Piece name: %c\n", p->p.name);
    fprintf(file, "Piece rows: %d\n", p->p.rows);
    fprintf(file, "Piece cols: %d\n", p->p.cols);
    for(int i = 0; i < PIECE_SIZE; ++i){
        for(int j = 0; j < PIECE_SIZE; ++j){
            fprintf(file, "%c", p->p.board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\nBoard dimensions: %d %d\n", session->current_game_state.rows, session->current_game_state.columns);
    for(int i = 0; i < session->current_game_state.rows; ++i){
        for(int j = 0; j < session->current_game_state.columns; ++j){
            fprintf(file, "%c", session->current_game_state.board[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void load_game(Session *session){
    // ToDo in Lab 2
    char filename[MAX_STR_LENGTH];
    printf("Enter filename to load: ");
    read_filename(filename);
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf("Error opening file!\n");
        return;
    }
    fscanf(file, "Score: %d\n\n", &session->current_game_state.score);
    PieceInfo *p = &session->current_game_state.current_piece;
    fscanf(file, "Piece at: %d %d\n", &p->at_row, &p->at_col);
    fscanf(file, "Piece name: %c\n", &p->p.name);
    fscanf(file, "Piece rows: %d\n", &p->p.rows);
    fscanf(file, "Piece cols: %d\n", &p->p.cols);
    for(int i = 0; i < PIECE_SIZE; ++i){
        for(int j = 0; j < PIECE_SIZE; ++j){
            fscanf(file, "%c", &p->p.board[i][j]);
        }
        fscanf(file, "\n");
    }
    fscanf(file, "\nBoard dimensions: %d %d\n", &session->current_game_state.rows, &session->current_game_state.columns);
    make_board(&session->current_game_state);
    for(int i = 0; i < session->current_game_state.rows; ++i){
        for(int j = 0; j < session->current_game_state.columns; ++j){
            fscanf(file, "%c", &session->current_game_state.board[i][j]);
        }
        fscanf(file, "\n");
    }
    fclose(file);
}

void resume_game(Session *session){
    // ToDo in Lab 2
    run_game(session);
}

void print_menu(){
    printf("Menu options:\n");
    printf("\t1. New game.\n");  // LAB1 - basic lab for creating board, pieces and movements
                                 // LAB1 - extend game with GameState data structure, current score, best score
                                 //      - also add standard (4x4) with a posible extension, read board from command line  
    printf("\t2. Save game.\n"); // LAB2 - Writing file
    printf("\t3. Load game.\n"); // LAB2 - Reading file
    printf("\t4. Resume game.\n"); // LAB2 - Continue game after reading file
    printf("\t5. Exit.\n");  // Free all reserved memory!
}


void run(Session *session){
    int option;
    do{
        print_menu();
        do{
            printf("Enter an integer (%d-%d): ", NEW_GAME, EXIT);
            option = read_int();
        }while(option < NEW_GAME || option > EXIT);

        switch (option){
        case NEW_GAME:
            new_game(session);
            break;
        case SAVE_GAME:
            save_game(session);
            break;
        case LOAD_GAME:
            load_game(session);
            break;
        case RESUME_GAME:
            resume_game(session);
            break;
        case EXIT:
            free_game_state(&session->current_game_state);
            break;
        }
    }while(option != EXIT);
}

int main(){
    Session session;
    init_session(&session);
    run(&session);
}
