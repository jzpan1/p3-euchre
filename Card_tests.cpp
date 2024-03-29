#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
    ASSERT_EQUAL(HEARTS, c.get_suit(DIAMONDS));
    Card a(string_to_rank("Three"), string_to_suit("Spades"));
    ASSERT_EQUAL(THREE, a.get_rank());
    ASSERT_EQUAL(SPADES, a.get_suit());
    ASSERT_EQUAL(SPADES, a.get_suit(CLUBS));
    ASSERT_EQUAL(TWO, Card().get_rank());
    ASSERT_EQUAL(SPADES, Card().get_suit());
    Card b;
    ASSERT_EQUAL(b, Card(TWO, SPADES));
}

TEST(test_card_values) {
    Card king_hearts(KING, HEARTS);
    Card ace_diamonds(ACE, DIAMONDS);
    ASSERT_FALSE(king_hearts > ace_diamonds);
}

TEST(test_face_or_ace){
    Card queen_hearts(QUEEN, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card ten_clubs(TEN, CLUBS);
    ASSERT_TRUE(queen_hearts.is_face_or_ace());
    ASSERT_FALSE(ten_clubs.is_face_or_ace());
    ASSERT_TRUE(jack_diamonds.is_face_or_ace());
    ASSERT_FALSE(Card(TWO, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(THREE, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(FOUR, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(FIVE, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(SIX, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(SEVEN, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(EIGHT, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(NINE, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(KING, SPADES).is_face_or_ace());
}

TEST(test_is_trump){
    Card queen_hearts(QUEEN, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    Card ten_clubs(TEN, CLUBS);
    ASSERT_TRUE(jack_diamonds.is_trump(HEARTS));
    ASSERT_FALSE(ten_clubs.is_trump(HEARTS));
    ASSERT_TRUE(queen_hearts.is_trump(HEARTS));
}

TEST(test_right_left_bower){
    Card queen_hearts(QUEEN, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_FALSE(queen_hearts.is_right_bower(CLUBS));
    ASSERT_FALSE(queen_hearts.is_right_bower(HEARTS));
    ASSERT_FALSE(queen_hearts.is_left_bower(CLUBS));

    ASSERT_TRUE(jack_diamonds.is_right_bower(DIAMONDS));
    ASSERT_FALSE(jack_diamonds.is_right_bower(HEARTS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(CLUBS));
    ASSERT_TRUE(jack_diamonds.is_left_bower(HEARTS));
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

    Card king_spades(KING, SPADES);
    ASSERT_FALSE(king_spades < Card(KING, SPADES));
    ASSERT_TRUE(king_spades <= Card(KING, SPADES));
    ASSERT_FALSE(king_spades > Card(KING, SPADES));
    ASSERT_TRUE(king_spades >= Card(KING, SPADES));
    ASSERT_TRUE(king_spades == Card(KING, SPADES));
    ASSERT_FALSE(king_spades != Card(KING, SPADES));
}

TEST(test_suit_next){
    Card jack_clubs(JACK, CLUBS);
    Card ace_diamonds(ACE, DIAMONDS);
    Card nine_hearts(NINE, HEARTS);
    Card ten_spades(TEN, SPADES);

    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
}

TEST(test_card_less){
    Card jack_clubs(JACK, CLUBS);
    Card ace_diamonds(ACE, DIAMONDS);
    Card nine_hearts(NINE, HEARTS);
    Card ten_spades(TEN, SPADES);
    ASSERT_FALSE(Card_less(jack_clubs, ace_diamonds, CLUBS));
    ASSERT_TRUE(Card_less(ace_diamonds, jack_clubs, nine_hearts,
                           CLUBS));
    ASSERT_TRUE(Card_less(nine_hearts, Card(NINE, DIAMONDS), SPADES));
    ASSERT_FALSE(Card_less(Card(NINE, DIAMONDS), nine_hearts, SPADES));
    ASSERT_FALSE(Card_less(nine_hearts, nine_hearts, HEARTS));
    ASSERT_TRUE(Card_less(ten_spades, Card(TEN, HEARTS), Card(JACK, SPADES),
                           CLUBS));
    ASSERT_TRUE(Card_less(Card(JACK, HEARTS), Card(JACK, CLUBS), Card(JACK, SPADES),
                           CLUBS));
    ASSERT_TRUE(Card_less(Card(NINE, SPADES), Card(KING, SPADES), Card(JACK, SPADES),
                           HEARTS));
    ASSERT_FALSE(Card_less(jack_clubs, Card(TEN, CLUBS), CLUBS));
    ASSERT_TRUE(Card_less(Card(TEN, CLUBS), jack_clubs, CLUBS));
    ASSERT_TRUE(Card_less(Card(NINE, HEARTS), Card(NINE, SPADES), Card(TEN, SPADES),
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
