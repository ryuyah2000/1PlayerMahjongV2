// function declarations for score calculator
// code by Ryuya


#include "score_calc.hpp"
#include <iostream>
#include <map>


Pattern::Pattern(std::string n, int p) {
	name = n;
	points = p;
}



// full flush
// hand is entirely 1 suit
// assumes hand is sorted
bool Full_Flush::has_pattern(std::vector<int>& hand) {
	// get first tile's suit
	int suit = (hand.at(0) & 0xF0);

	// return false if honors
	if (suit == 0x40) {
		return false;
	}

	// check for single suit
	for (int i = 0; i < hand.size(); ++i) {
		if ((hand.at(i) & 0xF0) != suit) {
			return false;
		}
	}

	return true;
}


// half flush
// hand is entirely 1 suit with honor tiles
// assumes hand is sorted
bool Half_Flush::has_pattern(std::vector<int>& hand) {
	// get first tile's suit
	int suit = 0;
	for (int i = 0; i < hand.size(); ++i) {
		suit = (hand.at(i) & 0XF0);

		// return false if all honors
		if (suit != 0x40) {
			break;
		}
	}

	// return false if all honors
	if (suit == 0x40) {
		return false;
	}

	// check for single suit  + honors
	int honor_count = 0;
	for (int i = 0; i < hand.size(); ++i) {
		if ((hand.at(i) & 0xF0) != suit) {
			if ((hand.at(i) & 0xF0) != 0x40) {
				return false;
			} else {
				++honor_count;
			}
		}
	}

	return honor_count > 0;
}


// all triplets
// hand is made up of only triplets
// assumes hand is sorted
bool All_Triplets::has_pattern(std::vector<int>& hand) {
	// find triplets
	bool pair_found = false;
	int i = 0;

	while (i < hand.size() - 1) {
		if (hand.at(i) == hand.at(i + 1)) {
			if (i + 2 < hand.size() && hand.at(i + 2) == hand.at(i)) {
				// triplet found
				// contiune
				i += 3;
			} else {
				// pair found
				if (!pair_found) {
					i += 2;
					pair_found = true;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	}

	return true;
}


// all honors
// hand is made up of only honors
// strictly harder than all triplets
// assumes hand is sorted
bool All_Honors::has_pattern(std::vector<int>& hand) {
	// find triplets
	bool pair_found = false;
	int i = 0;
	while (i < hand.size() - 1) {
		// check for honor tile
		if ((hand.at(i) & 0xF0) == 0x40) {
			if (hand.at(i) == hand.at(i + 1)) {
				if (i + 2 < hand.size() && hand.at(i + 2) == hand.at(i)) {
					// triplet found
					// contiune
					i += 3;
				} else {
					// pair found
					if (!pair_found) {
						i += 2;
						pair_found = true;
					} else {
						return false;
					}
				}
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	return true;
}


// mixed triple sequence
// hand has the same sequence in 3 different suits
// assumes hand has pair at end
bool Mixed_Triple_Sequence::has_pattern(std::vector<int>& hand) {
	// brute force it
	std::vector<int> manzu;
	std::vector<int> pinzu;
	std::vector<int> souzu;

	for (int i = 0; i < 12; i += 3) {
		if (hand.at(i) + hand.at(i + 1) + hand.at(i + 2) == (hand.at(i) + 1) * 3) {
			// sequence found, add to array
			if ((hand.at(i) & 0xF0) == 0x10) {
				manzu.push_back(hand.at(i) + 1);
			} else if ((hand.at(i) & 0xF0) == 0x20) {
				pinzu.push_back(hand.at(i) + 1);
			} else if ((hand.at(i) & 0xF0) == 0x30) {
				souzu.push_back(hand.at(i) + 1);
			}
		}
	}

	for (auto m : manzu) {
		for (auto p : pinzu) {
			for (auto s : souzu) {
				if (m + 0x10 == p && m + 0x20 == s) {
					return true;
				}
			}
		}
	}

	return false;
}


// simple constructor that takes a hand and initiates every variable used for checking for wins
Win_checker::Win_checker(std::vector<int>& h) : win(h.size(), false), visited(h.size(), false) {
	hand = h;
	num_combinations = 0;
	winning = false;
	score = 0;
	std::sort(hand.begin(), hand.end());

	// add all the hand shapes
	Full_Flush* f = new Full_Flush();
	patterns.push_back(f);

	Half_Flush* hf = new Half_Flush();
	patterns.push_back(hf);

	All_Triplets* at = new All_Triplets();
	patterns.push_back(at);

	All_Honors* ah = new All_Honors();
	patterns.push_back(ah);

	Mixed_Triple_Sequence* mts = new Mixed_Triple_Sequence();
	patterns.push_back(mts);
}


Win_checker::~Win_checker() {
	for (int i = 0; i < patterns.size(); ++i) {
		delete patterns.at(i);
	}
}


// prints out hand
// kinda became complicated but basically just groups each tile with their suit
void Win_checker::print_hand(const std::map<int, std::string> tile_names) const {
	std::string man, pin, sou, honor = "";
	for (int i = 0; i < hand.size(); ++i) {
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
	std::cout << honor;
	std::cout << std::endl;
}


int Win_checker::print_score(const std::map<int, std::string> tile_names) {
	print_hand(tile_names);
	std::vector<int> tmp_hand;
	for (int i = 0; i < combos.size(); ++i) {
		if (shapes.at(i) == Set_shape::PAIR) {
//std::cout << "PAIR: ";
		} else if (shapes.at(i) == Set_shape::TRIPLET) {
			tmp_hand.insert(tmp_hand.end(), combos.at(i).begin(), combos.at(i).end());
//std::cout << "TRIPLET: ";
		} else if (shapes.at(i) == Set_shape::SEQUENCE) {
			tmp_hand.insert(tmp_hand.end(), combos.at(i).begin(), combos.at(i).end());
//std::cout << "SEQUENCE: ";
		}
		for (int j = 0; j < combos.at(i).size(); ++j) {
//std::cout << std::hex << combos.at(i).at(j);
		}
//std::cout << std::endl;
	}

	for (int i = 0; i < combos.size(); ++i) {
		if (shapes.at(i) == Set_shape::PAIR) {
			tmp_hand.insert(tmp_hand.end(), combos.at(i).begin(), combos.at(i).end());
		}
	}

	for (auto p : patterns) {
		if (p->has_pattern(tmp_hand)) {
			std::cout << p->name << ": " << p->points << " points" << std::endl;
			score += p->points;
		}
	}
	std::cout << "Your hand is worth " << score << " points." << std::endl;
	return score;
}


// check win
bool Win_checker::check_win_all(bool pair_found) {

	// base case
	if (std::find(win.begin(), win.end(), false) == win.end()) {
		//++num_combinations;
		//winning = true;
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
//std::cout<<"DEBUG: PAIR: "<<std::hex<<hand.at(i)<<hand.at(j)<<" @"<<deep<<"deep"<<std::endl;
							// check hand again for more shapes
							if (check_win_all(true)) {
//std::cout<<"DEBUG: ADDING PAIR TO COMBINATIONS: "<<std::hex<<hand.at(i)<<hand.at(j)<<std::endl;
								std::vector<int> pair = { hand.at(i), hand.at(j) };
								combos.push_back(pair);
								shapes.push_back(Set_shape::PAIR);
//++num_combinations;
								winning = true;
//return true;
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
//std::cout<<"DEBUG: TRIP: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(j+1)<<" @"<<deep<<"deep"<<std::endl;
									if (check_win_all(pair_found)) {
//std::cout<<"DEBUG: ADDING TRIPLET TO COMBINATIONS: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(j+1)<<std::endl;
										std::vector<int> set = { hand.at(i), hand.at(j), hand.at(j + 1) };
										combos.push_back(set);
										shapes.push_back(Set_shape::TRIPLET);
//++num_combinations;
										winning = true;
//return true;
									} else {
										win.at(i) = win.at(j) = win.at(j + 1) = false;
										visited.at(i) = visited.at(j) = visited.at(j + 1) = false;
									}
								}
							}
						}
					} else {


						// check for sequence
						if (hand.at(i) + 1 == hand.at(j)) {
							// start of a sequence, check for another
							for (int k = j + 1; k < hand.size(); ++k) {
								if (hand.at(k) > hand.at(j) + 1) {
									break;
								} else {
									if (!visited.at(k)) {
										if (hand.at(j) + 1 == hand.at(k)) {
											// sequence found, mark as visited and won and search for next shape
											visited.at(i) = visited.at(j) = visited.at(k) = true;
											win.at(i) = win.at(j) = win.at(k) = true;
//std::cout<<"DEBUG: SEQU: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(k)<<" @"<<deep<<"deep"<<std::endl;
											if (check_win_all(pair_found)) {
//std::cout<<"DEBUG: ADDING SEQUENCE TO COMBINATIONS: "<<std::hex<<hand.at(i)<<hand.at(j)<<hand.at(k)<<std::endl;
												std::vector<int> set = { hand.at(i), hand.at(j), hand.at(k) };
												combos.push_back(set);
												shapes.push_back(Set_shape::SEQUENCE);
//++num_combinations;
												winning = true;
//return true;
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
		}
	}

	return winning;
}
