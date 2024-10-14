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
    SimplePlayer(const std::string& name_in) : name(name_in) {}

    const std::string &get_name() const override {
        return name;
    }

    // Function to decide whether to make trump
    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
        int high_trumps = 0;
        Suit upcard_suit = upcard.get_suit();
        Suit next_suit = Suit_next(upcard_suit);
        Suit trump_suit;

        if (round == 1) {
            trump_suit = upcard_suit;
        } else {
            trump_suit = next_suit;
        }

        // Count the number of high trump cards (Jack or higher)
        for (const Card& card : hand) {
            if (card.is_trump(trump_suit)) {
                if (card.get_rank() == JACK ||
                    card.get_rank() == ACE ||
                    card.get_rank() == KING ||
                    card.get_rank() == QUEEN ||
                    card.get_rank() == TEN) {
                    ++high_trumps;
                }
            }
        }

        if (round == 1) {
            if (high_trumps >= 2) {
                order_up_suit = trump_suit;
                return true;
            }
        } else {
            if (high_trumps >= 1) {
                order_up_suit = trump_suit;
                return true;
            }
            if (is_dealer) {
                order_up_suit = trump_suit;
                return true;
            }
        }
        return false;
    }

    // Lead a card
    Card lead_card(Suit trump) override {
        // Find highest non-trump card
        auto it = std::max_element(hand.begin(), hand.end(),
            [trump](const Card &a, const Card &b) {
                if (!a.is_trump(trump) && !b.is_trump(trump)) {
                    return a < b;  // Compare non-trump cards
                }
                if (!a.is_trump(trump)) {
                    return false; // a is non-trump, b is trump
                }
                if (!b.is_trump(trump)) {
                    return true;  // b is non-trump, a is trump
                }
                return Card_less(a, b, trump); // Both are trump
            });

        if (it != hand.end() && !it->is_trump(trump)) {
            // Play highest non-trump card
            Card to_play = *it;
            hand.erase(it);
            return to_play;
        } else {
            // Play highest trump card
            auto it_trump = std::max_element(hand.begin(), hand.end(),
                [trump](const Card &a, const Card &b) {
                    return Card_less(a, b, trump);
                });
            Card to_play = *it_trump;
            hand.erase(it_trump);
            return to_play;
        }
    }

    // Play a card
    Card play_card(const Card &led_card, Suit trump) override {
        Suit led_suit = led_card.get_suit(trump);

        // Collect cards that follow suit
        std::vector<Card> follow_suit_cards;
        for (const Card& card : hand) {
            if (card.get_suit(trump) == led_suit) {
                follow_suit_cards.push_back(card);
            }
        }

        if (!follow_suit_cards.empty()) {
            // Play highest card in led suit
            auto it = std::max_element(follow_suit_cards.begin(), follow_suit_cards.end(),
                [trump](const Card &a, const Card &b) {
                    return Card_less(a, b, trump);
                });
            Card to_play = *it;
            // Remove from hand
            auto hand_it = std::find(hand.begin(), hand.end(), to_play);
            hand.erase(hand_it);
            return to_play;
        } else {
            // Play lowest card
            return play_lowest_card(trump);
        }
    }

    // Add a card to the hand and discard the lowest card if hand is full
    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard); // Add upcard to hand

        // If there are more than 5 cards, find and discard the lowest card
        if (hand.size() > MAX_HAND_SIZE) {
            // Find the lowest card in hand to discard
            auto it = std::min_element(hand.begin(), hand.end(),
                [](const Card &a, const Card &b) {
                    return a < b;
                });
            // If the upcard is the lowest, discard it
            if (*it == upcard) {
                hand.pop_back(); // Discard upcard
            } else {
                hand.erase(it);  // Discard the lowest card
            }
        }
    }

    // Add a card to the player's hand
    void add_card(const Card &card) override {
        hand.push_back(card);
        std::sort(hand.begin(), hand.end());  // Ensure hand is sorted
    }

private:
    static const int MAX_HAND_SIZE = 5;
    std::string name;
    std::vector<Card> hand;

    // Helper function to play the lowest card
    Card play_lowest_card(Suit trump) {
        auto it = std::min_element(hand.begin(), hand.end(),
            [trump](const Card &a, const Card &b) {
                return Card_less(a, b, trump);
            });
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
        cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;
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
        cout << "Human player " << name << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }
    Card play_card(const Card &led_card, Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }
    void add_and_discard(const Card &upcard) override {
        print_hand();
        cout << "Discard upcard: [-1]" << endl;
        cout << "Human player " << name << ", please select a card to discard:" << endl;
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
            cout << "[" << i << "] " << hand[i] << endl;
        }
    }
};

////////////////// Player Factory Implementation ////////////////
Player* Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Simple") {
        return new SimplePlayer(name);  // Return a Simple AI Player
    }
    if (strategy == "Human") {
        return new HumanPlayer(name);   // Return a Human-controlled Player
    }
    // If an invalid strategy is provided, throw an exception
    throw std::invalid_argument("Invalid strategy: " + strategy);
}

// Operator overload for printing a player's name
std::ostream &operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}


// new 10.14
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
    SimplePlayer(const std::string& name_in) : name(name_in) {}

    const std::string &get_name() const override {
        return name;
    }

    // Function to decide whether to make trump
    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
        int high_trumps = 0;
        Suit upcard_suit = upcard.get_suit();
        Suit next_suit = Suit_next(upcard_suit);
        Suit trump_suit;

        if (round == 1) {
            trump_suit = upcard_suit;
        } else {
            trump_suit = next_suit;
        }

        // Count the number of high trump cards (Jack or higher)
        for (const Card& card : hand) {
            if (card.is_trump(trump_suit)) {
                if (card.get_rank() == JACK ||
                    card.get_rank() == ACE ||
                    card.get_rank() == KING ||
                    card.get_rank() == QUEEN ||
                    card.get_rank() == TEN) {
                    ++high_trumps;
                }
            }
        }

        if (round == 1) {
            if (high_trumps >= 2) {
                order_up_suit = trump_suit;
                return true;
            }
        } else {
            if (high_trumps >= 1) {
                order_up_suit = trump_suit;
                return true;
            }
            if (is_dealer) {
                order_up_suit = trump_suit;
                return true;
            }
        }
        return false;
    }

    // Lead a card
    Card lead_card(Suit trump) override {
        // Find highest non-trump card
        auto it = std::max_element(hand.begin(), hand.end(),
            [trump](const Card &a, const Card &b) {
                if (!a.is_trump(trump) && !b.is_trump(trump)) {
                    return a < b;  // Compare non-trump cards
                }
                if (!a.is_trump(trump)) {
                    return false; // a is non-trump, b is trump
                }
                if (!b.is_trump(trump)) {
                    return true;  // b is non-trump, a is trump
                }
                return Card_less(a, b, trump); // Both are trump
            });

        if (it != hand.end() && !it->is_trump(trump)) {
            // Play highest non-trump card
            Card to_play = *it;
            hand.erase(it);
            return to_play;
        } else {
            // Play highest trump card
            auto it_trump = std::max_element(hand.begin(), hand.end(),
                [trump](const Card &a, const Card &b) {
                    return Card_less(a, b, trump);
                });
            Card to_play = *it_trump;
            hand.erase(it_trump);
            return to_play;
        }
    }


    // Play a card
    Card play_card(const Card &led_card, Suit trump) override {
        assert(hand.size() >= 1);
        Suit led_suit = led_card.get_suit();
        vector<Card> led_suit_card;
        bool have_led = false;
        std::sort(hand.begin(), hand.end());
        int min = 0;
        int max = 0;
        // check for led suit
        for (int i = 0; i < hand.size(); i++){
            if (hand[i].get_suit() == led_suit){
                have_led = true;
                if (Card_less(hand[max], hand[i], led_card,trump)){
                    
                    max =i;
                }
            }
        }
        if (have_led){
        Card led_return = hand[max];
        hand.erase(hand.begin() + max);
        return led_return;
        }
        
        // if no led card in hand
   
        for (int i = 0; i < hand.size(); i++){
            if(Card_less(hand[i], hand[min], trump)){
                min = i;
            }
        }
        Card non_led_return = hand[min];
        hand.erase((hand.begin() + min));
        return non_led_return;
        
    }

    // Add a card to the hand and discard the lowest card if hand is full
    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard); // Add upcard to hand
        Suit trump = upcard.get_suit();
        int min = 0;
        int size = hand.size();

        for (int i = 1; i < size; ++i){
            if (Card_less(hand[i], hand[min],trump)){
                min = i;
            }
        }
        if(hand.size() > MAX_HAND_SIZE){
            hand.erase(hand.begin() + min);
        }

    }

    // Add a card to the player's hand
    void add_card(const Card &card) override {
        hand.push_back(card);
        std::sort(hand.begin(), hand.end());  // Ensure hand is sorted
    }

private:
    static const int MAX_HAND_SIZE = 5;
    std::string name;
    std::vector<Card> hand;

    // Helper function to play the lowest card
    Card play_lowest_card(Suit trump) {
        auto it = std::min_element(hand.begin(), hand.end(),
            [trump](const Card &a, const Card &b) {
                return Card_less(a, b, trump);
            });
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
        cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;
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
        cout << "Human player " << name << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }
    Card play_card(const Card &led_card, Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:" << endl;
        int index;
        cin >> index;
        Card to_play = hand[index];
        hand.erase(hand.begin() + index);
        return to_play;
    }
    void add_and_discard(const Card &upcard) override {
        print_hand();
        cout << "Discard upcard: [-1]" << endl;
        cout << "Human player " << name << ", please select a card to discard:" << endl;
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
            cout << "[" << i << "] " << hand[i] << endl;
        }
    }
};

////////////////// Player Factory Implementation ////////////////
Player* Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Simple") {
        return new SimplePlayer(name);  // Return a Simple AI Player
    }
    if (strategy == "Human") {
        return new HumanPlayer(name);   // Return a Human-controlled Player
    }
    // If an invalid strategy is provided, throw an exception
    throw std::invalid_argument("Invalid strategy: " + strategy);
}

// Operator overload for printing a player's name
std::ostream &operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}
