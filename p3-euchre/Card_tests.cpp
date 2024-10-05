#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here

// Test default constructor
TEST(test_card_default_ctor) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank()); // Default is Two of Spades
    ASSERT_EQUAL(SPADES, c.get_suit());
}

// Test is_face_or_ace method
TEST(test_card_is_face_or_ace) {
    Card c1(JACK, CLUBS);
    Card c2(QUEEN, SPADES);
    Card c3(NINE, HEARTS); // Not a face card or ace
    ASSERT_TRUE(c1.is_face_or_ace());
    ASSERT_TRUE(c2.is_face_or_ace());
    ASSERT_FALSE(c3.is_face_or_ace());
}

// Test right bower identification
TEST(test_card_is_right_bower) {
    Card right_bower(JACK, SPADES);
    ASSERT_TRUE(right_bower.is_right_bower(SPADES));
    ASSERT_FALSE(right_bower.is_right_bower(CLUBS));
}

// Test left bower identification
TEST(test_card_is_left_bower) {
    Card left_bower(JACK, CLUBS);  // Left bower is Jack of Clubs when trump is Spades
    ASSERT_TRUE(left_bower.is_left_bower(SPADES));
    ASSERT_FALSE(left_bower.is_left_bower(CLUBS));
}



// Test Card comparison operators (less than)
TEST(test_card_less_than) {
    Card c1(ACE, SPADES);
    Card c2(KING, SPADES);
    ASSERT_TRUE(c2 < c1);
    ASSERT_FALSE(c1 < c2);
}

// Test Card comparison operators (equal and not equal)
TEST(test_card_equal) {
    Card c1(ACE, HEARTS);
    Card c2(ACE, HEARTS);
    Card c3(KING, SPADES);
    ASSERT_TRUE(c1 == c2);
    ASSERT_FALSE(c1 == c3);
    ASSERT_TRUE(c1 != c3);
}

// Test Suit_next for correct next suit
TEST(test_suit_next) {
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}

TEST(test_card_comparison_different_suits) {
    Card ace_spades(ACE, SPADES);
    Card ace_hearts(ACE, HEARTS);
    ASSERT_TRUE(ace_spades < ace_hearts);  // Ensure proper comparison across suits
}

TEST(test_card_comparison_same_suit) {
    Card ace_spades(ACE, SPADES);
    Card ten_spades(TEN, SPADES);
    ASSERT_TRUE(ten_spades < ace_spades);  // Ensure proper comparison within the same suit
}

TEST(test_card_bowers) {
    Card jack_spades(JACK, SPADES);
    Card jack_clubs(JACK, CLUBS);  // Left bower if spades is trump
    ASSERT_TRUE(jack_spades.is_right_bower(SPADES));
    ASSERT_TRUE(jack_clubs.is_left_bower(SPADES));
}






TEST_MAIN()
