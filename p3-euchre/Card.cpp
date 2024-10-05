#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"
#include <string>
using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
// Default constructor initializes to Two of Spades
Card::Card() : rank(TWO), suit(SPADES) {}

// Constructor to initialize Card with a specified rank and suit
Card::Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in) {}

// Get the rank of the card
Rank Card::get_rank() const {
    return rank;
}

// Get the suit of the card (no trump considered)
Suit Card::get_suit() const {
    return suit;
}

// Get the suit of the card considering the trump suit
Suit Card::get_suit(Suit trump) const {
    // If it's the left bower, treat it as trump suit
    if (is_left_bower(trump)) {
        return trump;
    }
    return suit;
}

// Check if the card is a face card (Jack, Queen, King, or Ace)
bool Card::is_face_or_ace() const {
    return rank == JACK || rank == QUEEN || rank == KING || rank == ACE;
}

// Check if the card is the right bower (Jack of the trump suit)
bool Card::is_right_bower(Suit trump) const {
    return rank == JACK && suit == trump;
}

// Check if the card is the left bower (Jack of the same color suit as trump)
bool Card::is_left_bower(Suit trump) const {
    return rank == JACK && suit == Suit_next(trump);
}

// Check if the card is a trump card (either from trump suit or left bower)
bool Card::is_trump(Suit trump) const {
    // Right bower (Jack of the trump suit) is always trump
    if (is_right_bower(trump)) {
        return true;
    }

    // Left bower (Jack of the next suit) is trump, but only if the trump is the opposite color
    if (is_left_bower(trump)) {
        return true;
    }

    // Regular trump suit cards are trump
    return suit == trump;
}

///////////////////// Operator Overloading ///////////////////////////

// Overload the << operator to print the card in the form "Rank of Suit"
std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << RANK_NAMES[card.get_rank()] << " of " << SUIT_NAMES[card.get_suit()];
    return os;
}

// Overload the >> operator to read a card from input in the form "Rank of Suit"
std::istream &operator>>(std::istream &is, Card &card) {
    std::string rank_str, suit_str, of_str;
    is >> rank_str >> of_str >> suit_str;
    card.rank = string_to_rank(rank_str);
    card.suit = string_to_suit(suit_str);
    return is;
}

// Overload the < operator to compare cards by rank and suit
bool operator<(const Card &lhs, const Card &rhs) {
    if (lhs.get_rank() < rhs.get_rank()) {
        return true;
    }
    if (lhs.get_rank() == rhs.get_rank()) {
        return lhs.get_suit() < rhs.get_suit();
    }
    return false;
}

// Overload the <= operator
bool operator<=(const Card &lhs, const Card &rhs) {
    return lhs < rhs || lhs == rhs;
}

// Overload the > operator
bool operator>(const Card &lhs, const Card &rhs) {
    return !(lhs <= rhs);
}

// Overload the >= operator
bool operator>=(const Card &lhs, const Card &rhs) {
    return !(lhs < rhs);
}

// Overload the == operator
bool operator==(const Card &lhs, const Card &rhs) {
    return lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit();
}

// Overload the != operator
bool operator!=(const Card &lhs, const Card &rhs) {
    return !(lhs == rhs);
}

///////////////////// Utility Functions /////////////////////////////

// Get the next suit of the same color as the input suit
Suit Suit_next(Suit suit) {
    switch (suit) {
        case SPADES: return CLUBS;
        case CLUBS: return SPADES;
        case HEARTS: return DIAMONDS;
        case DIAMONDS: return HEARTS;
        default: assert(false); return SPADES;  // Should never reach here
    }
}

// Compare two cards, considering trump
bool Card_less(const Card &a, const Card &b, Suit trump) {
    // Check for bower (special trump rules)
    if (a.is_right_bower(trump)) return false;  // Right bower is always the highest
    if (b.is_right_bower(trump)) return true;
    if (a.is_left_bower(trump)) return false;
    if (b.is_left_bower(trump)) return true;

    // Compare by trump status
    bool a_trump = a.is_trump(trump);
    bool b_trump = b.is_trump(trump);
    if (a_trump && !b_trump) return false;
    if (!a_trump && b_trump) return true;

    // If both are non-trump or both are trump, compare by rank
    return a.get_rank() < b.get_rank();
}

// Compare two cards considering the trump suit and the led suit
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
    // If one card follows the led suit and the other doesn't, led suit card wins
    bool a_follows_led = a.get_suit(trump) == led_card.get_suit(trump);
    bool b_follows_led = b.get_suit(trump) == led_card.get_suit(trump);
    if (a_follows_led && !b_follows_led) return false;
    if (!a_follows_led && b_follows_led) return true;

    // Otherwise, use normal trump logic
    return Card_less(a, b, trump);
}
