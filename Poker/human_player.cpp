#include <iostream>

#include "human_player.h"
#include "textui.h"

unsigned HumanPlayer::bet(unsigned minBet, BoardState state) {
    return UI->askForUnsigned(this, minBet);
}
