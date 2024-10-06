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


TEST(test_simple_player_cannot_follow_suit) {
    Player* player = Player_factory("TestPlayer", "Simple");  // 使用 Player_factory 创建 SimplePlayer 实例
    player->add_card(Card(ACE, SPADES));
    player->add_card(Card(KING, SPADES)); 
    player->add_card(Card(QUEEN, SPADES)); 
    player->add_card(Card(JACK, SPADES));
    player->add_card(Card(TEN, DIAMONDS));

    Card led_card = Card(KING, HEARTS);  // Leading hearts
    Card played_card = player->play_card(led_card, SPADES);  // Trump is spades
    ASSERT_EQUAL(played_card, Card(TEN, DIAMONDS));  // Player must play lowest card since they can't follow suit

    delete player;
}

TEST(test_simple_player_lead_trump_when_only_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with only trump cards
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    alice->add_card(ace_spades);
    alice->add_card(king_spades);

    // Test leading when only trump cards are available
    Card lead = alice->lead_card(SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_spades, lead);  // Should lead the highest trump card

    delete alice;
}
TEST(test_simple_player_cannot_follow_led) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no cards matching the led suit
    Card nine_clubs(NINE, CLUBS);
    Card king_spades(KING, SPADES);  // Trump
    alice->add_card(nine_clubs);
    alice->add_card(king_spades);

    // Test playing a card when led suit is Hearts
    Card led_card(QUEEN, HEARTS);  // Led suit is Hearts
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(nine_clubs, played);  // Should play the lowest non-trump card

    delete alice;
}
TEST(test_simple_player_follow_led_with_highest_card) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with multiple cards of the led suit
    Card ten_clubs(TEN, CLUBS);
    Card ace_clubs(ACE, CLUBS);
    alice->add_card(ten_clubs);
    alice->add_card(ace_clubs);

    // Test playing a card when led suit is Clubs
    Card led_card(QUEEN, CLUBS);  // Led suit is Clubs
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_clubs, played);  // Should play the highest card of the led suit

    delete alice;
}
TEST(test_simple_player_make_trump_with_one_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with one trump card
    Card king_spades(KING, SPADES);
    alice->add_card(king_spades);

    // Test making trump with upcard in round 1
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));  // Player should pass if only one trump

    delete alice;
}
TEST(test_simple_player_pass_make_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no trump cards
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);

    // Test passing on making trump
    Card upcard(JACK, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));  // Should pass without trump cards

    delete alice;
}


TEST(test_simple_player_lead_highest_non_trump_when_trump_available) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with trump and non-trump cards
    Card ace_clubs(ACE, CLUBS);
    Card king_spades(KING, SPADES);  // Trump
    alice->add_card(ace_clubs);
    alice->add_card(king_spades);

    // Test leading a card
    Card lead = alice->lead_card(SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_clubs, lead);  // Should lead the highest non-trump card

    delete alice;
}
TEST(test_simple_player_follow_trump_when_led) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with trump cards
    Card ace_spades(ACE, SPADES);  // Trump
    Card nine_spades(NINE, SPADES);  // Trump
    alice->add_card(ace_spades);
    alice->add_card(nine_spades);

    // Test playing a card when led suit is trump
    Card led_card(KING, SPADES);  // Led suit is Spades (Trump)
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_spades, played);  // Should play the highest trump card

    delete alice;
}

TEST(test_simple_player_lead_highest_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with only trump cards
    Card nine_spades(NINE, SPADES);
    Card ace_spades(ACE, SPADES);
    alice->add_card(nine_spades);
    alice->add_card(ace_spades);

    // Test leading when only trump cards are available
    Card lead = alice->lead_card(SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_spades, lead);  // Should lead the highest trump card

    delete alice;
}
TEST(test_simple_player_compare_multiple_trumps) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with multiple trump cards
    Card jack_spades(JACK, SPADES);  // Right Bower
    Card nine_spades(NINE, SPADES);  // Trump
    alice->add_card(jack_spades);
    alice->add_card(nine_spades);

    // Test playing a card when led suit is trump
    Card led_card(KING, SPADES);  // Led suit is Spades (Trump)
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(jack_spades, played);  // Right Bower should be played (highest trump)

    delete alice;
}
TEST(test_simple_player_pass_make_trump_with_no_trump_cards) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no trump cards
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);

    // Test passing on making trump
    Card upcard(JACK, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));  // Should pass without trump cards

    delete alice;
}
TEST(test_simple_player_must_make_trump_as_dealer_in_round_2) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no trump cards
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);

    // Test making trump in round 2 when player is the dealer and forced to choose
    Card upcard(JACK, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    ASSERT_TRUE(alice->make_trump(upcard, true, 2, order_up_suit));  // Dealer must choose a suit

    delete alice;
}
TEST(test_simple_player_cannot_follow_led_suit_play_lowest_card) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no cards matching the led suit
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);

    // Test playing a card when led suit is Hearts
    Card led_card(QUEEN, HEARTS);  // Led suit is Hearts
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(nine_clubs, played);  // Should play the lowest non-trump card

    delete alice;
}
TEST(test_simple_player_follow_trump_when_led_suit_is_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with trump cards
    Card ace_spades(ACE, SPADES);  // Trump
    Card king_spades(KING, SPADES);  // Trump
    alice->add_card(ace_spades);
    alice->add_card(king_spades);

    // Test playing a card when led suit is trump
    Card led_card(JACK, SPADES);  // Led suit is Spades (Trump)
    Card played = alice->play_card(led_card, SPADES);  // Spades is trump
    ASSERT_EQUAL(ace_spades, played);  // Should play the highest trump card

    delete alice;
}
TEST(test_simple_player_make_trump_with_bowers) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with right bower and left bower
    Card jack_spades(JACK, SPADES);  // Right Bower
    Card jack_clubs(JACK, CLUBS);  // Left Bower
    alice->add_card(jack_spades);
    alice->add_card(jack_clubs);

    // Test making trump with upcard
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_TRUE(alice->make_trump(upcard, false, 1, order_up_suit));
    ASSERT_EQUAL(SPADES, order_up_suit);  // Should choose spades as trump due to having bowers

    delete alice;
}

TEST_MAIN()
