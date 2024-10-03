#include "Card.hpp"
#include <array>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <random>
#include <chrono>
#include "Pack.hpp"

using namespace std;


  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack() : next(0) {
    int index = 0;
    for (int suit = SPADES; suit <= DIAMONDS; suit++){
      for (int rank = NINE; rank <= ACE; rank++){
        cards[index++] = Card(static_cast<Rank>(rank),static_cast<Suit>(suit));
      }
    }

  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(std::istream& pack_input): next(0){
    int index = 0;
    string c_rank;
    string c_suit;
    string c_of;

    while(pack_input >> c_rank>> c_of >> c_suit){
      assert(c_of == "of");

      Rank rank = string_to_rank(c_rank);
      Suit suit = string_to_suit(c_suit);

      assert(rank >= NINE && rank <= ACE);
      assert(suit >= SPADES && suit <= DIAMONDS);

       cards[index++] = Card(rank, suit);

    }
    assert(index == PACK_SIZE);
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one(){
    assert(next < PACK_SIZE);
    return cards[next++];
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset(){
    next = 0;
    
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle(){
    for (int i = 0; i < 7; i++){
      // creat a new array to store
      std::array<Card, PACK_SIZE> shuffled;

      int mid = PACK_SIZE/2;
      for (int j = 0; j < mid; j++){
        shuffled[2*j] = cards[j+mid];
        shuffled[2*j+1] = cards[j];
      }
      // copy back
      std::copy(shuffled.begin(), shuffled.end(), cards.begin());
    }
    next = 0;
  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
    if(next == PACK_SIZE){
      return true;
    }
    return false;
  }

