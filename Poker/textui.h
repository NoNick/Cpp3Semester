#ifndef TEXTUI_H
#define TEXTUI_H
#include "player.h"
#include "cards.h"

class TextUI
{
public:
    TextUI();
    static void printState(const Player& p, const BoardState& bs);
    static void print(std::string s);
    static void raise(Player *p, unsigned to, unsigned bet);
    static void call(Player *p, unsigned to, unsigned bet);
    static void check(Player *p);
    static void fold(Player *p);
    static void smallBlind(Player *p, unsigned value);
    static void bigBlind(Player *p, unsigned value);
    static void showdown(Player *p, OpenCard *cards, unsigned *value);
    static void win(std::vector<Player*> winners);
    static unsigned askForUnsigned(Player *p, unsigned minBet);
};

#endif // TEXTUI_H
