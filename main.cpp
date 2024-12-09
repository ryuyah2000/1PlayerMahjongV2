// basic mahjong game
// code by Ryuya


#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm> // shuffle

#include "score_calc.hpp"

// puts 4 copies of each tile into the wall
void initialize_wall(std::array<int, 136> &wall, const std::map<int, std::string> tiles) {
	int i = 0;
	for (std::pair<int, std::string> t : tiles) {
		// put 4 copies of each tile into the wall
		wall.at(i++) = t.first;
		wall.at(i++) = t.first;
		wall.at(i++) = t.first;
		wall.at(i++) = t.first;
	}
}


// shuffles wall
void shuffle_wall(std::array<int, 136> &wall) {
	std::mt19937 rng(std::time(nullptr));
	std::shuffle(wall.begin(), wall.end(), rng);
}


// generates hand from the first 14 tiles of the wall
int generate_hand(std::array<int, 136> wall, std::vector<int> &hand) {
	for (int i = 0; i < 14; ++i) {
		hand.at(i) = wall.at(i);
	}
	return 14;
}


// prints out hand
// kinda became complicated but basically just groups each tile with their suit
void print_hand(std::vector<int> &hand, const std::map<int, std::string> tile_names) {
	std::string man, pin, sou, honor = "";
	for (int i = 0; i < hand.size() - 1; ++i) {
		switch (hand.at(i) & 0xF0) {
		case 0x10: // manzu
			man += std::to_string(hand.at(i) & 0x0F);
			break;
		case 0x20: // pinzu
			pin += std::to_string(hand.at(i) & 0x0F);
			break;
		case 0x30: // souzu
			sou += std::to_string(hand.at(i) & 0x0F);
			break;
		case 0x40: // honor
			honor += tile_names.at(hand.at(i));
			break;
		}
	}
	std::cout << man << ((man == "") ? "" : "m ");
	std::cout << pin << ((pin == "") ? "" : "p ");
	std::cout << sou << ((sou == "") ? "" : "s ");
	std::cout << honor << "  draw: " << tile_names.at(hand.at(hand.size() - 1));
	std::cout << std::endl;
}


// sorts hand
void sort_hand(std::vector<int> &hand) {
	std::sort(hand.begin(), hand.end());
}


// check win helper
// assumes hand is sorted
bool check_win(std::vector<int> &hand, std::vector<bool> &visited, std::vector<bool> &win, bool pair_found = false) {

	// base case
	if (std::find(win.begin(), win.end(), false) == win.end()) {
		return true;
	}

	for (int i = 0; i < hand.size(); ++i) {
		if (!visited.at(i)) {

			// check subsequent tiles to see if they make a sequence, pair, or triplet
			for (int j = i + 1; j < hand.size(); ++j) {
				if (!visited.at(j)) {
					// if not visited, check if it makes a pair with the original
					if (hand.at(i) == hand.at(j)) {
						// pair found, mark as visited and won and search for next shape
						// if no pair found yet
						if (!pair_found) {
							visited.at(i) = visited.at(j) = true;
							win.at(i) = win.at(j) = true;
//std::cout<<"DEBUG: PAIR: "<<std::hex<<hand.at(i)<<hand.at(j)<<std::endl;
							// check hand again for more shapes
							if (check_win(hand, visited, win, true)) {
								return true;
							} else {
								// unmark as visited and won
								win.at(i) = win.at(j) = false;
								visited.at(i) = visited.at(j) = false;
							}

						// otherwise, check for triplets
						} else {
							// check for triplet
							if (j + 1 >= hand.size()) {
								// invalid hand, return false
								return false;
							} else if (!visited.at(j + 1)) {
								if (hand.at(j) == hand.at(j + 1)) {
									// triplet found, mark as visited and won and search for next shape
									visited.at(i) = visited.at(j) = visited.at(j + 1) = true;
									win.at(i) = win.at(j) = win.at(j + 1) = true;
//std::cout<<"DEBUG: TRIP: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(j+1)<<std::endl;
									if (check_win(hand, visited, win, pair_found)) {
										return true;
									} else {
										win.at(i) = win.at(j) = win.at(j + 1) = false;
										visited.at(i) = visited.at(j) = visited.at(j + 1) = false;
									}
								}
							}
						}
					}


					// check for sequence
					if (hand.at(i) + 1 == hand.at(j)) {
						// start of a sequence, check for another
						for (int k = j + 1; k < hand.size(); ++k) {
							if (hand.at(k) > hand.at(j) + 1) {
								break;
							}
							if (!visited.at(k)) {
								if (hand.at(j) + 1 == hand.at(k)) {
									// sequence found, mark as visited and won and search for next shape
									visited.at(i) = visited.at(j) = visited.at(k) = true;
									win.at(i) = win.at(j) = win.at(k) = true;
//std::cout<<"DEBUG: SEQU: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(k)<<std::endl;
									if (check_win(hand, visited, win, pair_found)) {
										return true;
									} else {
										win.at(i) = win.at(j) = win.at(k) = false;
										visited.at(i) = visited.at(j) = visited.at(k) = false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}


// check for win
bool check_win(std::vector<int> &hand) {
	// create a checked tile array
	std::vector<bool> win(hand.size(), false);
	std::vector<bool> visited(hand.size(), false);
	sort_hand(hand);

	// run the actual checker
	return check_win(hand, visited, win);
}


int main() {
	// map tile codes to their names as string literals
	const std::map<int, std::string> tile_names {
		// manzu
		{ 0x11, "1m" },
		{ 0x12, "2m" },
		{ 0x13, "3m" },
		{ 0x14, "4m" },
		{ 0x15, "5m" },
		{ 0x16, "6m" },
		{ 0x17, "7m" },
		{ 0x18, "8m" },
		{ 0x19, "9m" },

		// pinzu
		{ 0x21, "1p" },
		{ 0x22, "2p" },
		{ 0x23, "3p" },
		{ 0x24, "4p" },
		{ 0x25, "5p" },
		{ 0x26, "6p" },
		{ 0x27, "7p" },
		{ 0x28, "8p" },
		{ 0x29, "9p" },

		// souzu
		{ 0x31, "1s" },
		{ 0x32, "2s" },
		{ 0x33, "3s" },
		{ 0x34, "4s" },
		{ 0x35, "5s" },
		{ 0x36, "6s" },
		{ 0x37, "7s" },
		{ 0x38, "8s" },
		{ 0x39, "9s" },

		// honor tiles
		{ 0x41, "E" },
		{ 0x42, "S" },
		{ 0x43, "W" },
		{ 0x44, "N" },
		{ 0x45, "Wh" },
		{ 0x46, "Gr" },
		{ 0x47, "Re" }
	};

	// possible inputs for tiles
	const std::map<std::string, int> eng_to_tilecodes {
		// manzu
		{ "1m", 0x11 },
		{ "2m", 0x12 },
		{ "3m", 0x13 },
		{ "4m", 0x14 },
		{ "5m", 0x15 },
		{ "6m", 0x16 },
		{ "7m", 0x17 },
		{ "8m", 0x18 },
		{ "9m", 0x19 },

		// pinzu
		{ "1p", 0x21 },
		{ "2p", 0x22 },
		{ "3p", 0x23 },
		{ "4p", 0x24 },
		{ "5p", 0x25 },
		{ "6p", 0x26 },
		{ "7p", 0x27 },
		{ "8p", 0x28 },
		{ "9p", 0x29 },

		// souzu
		{ "1s", 0x31 },
		{ "2s", 0x32 },
		{ "3s", 0x33 },
		{ "4s", 0x34 },
		{ "5s", 0x35 },
		{ "6s", 0x36 },
		{ "7s", 0x37 },
		{ "8s", 0x38 },
		{ "9s", 0x39 },

		// honor tiles
		{ "1z", 0x41 },
		{ "e", 0x41 },
		{ "east", 0x41 },

		{ "2z", 0x42 },
		{ "south", 0x42 },
		{ "s", 0x42 },

		{ "3z", 0x43 },
		{ "w", 0x43 },
		{ "west", 0x43 },

		{ "4z", 0x44 },
		{ "n", 0x44 },
		{ "north", 0x44 },

		{ "5z", 0x45 },
		{ "wh", 0x45 },
		{ "white", 0x45 },

		{ "6z", 0x46 },
		{ "gr", 0x46 },
		{ "green", 0x46 },

		{ "7z", 0x47 },
		{ "re", 0x47 },
		{ "red", 0x47 }
	};


	std::array<int, 136> wall; // 136 tiles total. 4 of each type of tile.
	std::vector<int> hand(14); // 14 tiles in your hand
	std::vector<int> discard(24); // discarded tiles, usually does not go above 24 from playtesting
	int depth; // how deep into the wall you are
	initialize_wall(wall, tile_names);

	// print opening message
	std::cout << "Welcome to mahjong!" << std::endl;
	std::cout << "The objective of the game is to create 4 sets of three sequences or triplets and one pair" << std::endl;
	std::cout << "Type \"mahjong\" to win" << std::endl;
	std::cout << "But be careful! If you hand isn't valid, the round is over." << std::endl;
	std::cout << "Type q to quit anytime" << std::endl;

	// generate wall and starting hand
	shuffle_wall(wall);
	depth = generate_hand(wall, hand);
	sort_hand(hand);

	// game loop
	std::string input;
	while (input != "q") {
		std::cout << "Your hand: ";
		print_hand(hand, tile_names);
		std::cout << "Type the tile to discard" << std::endl;
		std::cout << "> ";

		std::cin >> input;
		auto tile = eng_to_tilecodes.find(input); // validate tile input
		if (tile != eng_to_tilecodes.end() && depth < 136) {
			// valid tile input, look for it in hand
			auto tilecode = std::find(hand.begin(), hand.end(), tile->second);
			if (tilecode != hand.end()) {
				// found, replace the tile
				hand.erase(tilecode);
				sort_hand(hand);
				hand.push_back(wall.at(depth++));
			} else {
				// print tile not found message
				std::cout << "Tile not found in hand" << std::endl;
			}
		} else if (input == "mahjong" || depth >= 136) {
			// check for win
			Win_checker checker(hand);
			if (checker.check_win_all()) {
				// win, add score and reset game
				std::cout << "Congratulations! You won!" << std::endl;
				std::cout << "Your hand:" << std::endl;
				checker.print_score(tile_names);
				std::cout << "Play again? (Y/N)" << std::endl;
				std::cin >> input;
				if (input == "n" || input == "q") {
					std::cout << "Thanks for playing!";
					input = "q"; // quit the game loop
				} else {
					// reset game
					shuffle_wall(wall);
					depth = generate_hand(wall, hand);
					sort_hand(hand);
				}
			} else {
				// not a win, reset game
				std::cout << "This is not a winning hand..." << std::endl;
				std::cout << "Play again? (Y/N)" << std::endl;
				std::cin >> input;
				if (input == "n" || input == "q") {
					std::cout << "Thanks for playing!";
					input = "q"; // quit the game loop
				} else {
					// reset game
					shuffle_wall(wall);
					depth = generate_hand(wall, hand);
					sort_hand(hand);
				}
			}
		} else if (input == "q") {
			std::cout << "Thanks for playing!";
		} else {
			std::cout << "Invalid tile" << std::endl;
		}
	}

	return 0;
}
