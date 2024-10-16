#include "Player.hpp"
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

/////////////////////// SimplePlayer 类的实现 ///////////////////////
class SimplePlayer : public Player {
private:
  std::string name;            // 玩家姓名
  std::vector<Card> hand;      // 玩家手牌

public:
  // 构造函数，使用给定的姓名初始化玩家
  SimplePlayer(const std::string& name_in) : name(name_in) {}

  // 返回玩家的姓名
  const std::string& get_name() const override {
    return name;
  }

  // 向玩家手牌中添加一张牌
  void add_card(const Card& c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
  }

  // 玩家决定是否叫主
  bool make_trump(const Card& upcard, bool is_dealer,
                  int round, Suit& order_up_suit) const override {
    assert(round == 1 || round == 2);
    Suit potential_trump;
    int face_card_count = 0;

    if (round == 1) {
      potential_trump = upcard.get_suit();
      // 计算手牌中潜在主花色的高牌数量（JACK 或更高）
      for (const auto& card : hand) {
        if (card.is_trump(potential_trump) && card.is_face_or_ace()) {
          face_card_count++;
        }
      }
      if (face_card_count >= 2) {
        order_up_suit = potential_trump;
        return true;
      }
    } else if (round == 2) {
      potential_trump = Suit_next(upcard.get_suit());
      // 检查手牌中是否有潜在主花色的高牌（JACK 或更高）
      for (const auto& card : hand) {
        if (card.is_trump(potential_trump) && card.is_face_or_ace()) {
          order_up_suit = potential_trump;
          return true;
        }
      }
      // 如果是庄家，强制叫主
      if (is_dealer) {
        order_up_suit = potential_trump;
        return true;
      }
    }
    return false;
  }

  // 庄家加牌并弃牌
  void add_and_discard(const Card& upcard) override {
    assert(hand.size() >= 1);
    hand.push_back(upcard);  // 将翻开的牌加入手牌

    // 找到手牌中最小的牌
    auto min_it = hand.begin();
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (Card_less(*it, *min_it, upcard.get_suit())) {
        min_it = it;
      }
    }
    // 弃掉最小的牌
    hand.erase(min_it);
  }

  // 领先出牌
  Card lead_card(Suit trump) override {
    assert(hand.size() >= 1);
    // 查找非主牌中最大的牌
    auto max_non_trump_it = hand.end();
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (!it->is_trump(trump)) {
        if (max_non_trump_it == hand.end() || Card_less(*max_non_trump_it, *it, trump)) {
          max_non_trump_it = it;
        }
      }
    }
    // 如果有非主牌，出最大的非主牌
    if (max_non_trump_it != hand.end()) {
      Card lead = *max_non_trump_it;
      hand.erase(max_non_trump_it);
      return lead;
    } else {
      // 否则，出最大的主牌
      auto max_trump_it = hand.begin();
      for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (Card_less(*max_trump_it, *it, trump)) {
          max_trump_it = it;
        }
      }
      Card lead = *max_trump_it;
      hand.erase(max_trump_it);
      return lead;
    }
  }

  // 跟牌
  Card play_card(const Card& led_card, Suit trump) override {
    assert(hand.size() >= 1);
    Suit led_suit = led_card.get_suit(trump);
    // 查找是否有跟出的花色
    std::vector<Card>::iterator play_it = hand.end();
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (it->get_suit(trump) == led_suit) {
        if (play_it == hand.end() || Card_less(*play_it, *it, led_card, trump)) {
          play_it = it;
        }
      }
    }
    // 如果有跟出的花色，出最大的那张
    if (play_it != hand.end()) {
      Card play = *play_it;
      hand.erase(play_it);
      return play;
    } else {
      // 否则，出最小的牌
      auto min_it = hand.begin();
      for (auto it = hand.begin() + 1; it != hand.end(); ++it) {
        if (Card_less(*it, *min_it, led_card, trump)) {
          min_it = it;
        }
      }
      Card play = *min_it;
      hand.erase(min_it);
      return play;
    }
  }

  // 析构函数
  ~SimplePlayer() override = default;
};

/////////////////////// HumanPlayer 类的实现 ///////////////////////
class HumanPlayer : public Player {
private:
  std::string name;            // 玩家姓名
  std::vector<Card> hand;      // 玩家手牌

  // 打印玩家手牌
  void print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
      cout << "Human player " << name << "'s hand: "
           << "[" << i << "] " << hand[i] << "\n";
    }
  }

  // 对手牌进行排序
  void sort_hand() {
    std::sort(hand.begin(), hand.end());
  }

public:
  // 构造函数，使用给定的姓名初始化玩家
  HumanPlayer(const std::string& name_in) : name(name_in) {}

  // 返回玩家的姓名
  const std::string& get_name() const override {
    return name;
  }

  // 向玩家手牌中添加一张牌
  void add_card(const Card& c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
    sort_hand();
  }

  // 玩家决定是否叫主
  bool make_trump(const Card& upcard, bool is_dealer,
                  int round, Suit& order_up_suit) const override {
    print_hand();
    cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
    std::string decision;
    std::cin >> decision;

    if (decision != "pass") {
      order_up_suit = string_to_suit(decision);
      return true;
    } else {
      return false;
    }
  }

  // 庄家加牌并弃牌
  void add_and_discard(const Card& upcard) override {
    assert(hand.size() > 0);
    print_hand();
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n\n";
    int index;
    std::cin >> index;
    if (index == -1) {
      return;  // 弃掉翻开的牌
    } else {
      hand.push_back(upcard);
      hand.erase(hand.begin() + index);
      sort_hand();
    }
  }

  // 领先出牌
  Card lead_card(Suit trump) override {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    int index;
    std::cin >> index;
    Card lead_card = hand[index];
    hand.erase(hand.begin() + index);
    return lead_card;
  }

  // 跟牌
  Card play_card(const Card& led_card, Suit trump) override {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    int index;
    std::cin >> index;
    Card play_card = hand[index];
    hand.erase(hand.begin() + index);
    return play_card;
  }

  // 析构函数
  ~HumanPlayer() override = default;
};

/////////////////////// Player_factory 函数实现 ///////////////////////
// 创建玩家工厂函数，根据策略创建相应的玩家对象
Player* Player_factory(const std::string& name,
                       const std::string& strategy) {
  if (strategy == "Simple") {
    // 创建 SimplePlayer 对象
    return new SimplePlayer(name);
  }
  if (strategy == "Human") {
    // 创建 HumanPlayer 对象
    return new HumanPlayer(name);
  }
  // 如果策略无效，终止程序
  assert(false);
  return nullptr;
}

// 重载输出运算符，输出玩家姓名
std::ostream& operator<<(std::ostream& os, const Player& p) {
  os << p.get_name();
  return os;
}
