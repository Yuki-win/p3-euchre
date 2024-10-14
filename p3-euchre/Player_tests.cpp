#include "Player.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <vector>

using namespace std;
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
// Test SimplePlayer making trump when there are no trump cards
TEST(test_simple_player_no_trump_cards) {
    Player *alice = Player_factory("Alice", "Simple");
    // Simulate a hand with no trump cards
    alice->add_card(Card(NINE, CLUBS));
    alice->add_card(Card(TEN, DIAMONDS));
    // Test passing on making trump
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));
    delete alice;
}
// Test adding duplicate cards
TEST(test_simple_player_add_duplicate_card) {
    Player *alice = Player_factory("Alice", "Simple");
    // Add a card
    Card ace_spades(ACE, SPADES);
    alice->add_card(ace_spades);
    alice->add_card(ace_spades); // Add the same card again
    // Check if the card is still in hand
    ASSERT_EQUAL(alice->play_card(Card(ACE, SPADES), SPADES), Card(ACE, SPADES));
    delete alice;
}

TEST(test_player_make_trumps) {
    Player * player1 = Player_factory("Alice", "Simple");
    player1->add_card(Card(NINE, SPADES));
    player1->add_card(Card(QUEEN, SPADES));
    player1->add_card(Card(KING, SPADES));
    player1->add_card(Card(ACE, SPADES));
    player1->add_card(Card(TEN, SPADES));

    Card upcard1(NINE, SPADES);
    Suit trump1;
    bool orderUp1 = player1->make_trump(
        upcard1,    // Upcard
        true,           // Dealer
        1,              // First round
        trump1          // Suit ordered up (if any)
    );
    ASSERT_TRUE(orderUp1);
    ASSERT_EQUAL(trump1, SPADES);

    Card upcard2(NINE, HEARTS);
    Suit trump2;
    bool orderUp2 = player1->make_trump(
        upcard2,    // Upcard
        true,           // Dealer
        1,              // First round
        trump2          // Suit ordered up (if any)
    );
    ASSERT_TRUE(!orderUp2);

    bool orderUp3 = player1->make_trump(
        upcard2,    // Upcard
        false,          // Not dealer
        1,              // First round
        trump2          // Suit ordered up (if any)
    );
    ASSERT_TRUE(!orderUp3);

    delete player1;

    Player * player2 = Player_factory("Bob", "Simple");
    player2->add_card(Card(JACK, SPADES));
    player2->add_card(Card(EIGHT, SPADES));
    player2->add_card(Card(SEVEN, SPADES));
    player2->add_card(Card(SIX, SPADES));
    player2->add_card(Card(FIVE, SPADES));

    Card upcard3(ACE, SPADES);
    Suit trump3;
    bool orderUp4 = player2->make_trump(
        upcard3,    // Upcard
        true,           // Dealer
        1,              // First round
        trump3          // Suit ordered up (if any)
    );
    ASSERT_TRUE(!orderUp4);
    delete player2;

    Player * player3 = Player_factory("Ellen", "Simple");
    player3->add_card(Card(QUEEN, SPADES));
    player3->add_card(Card(EIGHT, SPADES));
    player3->add_card(Card(SEVEN, SPADES));
    player3->add_card(Card(SIX, SPADES));
    player3->add_card(Card(JACK, CLUBS));

    Card upcard4(ACE, SPADES);
    Suit trump4;
    bool orderUp5 = player3->make_trump(
        upcard4,    // Upcard
        true,           // Dealer
        1,              // First round
        trump4          // Suit ordered up (if any)
    );
    ASSERT_TRUE(orderUp5);
    ASSERT_EQUAL(trump4, SPADES);
    delete player3;

    Player * player4 = Player_factory("Cindy", "Simple");
    player4->add_card(Card(NINE, SPADES));
    player4->add_card(Card(QUEEN, SPADES));
    player4->add_card(Card(KING, SPADES));
    player4->add_card(Card(ACE, SPADES));
    player4->add_card(Card(TEN, SPADES));

    Card upcard5(NINE, CLUBS);
    Suit trump5;
    bool orderUp6 = player4->make_trump(
        upcard5,    // Upcard
        false,          // Not dealer
        2,              // Second round
        trump5          // Suit ordered up (if any)
    );
    ASSERT_TRUE(orderUp6);
    ASSERT_EQUAL(trump5, SPADES);

    Card upcard6(NINE, HEARTS);
    Suit trump6;
    bool orderUp7 = player4->make_trump(
        upcard6,    // Upcard
        true,           // Dealer
        2,              // Second round
        trump6          // Suit ordered up (if any)
    );
    ASSERT_TRUE(orderUp7);
    ASSERT_EQUAL(trump6, DIAMONDS);

    Card upcard7(NINE, HEARTS);
    Suit trump7;
    bool orderUp8 = player4->make_trump(
        upcard7,    // Upcard
        false,          // Not dealer
        2,              // Second round
        trump7          // Suit ordered up (if any)
    );
    ASSERT_TRUE(!orderUp8);
    delete player4;

    Player * player5 = Player_factory("Dany", "Simple");
    player5->add_card(Card(NINE, SPADES));
    player5->add_card(Card(QUEEN, HEARTS));
    player5->add_card(Card(KING, SPADES));
    player5->add_card(Card(ACE, SPADES));
    player5->add_card(Card(TEN, SPADES));

    Card upcard8(NINE, DIAMONDS);
    Suit trump8;
    bool orderUp9 = player5->make_trump(
        upcard8,    // Upcard
        false,          // Not dealer
        2,              // Second round
        trump8          // Suit ordered up (if any)
    );
    ASSERT_TRUE(orderUp9);
    ASSERT_EQUAL(trump8, HEARTS);
    delete player5;
}



TEST(test_player_add_and_discard) {
    
    Player * p1 = Player_factory("p1", "Simple");
    p1->add_card(Card(NINE, SPADES));
    p1->add_card(Card(TEN, SPADES));
    p1->add_card(Card(ACE, SPADES));
    p1->add_card(Card(QUEEN, SPADES));
    p1->add_card(Card(TEN, SPADES));
    p1->add_and_discard(
    Card(NINE, HEARTS) 
    );
    Card card_led1 = p1->lead_card(HEARTS);
    Card ace_spades(ACE, SPADES);
    ASSERT_EQUAL(card_led1, ace_spades); 

    Card led(KING, SPADES);
    Card king_spades(QUEEN, SPADES);
    Card play1 = p1->play_card(led, SPADES); // highest in suit
    ASSERT_EQUAL(play1, king_spades); 
    delete p1;

    // discard smallest
    Player * p2 = Player_factory("p2", "Simple");
    p2->add_card(Card(KING, SPADES));
    p2->add_card(Card(QUEEN, SPADES));
    p2->add_card(Card(TEN, SPADES));
    p2->add_card(Card(EIGHT, SPADES));
    p2->add_card(Card(NINE, SPADES));
    p2->add_and_discard(
    Card(JACK, SPADES) 
    );
    Card card_led2 = p2->lead_card(SPADES);
    Card jack_spades(JACK, SPADES);
    ASSERT_EQUAL(card_led2, jack_spades); 

    Card card_ledb2 = p2->lead_card(HEARTS);
    Card king_spades2(KING, SPADES);
    ASSERT_EQUAL(card_ledb2, king_spades2); 
    delete p2;

    // discard upcard
    Player * p3 = Player_factory("p3", "Simple");
    p3->add_card(Card(NINE, SPADES));
    p3->add_card(Card(QUEEN, SPADES));
    p3->add_card(Card(KING, SPADES));
    p3->add_card(Card(ACE, SPADES));
    p3->add_card(Card(TEN, SPADES));
    p3->add_and_discard(
    Card(SIX, SPADES) // upcard discard upcard
    );
    Card led3(KING, CLUBS);
    Card nine_spades(NINE, SPADES);
    Card play3 = p3->play_card(led3, SPADES); // not discard card in hand
    ASSERT_EQUAL(play3, nine_spades);

    delete p3;

    Player * p4 = Player_factory("p4", "Simple");
    p4->add_card(Card(EIGHT, SPADES));
    p4->add_card(Card(KING, HEARTS));
    p4->add_card(Card(KING, CLUBS));
    p4->add_card(Card(ACE, DIAMONDS));
    p4->add_card(Card(TEN, SPADES));
    p4->add_and_discard(
    Card(SIX, SPADES) // upcard discard upcard
    );
    Card led4(ACE, HEARTS);
    Card king_clubs(KING, CLUBS);
    Card play4 = p4->play_card(led4, SPADES); // lowest in hand
    ASSERT_EQUAL(play4, king_clubs);

    delete p4;
}


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
  Player *player3 = Player_factory("Player3", "Simple");
player3->add_card(Card(NINE, HEARTS));
player3->add_card(Card(KING, CLUBS));    // Added another trump card
player3->add_card(Card(KING, DIAMONDS));
player3->add_card(Card(ACE, CLUBS));
player3->add_card(Card(TEN, SPADES));

Card upcard1(JACK, CLUBS);
Suit trump1;
bool orderup1 = player3->make_trump(
    upcard1, false, 1, trump1
);
ASSERT_TRUE(orderup1);
ASSERT_EQUAL(trump1, CLUBS);

// Cleanup
delete player3;

}



TEST_MAIN();
