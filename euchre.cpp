#include <iostream>
#include <fstream>

#include "Pack.hpp"
#include "Player.hpp"
using namespace std;

class Game {
 public:
 Game(bool should_shuffle, int win_points, Pack &pack, vector<Player*> &game_players) :
 shuffle(should_shuffle), points_to_win(win_points), pack(pack), players(game_players), 
 team0_points(0), team1_points(0) {}

	void play() {
		int dealer_index = 0;
		int hand = 0;
		if (shuffle) {
			pack.shuffle();
		}
		while (team0_points < points_to_win && team1_points < points_to_win) {
			cout << "Hand " << hand;
			++hand;
			play_hand(dealer_index);
			pack.shuffle();
			dealer_index = (dealer_index + 1) % NUM_PLAYERS;
		}
	}
 private:
	//EFFECTS: play the entirety of a single hand.
	void play_hand(int const dealer_index) {
		const int num_tricks = 5;
		int team0_tricks = 0;
		int team1_tricks = 0;
		//the index of the player who made trump
		int trump_maker = 0;
		//corresponds to the number of the team who made trump
		int trump_team; //(false if team 0, true if team 1)

		cout << players[dealer_index]->get_name() << " deals";
		deal(dealer_index);

		trump_maker = make_trump(dealer_index);
		if (trump_maker == 0 || trump_maker == 2) {
			trump_team = 0;
		}
		else if (trump_maker == 1 || trump_maker == 3) {
			trump_team = 1;
		}
		int leader_index = dealer_index;
		for (int i = 0; i < num_tricks; i++) {
			leader_index = play_trick(leader_index);
			if (leader_index == 0 || leader_index == 2) {
				team0_tricks++;
			}
			else if (leader_index == 1 || leader_index == 3) {
				team1_tricks++;
			}
		}

		//check if euchred or marched
		if (team0_tricks == 5) {
			cout << "march!" << endl;
			team0_points += 2;
		}
		else if (team1_tricks == 5) {
			cout << "march!" << endl;
			team1_points += 2;
		}
		else if (team0_tricks >= 3 && trump_team) {
			cout << "euchred!" << endl;
			team0_points += 2;
		}
		else if (team1_tricks >= 3 && !trump_team) {
			cout << "euchred!" << endl;
			team1_points += 2;
		}
	}
	
	//EFFECTS: Deal 5 cards to each player, starting
	//         to the right(?) of the dealer
	void deal(const int dealer_index) {};
	//EFFECTS: Go through the process of choosing a trump suit
	void make_trump(const int dealer_index) {};
	
	//EFFECTS: Play a single trick, given the leader.
	//         Returns the winner of the trick.
	int play_trick(const int leader_index) {return 0;};

	static const int NUM_PLAYERS = 4;

	bool shuffle;
	int points_to_win;
	
	//team 0 = player indexes 0 and 2
	int team0_points;
	//team 1 = player indexes 1 and 3
	int team1_points;
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
	
	for (int i = 0; i < argc; i++) {
		cout << argv[i] << " ";
	}
	cout << endl;

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