// function declarations for score calculator
// code by Ryuya


#include "score_calc.hpp"
#include <iostream>



// simple constructor that takes a hand and initiates every variable used for checking for wins
Win_checker::Win_checker(std::vector<int>& h) : win(h.size(), false), visited(h.size(), false) {
	hand = h;
	num_combinations = 0;
	winning = false;
	std::sort(hand.begin(), hand.end());
}


int Win_checker::get_score() {
	for (int i = 0; i < combos.size(); ++i) {
		if (shapes.at(i) == Set_shape::PAIR) {
			std::cout << "PAIR: ";
		} else if (shapes.at(i) == Set_shape::TRIPLET) {
			std::cout << "TRIPLET: ";
		} else if (shapes.at(i) == Set_shape::SEQUENCE) {
			std::cout << "SEQUENCE: ";
		}
		for (int j = 0; j < combos.at(i).size(); ++j) {
			std::cout << std::hex << combos.at(i).at(j);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
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
