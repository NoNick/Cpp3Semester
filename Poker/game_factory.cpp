#include <algorithm>
#include <ctime>

#include "texas_holdem.h"
#include "cards.h"

template <class ... Players>
TexasHoldem* createGame(unsigned n, unsigned initCash, unsigned smallBlind, Players* ... p) {
    TexasHoldem* result = new TexasHoldem();
    result->playersN = n;
    result->smallBlind = smallBlind;
    result->deck = std::vector <CloseCard>();
    result->board = std::vector <OpenCard>();

    result->players = new TexasHoldem::PlayerData[result->playersN];
    result->addPlayers(0, p...);

    for (unsigned i = 0; i < result->playersN; i++) {
        result->players[i].cash = initCash;
        result->players[i].hand = new OpenCard[2];
    }
    result->shuffle();

    return result;
}

template <class First>
void TexasHoldem::addPlayers(unsigned cnt, First* first) {
    if (cnt < playersN) {
        players[cnt].p = first;
    }
}

template <class First, class... Rest>
void TexasHoldem::addPlayers(unsigned cnt, First* first, Rest* ... rest) {
    if (cnt < playersN) {
        players[cnt].p = first;
        addPlayers(++cnt, rest...);
    }
}