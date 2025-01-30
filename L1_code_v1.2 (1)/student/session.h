#ifndef _SESSION_H_
#define _SESSION_H_

#include "game.h"

/// LAB 1 - data structure
typedef struct {
    int best_score;               // It stores the best score
    GameState current_game_state; // To stores the current game(with the board and the pieces)
} Session;

/// LAB 1 - functions
void restart_session_game_state(Session *session);
void init_session(Session *session);

#endif
