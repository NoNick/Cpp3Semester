#include <iostream>

#include "textui.h"

namespace {
    std::string rankToString(unsigned in) {
        switch (in) {
        case 14:
            return "A";
        case 13:
            return "K";
        case 12:
            return "Q";
        case 11:
            return "J";
        default:
            return std::to_string(in);
        }
    }
}

TextUI::TextUI(bool ui) { noUI = ui; }

void TextUI::printState(const Player& p, const BoardState& bs) {
    if (!noUI) {
        std::cout << "========" << p.getName() << "'s state========\n";
        bs.print();
    }
}

void TextUI::print(std::string s) {
    if (!noUI) {
        std::cout << s;
    }
}

void TextUI::call(Player *p, unsigned to, unsigned bet) {
    if (!noUI) {
        std::cout << p->getName() << " calls total bet to " << to << ": adds " << bet << std::endl;
    }
}

void TextUI::raise(Player *p, unsigned to, unsigned bet) {
    if (!noUI) {
        std::cout << p->getName() << " raises total bet to " << to << ": adds " << bet << std::endl;
    }
}

void TextUI::check(Player *p) {
    if (!noUI) {
        std::cout << p->getName() << " checks" << std::endl;
    }
}

void TextUI::fold(Player *p) {
    if (!noUI) {
        std::cout << p->getName() << " folds" << std::endl;
    }
}

void TextUI::smallBlind(Player *p, unsigned value) {
    if (!noUI) {
        std::cout << p->getName() << " did small blind: adds " << value << std::endl;
    }
}

void TextUI::bigBlind(Player *p, unsigned value) {
    if (!noUI) {
        std::cout << p->getName() << " did big blind: adds " << value << std::endl;
    }
}

void TextUI::showdown(Player *p, OpenCard *cards, unsigned *value) {
    if (!noUI) {
        std::cout << p->getName() << " has ";
        switch (value[0]) {
        case 8:
            if (value[1] == 14) {
                std::cout << "royal flush";
            }
            else {
                std::cout << "straight flush with " << rankToString(value[1]) << " highest";
            }
            break;
        case 7:
            std::cout << "four of a kind of " << rankToString(value[1]) << " and " << rankToString(value[2]) << " kicker";
            break;
        case 6:
            std::cout << "full house with " << rankToString(value[1] / 14) << " and " << rankToString(value[1] % 14) << " highest";
            break;
        case 5:
            std::cout << "flush with " << rankToString(value[1]) << " highest";
            break;
        case 4:
            std::cout << "straight with " << rankToString(value[1]) << " highest";
            break;
        case 3:
            std::cout << "triple of " << rankToString(value[1]) << " and " <<
                                    rankToString(value[2]) << ", " << rankToString(value[3]) << "kickers";
            break;
        case 2:
            std::cout << "two pairs: " << rankToString(value[1] / 14) << " and " << rankToString(value[1] % 14) << "; " <<
                                          rankToString(value[2]) << " kicker";
            break;
        case 1:
            std::cout << "pair of " << rankToString(value[1]) << " with " << rankToString(value[2]) << ", " << rankToString(value[3]) <<
                                        ", " << rankToString(value[4]) << " kickers";
            break;
        case 0:
            std::cout << rankToString(value[1]) << " highest card with " << rankToString(value[2]) << ", " << rankToString(value[3]) << ", " <<
                                        rankToString(value[4]) << ", " << rankToString(value[5]) << " kickers";
            break;
        default:
            break;
        }

        std::cout << " (combination:";
        for (int i = 0; i < 5; i++)
            std::cout << " " << cards[i].toString();
        std::cout << ")\n";
    }
}

void TextUI::win(std::vector<Player *> winners) {
    if (!noUI) {
        if (winners.size() == 1) {
            std::cout << "Winner:";
        }
        else {
            std::cout << "Winners:";
        }
        for (auto it = winners.begin(); it != winners.end(); it++) {
            std::cout << " " << (*it)->getName();
        }
        std::cout << "\n";
    }
}

unsigned TextUI::askForUnsigned(Player *p, unsigned minBet) {
    if (!noUI) {
        std::cout << "[" << p->getName() << "] Enter your bet (min is " << minBet << ", 0 for check/fold): ";
        std::string s;
        std::getline(std::cin, s);
        // here is might be an exceptions: std::invalid_argument, std::out_of_range
        return std::stoul(s);
    }
    else
        return 0;
}

