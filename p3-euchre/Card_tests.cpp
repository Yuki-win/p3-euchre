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

// Test if a card is trump
TEST(test_card_is_trump) {
    Card c1(QUEEN, HEARTS);
    Card right_bower(JACK, DIAMONDS);
    Card left_bower(JACK, HEARTS); // Left bower for DIAMONDS

    ASSERT_TRUE(c1.is_trump(HEARTS));
    ASSERT_FALSE(c1.is_trump(SPADES));
    ASSERT_TRUE(right_bower.is_trump(DIAMONDS));  // Right bower is always trump
    ASSERT_TRUE(left_bower.is_trump(DIAMONDS));   // Left bower is trump in Diamonds
    ASSERT_FALSE(left_bower.is_trump(HEARTS));    // Not trump in Hearts
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

TEST_MAIN()
