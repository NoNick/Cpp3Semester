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
    Card() {}
};

class OpenCard: Card {
public:
    // returns value 2..14: rank of a card
    int getRank()const;
    // returns suit of a card
    suit getSuit()const;
    OpenCard();
    OpenCard(const suit, const int);
    // required for std::count
    bool operator==(OpenCard const& another);
    // returns string description of a card
    std::string toString() const;
};

class CloseCard: Card {
public:
    friend class TexasHoldem;
    CloseCard();
    CloseCard(const suit, const int);
};

#endif
