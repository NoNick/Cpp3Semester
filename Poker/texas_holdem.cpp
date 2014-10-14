#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <string.h>
#include <cmath>
#include <ksba.h>

#include "cards.h"
#include "texas_holdem.h"
#include "board_state.h"
#include "textui.h"

namespace {
    // returns upper or equal value from inGame
    // or first value if x is max
    unsigned myUpperBound(std::vector<unsigned> inGame, unsigned x) {
        if (std::count(inGame.begin(), inGame.end(), x))
            return x;

        auto result = std::upper_bound(inGame.begin(), inGame.end(), x);
        if (result == inGame.end())
            return inGame[0];
        return *result;
    }

    bool highestRankFirst(const OpenCard &a, const OpenCard &b) {
        return a.getRank() > b.getRank();
    }

    bool bySuit(const OpenCard &a, const OpenCard &b) {
        if ((int) a.getSuit() == (int) b.getSuit())
            return a.getRank() < b.getRank();
        return (int) a.getSuit() < (int) b.getSuit();
    }
}

TexasHoldem::TexasHoldem() {}

unsigned* TexasHoldem::evaluate(OpenCard *cards) {
    unsigned *result = new unsigned[7];
    for (int i = 0; i < 7; i++) {
        result[i] = 0;
    }

    std::sort(cards, cards + 5, bySuit);
    if (cards[0].getSuit() == cards[4].getSuit()) {
        result[0] = 5;
        result[1] = (unsigned int) cards[4].getRank();;
        result[2] = (unsigned int) cards[4].getRank();
        result[3] = (unsigned int) cards[3].getRank();
        result[4] = (unsigned int) cards[2].getRank();
        result[5] = (unsigned int) cards[1].getRank();
        result[6] = (unsigned int) cards[0].getRank();
    }

    std::sort(cards, cards + 5, highestRankFirst);
    int diff[4], pos = 0;
    for (int i = 0; i < 4; i++) {
        if (cards[i].getRank() - cards[i + 1].getRank() > 1)
            pos = i + 1;
        diff[i] = cards[i].getRank() - cards[i + 1].getRank();
    }

    bool straight = true;
    for (int i = pos; i < pos + 4; i++) {
        if ((i + 1) % 5 == 0) {
            if ((cards[4].getRank() != 2 || cards[0].getRank() != 14) && pos != 0)
                straight = false;
        }
        else if (cards[i % 5].getRank() - cards[(i + 1) % 5].getRank() != 1)
            straight = false;
    }

    if (straight) {
        result[1] = (unsigned int) cards[std::max(pos - 1, 0)].getRank();
        if (result[0] == 5) {
            if (cards[0].getRank() == 14 && cards[4].getRank() == 10)
                result[0] = 9;
            else
                result[0] = 8;
        }
        else
            result[0] = 4;
    }

    unsigned code = 0;
    for (int i = 0; i < 4; i++) {
        code |= (1 << i) * (diff[3 - i] != 0);
    }

    // carte: XXXXy | yXXXX, i.e. 0001|1000
    if (code == 1 && result[0] < 7) {
        result[0] = 7;
        result[1] = (unsigned int) cards[0].getRank();
        result[2] = (unsigned int) cards[4].getRank();
    }
    else if (code == 8 && result[0] < 7) {
        result[0] = 7;
        result[1] = (unsigned int) cards[1].getRank();
        result[2] = (unsigned int) cards[0].getRank();
    }
        // 3+2: XXXYY=0010, YYXXX=0100
    else if (code == 2 && result[0] < 6) {
        result[0] = 6;
        result[1] = (unsigned int) cards[0].getRank() * 14 + cards[3].getRank();
    }
    else if (code == 4 && result[0] < 6) {
        result[0] = 6;
        result[1] = (unsigned int) cards[3].getRank() * 14 + cards[0].getRank();
    }
        // 3: XXXyz=0011, yXXXz=1001, yzXXX=1100
    else if (code == 3 && result[0] < 3) {
        result[0] = 3;
        result[1] = (unsigned int) cards[0].getRank();
        result[2] = (unsigned int) cards[3].getRank();
        result[3] = (unsigned int) cards[4].getRank();
    }
    else if (code == 9 && result[0] < 3) {
        result[0] = 3;
        result[1] = (unsigned int) cards[1].getRank();
        result[2] = (unsigned int) cards[0].getRank();
        result[3] = (unsigned int) cards[4].getRank();
    }
    else if (code == 12 && result[0] < 3) {
        result[0] = 3;
        result[1] = (unsigned int) cards[2].getRank();
        result[2] = (unsigned int) cards[0].getRank();
        result[3] = (unsigned int) cards[1].getRank();
    }
        // 2+2: XXYYz=0101, XXzYY=0110, zXXYY=1010
    else if (code == 5 && result[0] < 2) {
        result[0] = 2;
        result[1] = (unsigned int) std::max(cards[0].getRank(), cards[2].getRank()) * 14 +
                std::min(cards[0].getRank(), cards[2].getRank());
        result[2] = (unsigned int) cards[4].getRank();
    }
    else if (code == 6 && result[0] < 2) {
        result[0] = 2;
        result[1] = (unsigned int) std::max(cards[0].getRank(), cards[3].getRank()) * 14 +
                std::min(cards[0].getRank(), cards[3].getRank());
        result[2] = (unsigned int) cards[2].getRank();
    }
    else if (code == 10 && result[0] < 2) {
        result[0] = 2;
        result[1] = (unsigned int) std::max(cards[1].getRank(), cards[3].getRank()) * 14 +
                std::min(cards[1].getRank(), cards[3].getRank());
        result[2] = (unsigned int) cards[0].getRank();
    }
        // 2: XXyzt=0111, yXXzt=1011, yzXXt=1101, yztXX=1110
    else if (code == 7 && result[0] < 1) {
        result[0] = 1;
        result[1] = (unsigned int) cards[0].getRank();
        result[2] = (unsigned int) cards[2].getRank();
        result[3] = (unsigned int) cards[3].getRank();
        result[4] = (unsigned int) cards[4].getRank();
    }
    else if (code == 11 && result[0] < 1) {
        result[0] = 1;
        result[1] = (unsigned int) cards[1].getRank();
        result[2] = (unsigned int) cards[0].getRank();
        result[3] = (unsigned int) cards[3].getRank();
        result[4] = (unsigned int) cards[4].getRank();
    }
    else if (code == 13 && result[0] < 1) {
        result[0] = 1;
        result[1] = (unsigned int) cards[2].getRank();
        result[2] = (unsigned int) cards[0].getRank();
        result[3] = (unsigned int) cards[1].getRank();
        result[4] = (unsigned int) cards[4].getRank();
    }
    else if (code == 14 && result[0] < 1) {
        result[0] = 1;
        result[1] = (unsigned int) cards[3].getRank();
        result[2] = (unsigned int) cards[0].getRank();
        result[3] = (unsigned int) cards[1].getRank();
        result[4] = (unsigned int) cards[2].getRank();
    }
    else if (result[0] == 0) {
        result[1] = 0;
        for (int i = 0; i < 5; i++)
            result[1 + i] = (unsigned int) cards[i].getRank();
    }

    return result;
}

TexasHoldem::~TexasHoldem() {
    delete[] players;
    delete UI;
}

TexasHoldem::PlayerData::~PlayerData() {
    delete[] hand;
    delete p;
}

namespace {
    bool nextCombination(size_t* item, size_t n, size_t N) {
        for (size_t i = 1; i <= n; ++i) {
            if (item[n-i] < N-i) {
                ++item[n-i];
                for (size_t j = n-i+1; j < n; ++j) {
                    item[j] = item[j-1] + 1;
                }
                return true;
            }
        }
        return false;
    }

    // returns:
    //     -1: a < b
    //     0: a == b
    //     1: a > b
    int unsigned7cmp (unsigned* a, unsigned* b) {
        for (int i = 0; i < 7; i++) {
            if (a[i] != b[i]) {
                return - (a[i] < b[i]) + (a[i] > b[i]);
            }
        }
        return 0;
    }
}

std::pair <OpenCard*, unsigned*> TexasHoldem::highestComb(OpenCard* source) {
    size_t cardIndex[5], n = 7, k = 5;
    for (size_t i = 0; i < k; i++) {
        cardIndex[i] = i;
    }
    unsigned *currValue, *maxValue = new unsigned[7];
    memset(maxValue, 0, sizeof(unsigned) * 7);
    OpenCard *curr = new OpenCard[k], *maxCard = new OpenCard[k];
    do {
        for (size_t i = 0; i < k; i++) {
            curr[i] = source[cardIndex[i]];
        }
        currValue = evaluate(curr);
        if (unsigned7cmp(currValue, maxValue) == 1) {
            memcpy(maxValue, currValue, sizeof(unsigned) * 7);
            memcpy(maxCard, curr, sizeof(OpenCard) * k);
        }
        delete[] currValue;
    } while (nextCombination(cardIndex, k, n));
    delete[] curr;

    return std::make_pair(maxCard, maxValue);
}

std::vector<unsigned> TexasHoldem::refresh() {
    std::vector<unsigned> result;
    for (unsigned i = 0; i < playersN; i++) {
        if (players[i].cash != 0)
            result.push_back((unsigned int const &) i);
    }
    return result;
}

void TexasHoldem::doRound() {
    UI->print("New round\n");
    board.clear();
    unsigned small = myUpperBound(inGame, (dealer + 1) % playersN),
            big = myUpperBound(inGame, (small + 1) % playersN);
    players[small].cash -= smallBlind;
    players[small].currBet = smallBlind;
    UI->smallBlind(players[small].p, smallBlind);
    players[big].cash -= smallBlind * 2;
    players[big].currBet = smallBlind * 2;
    UI->bigBlind(players[big].p, smallBlind * 2);
    pot = smallBlind * 3;

    unsigned underTheGun = big = myUpperBound(inGame, (big + 1) % playersN);
    doBets(underTheGun);
    for (int i = 0; i < 3; i++)
        openNext();
    if (inGame.size() > 1)
        doBets(underTheGun);
    openNext();
    if (inGame.size() > 1)
        doBets(underTheGun);
    openNext();
    if (inGame.size() > 1)
        doBets(underTheGun);
    std::vector<unsigned> winners = showdown();
    assert(winners.size() > 0);
    std::vector<Player*> winnersPointers;
    for (unsigned i = 0; i < winners.size(); i++) {
        // TODO: handle all-in cases
        players[winners[i]].cash += pot / winners.size();
        winnersPointers.push_back(players[winners[i]].p);
    }
    UI->win(winnersPointers);
}

void TexasHoldem::doBets(unsigned underTheGun) {
    unsigned last = underTheGun, curr = last, value, currBet = 0;
    for (unsigned i = 0; i < inGame.size(); i++) {
        currBet = std::max(currBet, players[inGame[i]].currBet);
    }

    do {
        if (players[curr].cash != 0) {
            UI->printState(*players[curr].p, getState(curr, inGame));
            for (int i = 0; i < 3; i++) {
                bool wrongInput = false;
                try {
                    value = players[curr].p->bet(currBet - players[curr].currBet, getState(curr, inGame));
                }
                catch (...){
                    // std::invalid_argument, std::out_of_range
                    wrongInput = true;

                }
                if (((players[curr].cash >= value && value + players[curr].currBet >= currBet) || value == 0) && !wrongInput)
                    break;
                if (i == 2) {
                    value = 0;
                    UI->print("wrong value, check/fold by default\n");
                    break;
                }
                else
                    UI->print("wrong value, try again (" + std::to_string(2 - i) + ")\n");
            }
            // call/raise
            if (value > 0) {
                if (value + players[curr].currBet > currBet) {
                    last = curr;
                    currBet = value + players[curr].currBet;
                    UI->raise(players[curr].p, currBet, value);
                }
                else {
                    UI->call(players[curr].p, currBet, value);
                }
                players[curr].cash -= value;
                players[curr].currBet += value;
                pot += value;
            }
            // fold/check
            else {
                // check
                if (currBet - players[curr].currBet == 0) {
                    UI->check(players[curr].p);
                }
                // fold
                else {
                    UI->fold(players[curr].p);
                    assert(std::find(inGame.begin(), inGame.end(), curr) != inGame.end());
                    inGame.erase(std::find(inGame.begin(), inGame.end(), curr));
                    if (inGame.size() == 1)
                        return;
                }
            }
        }
        curr = myUpperBound(inGame, (curr + 1) % playersN);
    } while (curr != last);
}

std::vector<unsigned> TexasHoldem::showdown() {
    std::vector<unsigned> winners;
    unsigned *maxValue = new unsigned[7], *currValue;
    memset(maxValue, 0, sizeof(unsigned) * 7);
    OpenCard currCards[7];
    OpenCard *highest;
    assert(board.size() == 5);
    for (int j = 0; j < 5; j++) {
        currCards[j] = board[j];
    }
    for (unsigned i = 0; i < inGame.size(); i++) {
        // TODO: write combination to log
        currCards[6] = players[inGame[i]].hand[0];
        currCards[7] = players[inGame[i]].hand[1];
        std::pair<OpenCard*, unsigned*> comb = highestComb(currCards);
        currValue = comb.second;
        highest = comb.first;
        switch(unsigned7cmp(currValue, maxValue)) {
            case 1:
                winners.clear();
            case 0:
                winners.push_back(inGame[i]);
                break;
            default:
                break;
        }

        if (inGame.size() > 1)
            UI->showdown(players[inGame[i]].p, highest, currValue);
        delete[] highest;
        delete[] currValue;
    }
    delete[] maxValue;

    return winners;
}

void TexasHoldem::shuffle() {
    UI->print("Shuffling cards...\n\n");

    const int N = 52;
    std::pair<suit, int> deckTmp[N];
    for (int i = 2; i <= 14; i++) {
        for (int j = suit::clubs; j <= suit::spades; j++) {
            deckTmp[(i - 2) * 4 + j] = std::make_pair((suit) j, i);
        }
    }

    std::srand((unsigned) (time(0)));
    std::random_shuffle(deckTmp, deckTmp + N);
    for (unsigned i = 0; i < playersN; i++) {
        players[i].hand[0] = OpenCard(deckTmp[i * 2].first, deckTmp[i * 2].second);
        players[i].hand[1] = OpenCard(deckTmp[i * 2 + 1].first, deckTmp[i * 2 + 1].second);
    }

    for (unsigned i = playersN * 2; i < playersN * 2 + 5; i++) {
        deck.push_back(CloseCard(deckTmp[i * 2].first, deckTmp[i * 2].second));
    }
}

void TexasHoldem::openNext() {
    CloseCard chosen = deck.back();
    deck.erase(--deck.end());
    board.push_back(OpenCard(chosen.s, chosen.rank));
}

BoardState TexasHoldem::getState(unsigned player, std::vector <unsigned> inGame) {
    int isActive = (int) std::count(inGame.begin(), inGame.end(), player);

    BoardState bs;
    bs.enemyN = (unsigned int) (inGame.size() - isActive);
    bs.pot = pot;
    bs.smallBlind = smallBlind;
    bs.plBet = players[player].currBet;
    bs.plCash = players[player].cash;
    for (unsigned i = 0; i < inGame.size(); i++) {
        if (inGame[i] != player) {
            bs.bets.push_back(players[inGame[i]].currBet);
            bs.cash.push_back(players[inGame[i]].cash);
            bs.names.push_back(players[inGame[i]].p->getName());
        }
    }
    bs.hand = new OpenCard[2];
    memcpy(bs.hand, players[player].hand, sizeof(OpenCard) * 2);
    bs.board = board;

    return bs;
}

void TexasHoldem::run() {
    inGame = refresh();
    dealer = 0;
    pot = 0;

    while (inGame.size() > 1) {
        dealer = myUpperBound(inGame, (dealer + 1) % playersN);
        shuffle();

        doRound();
        pot = 0;
        inGame = refresh();
    }
}
