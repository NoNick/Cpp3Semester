#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.h"
#include "cards.h"

class HumanPlayer: public Player {
public:
    HumanPlayer() {};
    HumanPlayer(std::string s): Player(s) {};
    ~HumanPlayer() {};
    // print state & ask user for action
    // throws std::invalid_argument, std::out_of_range in case of incorrect input
    unsigned bet(unsigned minBet, BoardState state);
};

#endif
