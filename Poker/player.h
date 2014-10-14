#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "texas_holdem.h"
#include "board_state.h"

class BoardState;

class Player {
private:
    std::string name;
public:
    Player() {};
    Player(std::string n) { name = n; }
    std::string getName() const { return name; }
    virtual ~Player() {};
    virtual unsigned bet(unsigned minBet, BoardState state) = 0;
};

#endif
