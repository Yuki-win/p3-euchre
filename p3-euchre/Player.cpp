#include "Player.hpp"
#include "Card.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

////////////////// Simple Player Implementation ////////////////
class SimplePlayer : public Player {
public:
    const std::vector<Card>& get_hand() const {
        return hand;
    }

    SimplePlayer(const std::string& name_in) : name(name_in) {}

    const std::string &get_name() const override {
        return name;
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
        if (round == 1) {
            int trump_count = count_trump_cards(upcard.get_suit());
            if (trump_count >= 2) {
                order_up_suit = upcard.get_suit();
                return true;
            }
        } else if (round == 2) {
            Suit next_suit = Suit_next(upcard.get_suit());
            if (count_trump_cards(next_suit) >= 1 || is_dealer) {
                order_up_suit = next_suit;
                return true;
            }
        }
        return false;
    }

    Card lead_card(Suit trump) override {
        // 找到最高的非主牌
        auto it = std::max_element(hand.begin(), hand.end(),
            [trump](const Card &a, const Card &b) {
                if (!a.is_trump(trump) && !b.is_trump(trump)) {
                    return a < b;  // 比较非主牌
                }
                return a.is_trump(trump) && !b.is_trump(trump); // 让主牌比非主牌"大"
            });

        // 如果有非主牌，则出最高的非主牌
        if (it != hand.end() && !it->is_trump(trump)) {
            Card to_play = *it; // 选择要出的牌
            hand.erase(it); // 从手牌中删除
            return to_play; // 返回出的牌
        }

        // 否则出最高的主牌
        return play_highest_trump(trump);
    }

    Card play_card(const Card &led_card, Suit trump) override {
        Suit led_suit = led_card.get_suit(trump);

        // 找到最高的 led suit 牌
        auto it = std::max_element(hand.begin(), hand.end(),
            [led_suit, trump](const Card &a, const Card &b) {
                if (a.get_suit(trump) == led_suit && b.get_suit(trump) == led_suit) {
                    return a < b;  // 比较相同花色的牌
                }
                return false;  
            });

        // 如果有 led suit 牌，出最高的那张
        if (it != hand.end() && it->get_suit(trump) == led_suit) {
            Card to_play = *it;
            hand.erase(it);
            return to_play;
        }

        // 否则出最小的牌
        return play_lowest_card();
    }

    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard); // 添加 upcard 到手牌

        // 丢弃最小的牌，如果手牌超过 5 张
        if (hand.size() > 5) {
            auto it = std::min_element(hand.begin(), hand.end()); // 找到最小的牌
            hand.erase(it); // 删除最小的牌
        }
    }

    void add_card(const Card &card) override {
        hand.push_back(card);
        std::sort(hand.begin(), hand.end()); // 确保手牌有序
    }

private:
    std::string name;
    std::vector<Card> hand;

    int count_trump_cards(Suit trump) const {
        return std::count_if(hand.begin(), hand.end(), 
                             [trump](const Card &card) {
                                 return card.is_trump(trump);
                             });
    }

    Card play_highest_trump(Suit trump) {
        auto it = std::max_element(hand.begin(), hand.end(), 
            [trump](const Card &a, const Card &b) {
                return Card_less(a, b, trump);
            });
        Card to_play = *it;
        hand.erase(it);
        return to_play;
    }

    Card play_lowest_card() {
        auto it = std::min_element(hand.begin(), hand.end());
        Card to_play = *it;
        hand.erase(it);
        return to_play;
    }
};

////////////////// Human Player Implementation ////////////////

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name_in) : name(name_in) {}

    const std::string &get_name() const override {
        return name;
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
        print_hand();
        cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
        std::string decision;
        cin >> decision;

        if (decision != "pass") {
            order_up_suit = string_to_suit(decision);
            return true;
        }
        return false;
    }

    Card lead_card(Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int index;
        cin >> index;

        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }

    Card play_card(const Card &led_card, Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int index;
        cin >> index;

        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }

    void add_and_discard(const Card &upcard) override {
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";
        int index;
        cin >> index;

        if (index == -1) {
            // Discard the upcard
        } else {
            hand.push_back(upcard);
            hand.erase(hand.begin() + index);
        }
    }

    void add_card(const Card &card) override {
        hand.push_back(card);
        std::sort(hand.begin(), hand.end());
    }

private:
    std::string name;
    std::vector<Card> hand;

    void print_hand() const {
        for (size_t i = 0; i < hand.size(); ++i) {
            cout << "[" << i << "] " << hand[i] << "\n";
        }
    }
};

////////////////// Player Factory //////////////////

Player* Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Simple") {
        return new SimplePlayer(name);  // Return a Simple AI Player
    }
    if (strategy == "Human") {
        return new HumanPlayer(name);   // Return a Human-controlled Player
    }

    // If an invalid strategy is provided, trigger an assertion failure
    assert(false);
    return nullptr;  // To prevent compiler warnings
}

std::ostream &operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}
