#include "Player.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <vector>

using namespace std;

// Test Player's get_name() function
TEST(test_player_get_name) {
    Player *alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Test SimplePlayer making trump in round 1
TEST(test_simple_player_make_trump_round_1) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with two trump cards
    Card ace_spades(ACE, SPADES);
    Card jack_spades(JACK, SPADES);
    alice->add_card(ace_spades);
    alice->add_card(jack_spades);

    // Test making trump with upcard in round 1
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_TRUE(alice->make_trump(upcard, false, 1, order_up_suit));
    ASSERT_EQUAL(SPADES, order_up_suit);

    delete alice;
}

// Test SimplePlayer making trump in round 2
TEST(test_simple_player_make_trump_round_2) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with one trump card (next suit)
    Card king_diamonds(KING, DIAMONDS);
    alice->add_card(king_diamonds);

    // Test making trump in round 2 (next suit is Diamonds)
    Card upcard(QUEEN, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    ASSERT_TRUE(alice->make_trump(upcard, false, 2, order_up_suit));
    ASSERT_EQUAL(DIAMONDS, order_up_suit);  // Diamonds is the next suit (same color as Hearts)

    delete alice;
}

// Test SimplePlayer not ordering up in round 1
TEST(test_simple_player_pass_trump_round_1) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no trump cards
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);

    // Test passing on making trump
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));

    delete alice;
}

// Test SimplePlayer leading a trick
TEST(test_simple_player_lead_card) {
    Player *alice = Player_factory("Alice", "Simple");

    // Add some non-trump cards and trump cards
    Card queen_clubs(QUEEN, CLUBS);
    Card ace_clubs(ACE, CLUBS);
    Card king_spades(KING, SPADES);  // Trump
    alice->add_card(queen_clubs);
    alice->add_card(ace_clubs);
    alice->add_card(king_spades);

    // Test leading non-trump card
    Card lead = alice->lead_card(SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_clubs, lead);  // Should lead the highest non-trump card (Ace of Clubs)

    delete alice;
}

// Test SimplePlayer playing a card
TEST(test_simple_player_play_card) {
    Player *alice = Player_factory("Alice", "Simple");

    // Add some cards to the player's hand
    Card queen_clubs(QUEEN, CLUBS);
    Card ace_clubs(ACE, CLUBS);
    Card king_spades(KING, SPADES);  // Trump
    alice->add_card(queen_clubs);
    alice->add_card(ace_clubs);
    alice->add_card(king_spades);

    // Test playing a card (must follow led suit)
    Card led_card(NINE, CLUBS);  // Led suit is Clubs
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_clubs, played);  // Should play the highest card of Clubs (led suit)

    delete alice;
}

#include "Player.hpp"  // 确保包含 Player.hpp 头文件
#include "Card.hpp"
#include "unit_test_framework.hpp"

TEST(test_simple_player_lead_highest_non_trump) {
    Player* player = Player_factory("TestPlayer", "Simple");  // 使用 Player_factory 创建 SimplePlayer 实例
    player->add_card(Card(ACE, SPADES));
    player->add_card(Card(KING, HEARTS)); 
    player->add_card(Card(QUEEN, SPADES)); 
    player->add_card(Card(JACK, HEARTS));
    player->add_card(Card(TEN, DIAMONDS));

    Card led_card = player->lead_card(SPADES);  // Trump is spades
    ASSERT_EQUAL(led_card, Card(KING, HEARTS));  // Expect highest non-trump card to be led

    delete player;  // 确保删除指针，避免内存泄漏
}

TEST(test_simple_player_follow_suit) {
    Player* player = Player_factory("TestPlayer", "Simple");  // 使用 Player_factory 创建 SimplePlayer 实例
    player->add_card(Card(ACE, HEARTS));
    player->add_card(Card(KING, HEARTS)); 
    player->add_card(Card(QUEEN, SPADES)); 
    player->add_card(Card(JACK, HEARTS));
    player->add_card(Card(TEN, DIAMONDS));

    Card led_card = Card(KING, HEARTS);  // Leading hearts
    Card played_card = player->play_card(led_card, SPADES);  // Trump is spades
    ASSERT_EQUAL(played_card, Card(ACE, HEARTS));  // Player must follow suit and play highest card

    delete player;
}



TEST_MAIN()
