#include "Pack.hpp"
#include "Card.hpp"
#include <array>
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

// 默认构造函数：初始化牌堆，按照标准顺序创建一副牌
Pack::Pack() {
  reset();  // 重置牌堆的下一个索引
  int num = 0;
  // 定义花色的顺序数组
  const Suit suits[] = {SPADES, HEARTS, CLUBS, DIAMONDS};
  // 定义牌面值的顺序数组
  const Rank ranks[] = {NINE, TEN, JACK, QUEEN, KING, ACE};
  // 按照花色和牌面值的顺序创建牌堆
  for (int s = 0; s < 4; s++) {
    for (int r = 0; r < 6; r++) {
      cards[num] = Card(ranks[r], suits[s]);
      num++;
    }
  }
}

// 从输入流构造牌堆，从文件中读取牌的顺序
Pack::Pack(std::istream& pack_input) {
  reset();  // 重置牌堆的下一个索引
  for (int num = 0; num < PACK_SIZE; num++) {
    pack_input >> cards[num];
  }
}

// 发一张牌
Card Pack::deal_one() {
  assert(next < PACK_SIZE);  // 确保还有牌可以发
  return cards[next++];
}

// 重置牌堆，将 next 设为 0
void Pack::reset() {
  next = 0;
}

// 洗牌函数，按照特定的算法洗牌，共洗 7 次
void Pack::shuffle() {
  next = 0;
  std::array<Card, PACK_SIZE> temp;
  const size_t half_size = PACK_SIZE / 2;
  // 进行 7 次洗牌
  for (size_t shuffle_count = 0; shuffle_count < 7; shuffle_count++) {
    // 交替放置牌到临时数组
    // i 从 0 到 half_size - 1（0 到 11）
    for (size_t i = 0; i < half_size; i++) {
      temp[2 * i] = cards[half_size + i]; 
      temp[2 * i + 1] = cards[i];
    }
    // 将临时数组复制回原牌堆
    for (size_t i = 0; i < PACK_SIZE; i++) {
      cards[i] = temp[i];
    }
  }
}

bool Pack::empty() const {
  return (next == PACK_SIZE);
}

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
    if(next == PACK_SIZE){
      return true;
    }
    return false;
  }

