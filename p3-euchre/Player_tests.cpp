#include "Player.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <vector>

using namespace std;

// Test Player's get_name() function
TEST(test_player_get_name) {
    Player *player1 = Player_factory("Player1", "Simple");
    Player *player2 = Player_factory("Player2", "Human");

    ASSERT_EQUAL("Player1", player1->get_name());
    ASSERT_EQUAL("Player2", player2->get_name());

    ostringstream oss1;
    oss1 << *player1;
    ostringstream oss2;
    oss2 << *player2;
    ASSERT_EQUAL(oss1.str(), "Player1");
    ASSERT_EQUAL(oss2.str(), "Player2");

    delete player1;
    delete player2;
}

TEST(test_player_make_trump) {
    Player *player1 = Player_factory("Player1", "Simple");
    player1->add_card(Card(NINE, SPADES));
    player1->add_card(Card(QUEEN, SPADES));
    player1->add_card(Card(KING, SPADES));
    player1->add_card(Card(ACE, SPADES));
    player1->add_card(Card(TEN, SPADES));

    Card nine_spadesa(NINE, SPADES);
    Suit trumpa;
    bool orderupa = player1->make_trump(
        nine_spadesa, true, 1,trumpa           
    );
    ASSERT_TRUE(orderupa);
    ASSERT_EQUAL(trumpa, SPADES);

    Card nine_spadesa2(NINE, HEARTS);
    Suit trumpa2;
    bool orderupa2 = player1->make_trump(
        nine_spadesa2,    // Upcard
        true,           // 玩家是庄家
        1,              // 第一轮
        trumpa2           // 叫主的花色（如果选择）
    );
    ASSERT_TRUE(!orderupa2);

    delete player1;

    Player *player2 = Player_factory("Player2", "Simple");
    player2->add_card(Card(NINE, SPADES));
    player2->add_card(Card(QUEEN, SPADES));
    player2->add_card(Card(KING, SPADES));
    player2->add_card(Card(ACE, SPADES));
    player2->add_card(Card(TEN, SPADES));

    Card nine_spadesc(NINE, CLUBS);
    Suit trumpc;
    bool orderupc = player2->make_trump(
        nine_spadesc,    // Upcard
        false,           // 玩家不是庄家
        2,              // 第二轮
        trumpc           // 叫主的花色（如果选择）
    );
    ASSERT_TRUE(orderupc);
    ASSERT_EQUAL(trumpc, SPADES);

    Card nine_spadesc2(NINE, HEARTS);
    Suit trumpc2;
    bool orderupc2 = player2->make_trump(
        nine_spadesc2,    // Upcard
        true,           // 玩家是庄家
        2,              // 第二轮
        trumpc2           // 叫主的花色（如果选择）
    );
    ASSERT_TRUE(orderupc2);
    ASSERT_EQUAL(trumpc2, DIAMONDS);

    Card nine_spadesc3(NINE, HEARTS);
    Suit trumpc3;
    bool orderupc3 = player2->make_trump(
        nine_spadesc3,    // Upcard
        false,           // 玩家不是庄家
        2,              // 第二轮
        trumpc3           // 叫主的花色（如果选择）
    );
    ASSERT_TRUE(!orderupc3);

    delete player2;
  

}

TEST(test_player_add_and_discard) {
    Player* p = Player_factory("alice", "Simple");
    // Add 5 cards
    p->add_card(Card(NINE, SPADES));  // This should be discarded
    p->add_card(Card(KING, HEARTS));  // Keep
    p->add_card(Card(JACK, DIAMONDS)); // Keep
    p->add_card(Card(ACE, CLUBS));    // Keep
    p->add_card(Card(TEN, HEARTS));   // Keep
    // Add an upcard
    Card upcard(QUEEN, SPADES); 
    p->add_and_discard(upcard);
    // Check if the smallest card (NINE of SPADES) was discarded
    // Here, you would expect the hand to no longer contain NINE of SPADES.
    // You might need to check through `lead_card()` or other public functions.
    Card led_card = p->lead_card(SPADES);
    ASSERT_TRUE(led_card != Card(NINE, SPADES));


    // 检查 bob 的出牌
    Player *bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, SPADES));
    bob->add_card(Card(TEN, SPADES));

    // 添加第六张牌 (upcard)
    bob->add_and_discard(Card(JACK, SPADES)); // 添加 upcard
    Card card_ledb = bob->lead_card(SPADES); // 领牌为 SPADES
    Card jack_spades(JACK, SPADES);
    ASSERT_EQUAL(card_ledb, jack_spades); // 应该打出 JACK of SPADES

    delete bob; // 释放内存

    // 检查 cindy 的出牌
    Player *cindy = Player_factory("Cindy", "Simple");
    cindy->add_card(Card(NINE, SPADES));
    cindy->add_card(Card(QUEEN, SPADES));
    cindy->add_card(Card(KING, SPADES));
    cindy->add_card(Card(ACE, SPADES));
    cindy->add_card(Card(TEN, SPADES));

    // 添加第六张牌 (upcard)
    cindy->add_and_discard(Card(EIGHT, SPADES)); // 添加 upcard
    Card ledc(ACE, HEARTS); // 领牌为 HEARTS
    Card nine_spades(NINE, SPADES);
    Card playc = cindy->play_card(ledc, SPADES); // 牌是 SPADES
    ASSERT_EQUAL(playc, nine_spades); // 应该打出 NINE of SPADES

    delete cindy; // 释放内存
    delete p;
}

TEST_MAIN();
