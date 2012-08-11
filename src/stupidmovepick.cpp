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

#include <algorithm>
#include <tr1/functional>
#include <cassert>
#include "stupidmovepick.h"

/// Constructors of the MovePicker class. As arguments we pass information
/// to help it to return the presumably good moves first, to decide which
/// moves to return (in the quiescence search, for instance, we only want to
/// search captures, promotions and some checks) and about how important good
/// move ordering is at the current node.

StupidMovePicker::StupidMovePicker(
    const Position& p, Move ttm, Depth d, const History& h, Search::Stack* ss,
    Value beta, int skl, Key foc
      ) : MovePicker::MovePicker(p,ttm,d,h,ss,beta), skill(skl), focus(foc) {
  
  lastPrunedLegalMove = prunedLegalMoves-1;
  haveReturnedLegalMove = false;
}

StupidMovePicker::StupidMovePicker(
    const Position& p, Move ttm, Depth d, const History& h, Square sq, int skl,
    Key foc
      ) : MovePicker::MovePicker(p,ttm,d,h,sq), skill(skl), focus(foc) {

  lastPrunedLegalMove = prunedLegalMoves-1;
  haveReturnedLegalMove = false;
}

StupidMovePicker::StupidMovePicker(
    const Position& p, Move ttm, const History& h, PieceType pt, int skl,
    Key foc
      ) : MovePicker::MovePicker(p,ttm,h,pt), skill(skl), focus(foc) {

  lastPrunedLegalMove = prunedLegalMoves-1;
  haveReturnedLegalMove = false;
}

Move StupidMovePicker::next_move() {
  Move move;
  while ((move=MovePicker::next_move()) != MOVE_NONE) {
    if (!willPrune(move)) {
      if  (!haveReturnedLegalMove) {
        if (pos.pl_move_is_legal(move, pos.pinned_pieces())) {

          haveReturnedLegalMove = true;
          return move;
        } else continue;
      } else return move;
    } else if ( !haveReturnedLegalMove
                && pos.pl_move_is_legal(move, pos.pinned_pieces())) {

      *++lastPrunedLegalMove = move;
    }
  }
  if (!haveReturnedLegalMove && lastPrunedLegalMove >= prunedLegalMoves) {
    unPruneIndex = hash(pos.key() ^ focus ^ 0x2425910a) % (lastPrunedLegalMove-prunedLegalMoves+1);

    haveReturnedLegalMove = true;
    return prunedLegalMoves[unPruneIndex];
  }
  assert(move == MOVE_NONE);
  return move;
}

bool StupidMovePicker::willPrune(Move m) {
  // FIXME: DUMB PLACEHOLDER IMPLEMENTATION
  number = hash(pos.key() ^ focus ^ 0x8ed1c471 + m) & 0xff;
  if (number > 155+skill*5) return true; else return false;
}

// Naively borrowed from Paul Hsieh; see http://www.azillionmonkeys.com/qed/hash.html
uint64_t StupidMovePicker::hash(uint64_t n) {
    n ^= n << 3;
    n += n >> 5;
    n ^= n << 4;
    n += n >> 17;
    n ^= n << 25;
    n += n >> 6;
                        
    return n;
}
