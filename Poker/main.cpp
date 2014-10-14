#include <iostream>
#include "board_state.h"
#include "texas_holdem.h"
#include "human_player.h"
#include "game_factory.cpp"

using namespace std;

int main() {
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10,
            new HumanPlayer("First player"), new HumanPlayer("Second player"));
    game->run();
    delete game;
    return 0;
}