#ifndef TEXAS_HOLDEM_H
#define TEXAS_HOLDEM_H

#include <vector>
#include "gtest/gtest.h"
#include "cards.h"
#include "player.h"
#include "board_state.h"
#include "textui.h"

class OpenCard;
class CloseCard;
class Player;
class BoardState;
class TextUI;

class TexasHoldem {
private:
    struct PlayerData {
        unsigned cash;
        OpenCard* hand;
        unsigned currBet;
        Player* p;

        ~PlayerData();
    } *players;
    unsigned playersN, pot, smallBlind, dealer;

    // takes pointer to 5 elements array
    // returns {combitation_id, combination_relevance, 1st_kicker, 2nd_kicker, 3rd_kicker, 4th_kicker, 5th_kicker}
    // combitation_id:
    //     highest card: 0
    //     pair: 1
    //     two pairs: 2
    //     triple: 3
    //     straight: 4
    //     flush: 5
    //     full house: 6
    //     four of a kind: 7
    //     straight flush: 8
    //     royal flush: 9
    // combination_relevance: rank among the same combination, for example pair of aces > pair of kings
    unsigned* evaluate(OpenCard* cards);
    // takes pointer to 7-elements array
    // returns pointer to 5-elements array with combination with highest ranks
    // ranks is 7-elements array, second element in pair
    std::pair <OpenCard*, unsigned*> highestComb(OpenCard*);
    // returns vector with winnerses ids
    std::vector<unsigned> showdown();
    // player bets starting from the next to big blind
    void doBets(unsigned underTheGun);
    // play round with players[i] (i in inGame)
    void doRound();
    // give new cards to players & deck
    void shuffle();
    // return ids of players with cash >= smallBlind * 2 (big blind)
    std::vector<unsigned> refresh();
    // remove card from deck and add to board
    void openNext();
    // returns number of players which are able to bet
    size_t inGameWithCash();
    // returns state for players[player].p
    BoardState getState(unsigned player, std::vector<unsigned> inGame);
    std::vector <OpenCard> board;
    std::vector <CloseCard> deck;

    // ids of players in game
    std::vector<unsigned> inGame;

    // create new player from templates & add to instance
    template <class First>
    void addPlayers(unsigned cnt, First* first);
    template <class First, class... Rest>
    void addPlayers(unsigned cnt, First* first, Rest* ... rest);

    TexasHoldem();
    // static no-class function permits to create object without template in it
    // should be used instead of default constructor
    template <class ... Players>
    friend TexasHoldem* createGame(unsigned n, unsigned initCash, unsigned smallBlind,
                                   TextUI* ui, Players* ... p);

    TextUI* UI;

    FRIEND_TEST(correctness, evaluateCombination);
    FRIEND_TEST(correctness, pickCombination);
    FRIEND_TEST(correctness, shuffleTest);
public:
    ~TexasHoldem();
    // running until somebody wins
    void run();
};

#endif
