#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.h"
#include "cards.h"
#include "textui.h"

class HumanPlayer: public Player {
private:
    TextUI* UI;
public:
    HumanPlayer() {};
    HumanPlayer(std::string s, TextUI* ui): Player(s) { UI = ui; }
    ~HumanPlayer() {};
    // print state & ask user for action
    // throws std::invalid_argument, std::out_of_range in case of incorrect input
    unsigned bet(unsigned minBet, BoardState state);
};

#endif
