#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "cards.h"

class OpenCard;

struct BoardState {
    unsigned enemyN, pot, smallBlind, plCash, plBet;
    OpenCard *hand;
    std::vector <OpenCard> board;
    std::vector <std::string> names;
    std::vector <unsigned> cash, bets;

    BoardState();
    ~BoardState();
    // prints all info into stdout
    void print() const;
};

#endif
