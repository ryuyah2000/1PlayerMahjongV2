#ifndef __SCORE_CALC_HPP__
#define __SCORE_CALC_HPP__

#include <vector>
#include <string>
#include <map>

enum class Set_shape {
	PAIR, TRIPLET, SEQUENCE
};


class Pattern {
public:
	int points;
	std::string name;
	Pattern(std::string n, int p);
	virtual ~Pattern() = default;
	virtual bool has_pattern(std::vector<int>& hand) {
		return false;
	}
};


class Full_Flush : public Pattern {
public:
	using Pattern::Pattern;
	Full_Flush() : Pattern("Full Flush", 6) {}; // full flush valued at 6 points
	~Full_Flush() = default;
	bool has_pattern(std::vector<int>& hand);
};


class Half_Flush : public Pattern {
public:
	using Pattern::Pattern;
	Half_Flush() : Pattern("Half Flush", 3) {}; // half flush valued at 3 points
	~Half_Flush() = default;
	bool has_pattern(std::vector<int>& hand);
};


class All_Triplets : public Pattern {
public:
	using Pattern::Pattern;
	All_Triplets() : Pattern("All Triplets", 3) {}; // all triplets valued at 3 points
	~All_Triplets() = default;
	bool has_pattern(std::vector<int>& hand);
};


class All_Honors : public Pattern {
public:
	using Pattern::Pattern;
	All_Honors() : Pattern("All Honors", 16) {}; // all honors valued at 16 points
	~All_Honors() = default;
	bool has_pattern(std::vector<int>& hand);
};


class Mixed_Triple_Sequence : public Pattern {
public:
	using Pattern::Pattern;
	Mixed_Triple_Sequence() : Pattern("Mixed Triple Sequence", 3) {}; // mixed triple sequence valued at 3 points
	~Mixed_Triple_Sequence() = default;
	bool has_pattern(std::vector<int>& hand);
};





// win checker class
class Win_checker {
private:
	std::vector<Pattern*> patterns;

	std::vector<int> hand;
	std::vector<bool> win;
	std::vector<bool> visited;

	//std::vector< std::vector<std::vector<int>> > combinations;
	std::vector<std::vector<int>> combos;
	std::vector<Set_shape> shapes;
	int num_combinations;

	bool check_win_helper();
	bool winning;
	int score;
public:
	Win_checker(std::vector<int>& h);
	~Win_checker();

	bool check_win_all(bool pair_found = false);
	int print_score(const std::map<int, std::string> tile_names);
	void print_hand(const std::map<int, std::string> tile_names) const;
};


#endif
