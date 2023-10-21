#include <iostream>
#include <fstream>

#include "Pack.hpp"
#include "Player.hpp"
using namespace std;

class Game {
 public:
 Game(bool should_shuffle, int win_points, Pack &pack, vector<Player*> &game_players) :
 shuffle(should_shuffle), points_to_win(win_points), pack(pack), players(game_players) {}

	void play() {
		int team0_points = 0;
		int team1_points = 0;
		int dealer_index = 0;
		if (shuffle) {
			pack.shuffle();
		}
		while (team0_points != points_to_win && team1_points != points_to_win) {
			play_hand(0);
			pack.shuffle();
			dealer_index = (dealer_index + 1) % NUM_PLAYERS;
		}
	}
 private:
	//EFFECTS: play the entirety of a single hand.
	void play_hand(int const dealer_index) {
		const int num_tricks = 5;

		deal(dealer_index);
		make_trump(dealer_index);
		int leader_index = dealer_index;
		for (int i = 0; i < num_tricks; i++) {
			leader_index = play_trick(leader_index);
		}
	}
	
	//EFFECTS: Deal 5 cards to each player, starting
	//         to the left of the dealer
	void deal(int const dealer_index) {
		int dealplayer = dealer_index;
		for(int j = 0; j < 5; j++){
			for (int i = 0; i < 4; i++) {
				if(dealplayer == 3){
					dealplayer = -1;
				}
				dealplayer++;
				players[dealplayer]->add_card(pack.deal_one());
			}
		}
	};
	//EFFECTS: Go through the process of choosing a trump suit
	int  make_trump(int dealer_index) {
		Card c = pack.deal_one();
		int dealplayer = dealer_index;
		for (int i = 0; i < 4; i++) {
			if(dealplayer == 3){
				dealplayer = -1;
			}
			dealplayer++;
			if(players[dealplayer]->make_trump(c,dealer_index == dealplayer, 1, trump)){
				return dealplayer;
			}
		}
		for (int i = 0; i < 4; i++) {
			if(dealplayer == 3){
				dealplayer = -1;
			}
			dealplayer++;
			if(players[dealplayer]->make_trump(c,dealer_index == dealplayer, 2, trump)){
				return dealplayer;
			}
		}
	};
	
	//EFFECTS: Play a single trick, given the leader
	int play_trick(int leader_index) {
		int playindex = leader_index;
		int winner = 0;
		Card led_card = players[leader_index]->lead_card(trump);
		Card hightest = led_card;
		Card current;
		for (int i = 0; i < 4; i++) {
			if(playindex == 3){
				playindex = -1;
			}
			playindex++;
			current = players[playindex]->play_card(led_card, trump);
			if(!Card_less(current, hightest, led_card, trump)){
				hightest = current;
				winner = playindex;
			}
		}
		return winner;
	};

	static const int NUM_PLAYERS = 4;

	bool shuffle;
	int points_to_win;
	
	Suit trump;
	Pack &pack;
	vector<Player*> &players;

};

//EFFECTS: prints instructions for usage of this program. Should be 
//         used if the user's arguments are invalid.
void print_usage() {
	cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
         << "NAME4 TYPE4" << endl;
}

int main(int argc, char *argv[]) {
	//should have 12 command line arguments
	if (argc != 12) {
		print_usage();
		return 1;
	}

	//read in card pack
  string pack_in = argv[1];
	ifstream pack_file;
	try {
		pack_file.open(pack_in);
	}
	catch (int e) {
		print_usage();
		return 1;
	}
	Pack pack(pack_file);
	pack_file.close();

	//configure shuffling
	bool should_shuffle = false;
  string shuffle = argv[2];
	if (shuffle == "shuffle") should_shuffle = true;
	else if (shuffle == "noshuffle") should_shuffle = false;	
	else {
		print_usage();
		return 1;
	}

	//number of points to win
	int points_to_win = 0;
	try {
		points_to_win = stoi(argv[3]);
	}
	catch (int e) {
		print_usage();
		return 1;
	}
	if (!(points_to_win >= 1 && points_to_win <= 100)
		 ) {
		print_usage();
		return 1;
	}

	//initialize players
  vector<Player*> players;
	for (int i = 0; i < 4; i++) {
		string player_name = argv[4 + 2 * i];
		string player_strategy = argv[4 + 2 * i];

		if (player_strategy != "Human" && player_strategy != "Simple") {
			print_usage();
			return 1;
		}
		players.push_back(Player_factory(player_name, player_strategy));
	}

	Game game(should_shuffle, points_to_win, pack, players);
	game.play();

	for (int i = 0; i < 4; i++) {
		delete players[i];
	}
}