#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"

using namespace std;

class EuchreGame {
 public:
  EuchreGame(Pack gamePack, bool shuffleCards, int winPoints,
    vector<Player*> &playersInGame):
    shuffle(shuffleCards), pointsNeededToWin(winPoints){
        players = playersInGame;

        pack = gamePack;
        team1Points = 0;
        team2Points = 0;
    };

  void startGame() {
    while (team1Points < pointsNeededToWin && team2Points < pointsNeededToWin){
        // which hand(s);
        cout << "Hand " << currentHand << endl;
        currentHand++;

        Suit currentTrump = SPADES;
        shufflePack();
        dealCards(players, trumpCard);
        // who is the dealer;
        cout << players[dealerIndex % 4]->get_name() << " deals" << endl;
        // upcard;
        cout << trumpCard << " turned up" << endl;

        playHand(currentTrump, dealerIndex);
        dealerIndex++;
    }
    if (team1Points >= pointsNeededToWin){
        cout << players[0]->get_name() << " and " <<
        players[2]->get_name() << " win!" << endl;
        return;
    }
    else {
        cout << players[1]->get_name() << " and " <<
        players[3]->get_name() << " win!" << endl;
        return;
    }
  }

 private:
  std::vector<Player*> players;
  Card trumpCard; // to make trump
  Pack pack;
  bool shuffle = false; 
  int pointsNeededToWin; 
  int dealerIndex = 0; 
  int team1Points; int team2Points; 
  int currentHand = 0;
  
  
  void shufflePack(){
    if (shuffle == true){
        pack.shuffle();
    }
    else{
        pack.reset();
    }
  }
  
  void dealTwoCards(int playerIndex){
    players[playerIndex]->add_card(pack.deal_one());
    players[playerIndex]->add_card(pack.deal_one());
    return;
  }

  void dealThreeCards(int playerIndex){
    players[playerIndex]->add_card(pack.deal_one());
    players[playerIndex]->add_card(pack.deal_one());
    players[playerIndex]->add_card(pack.deal_one());
    return;
  }

  void dealCards(vector<Player*> players, Card &firstCard){
    
    // first round;
    for (int i = 1; i <= 4; i++){
        if(i % 2 == 1){
            dealThreeCards((dealerIndex + i) % 4);
        }
        else if (i % 2 == 0){
            dealTwoCards((dealerIndex + i) % 4);
        }
    }

    // second round;
    for (int i = 1; i <= 4; i++){
        if (i % 2 == 1){
            dealTwoCards((dealerIndex + i) % 4);
        }
        else if (i % 2 == 0){
            dealThreeCards((dealerIndex + i) % 4);
        }
    }

    // trumpCard;
    firstCard = pack.deal_one();
  };

  
   int selectTrump(int dealerIdx, const Card trumpCard, Suit &trumpSuit){
    
    // round 1;
    int round = 1;
    for(int i = 1; i <= 4; i++){
        int currentPlayer = (dealerIdx + i) % 4;
        if (players[currentPlayer]->make_trump(trumpCard, 
            dealerIdx % 4 == currentPlayer, round, trumpSuit)){ 
                cout << players[currentPlayer]->get_name() << " orders up " << trumpSuit 
                << endl;
                players[dealerIdx % 4]->add_and_discard(trumpCard);
                return currentPlayer;
        }
        else{
            cout << players[currentPlayer]->get_name() << 
                " passes" << endl;
        }
    }

    // round 2;
    round = 2;
    for (int i = 1; i <= 4; i++){
        int currentPlayer = (dealerIdx + i) % 4;
        if (players[currentPlayer]->make_trump(trumpCard,
            dealerIdx % 4 == currentPlayer, round, trumpSuit)){
                cout << players[currentPlayer]->get_name() << " orders up " << trumpSuit 
                    << "\n\n";
                return currentPlayer;
        }
        else{
            cout << players[currentPlayer]->get_name() <<
                " passes" << endl;
        }
    }
    return dealerIdx % 4;
  };

void displayScoreMessage(int team1Score, int team2Score, int trumpTeam){
    if (team1Score > team2Score){
        if (trumpTeam == 2){
            team1Points += 2;
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " win the hand" << endl;
            cout << "euchred!" << endl;
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " have " << team1Points << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " have " << team2Points << " points" << endl << endl;
        }
        else if(trumpTeam == 1){
            team1Points += 1;
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " win the hand" << endl;
            if (team1Score == 5) {
                cout << "march!" << endl;
                team1Points++;
            }
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " have " << team1Points << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " have " << team2Points << " points" << endl << endl;
            
        }
    }
    else if (team2Score > team1Score){
        if (trumpTeam == 1){
            team2Points += 2;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " win the hand" << endl;
            cout << "euchred!" << endl;
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " have " << team1Points << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " have " << team2Points << " points" << endl << endl;
        }
        else if (trumpTeam == 2){
            team2Points++;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " win the hand" << endl;
            if (team2Score == 5){
                cout << "march!" << endl;
                team2Points++;
            }
            cout << players[0]->get_name() << " and " << players[2]->get_name() <<
                " have " << team1Points << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name() <<
                " have " << team2Points << " points" << endl << endl;
        }
    }
    }

  int determineTrickWinner(const Suit trumpSuit, int dealerIdx){
     
    // play a trick

    // first player leads a card;
    Card card1 = players[dealerIdx % 4]->lead_card(trumpSuit);
    cout << card1 << " led by " << players[dealerIdx % 4]->get_name() << endl;
    // second player leads a card;
    Card card2 = players[(dealerIdx + 1) % 4]->play_card(card1, trumpSuit);
    cout << card2 << " played by " << players[(dealerIdx + 1) % 4]->get_name() << endl;
    // third player leads a card;
    Card card3 = players[(dealerIdx + 2) % 4]->play_card(card1, trumpSuit);
    cout << card3 << " played by " << players[(dealerIdx + 2) % 4]->get_name() << endl;
    // forth player leads a card;
    Card card4 = players[(dealerIdx + 3) % 4]->play_card(card1, trumpSuit);
    cout << card4 << " played by " << players[(dealerIdx + 3) % 4]->get_name() << endl;

    // find the winning card
    Card winningCard = card1;

    if (Card_less(winningCard, card2, card1, trumpSuit)){
        winningCard = card2;
    }
    if (Card_less(winningCard, card3, card1, trumpSuit)){
        winningCard = card3;
    }
    if (Card_less(winningCard, card4, card1, trumpSuit)){
        winningCard = card4;
    }

    // find the card owner;
    if (winningCard == card1) return dealerIdx % 4;
    else if (winningCard == card2) return (dealerIdx + 1) % 4;
    else if (winningCard == card3) return (dealerIdx + 2) % 4;
    else if (winningCard == card4) return (dealerIdx + 3) % 4;
    return -1;
  };

  void playHand(Suit trumpSuit, int dealerIdx){
    int winningPlayer;
    int team1Score = 0;
    int team2Score = 0;
    int trumpTeam = 0;
    // make trump;
    int trumpMaker = selectTrump(dealerIdx, trumpCard, trumpSuit);
    // which is trump maker's team;
    if (trumpMaker == 0 || trumpMaker == 2){
        trumpTeam = 1;
    }
    else if (trumpMaker == 1 || trumpMaker == 3){
        trumpTeam = 2;
    }
    // for first round; first left player of dealer leads;
    winningPlayer = determineTrickWinner(trumpSuit, (dealerIdx + 1) % 4);
    //cout << winningPlayer << endl;
    cout << players[winningPlayer]->get_name() << " takes the trick" << 
        endl << endl;
    if (winningPlayer == 0 || winningPlayer == 2){
        team1Score++;
    }
    else if(winningPlayer == 1 || winningPlayer == 3){
        team2Score++;
    }
    // for the next 4 rounds;
    for(int i = 1; i < 5; i++){
        winningPlayer = determineTrickWinner(trumpSuit, winningPlayer);
        cout << players[winningPlayer]->get_name() << " takes the trick" << 
            endl << endl;
        if (winningPlayer == 0 || winningPlayer == 2){
            team1Score++;
        }
        else if(winningPlayer == 1 || winningPlayer == 3){
            team2Score++;
        }
    }
    // find hand winners;

    displayScoreMessage(team1Score, team2Score, trumpTeam);
  }
};

 

void displayWarning() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
}


int main(int argc, char **argv) {
  string inputFile = argv[1];
  ifstream inFile(argv[1]);
  if (!inFile.is_open()){
    cout << "Error opening " << inputFile << endl;
  }
  Pack gamePack(inFile);


  // read win points;
  int winPoints = atoi(argv[3]);

  // check shuffle or not;
  string shuffleOption = argv[2];
    bool shuffleCards;
    if(shuffleOption == "shuffle"){
        shuffleCards = true;
    }
    else{
        shuffleCards = false;
    }
  
  // read players;  
  vector<Player*> playersInGame;
    playersInGame.push_back(Player_factory(argv[4], argv[5]));
    playersInGame.push_back(Player_factory(argv[6], argv[7]));
    playersInGame.push_back(Player_factory(argv[8], argv[9]));
    playersInGame.push_back(Player_factory(argv[10], argv[11]));

    // give command;
    cout << "./euchre.exe " << inputFile << " " <<
    argv[2] << " " << winPoints << " " << 
    playersInGame[0]->get_name() << " " << argv[5] << " " << 
    playersInGame[1]->get_name() << " " << argv[7] << " " << 
    playersInGame[2]->get_name() << " " << argv[9] << " " << 
    playersInGame[3]->get_name() << " " << argv[11] << " " << endl;

    // set up game;
    EuchreGame *game = new EuchreGame(gamePack, shuffleCards, winPoints,
                        playersInGame); 
    
    // play the game;    
    game->startGame();

    // delete dynamics;
    for (int i = 0; i < 4; i++) {
		delete playersInGame[i];
	}
	
    delete game;
    return 0;
}
