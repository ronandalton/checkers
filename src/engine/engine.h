#ifndef ENGINE_H
#define ENGINE_H


class Game;
class Move;


class Engine {
public:
	Move findBestMove(const Game &game);

private:
};


#endif
