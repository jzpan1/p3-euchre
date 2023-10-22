#include <iostream>
#include <fstream>

#include "Pack.hpp"
#include "Player.hpp"
using namespace std;

class Game {
 public:
 Game(bool should_shuffle, int win_points, Pack &pack, vector<Player*> &game_players) :
	 shuffle(should_shuffle), points_to_win(win_points), team0_points(0), 
	 team1_points(0), pack(pack), players(game_players)
	{}

	void play() {
		int dealer_index = 0;
		int hand = 0;
		if (shuffle) {
			pack.shuffle();
		}
		while (team0_points < points_to_win && team1_points < points_to_win) {
			cout << "Hand " << hand << endl;
			++hand;
			play_hand(dealer_index);
			if (shuffle) pack.shuffle();
			else pack.reset();
			dealer_index = (dealer_index + 1) % NUM_PLAYERS;
		}
		if (team0_points >= points_to_win) {
			cout << *players[0] << " and " << *players[2] << " win!" << endl;
		}
		if (team1_points >= points_to_win) {
			cout << *players[1] << " and " << *players[3] << " win!" << endl;
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

		Card upcard = deal(dealer_index);

		trump_maker = make_trump(dealer_index, upcard);
		if (trump_maker == 0 || trump_maker == 2) {
			trump_team = 0;
		}
		else if (trump_maker == 1 || trump_maker == 3) {
			trump_team = 1;
		}
		cout << endl;

		int leader_index = (dealer_index + 1) % NUM_PLAYERS;
		for (int i = 0; i < num_tricks; i++) {
			leader_index = play_trick(leader_index);
			if (leader_index == 0 || leader_index == 2) {
				team0_tricks++;
			}
			else if (leader_index == 1 || leader_index == 3) {
				team1_tricks++;
			}
		}

		if (team1_tricks < team0_tricks) {
			cout << *players[0] << " and " << *players[2] 
					 << " win the hand" << endl;
			team0_points++;
		}
		else if (team1_tricks > team0_tricks) {
			cout << *players[1] << " and " << *players[3] 
					 << " win the hand" << endl;
			team1_points++;
		}
		//check if euchred or marched
		if (team0_tricks >= 3 && trump_team) {
			cout << "euchred!" << endl;
			team0_points++;
		}
		else if (team1_tricks >= 3 && !trump_team) {
			cout << "euchred!" << endl;
			team1_points++;
		}
		else if (team0_tricks == 5) {
			cout << "march!" << endl;
			team0_points++;
		}
		else if (team1_tricks == 5) {
			cout << "march!" << endl;
			team1_points++;
		}
		
		cout << *players[0] << " and " << *players[2] 
				 << " have " << team0_points << " points\n";
		cout << *players[1] << " and " << *players[3] 
				 << " have " << team1_points << " points\n\n";
	}
	
	//EFFECTS: Deal 5 cards to each player, starting to the left of the dealer.
	//         Returns the upcard.
	Card deal(int const dealer_index) {
		cout << *players[dealer_index] << " deals" << endl;

		int dealplayer = dealer_index;
		//first round
		for (int i = 0; i < 4; i++) {
			dealplayer = (dealplayer + 1) % NUM_PLAYERS;
			if (i%2 == 0) deal_three(dealplayer);
			else deal_two(dealplayer);
		}
		//second round
		for (int i = 0; i < 4; i++) {
			dealplayer = (dealplayer + 1) % NUM_PLAYERS;
			if (i%2 == 0) deal_two(dealplayer);
			else deal_three(dealplayer);
		}
		Card upcard = pack.deal_one();
		cout << upcard << " turned up" << endl;
		return upcard;
	};

	void deal_two(const int player_index) {
		players[player_index]->add_card(pack.deal_one());
		players[player_index]->add_card(pack.deal_one());
	}
	void deal_three(const int player_index) {
		players[player_index]->add_card(pack.deal_one());
		players[player_index]->add_card(pack.deal_one());
		players[player_index]->add_card(pack.deal_one());
	}
	//EFFECTS: Go through the process of choosing a trump suit
	int make_trump(int dealer_index, const Card upcard) {
		
		int curr_player = dealer_index;
		//round 1
		for (int i = 0; i < 4; i++) {
			curr_player = (curr_player + 1) % NUM_PLAYERS;
			if(players[curr_player]->make_trump(upcard, dealer_index == curr_player, 1, trump)){
				cout << *players[curr_player] << " orders up " << trump << endl;
				players[dealer_index]->add_and_discard(upcard);
				return curr_player;
			}
			else {
				cout << *players[curr_player] << " passes\n";
			}
		}
		//round 2
		for (int i = 0; i < 4; i++) {
			curr_player = (curr_player + 1) % NUM_PLAYERS;
			if(players[curr_player]->make_trump(upcard, dealer_index == curr_player, 2, trump)){
				cout << *players[curr_player] << " orders up " << trump << endl;
				return curr_player;
			}
			else {
				cout << *players[curr_player] << " passes\n";
			}
		}
		return dealer_index;
	};
	
	//EFFECTS: Play a single trick, given the leader.
	//         Returns the winner of the trick.
	int play_trick(int leader_index) {
		int play_index = leader_index;
		int winner = leader_index;
		Card led_card = players[leader_index]->lead_card(trump);
		cout << led_card << " led by " << *players[leader_index] << endl;
		Card highest = led_card;
		Card current_card;
		for (int i = 0; i < 3; i++) {
			play_index = (play_index + 1) % NUM_PLAYERS;
			current_card = players[play_index]->play_card(led_card, trump);
			cout << current_card << " played by " << *players[play_index] << endl;
			if(!Card_less(current_card, highest, led_card, trump)){
				highest = current_card;
				winner = play_index;
			}
		}
		cout << *players[winner] << " takes the trick\n\n";
		return winner;
	}

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
		string player_strategy = argv[4 + 2 * i + 1];

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