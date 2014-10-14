#ifndef TEXTUI_H
#define TEXTUI_H
#include "player.h"
#include "cards.h"

class Player;
class OpenCard;

class TextUI
{
public:
    bool noUI;
    TextUI() { noUI = false; }
    TextUI(bool ui);
    void printState(const Player& p, const BoardState& bs);
    void print(std::string s);
    void raise(Player *p, unsigned to, unsigned bet);
    void call(Player *p, unsigned to, unsigned bet);
    void check(Player *p);
    void fold(Player *p);
    void smallBlind(Player *p, unsigned value);
    void bigBlind(Player *p, unsigned value);
    void showdown(Player *p, OpenCard *cards, unsigned *value);
    void win(std::vector<Player*> winners);
    unsigned askForUnsigned(Player *p, unsigned minBet);
};

#endif // TEXTUI_H
