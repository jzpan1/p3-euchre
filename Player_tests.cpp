#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <fstream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    Player * david = Player_factory("David", "Human");
    ASSERT_EQUAL("Alice", alice->get_name());
    ASSERT_EQUAL("David", david->get_name());

    delete alice;
    delete david;
}

TEST(test_player_insertion) {
  Player *david = Player_factory("David", "Human");

  ostringstream oss1;
  oss1 << *david;

  ASSERT_EQUAL(oss1.str(), "David");

  Player *alice = Player_factory("Alice", "Simple");

  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  delete david;
  delete alice;
}

TEST(test_simple_player_play_card) {
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,
    HEARTS
  );
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;
}

TEST(test_simple_player_make_cards) {
  Player * david = Player_factory("David", "Simple");
  david->add_card(Card(NINE, HEARTS));
  david->add_card(Card(TEN, SPADES));
  david->add_card(Card(QUEEN, DIAMONDS));
  david->add_card(Card(KING, CLUBS));
  david->add_card(Card(ACE, SPADES));

  Player * tom = Player_factory("Tom", "Simple");
  tom->add_card(Card(NINE, CLUBS));
  tom->add_card(Card(TEN, DIAMONDS));
  tom->add_card(Card(QUEEN, SPADES));
  tom->add_card(Card(KING, SPADES));
  tom->add_card(Card(ACE, SPADES));

  david->add_and_discard(
    Card(JACK, HEARTS)
  );

  Suit trump;
  ASSERT_TRUE(david->make_trump(Card(NINE, SPADES), true, 2, trump));
  ASSERT_EQUAL(trump, CLUBS);

  ASSERT_TRUE(david->make_trump(Card(NINE, SPADES), false, 1, trump)); //true

  ASSERT_TRUE(tom->make_trump(Card(NINE, SPADES), true, 1, trump));
  ASSERT_EQUAL(trump, SPADES);

  ASSERT_FALSE(tom->make_trump(Card(NINE, SPADES), false, 2, trump));
  ASSERT_TRUE(tom->make_trump(Card(NINE, SPADES), true, 2, trump));
  ASSERT_EQUAL(trump, CLUBS);


  delete david;
}

TEST_MAIN()
