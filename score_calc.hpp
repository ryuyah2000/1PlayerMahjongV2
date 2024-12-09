#ifndef __SCORE_CALC_HPP__
#define __SCORE_CALC_HPP__

#include <vector>


enum class Set_shape {
	PAIR, TRIPLET, SEQUENCE
};


class Win_checker {
private:
	std::vector<int> hand;
	std::vector<bool> win;
	std::vector<bool> visited;

	//std::vector< std::vector<std::vector<int>> > combinations;
	std::vector<std::vector<int>> combos;
	std::vector<Set_shape> shapes;
	int num_combinations;

	bool check_win_helper();
	bool winning;
public:
	Win_checker(std::vector<int>& h);
	~Win_checker() = default;

	bool check_win_all(bool pair_found = false);
	int get_score();
};


#endif
