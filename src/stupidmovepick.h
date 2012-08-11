/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2012 Marco Costalba, Joona Kiiski, Tord Romstad

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined STUPIDMOVEPICK_H_INCLUDED
#define STUPIDMOVEPICK_H_INCLUDED

#include "movepick.h"

/// MovePicker class is used to pick one pseudo legal move at a time from the
/// current position. The most important method is next_move(), which returns a
/// new pseudo legal move each time it is called, until there are no moves left,
/// when MOVE_NONE is returned. In order to improve the efficiency of the alpha
/// beta algorithm, MovePicker attempts to return the moves which are most likely
/// to get a cut-off first.

class StupidMovePicker : public MovePicker {

public:
  StupidMovePicker(const Position&, Move, Depth, const History&,
    Search::Stack*, Value, int skill, Key focus);
  StupidMovePicker(const Position&, Move, Depth, const History&, Square, 
    int skill, Key focus);
  StupidMovePicker(const Position&, Move, const History&, PieceType, int skill,
    Key focus);
  Move next_move(); // Override MovePicker::next_move()

private:
  bool willPrune(Move);
  uint64_t hash(uint64_t);

  int skill;
  Key focus;
  bool haveReturnedLegalMove;
  int unPruneIndex, number;
  Move *lastPrunedLegalMove;
  Move prunedLegalMoves[MAX_MOVES];
};

#endif // !defined(STUPIDMOVEPICK_H_INCLUDED)
