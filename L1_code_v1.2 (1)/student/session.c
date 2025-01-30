#include "session.h"

void restart_session_game_state(Session *session){
    // ToDo in LAB 1
    init_game_state(&session->current_game_state);   //It call to the initialization of the game state(to continue with your previues game)
}

void init_session(Session *session){
    session->best_score = 0;              // It reset the best score to 0
    restart_session_game_state(session);  // Restart te game state ot the session
    // ToDo in LAB 1
}

