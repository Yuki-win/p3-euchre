#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

using namespace std;

// Rank 操作符的实现 - 不要更改
// RANK_NAMES 数组存储所有的牌面值名称
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

// REQUIRES: str 表示一个有效的牌面值 ("Two", "Three", ..., "Ace")
// EFFECTS: 返回与字符串对应的 Rank，例如 "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for (int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // 输入字符串不匹配任何牌面值
  return {};
}

// EFFECTS: 将 Rank 输出到流，例如 "Two"
std::ostream &operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

// REQUIRES: 输入必须是一个有效的牌面值
// EFFECTS: 从流中读取一个 Rank，例如 "Two" -> TWO
std::istream &operator>>(std::istream &is, Rank &rank) {
  string str;
  if (is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}

// Suit 操作符的实现 - 不要更改
// SUIT_NAMES 数组存储所有的花色名称
constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

// REQUIRES: str 表示一个有效的花色 ("Spades", "Hearts", "Clubs", 或 "Diamonds")
// EFFECTS: 返回与字符串对应的 Suit，例如 "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for (int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // 输入字符串不匹配任何花色
  return {};
}

// EFFECTS: 将 Suit 输出到流，例如 "Spades"
std::ostream &operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

// REQUIRES: 输入必须是一个有效的花色
// EFFECTS: 从流中读取一个 Suit，例如 "Spades" -> SPADES
std::istream &operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}

// 写下你对 Card 类的实现
// Card 默认构造函数，初始化为 TWO of SPADES
Card::Card()
    : rank(TWO), suit(SPADES) {}

// Card 构造函数，使用指定的 Rank 和 Suit
Card::Card(Rank rank_in, Suit suit_in)
    : rank(rank_in), suit(suit_in) {}

// 获取牌的 Rank
Rank Card::get_rank() const {
  return rank;
}

// 获取牌的 Suit
Suit Card::get_suit() const {
  return suit;
}

// 获取牌的 Suit，如果是左 Bauer，则返回 trump
Suit Card::get_suit(Suit trump) const {
  if (is_left_bower(trump)) {
    return trump;
  }
  return suit;
}

// 判断牌是否为人头牌或 Ace
bool Card::is_face_or_ace() const {
  return (rank == JACK || rank == QUEEN || rank == KING || rank == ACE);
}

// 判断牌是否为右 Bauer（最高的 Trump 牌）
bool Card::is_right_bower(Suit trump) const {
  return (suit == trump && rank == JACK);
}

// 判断牌是否为左 Bauer（第二高的 Trump 牌）
bool Card::is_left_bower(Suit trump) const {
  return (rank == JACK && suit == Suit_next(trump));
}

// 判断牌是否为 Trump 牌
bool Card::is_trump(Suit trump) const {
  return (suit == trump || is_left_bower(trump));
}

// 重载输出运算符，将 Card 输出到流，例如 "Ace of Spades"
std::ostream &operator<<(std::ostream &os, const Card &card) {
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

// 重载输入运算符，从流中读取 Card
std::istream &operator>>(std::istream &is, Card &card) {
  string rank_str, of_str, suit_str;
  if (is >> rank_str >> of_str >> suit_str) {
    card.rank = string_to_rank(rank_str);
    card.suit = string_to_suit(suit_str);
  }
  return is;
}

// 重载小于运算符，根据 Rank 和 Suit 比较两张牌
bool operator<(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() < rhs.get_rank()) {
    return true;
  } else if (lhs.get_rank() == rhs.get_rank()) {
    return lhs.get_suit() < rhs.get_suit();
  } else {
    return false;
  }
}

// 重载小于等于运算符
bool operator<=(const Card &lhs, const Card &rhs) {
  return (lhs < rhs || lhs == rhs);
}

// 重载大于运算符
bool operator>(const Card &lhs, const Card &rhs) {
  return !(lhs <= rhs);
}

// 重载大于等于运算符
bool operator>=(const Card &lhs, const Card &rhs) {
  return !(lhs < rhs);
}

// 重载等于运算符
bool operator==(const Card &lhs, const Card &rhs) {
  return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}

// 重载不等于运算符
bool operator!=(const Card &lhs, const Card &rhs) {
  return !(lhs == rhs);
}

// 获取下一个同色花色
Suit Suit_next(Suit suit) {
  if (suit == DIAMONDS) {
    return HEARTS;
  } else if (suit == HEARTS) {
    return DIAMONDS;
  } else if (suit == CLUBS) {
    return SPADES;
  } else { // SPADES
    return CLUBS;
  }
}

// 比较两张牌的大小，考虑 Trump
bool Card_less(const Card &a, const Card &b, Suit trump) {
  if (a.is_right_bower(trump)) {
    return false;
  } else if (b.is_right_bower(trump)) {
    return true;
  } else if (a.is_left_bower(trump)) {
    return false;
  } else if (b.is_left_bower(trump)) {
    return true;
  } else if (a.is_trump(trump) && b.is_trump(trump)) {
    return a.get_rank() < b.get_rank();
  } else if (a.is_trump(trump)) {
    return false;
  } else if (b.is_trump(trump)) {
    return true;
  } else {
    if (a.get_rank() < b.get_rank()) {
      return true;
    } else if (a.get_rank() == b.get_rank()) {
      return a.get_suit() < b.get_suit();
    } else {
      return false;
    }
  }
}

// 比较两张牌的大小，考虑 Trump 和 led_card
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
  if (a.is_right_bower(trump)) {
    return false;
  } else if (b.is_right_bower(trump)) {
    return true;
  } else if (a.is_left_bower(trump)) {
    return false;
  } else if (b.is_left_bower(trump)) {
    return true;
  } else if (a.is_trump(trump) && !b.is_trump(trump)) {
    return false;
  } else if (!a.is_trump(trump) && b.is_trump(trump)) {
    return true;
  } else if (a.get_suit(trump) == led_card.get_suit(trump) &&
             b.get_suit(trump) != led_card.get_suit(trump)) {
    return false;
  } else if (a.get_suit(trump) != led_card.get_suit(trump) &&
             b.get_suit(trump) == led_card.get_suit(trump)) {
    return true;
  } else {
    return a < b;
  }
}
