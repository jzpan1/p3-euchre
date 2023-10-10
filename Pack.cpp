#include "Card.hpp"
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include "Pack.hpp"

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on.
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  Pack::Pack(){
    Card current;
    next = 0;
    for (int i = 0; i < PACK_SIZE; i++) {
        Card c((Rank)(7 + i % 6), (Suit)(i/(PACK_SIZE / 4)));
        cards[i] = c;
    }
  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  Pack::Pack(std::istream& pack_input){
    next = 0;
    std::string s;
    Card c;
    int i = 0;
    while (pack_input >> c) {
        cards[i] = c;
        i++;
    }
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one(){
    Card c = cards[next];
    next ++;
    return c;
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset(){
    next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle(){
    next = 0;
    std::array <int, 24> shuff = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
    for(int j = 0; j < 7; j++){
      for(int i = 0; i < PACK_SIZE; i++){
        cards[shuff[i]] = cards[i];
      }
    }
  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const{
    if(next == 24){
        return true;
    }
    return false;
  }
