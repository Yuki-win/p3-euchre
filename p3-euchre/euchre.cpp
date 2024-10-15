#include <iostream>
#include "Pack.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include <fstream>
using namespace std;



class Game {
 public:
  Game(string pack_file_name, bool shuffle, int winner_point, vector<string> Player_name, vector<string> Player_type){
    ifstream pack_file(pack_file_name);
    if (!pack_file.is_open()){
      cout << "Error opening " << pack_file_name << endl;
    }
    Pack(pack_file);

   this->shuffle(shuffle);

    vector<Player*> Player;
    for (int i = 0; i < Player_name.size(); ++i){
      Player.push_back(Player_factory(Player_name[i], Player_type[i]));
    }
  }
  void play(){
    int count_hand = 0;
    
  }
  
  

  private:
  Pack pack;
  vector <Player*> player;

  //private function 
  void shuffle(bool should_shuffle){
    if (should_shuffle){
      pack.shuffle();
    }
  }
  void deal(/* ... */);
  void make_trump(/* ... */);
  void play_hand(/* ... */);

};

int main(int argc, char* argv[]) {
// command line 
  if (argc != 12){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return -1;
  }
  
  string file_name = argv[1];
  bool shuffle = argv[2];
  int winner_point = stoi(argv[3]);
  vector<string> player_name = {argv[4],argv[6],argv[8],argv[10]};
  vector<string> player_type = {argv[5],argv[7],argv[9],argv[11]};

  


  Game game(file_name, shuffle, winner_point, player_name, player_type);
  game.play();

  // end of game
  for (size_t i = 0; i < players.size(); ++i) {
  delete players[i];
}


}


