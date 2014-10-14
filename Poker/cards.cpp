#include "cards.h"

OpenCard::OpenCard() {}

OpenCard::OpenCard(suit const x, int const y) {
    s = x;
    rank = y;
}

int OpenCard::getRank()const {
    return rank;
}

suit OpenCard::getSuit()const {
    return s;
}

CloseCard::CloseCard() {}

CloseCard::CloseCard(suit const x, int const y) {
    s = x;
    rank = y;
}

bool OpenCard::operator==(OpenCard const &another) {
    return rank == another.getRank() && s == another.getSuit();
}

std::string OpenCard::toString() const {
    std::string rankStr;
    switch (rank) {
        case 14:
            rankStr = "A";
            break;
        case 13:
            rankStr = "K";
            break;
        case 12:
            rankStr = "Q";
            break;
        case 11:
            rankStr = "J";
            break;
        default:
            rankStr = std::to_string(rank);
            break;
    }
//    std::string suitChars[] = {"♣", "♦", "♥", "♠"};
    std::string suitChars[] = {" clubs", " diamonds", " hearts", " spades"};
    return (rankStr + suitChars[(int)s]);
}
