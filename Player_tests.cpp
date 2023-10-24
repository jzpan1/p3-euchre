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

TEST(test_simple_player_play_card2) {
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));

  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,
    HEARTS
  );
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));

  bob->add_card(Card(NINE, SPADES));
  card_played = bob->play_card(
    Card(NINE, HEARTS),
    HEARTS
  );
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));

  bob->add_card(Card(NINE, SPADES));
  card_played = bob->play_card(
    Card(NINE, CLUBS),
    HEARTS
  );
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));

  bob->add_card(Card(NINE, SPADES));
  card_played = bob->play_card(
    Card(JACK, SPADES),
    HEARTS
  );
  ASSERT_EQUAL(card_played, Card(QUEEN, SPADES));

  delete bob;
}

TEST(test_simple_player_lead_card) {
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, HEARTS));
  bob->add_card(Card(ACE, DIAMONDS));
  bob->add_card(Card(JACK, DIAMONDS));

  Card card_played = bob->lead_card(HEARTS);
  ASSERT_EQUAL(card_played, Card(ACE, DIAMONDS));
  bob->add_card(Card(ACE, DIAMONDS));
  
  card_played = bob->lead_card(CLUBS);
  ASSERT_EQUAL(card_played, Card(ACE, DIAMONDS));
  bob->add_card(Card(ACE, DIAMONDS));

  card_played = bob->lead_card(DIAMONDS);
  ASSERT_EQUAL(card_played, Card(KING, HEARTS));
  bob->add_card(Card(KING, HEARTS));
  
  card_played = bob->lead_card(SPADES);
  ASSERT_EQUAL(card_played, Card(ACE, DIAMONDS));

  delete bob;
}

TEST(test_random){
  Player * bob = Player_factory("Tom", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, HEARTS));
  bob->add_card(Card(ACE, DIAMONDS));
  bob->add_card(Card(JACK, DIAMONDS));
  
  ASSERT_EQUAL(bob->lead_card(DIAMONDS), Card(KING, HEARTS));
  ASSERT_EQUAL(bob->lead_card(DIAMONDS), Card(QUEEN, SPADES));
  ASSERT_EQUAL(bob->lead_card(DIAMONDS), Card(NINE, SPADES));
  ASSERT_EQUAL(bob->lead_card(DIAMONDS), Card(JACK, DIAMONDS));
  ASSERT_EQUAL(bob->lead_card(DIAMONDS), Card(ACE, DIAMONDS));

}

TEST(test_all_jack_lead_card) {
  Player * steve = Player_factory("Tom", "Simple");

  steve->add_card(Card(JACK, CLUBS));
  steve->add_card(Card(JACK, SPADES));

  Card card_played = steve->lead_card(CLUBS);
  ASSERT_EQUAL(card_played, Card(JACK, CLUBS));
  steve->add_card(Card(JACK, CLUBS));
  
  card_played = steve->lead_card(SPADES);
  ASSERT_EQUAL(card_played, Card(JACK, SPADES));
  steve->add_card(Card(JACK, SPADES));

  steve->add_card(Card(KING, SPADES));
  card_played = steve->lead_card(DIAMONDS);
  ASSERT_EQUAL(card_played, Card(KING, SPADES));
  steve->add_card(Card(KING, SPADES));
  
  card_played = steve->lead_card(HEARTS);
  ASSERT_EQUAL(card_played, Card(KING, SPADES));

  delete steve;
}

TEST(test_simple_player_make_card1) {
  Player * david = Player_factory("David", "Simple");
  david->add_card(Card(NINE, HEARTS));
  david->add_card(Card(TEN, SPADES));
  david->add_card(Card(QUEEN, DIAMONDS));
  david->add_card(Card(KING, CLUBS));
  david->add_card(Card(ACE, SPADES));


  david->add_and_discard(
    Card(JACK, HEARTS)
  );

  Suit trump;
  ASSERT_TRUE(david->make_trump(Card(NINE, SPADES), true, 2, trump));
  ASSERT_EQUAL(trump, CLUBS);

  ASSERT_FALSE(david->make_trump(Card(NINE, SPADES), false, 1, trump));

  delete david;
}

TEST(test_add_and_discard){
  Player* james = Player_factory("James", "Simple");
  james->add_card(Card(NINE, HEARTS));
  james->add_card(Card(TEN, SPADES));
  james->add_card(Card(QUEEN, DIAMONDS));
  james->add_card(Card(KING, CLUBS));
  james->add_card(Card(ACE, SPADES));

  james->add_and_discard(Card(JACK, HEARTS));
  james->add_and_discard(Card(KING, DIAMONDS));

  ASSERT_EQUAL(james->play_card(Card(NINE, DIAMONDS), CLUBS), Card(KING, DIAMONDS));
  ASSERT_EQUAL(james->play_card(Card(NINE, SPADES), CLUBS), Card(ACE, SPADES));
  ASSERT_EQUAL(james->play_card(Card(NINE, SPADES), CLUBS), Card(JACK, HEARTS));
  ASSERT_EQUAL(james->play_card(Card(ACE, HEARTS), CLUBS), Card(QUEEN, DIAMONDS));
  ASSERT_EQUAL(james->play_card(Card(ACE, HEARTS), CLUBS), Card(KING, CLUBS));
  
  delete james;
}

TEST(test_simple_player_make_card2) {
  Player * tom = Player_factory("Tom", "Simple");
  tom->add_card(Card(NINE, CLUBS));
  tom->add_card(Card(TEN, DIAMONDS));
  tom->add_card(Card(QUEEN, SPADES));
  tom->add_card(Card(KING, SPADES));
  tom->add_card(Card(ACE, SPADES));

  Suit trump;
  ASSERT_FALSE(tom->make_trump(Card(NINE, HEARTS), true, 1, trump));
  ASSERT_FALSE(tom->make_trump(Card(NINE, DIAMONDS), true, 1, trump));
  ASSERT_FALSE(tom->make_trump(Card(NINE, CLUBS), true, 1, trump));
  ASSERT_TRUE(tom->make_trump(Card(NINE, SPADES), true, 1, trump));
  ASSERT_EQUAL(trump, SPADES);

  ASSERT_FALSE(tom->make_trump(Card(NINE, HEARTS), false, 2, trump));
  ASSERT_FALSE(tom->make_trump(Card(NINE, DIAMONDS), false, 2, trump));
  ASSERT_FALSE(tom->make_trump(Card(NINE, SPADES), false, 2, trump));
  ASSERT_TRUE(tom->make_trump(Card(NINE, CLUBS), false, 2, trump));
  ASSERT_EQUAL(trump, SPADES);
  
  delete tom;
}

TEST(test_all_jack_make_card3) {
  Player * steve = Player_factory("Tom", "Simple");
  steve->add_card(Card(JACK, CLUBS));
  steve->add_card(Card(JACK, DIAMONDS));
  steve->add_card(Card(JACK, SPADES));
  steve->add_card(Card(JACK, HEARTS));

  Suit trump;
  ASSERT_TRUE(steve->make_trump(Card(NINE, HEARTS), true, 1, trump));
  ASSERT_EQUAL(trump, HEARTS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, DIAMONDS), true, 1, trump));
  ASSERT_EQUAL(trump, DIAMONDS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, CLUBS), true, 1, trump));
  ASSERT_EQUAL(trump, CLUBS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, SPADES), true, 1, trump));
  ASSERT_EQUAL(trump, SPADES);

  ASSERT_TRUE(steve->make_trump(Card(NINE, HEARTS), false, 2, trump));
  ASSERT_EQUAL(trump, DIAMONDS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, DIAMONDS), false, 2, trump));
  ASSERT_EQUAL(trump, HEARTS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, SPADES), false, 2, trump));
  ASSERT_EQUAL(trump, CLUBS);
  ASSERT_TRUE(steve->make_trump(Card(NINE, CLUBS), false, 2, trump));
  ASSERT_EQUAL(trump, SPADES);
  
  delete steve;
}



TEST_MAIN()
