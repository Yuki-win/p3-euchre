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

// Test playing a card when unable to follow suit
TEST(test_simple_player_play_card_unable_to_follow_suit) {
    Player *alice = Player_factory("Alice", "Simple");

    // Add cards that do not match the led suit
    alice->add_card(Card(ACE, SPADES)); // Trump
    alice->add_card(Card(KING, SPADES)); // Trump
    alice->add_card(Card(QUEEN, SPADES)); // Trump
    alice->add_card(Card(JACK, SPADES)); // Trump
    alice->add_card(Card(TEN, DIAMONDS)); // Non-trump

    Card led_card(KING, HEARTS); // Leading hearts
    Card played_card = alice->play_card(led_card, SPADES); // Trump is spades
    ASSERT_EQUAL(played_card, Card(TEN, DIAMONDS)); // Player must play the lowest card since they can't follow suit

    delete alice;
}

// New: Test SimplePlayer making trump with no cards (empty hand case)
TEST(test_simple_player_make_trump_empty_hand) {
    Player *alice = Player_factory("Alice", "Simple");

    // Test making trump with no cards in hand
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));  // Should pass due to empty hand

    delete alice;
}

// New: Test SimplePlayer with a full hand of non-trump cards in round 1
TEST(test_simple_player_pass_trump_full_non_trump_hand) {
    Player *alice = Player_factory("Alice", "Simple");

    // Simulate a hand with no trump cards
    Card nine_clubs(NINE, CLUBS);
    Card ten_diamonds(TEN, DIAMONDS);
    Card jack_hearts(JACK, HEARTS);
    Card seven_clubs(SEVEN, CLUBS);
    alice->add_card(nine_clubs);
    alice->add_card(ten_diamonds);
    alice->add_card(jack_hearts);
    alice->add_card(seven_clubs);

    // Test passing on making trump
    Card upcard(QUEEN, SPADES);  // Spades is proposed as trump
    Suit order_up_suit;
    ASSERT_FALSE(alice->make_trump(upcard, false, 1, order_up_suit));  // Should pass

    delete alice;
}


TEST(test_pass_trump_no_trump_cards) {
    Player *player = Player_factory("Player2", "Simple");

    // Player has no trump cards
    player->add_card(Card(NINE, HEARTS));
    player->add_card(Card(ACE, CLUBS));

    // Upcard and trump
    Card upcard(QUEEN, SPADES);
    Suit order_up_suit;

    // Player should pass on trump
    ASSERT_FALSE(player->make_trump(upcard, false, 1, order_up_suit));

    delete player;
}
TEST(test_right_bower_wins_trick) {
    Player *player1 = Player_factory("Player1", "Simple");
    Player *player2 = Player_factory("Player2", "Simple");

    // Add cards
    player1->add_card(Card(JACK, SPADES));  // Right bower
    player2->add_card(Card(ACE, HEARTS));

    // Led suit is Hearts, but Spades is trump
    Card led_card(ACE, HEARTS);
    Card played_card = player1->play_card(led_card, SPADES);  // Spades is trump

    ASSERT_EQUAL(played_card, Card(JACK, SPADES));  // Right bower should be played

    delete player1;
    delete player2;
}
TEST(test_follow_led_suit) {
    Player *player = Player_factory("Player3", "Simple");

    // Add cards
    player->add_card(Card(NINE, DIAMONDS));
    player->add_card(Card(ACE, SPADES));  // Trump card

    // Led suit is Diamonds
    Card led_card(TEN, DIAMONDS);
    Card played_card = player->play_card(led_card, SPADES);  // Trump is Spades

    ASSERT_EQUAL(played_card, Card(NINE, DIAMONDS));  // Should follow led suit

    delete player;
}



// New: Test SimplePlayer play_highest_trump with only trump cards
TEST(test_simple_player_play_highest_trump) {
    Player *alice = Player_factory("Alice", "Simple");

    // Add all trump cards
    alice->add_card(Card(ACE, SPADES));
    alice->add_card(Card(KING, SPADES));
    alice->add_card(Card(QUEEN, SPADES));

    // Test lead_card to ensure highest trump is played
    Card lead = alice->lead_card(SPADES);  // Trump is Spades
    ASSERT_EQUAL(lead, Card(ACE, SPADES));  // Should lead the highest trump (Ace of Spades)

    delete alice;
}

// New: Test SimplePlayer play_lowest_card when no trump
TEST(test_simple_player_play_lowest_card) {
    Player *alice = Player_factory("Alice", "Simple");

    // Add non-trump cards only
    alice->add_card(Card(ACE, CLUBS));
    alice->add_card(Card(KING, DIAMONDS));
    alice->add_card(Card(QUEEN, HEARTS));

    // Test play_card to ensure lowest card is played when no trump
    Card led_card(KING, SPADES);  // Trump is Spades, but no trump cards in hand
    Card played = alice->play_card(led_card, SPADES);  // Should play lowest card (Queen of Hearts)

    ASSERT_EQUAL(played, Card(QUEEN, HEARTS));

    delete alice;
}






TEST(test_make_trump_round_1) {
    Player* p = Player_factory("Player1", "Simple");

    // Add cards, making sure there are fewer than 2 trump cards
    p->add_card(Card(NINE, HEARTS));  // Non-trump
    p->add_card(Card(KING, HEARTS));  // Non-trump
    p->add_card(Card(JACK, DIAMONDS)); // Trump
    p->add_card(Card(ACE, CLUBS));     // Non-trump
    p->add_card(Card(TEN, HEARTS));    // Non-trump

    Card upcard(JACK, DIAMONDS); // Upcard as trump
    Suit order_up_suit;
    bool made_trump = p->make_trump(upcard, false, 1, order_up_suit);

    // Expect false because there are less than 2 trump cards
    ASSERT_FALSE(made_trump);

    delete p;
}
TEST(test_make_trump_round_2) {
    Player* p = Player_factory("Player1", "Simple");

    // Add one card of a suit with the same color as upcard
    p->add_card(Card(KING, HEARTS));  // Same color as upcard (DIAMONDS)
    p->add_card(Card(NINE, SPADES));  // Non-trump
    p->add_card(Card(JACK, CLUBS));   // Non-trump
    p->add_card(Card(ACE, CLUBS));    // Non-trump
    p->add_card(Card(TEN, SPADES));   // Non-trump

    Card upcard(JACK, DIAMONDS); // Upcard suit is DIAMONDS
    Suit order_up_suit;
    bool made_trump = p->make_trump(upcard, false, 2, order_up_suit);

    // Expect true because there is a HEART card, same color as upcard
    ASSERT_TRUE(made_trump);
    ASSERT_EQUAL(order_up_suit, HEARTS);

    delete p;
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

TEST(test_simple_player_play_card_multiple_same_rank) {
    Player *alice = Player_factory("Alice", "Simple");

    // 添加同花色的多张相同等级的牌
    alice->add_card(Card(TEN, CLUBS));
    alice->add_card(Card(TEN, CLUBS));  // 重复的牌
    alice->add_card(Card(QUEEN, SPADES));

    // 领牌是 Clubs
    Card led_card(NINE, CLUBS);
    Card played = alice->play_card(led_card, SPADES);  // SPADES 是主牌

    // 由于两张十的等级相同，玩家可以打出其中一张；检查其中一张是否被打出
    ASSERT_EQUAL(played.get_rank(), TEN);
    ASSERT_EQUAL(played.get_suit(), CLUBS);

    delete alice; // 释放内存
}

// Test SimplePlayer making trump in round 1 with two high trump cards
TEST(test_simple_player_first_round_orders_trump) {
    Player *simple_player = Player_factory("Alice", "Simple");

    // Simulate a hand with two high trump cards
    simple_player->add_card(Card(JACK, HEARTS));
    simple_player->add_card(Card(QUEEN, HEARTS));
    simple_player->add_card(Card(NINE, CLUBS));

    // Test making trump with Hearts as upcard in round 1
    Card upcard(NINE, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    bool ordered = simple_player->make_trump(upcard, false, 1, order_up_suit);

    // Expect the player to order up Hearts
    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(HEARTS, order_up_suit);

    delete simple_player;
}

// Test SimplePlayer passing on trump in round 1 without enough trump cards
TEST(test_simple_player_first_round_passes_trump) {
    Player *simple_player = Player_factory("Alice", "Simple");

    // Simulate a hand without enough trump cards
    simple_player->add_card(Card(NINE, DIAMONDS));
    simple_player->add_card(Card(TEN, CLUBS));
    simple_player->add_card(Card(JACK, SPADES));

    // Test passing on making trump with Hearts as upcard in round 1
    Card upcard(NINE, HEARTS);
    Suit order_up_suit;
    bool ordered = simple_player->make_trump(upcard, false, 1, order_up_suit);

    // Expect the player to pass
    ASSERT_FALSE(ordered);

    delete simple_player;
}

// Test SimplePlayer making trump in round 2 by choosing the next suit (same color)
TEST(test_simple_player_second_round_orders_same_color_trump) {
    Player *simple_player = Player_factory("Alice", "Simple");

    // Simulate a hand with a card from the next suit (same color)
    simple_player->add_card(Card(KING, DIAMONDS));
    simple_player->add_card(Card(QUEEN, SPADES));
    simple_player->add_card(Card(TEN, HEARTS));

    // Test making trump with Diamonds (next suit) in round 2
    Card upcard(NINE, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    bool ordered = simple_player->make_trump(upcard, false, 2, order_up_suit);

    // Expect the player to order up Diamonds (same color as Hearts)
    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(DIAMONDS, order_up_suit);

    delete simple_player;
}


// Test SimplePlayer as the dealer in round 2 (Screw the Dealer rule)
TEST(test_simple_player_screw_the_dealer_forced_trump) {
    Player *simple_player = Player_factory("Alice", "Simple");

    // Simulate a hand where the dealer is forced to order trump
    simple_player->add_card(Card(NINE, CLUBS));
    simple_player->add_card(Card(TEN, SPADES));
    simple_player->add_card(Card(JACK, DIAMONDS));

    // Test the dealer being forced to make trump in round 2
    Card upcard(NINE, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    bool ordered = simple_player->make_trump(upcard, true, 2, order_up_suit);

    // Expect the dealer to order Diamonds as trump (next suit)
    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(DIAMONDS, order_up_suit);

    delete simple_player;
}

// Test SimplePlayer in round 1 with only one trump card
TEST(test_simple_player_first_round_one_trump_pass) {
    Player *simple_player = Player_factory("Alice", "Simple");

    // Simulate a hand with only one trump card
    simple_player->add_card(Card(QUEEN, HEARTS));
    simple_player->add_card(Card(TEN, CLUBS));
    simple_player->add_card(Card(JACK, SPADES));

    // Test passing on making trump in round 1
    Card upcard(NINE, HEARTS);  // Hearts is proposed as trump
    Suit order_up_suit;
    bool ordered = simple_player->make_trump(upcard, false, 1, order_up_suit);

    // Expect the player to pass
    ASSERT_EQUAL(ordered, false);

    delete simple_player;
}

TEST_MAIN();
