#include "gtest/gtest.h"
#include "texas_holdem.h"
#include "game_factory.cpp"
#include "human_player.h"

namespace {
    const int N = 1000;
}

TEST(correctness, constructor) {
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10,
            new HumanPlayer("First player"), new HumanPlayer("Second player"));
    delete game;
}

TEST(correctness, shuffleTest) {
    TexasHoldem* game = createGame((unsigned)2, (unsigned)100, (unsigned) 10,
            new HumanPlayer("First player"), new HumanPlayer("Second player"));
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
