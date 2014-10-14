#include <iostream>
#include "board_state.h"
#include "texas_holdem.h"
#include "human_player.h"
#include "game_factory.cpp"

using namespace std;

int main() {
    TextUI* ui = new TextUI(false);
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10, ui,
            new HumanPlayer("First player", ui), new HumanPlayer("Second player", ui));
    game->run();
    delete game;
    return 0;
}
