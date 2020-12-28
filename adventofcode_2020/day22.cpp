#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>

using namespace std;

struct Player {
	queue<int> cards;
};

vector<Player> players;

struct Game {
	vector<Player> players;
	int round = 0;
	unordered_set<string> previousRoundsSet;
	int winner = 0;	// only valid if bGameOver = true
	bool bGameOver = false;

	void run();
	void runRecursive();

	string createRoundId()
	{
		string str;
	
		for (int i = 0; i < players.size(); i++) {
			str += "Player " + to_string(i) + ":";
			auto q = players[i].cards;	// create copy
			while (!q.empty()) {
				str += to_string(q.front()) + ",";	
				q.pop();
			}
		}
		return str;
	}


	int computePlayerScore(int index)
	{
		int score = 0;
		auto& player = players[index];
		auto queueLength = player.cards.size();
		auto q = player.cards;

		while (!q.empty()) {
			score += q.front() * (int)queueLength;
			q.pop();
			queueLength--;
		}
		return score;
	}

	void displayPlayers()
	{
		for (auto& player : players) {
			cout << "Player: ";
			auto cards = player.cards;
			while (!cards.empty()) {
				cout << cards.front() << " ";
				cards.pop();
			}
			cout << endl;
		}
	}

	void displayGameInfo() {
		cout << "Round " << round;
		if (bGameOver) {
			cout << " Game Over = " << bGameOver << " Winner = " << winner;
		}
		cout << endl;
		displayPlayers();
	}
};

void Game::run()
{
	while (!bGameOver) {

		round++;

		int card0 = players[0].cards.front();
		int card1 = players[1].cards.front();

		players[0].cards.pop();
		players[1].cards.pop();

		if (card0 > card1) {
			// player 0 wins
			players[0].cards.push(card0);
			players[0].cards.push(card1);
		}
		else {
			// player 1 wins
			players[1].cards.push(card1);
			players[1].cards.push(card0);
		}

		for (int i = 0; i < players.size(); i++) {
			if (players[i].cards.size() == 0) {
				bGameOver = true;
				winner = i ? 0 : 1;
			}
		}

		//displayGameInfo();

	}
}

auto runGame()
{
	Game game;
	game.players = players; // make copy

	game.run();
	game.displayPlayers();

	return make_tuple(game.winner, game.computePlayerScore(game.winner));
}


void Game::runRecursive()
{

	while (!bGameOver) {
		round++;

		// check if card configuration has already occurred in a previous round
		// => instant win for player 1
		string roundId = createRoundId();
		if (previousRoundsSet.find(roundId) != previousRoundsSet.end()) {
			// found
			bGameOver = true;
			winner = 0;	// 0 - based index
			return;
		}
		previousRoundsSet.insert(roundId);

		// each player draws a card
		int card0 = players[0].cards.front();
		players[0].cards.pop();
		int card1 = players[1].cards.front();
		players[1].cards.pop();

		// if both players remaining cards >= drawn card value
		// => winner of round = runRecursiveRound();		i.e. run subgame with copy of decks
		// else
		// => winner of the round = player with higher card
		int roundWinner = -1;

		if (players[0].cards.size() >= card0 &&
			players[1].cards.size() >= card1) {

			// create new game
			Game newGame;
			newGame.players.push_back(Player());
			newGame.players.push_back(Player());

			// copy queues
			queue q0 = players[0].cards;
			for (int i = 0; i < card0; i++) {
				newGame.players[0].cards.push(q0.front());
				q0.pop();
			}

			queue q1 = players[1].cards;
			for (int i = 0; i < card1; i++) {
				newGame.players[1].cards.push(q1.front());
				q1.pop();
			}

			newGame.runRecursive();
			if (newGame.bGameOver == false) {
				cout << "ERROR - expected game over" << endl;
			}
			roundWinner = newGame.winner;
		}
		else {
			if (card0 > card1) {
				roundWinner = 0;
			}
			else {
				roundWinner = 1;

			}
		}
		// place winning cards at bottom of winner's deck
		if (roundWinner != 0 && roundWinner != 1) {
			cout << "ERROR - roundWinner = " << roundWinner << endl;
		}
		if (roundWinner == 0) {
			players[0].cards.push(card0);
			players[0].cards.push(card1);
		}
		else {
			players[1].cards.push(card1);
			players[1].cards.push(card0);
		}

		// check for game over
		for (int i = 0; i < players.size(); i++) {
			if (players[i].cards.size() == 0) {
				bGameOver = true;
				winner = i ? 0 : 1;
			}
		}

		//displayGameInfo();
	}
}


auto runRecursiveGame()
{
	Game game;

	game.players = players;	//make copy
	game.runRecursive();
	game.displayPlayers();

	return make_tuple(game.winner, game.computePlayerScore(game.winner));
}

void day22()
{
	fstream myfile("day22_input.txt", ios_base::in);
	string str;

	// read rules
	int numPlayers = 0;
	while (getline(myfile, str)) {
		cout << str << endl;
		if (str.substr(0, 6) == "Player") {
			players.push_back(Player());
			numPlayers++;
			continue;
		}
		if (str.length() == 0) {
			continue;
		}
		players[numPlayers - 1].cards.push(stoi(str));
	}

	// PART 1
	auto [winner, score] = runGame();
	cout << "Part1: Winner is Player " << winner << endl;
	cout << "Score = " << score << endl;

	// PART 2
	auto [winner2, score2] = runRecursiveGame();
	cout << "Part2: Winner is Player " << winner2 << endl;
	cout << "Score = " << score2 << endl;

}