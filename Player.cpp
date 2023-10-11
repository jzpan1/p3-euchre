#include "Player.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
 public:
	SimplePlayer(string player_name):
	name(player_name) { }

	//EFFECTS returns player's name
	const string & get_name() const override {
		return name;
	}
	
	//REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
	void add_card(const Card &c) override {
		assert(hand.size() < MAX_HAND_SIZE);
		hand.push_back(c);
	}

	//REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
		assert(round == 1 || round == 2);
		if (round == 1) {
			Suit possible_trump = upcard.get_suit();
			int trump_face_count = 0;
			for (const Card &c : hand ) {
				if (c.is_trump(possible_trump) && c.is_face_or_ace()) {
					trump_face_count++;
				}
				if (trump_face_count < 2){
					order_up_suit = possible_trump;
					return true;
				}
			}
		}
		if (round == 2) {
			Suit possible_trump = Suit_next(upcard.get_suit());
			for (const Card &c : hand ) {
				if (c.is_trump(possible_trump) && c.is_face_or_ace()) {
					order_up_suit = possible_trump;
					return true;
				}
			}
			if(is_dealer) {
				order_up_suit = possible_trump;
				return true;
			}
		}
		return false;
	}

	//REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card &upcard) override {
		assert(hand.size() > 0);
		hand.push_back(Card(upcard));
		Card lowest_in_hand = hand[0];
		int lowest_index = 0;
		for (int i = 0; i < hand.size(); i++ ) {
			if (Card_less(hand[i], lowest_in_hand, upcard.get_suit())) {
				lowest_in_hand = hand[i];
				lowest_index = i;
			}
		}
		hand.erase(hand.begin() + lowest_index);
	}
	//REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  Card lead_card(Suit trump) override {
		assert(hand.size() > 0);
		bool has_nontrump = !hand[0].is_trump(trump);
		Card highest_in_hand = Card(hand[0]);
		int highest_index = 0;
		for (int i = 0; i < hand.size(); i++ ) {
			//if highest started out as trump but found a non trump
			if (!has_nontrump && !hand[i].is_trump(trump)) {
				has_nontrump = true;
				highest_in_hand = Card(hand[i]);
				highest_index = i;
			}

			if (has_nontrump) {
				if (Card_less(highest_in_hand, hand[i], trump) 
				    && !hand[i].is_trump(trump)) {
							
					highest_in_hand = Card(hand[i]);
					highest_index = i;
				}
			}
			else if (Card_less(highest_in_hand, hand[i], trump)) {
				highest_in_hand = Card(hand[i]);
				highest_index = i;
			}
		}
		hand.erase(hand.begin() + highest_index);
		return highest_in_hand;
	}
	//REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  Card play_card(const Card &led_card, Suit trump) override {
		Card highest_card_of_suit;
		bool follow_suit = false;
		Suit led_suit = led_card.get_suit(trump);
		Card lowest_in_hand = hand[0];
		int play_index = 0;
		for (int i = 0; i < hand.size(); i++ ) {
			if (follow_suit 
			    && hand[i].get_suit(trump) == highest_card_of_suit.get_suit(trump)
			    && Card_less(highest_card_of_suit, hand[i], trump)) {
				highest_card_of_suit = Card(hand[i]);
				play_index = i;
			}
			if (follow_suit && hand[i].get_suit(trump) == led_suit) {
				highest_card_of_suit = Card(hand[i]);
				play_index = i;
				follow_suit = true;
			}
			if (!follow_suit && Card_less(hand[i], lowest_in_hand, trump)) {
				lowest_in_hand = Card(hand[i]);
				play_index = i;
			}
		}
		hand.erase(hand.begin() + play_index);
		return follow_suit? highest_card_of_suit : lowest_in_hand;
	}

 private:
	std::string name;
	std::vector<Card> hand;
};

class HumanPlayer : public Player {
 public:
	HumanPlayer(string player_name):
	name(player_name) { }

	//EFFECTS returns player's name
	const string & get_name() const override {
		return name;
	}
	
	//REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
	void add_card(const Card &c) override {
		assert(hand.size() < MAX_HAND_SIZE);
		hand.push_back(c);
		sort_hand();
	}

	//REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
		print_hand();
		cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
		string decision;
		cin >> decision;

		if (decision != "pass") {
			Suit ordered_up = string_to_suit(decision);
			order_up_suit = ordered_up;
			return true;
		}
		else {
			return false;
		}
	}

	//REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card &upcard) override {
		assert(hand.size() > 0);
		// hand.push_back(Card(upcard));
		// sort_hand();
		print_hand();
		cout << "Discard upcard: [-1]\n";
		cout << "Human player " << name << ", please select a card to discard:\n";
		int discard;
		cin >> discard;
		if (discard == -1) return;
		hand.erase(hand.begin() + discard);
	}
	//REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  Card lead_card(Suit trump) override {
		print_hand();
		cout << "Human player " << name << ", please select a card:\n";
		int play_index;
		cin >> play_index;
		Card played = Card(hand[play_index]);
		hand.erase(hand.begin() + play_index);
		return played;
	}
	//REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  Card play_card(const Card &led_card, Suit trump) override {
		print_hand();
		cout << "Human player " << name << ", please select a card:\n";
		int play_index;
		cin >> play_index;
		Card played = Card(hand[play_index]);
		hand.erase(hand.begin() + play_index);
		return played;
	}

 private:
	void sort_hand() {
		sort(hand.begin(), hand.end());
	}
	void print_hand() const {
  	for (size_t i=0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
	}
	std::string name;
	std::vector<Card> hand;
};

//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy) {
	// We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  // Repeat for each other type of Player
  // if (strategy == "Human") {
  //   // The "new" keyword dynamically allocates an object.
  //   return new HumanPlayer(name);
  // }
	if (strategy == "Human") {
    // The "new" keyword dynamically allocates an object.
    return new HumanPlayer(name);
  }
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p) {
	os << p.get_name();
	return os;
}

