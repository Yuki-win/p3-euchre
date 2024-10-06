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

TEST(test_card_trump_vs_non_trump) {
    Card ace_hearts(ACE, HEARTS);  // Non-trump
    Card nine_spades(NINE, SPADES);  // Trump
    ASSERT_TRUE(nine_spades.is_trump(SPADES));
    ASSERT_TRUE(Card_less(ace_hearts, nine_spades, SPADES));  // Trump beats non-trump
}
TEST(test_card_bowers_comparison) {
    Card jack_hearts(JACK, HEARTS);  // Left Bower if diamonds is trump
    Card jack_diamonds(JACK, DIAMONDS);  // Right Bower
    ASSERT_TRUE(jack_hearts.is_left_bower(DIAMONDS));
    ASSERT_TRUE(jack_diamonds.is_right_bower(DIAMONDS));
    ASSERT_TRUE(Card_less(jack_hearts, jack_diamonds, DIAMONDS));  // Right Bower beats Left Bower
}
TEST(test_card_trump_comparison) {
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    ASSERT_TRUE(Card_less(king_spades, ace_spades, SPADES));  // Trump suit comparison, Ace beats King
}

TEST(test_card_led_vs_trump) {
    Card ace_clubs(ACE, CLUBS);  // Led suit
    Card nine_spades(NINE, SPADES);  // Trump suit
    ASSERT_TRUE(Card_less(ace_clubs, nine_spades, SPADES));  // Trump beats led suit if no matching suit
}
TEST(test_card_output_operator) {
    Card ace_spades(ACE, SPADES);
    stringstream ss;
    ss << ace_spades;
    ASSERT_EQUAL(ss.str(), "Ace of Spades");  // 验证输出格式是否正确
}

TEST(test_led_suit_same_as_trump) {
    Card ace_spades(ACE, SPADES);  // Led suit and trump suit are both Spades
    Card king_spades(KING, SPADES);
    ASSERT_TRUE(Card_less(king_spades, ace_spades, SPADES));  // Ace should still beat King
}
TEST(test_card_get_suit_with_trump) {
    Card jack_clubs(JACK, CLUBS);  // Left Bower when trump is Spades
    ASSERT_EQUAL(jack_clubs.get_suit(SPADES), SPADES);  // Left bower should return trump suit
    ASSERT_EQUAL(jack_clubs.get_suit(CLUBS), CLUBS);    // When not trump, should return its own suit
}

TEST(test_card_trump_vs_same_suit_non_trump) {
    Card jack_spades(JACK, SPADES);  // Right Bower, Trump
    Card ace_spades(ACE, SPADES);  // 非 Trump
    ASSERT_TRUE(Card_less(ace_spades, jack_spades, SPADES));  // Trump 牌优先于同花色的非 Trump 牌
}
TEST(test_trump_vs_high_non_trump) {
    Card ace_diamonds(ACE, DIAMONDS);  // 非 Trump 牌
    Card nine_spades(NINE, SPADES);  // Trump 牌
    ASSERT_TRUE(Card_less(ace_diamonds, nine_spades, SPADES));  // Trump 牌优先于非 Trump 牌
}
TEST(test_led_vs_trump_priority) {
    Card ace_clubs(ACE, CLUBS);  // Led Suit
    Card jack_spades(JACK, SPADES);  // Trump 牌
    ASSERT_TRUE(Card_less(ace_clubs, jack_spades, SPADES));  // Trump 牌优先于 Led Suit 牌
}
TEST(test_card_led_vs_trump_when_same) {
    Card ace_spades(ACE, SPADES);  // Led suit
    Card jack_spades(JACK, SPADES);  // Trump suit (Right Bower)
    ASSERT_TRUE(Card_less(ace_spades, jack_spades, SPADES));  // Right Bower (Trump) should beat Ace (Led suit)
}

TEST(test_card_get_suit_with_trump_edge_case) {
    Card jack_clubs(JACK, CLUBS);  // 左王牌
    ASSERT_EQUAL(jack_clubs.get_suit(SPADES), SPADES);  // 当 Trump 为 Spades 时，左王牌应被视为 Trump 花色
    ASSERT_EQUAL(jack_clubs.get_suit(CLUBS), CLUBS);  // 否则应返回其本身的花色
}

TEST(test_card_less_operator_basic) {
    Card card1(KING, HEARTS);
    Card card2(ACE, HEARTS);
    ASSERT_TRUE(card1 < card2);  // King 应该小于 Ace
}
TEST(test_card_comparison_different_suits_basic) {
    Card card1(NINE, CLUBS);
    Card card2(TEN, SPADES);
    ASSERT_TRUE(Card_less(card1, card2, HEARTS));  // 不同花色时，Spades 应该优先于 Clubs
}
TEST(test_suit_next_basic) {
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);  // Spades 之后应该是 Clubs
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);  // Clubs 之后应该是 Spades
}

// Test is_trump method
TEST(test_card_is_trump) {
    Card trump_card(ACE, SPADES); // Trump card
    Card non_trump_card(KING, HEARTS); // Non-trump card
    ASSERT_TRUE(trump_card.is_trump(SPADES)); // Should be true for trump suit
    ASSERT_FALSE(non_trump_card.is_trump(SPADES)); // Should be false for non-trump suit
}

// Test get_suit method with trump
TEST(test_card_get_suit) {
    Card left_bower(JACK, CLUBS); // Left bower when trump is SPADES
    Card normal_card(QUEEN, HEARTS);
    ASSERT_EQUAL(left_bower.get_suit(SPADES), SPADES); // Should return trump suit
    ASSERT_EQUAL(normal_card.get_suit(SPADES), HEARTS); // Should return normal suit
}





// Test operator>> for Card
TEST(test_card_input_operator) {
    Card c;
    std::istringstream iss("King of Diamonds");
    iss >> c;
    ASSERT_EQUAL(KING, c.get_rank());
    ASSERT_EQUAL(DIAMONDS, c.get_suit());
}



// Test comparison with trump
TEST(test_card_comparison_with_trump) {
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    Card ace_hearts(ACE, HEARTS);
    ASSERT_TRUE(Card_less(king_spades, ace_spades, SPADES)); // Ace beats King in trump suit
    ASSERT_TRUE(Card_less(ace_hearts, king_spades, SPADES)); // Non-trump comparison
}

// Test Card comparison operators (greater than)
TEST(test_card_greater_than) {
    Card c1(ACE, SPADES);
    Card c2(KING, SPADES);
    ASSERT_TRUE(c1 > c2);
    ASSERT_FALSE(c2 > c1);
}

// Test Card comparison operators (greater than or equal)
TEST(test_card_greater_than_or_equal) {
    Card c1(ACE, SPADES);
    Card c2(ACE, SPADES);
    ASSERT_TRUE(c1 >= c2);

}

// Test Card comparison operators (less than or equal)
TEST(test_card_less_than_or_equal) {
    Card c1(ACE, SPADES);
    Card c2(KING, SPADES);
    ASSERT_TRUE(c2 <= c1);
    ASSERT_FALSE(c1 <= c2);
}

// Test Card_less function with trump
TEST(test_card_less_with_trump) {
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    Card nine_hearts(NINE, HEARTS);
    ASSERT_TRUE(Card_less(king_spades, ace_spades, SPADES)); // Ace beats King in trump suit
    ASSERT_TRUE(Card_less(nine_hearts, king_spades, SPADES)); // Non-trump comparison
}

// Test all face cards and ace recognition
TEST(test_card_face_or_ace_recognition) {
    Card face_cards[] = {
        Card(JACK, SPADES),
        Card(QUEEN, SPADES),
        Card(KING, SPADES),
        Card(ACE, SPADES)
    };
    for (const auto& card : face_cards) {
        ASSERT_TRUE(card.is_face_or_ace());
    }
}

// Test comparison operators with edge cases
TEST(test_card_comparison_edge_cases) {
    Card c1(ACE, SPADES);
    Card c2(ACE, HEARTS);
    Card c3(KING, SPADES);
    ASSERT_TRUE(c1 > c3); // Ace should be greater than King
    ASSERT_TRUE(c1 >= c1); // Ace should be equal to itself
    ASSERT_TRUE(c2 > c1); // Ace of Hearts should be less than Ace of Spades
}

// Test Card_less function with edge cases
TEST(test_card_less_edge_cases) {
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    Card ace_hearts(ACE, HEARTS);
    ASSERT_TRUE(Card_less(king_spades, ace_spades, SPADES)); // Ace beats King in trump suit
    ASSERT_TRUE(Card_less(ace_hearts, king_spades, SPADES)); // Non-trump comparison
}

// Test get_suit method with trump edge cases
TEST(test_card_get_suit_edge_cases) {
    Card left_bower(JACK, CLUBS); // Left bower when trump is SPADES
    Card normal_card(QUEEN, HEARTS);
    ASSERT_EQUAL(left_bower.get_suit(SPADES), SPADES); // Should return trump suit
    ASSERT_EQUAL(normal_card.get_suit(SPADES), HEARTS); // Should return normal suit
}

TEST_MAIN()
