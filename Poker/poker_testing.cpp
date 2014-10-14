#include <fstream>
#include <time.h>
#include <stdlib.h>

#include "gtest/gtest.h"
#include "texas_holdem.h"
#include "game_factory.cpp"
#include "human_player.h"
#include "textui.h"

namespace {
    const int N = 1000;
    const char *evaluateFile = "evaluateCards.txt",
               *pickFile = "pickCards.txt";

    // read m pairs of two numbers (cards) from file
    // 0 is clubs, 1 is diamonds, 2 is hearts and 3 is spades
    // m is the first number in file
    std::pair<size_t, OpenCard*> readCards(const char* path) {
        std::ifstream fin(path);
        int m, rank, s;
        fin >> m;
        OpenCard* out = new OpenCard[m];
        for (int i = 0; i < m; i++) {
            fin >> rank >> s;
            out[i] = OpenCard((suit)s, rank);
        }

        return std::make_pair(m, out);
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

TEST(correctness, constructor) {
    TextUI* ui = new TextUI(true);
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10, ui,
            new HumanPlayer("First player", ui), new HumanPlayer("Second player", ui));
    delete game;
}

TEST(correctness, shuffleTest) {
    TextUI* ui = new TextUI(true);
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10, ui,
            new HumanPlayer("First player", ui), new HumanPlayer("Second player", ui));
    OpenCard cards[9];
    for (int i = 0; i < N; i++) {
        game->shuffle();
        for (int j = 0; j < 5; j++) {
            game->openNext();
            cards[j] = game->board.back();
        }
        cards[5] = game->players[0].hand[0];
        cards[6] = game->players[0].hand[1];
        cards[7] = game->players[1].hand[0];
        cards[8] = game->players[1].hand[1];

        for (int j = 0; j < 9; j++) {
            EXPECT_TRUE(cards[j].getRank() >= 2);
            EXPECT_TRUE(cards[j].getRank() <= 14);
            EXPECT_TRUE((int)cards[j].getSuit() <= 4);
            EXPECT_EQ(1, std::count(cards, cards + 9, cards[j]));
        }
    }
    delete game;
}

// Combinations compare to each over, order has to be equal to following:
// (less first)
//     2♣ 4♦ 6♥ 8♠ 10♣ (highest card)
//     3♣ 4♦ 6♥ 8♠ 10♣ (highest card & kicker)
//     2♣ 4♦ 6♥ 8♠ Q♣ (highest card)
//     2♣ 2♦ 6♥ 8♠ Q♣ (pair)
//     2♣ 2♦ 7♥ 8♠ Q♣ (pair & kicker)
//     3♣ 3♦ 2♥ 4♠ 6♣ (pair)
//     2♣ 2♦ 3♥ 3♠ 6♣ (two pairs)
//     2♣ 2♦ 3♥ 3♠ A♣ (two pairs & kicker)
//     K♣ K♦ Q♥ Q♠ 6♣ (two pairs)
//     A♣ A♦ Q♥ Q♠ 6♣ (two pairs)
//     2♣ 2♦ 2♥ 3♠ 6♣ (triple)
//     2♣ 2♦ 2♥ 4♠ 6♣ (triple & kicker)
//     3♣ 3♦ 3♥ 2♠ 4♣ (triple)
//     2♣ 3♦ 4♥ 5♠ 6♣ (straight)
//     3♣ 4♦ 5♥ 6♠ 7♣ (straight)
//     3♣ 2♦ A♥ K♠ Q♣ (straight)
//     4♣ 3♦ 2♥ A♠ K♣ (straight)
//     2♣ 4♣ 6♣ 8♣ 10♣ (flush)
//     3♠ 4♠ 6♠ 8♠ 10♠ (flush & kicker)
//     2♣ 2♦ 2♥ 3♠ 3♣ (full house)
//     2♣ 2♦ 2♥ A♠ A♣ (full house)
//     2♣ 2♦ 3♥ 3♠ 3♣ (full house)
//     2♣ 2♦ 2♥ 2♠ 3♣ (four of a kind)
//     2♣ 2♦ 2♥ 2♠ A♣ (four of a kind)
//     3♣ 3♦ 3♥ 3♠ 2♣ (four of a kind)
//     2♥ 3♥ 4♥ 5♥ 6♥ (straight flush)
//     3♦ 2♦ A♦ K♦ Q♦ (straight flush)
//     A♥ K♥ Q♥ J♥ 10♥ (royal flush)
TEST(correctness, evaluateCombination) {
    TextUI* ui = new TextUI(true);
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10, ui,
            new HumanPlayer("First player", ui), new HumanPlayer("Second player", ui));
    std::pair<size_t, OpenCard*> out = readCards(evaluateFile);
    OpenCard *testCards = out.second;

    srand (time(NULL));
    for (size_t i = 0; i < out.first / 5; i++) {
        for (int j = 0; j < 5; j++) {
            std::swap(testCards[5 * i + j], testCards[5 * i + rand() % 5]);
        }
    }

    unsigned *valueI, *valueJ;
    for (size_t i = 0; i < out.first / 5; i++) {
        valueI = game->evaluate(&testCards[5 * i]);
        for (size_t j = 0; j < out.first / 5; j++) {
            valueJ = game->evaluate(&testCards[5 * j]);
            if (i != j) {
                EXPECT_EQ(i < j ? -1 : 1, unsigned7cmp(valueI, valueJ));
                if ((i < j ? -1 : 1) != unsigned7cmp(valueI, valueJ)) {
                    std::cout << i << " " << j << "\n";
                }
            }
            else {
                EXPECT_EQ(0, unsigned7cmp(valueI, valueJ));
            }
            delete[] valueJ;
        }
        delete[] valueI;
    }

    delete[] testCards;
    delete game;
}

// highestComb should pick first five cards of 7 cards combination:
//     5♣ 4♦ K♥ 8♠ 10♣ 2♣ 3♠ (highest card)
//     9♥ 9♠ A♠ 8♠ J♦ 3♠ 5♥ (pair)
//     5♣ 4♣ A♣ 8♣ 10♣ 5♠ 3♠ (flush, not the pair)
//     5♣ 4♣ A♣ 8♣ 10♣ 5♠ 4♠ (flush, not the two pairs)
//     4♣ 5♦ 6♥ 7♠ 8♣ 2♣ 3♠ (straight with 8 highest, not the one with 6 highest)
//     3♣ 3♦ 4♥ 4♠ A♣ 2♣ 2♠ (3&4 pairs, without 2)
//     A♣ A♦ A♥ 2♠ 2♣ K♣ 2♠ (full house with A triple, not with 2 triple)
TEST(correctness, pickCombination) {
    TextUI* ui = new TextUI(true);
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10, ui,
            new HumanPlayer("First player", ui), new HumanPlayer("Second player", ui));
    std::pair<size_t, OpenCard*> out = readCards(pickFile);
    OpenCard *testCards = out.second;
    OpenCard *shuffledCards = new OpenCard[out.first];
    memcpy(shuffledCards, out.second, out.first * sizeof(OpenCard));

    srand (time(NULL));
    for (size_t i = 0; i < out.first / 7; i++) {
        for (int j = 0; j < 7; j++) {
            std::swap(shuffledCards[7 * i + j], shuffledCards[7 * i + rand() % 7]);
        }
    }

    for (unsigned i = 0; i < out.first / 7; i++) {
        std::pair<OpenCard*, unsigned*> result = game->highestComb(&shuffledCards[i * 7]);
        for (unsigned j = 0; j < 5; j++) {
            if (std::count(testCards + i * 7, testCards + i * 7 + 5, result.first[j]) != 1) {
                int x;
                x++;
            }
            EXPECT_EQ(1, std::count(testCards + i * 7, testCards + i * 7 + 5, result.first[j]));
        }
        delete[] result.first;
        delete[] result.second;
    }

    delete[] testCards;
    delete[] shuffledCards;
    delete game;
}
