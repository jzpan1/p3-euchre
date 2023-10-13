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
		cout << shuffle << points_to_win << pack.deal_one() << players.size();
	}
 private:
	bool shuffle;
	int points_to_win;
	
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