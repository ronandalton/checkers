#ifndef UTIL_H
#define UTIL_H


class Board;
class Move;

#include <string>
#include <vector>


class Util {
public:
	Util() = delete;

	static Board createBoard(const std::string &input);
	static bool moveListsAreEquivalent(const std::vector<Move> &list_1, const std::vector<Move> &list_2);
};


#endif
