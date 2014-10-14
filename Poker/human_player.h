#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.h"
#include "cards.h"

class HumanPlayer: public Player {
public:
    HumanPlayer() {};
    HumanPlayer(std::string s): Player(s) {};
    ~HumanPlayer() {};
    // throws std::invalid_argument, std::out_of_range
    unsigned bet(unsigned minBet, BoardState state);
};

#endif
