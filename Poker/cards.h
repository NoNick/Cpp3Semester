#ifndef CARDS_H
#define CARDS_H

#include "texas_holdem.h"

// ♣ ♦ ♥ ♠
enum suit {
    clubs, diamonds, hearts, spades
};

class Card {
protected:
    suit s;
    int rank;
public:
    Card() {};
};

class OpenCard: Card {
public:
    int getRank()const;
    suit getSuit()const;
    OpenCard();
    OpenCard(const suit, const int);
    bool operator==(OpenCard const& another);
    std::string toString() const;
};

class CloseCard: Card {
public:
    friend class TexasHoldem;
    CloseCard();
    CloseCard(const suit, const int);
};

#endif
