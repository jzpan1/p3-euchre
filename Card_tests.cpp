#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
    ASSERT_EQUAL(HEARTS, c.get_suit(HEARTS));
    Card a(string_to_rank("Three"), string_to_suit("Diamonds"));
    ASSERT_EQUAL(THREE, a.get_rank());
    ASSERT_EQUAL(DIAMONDS, a.get_suit());
}

TEST(test_card_values) {
    Card king_hearts(KING, HEARTS);
    Card ace_diamonds(ACE, DIAMONDS);
    ASSERT_FALSE(king_hearts > ace_diamonds);
}

TEST(test_face_or_ace){
    Card queen_hearts(QUEEN, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(queen_hearts.is_face_or_ace());
    ASSERT_FALSE(queen_hearts.is_right_bower(CLUBS));
    ASSERT_FALSE(queen_hearts.is_left_bower(CLUBS));
    ASSERT_TRUE(queen_hearts.is_trump(HEARTS));
    ASSERT_TRUE(jack_diamonds.is_face_or_ace());
    ASSERT_TRUE(jack_diamonds.is_right_bower(DIAMONDS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(CLUBS));
    ASSERT_TRUE(jack_diamonds.is_trump(HEARTS));
}

TEST(test_card_comparison){
    Card queen_clubs(QUEEN, CLUBS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_TRUE(jack_diamonds < queen_clubs);
    ASSERT_TRUE(jack_diamonds <= queen_clubs);
    ASSERT_FALSE(jack_diamonds > queen_clubs);
    ASSERT_FALSE(jack_diamonds >= queen_clubs);
    ASSERT_FALSE(jack_diamonds == queen_clubs);
    ASSERT_TRUE(jack_diamonds != queen_clubs);
}

TEST(test_miscellaneous){
    Card jack_clubs(JACK, CLUBS);
    Card ace_diamonds(ACE, DIAMONDS);
    Card nine_heats(NINE, HEARTS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_FALSE(Card_less(jack_clubs, ace_diamonds, CLUBS));
    ASSERT_TRUE(Card_less(ace_diamonds, jack_clubs, nine_heats,
                           CLUBS));
}

TEST(test_insetion_exertion){
    Card jack_hearts = Card(JACK, HEARTS);
    ostringstream oss;
    oss << jack_hearts;
    ASSERT_EQUAL(oss.str(), "Jack of Hearts");
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}

TEST_MAIN()
