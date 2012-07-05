
#include "/module/fight/fight.h"

static mapping sideRow = ([
	FT_SIDE_ATTACK:({
		({1, 2, 3}),
		({4, 5, 6}),
		({7, 8, 9}),
	}),
				 FT_SIDE_DEFENSE:({
					({12, 11, 10}),
					({15, 14, 13}),
					({18, 17, 16}),
				}),
]);

static mapping sideCol = ([
	FT_SIDE_ATTACK:({
		({3, 6, 9}),
		({2, 5, 8}),
		({1, 4, 7}),
	}),
	FT_SIDE_DEFENSE:({
		({12, 15, 18}),
		({11, 14, 17}),
		({10, 13, 16}),
	}),
]);

int position2row(int pos)
{
	if (pos > FT_SIDE_SIZE) {
		pos -= FT_SIDE_SIZE;
	}

	if (pos <= 3) {
		return 0;
	} else if (pos <= 6) {
		return 1;
	} else {
		return 2;
	}
}

int position2col(int pos)
{
	if (pos > FT_SIDE_SIZE) {
		pos -= FT_SIDE_SIZE;
	}

	int ret = pos % 3;
	if (ret == 0) return 0;
	else if(ret == 1) return 2;
	else return 1;
}

static class Warrior * select(class Fight fight, int side, int *ps)
{
	class Warrior w;
	class Warrior *ret = ({});
	mapping pw = fight->side[side]->warriors;

	foreach(int p in ps) {
		w = pw[p];
		if (w) {
			ret += ({w});
		}
	}

	return ret;
}

static class Warrior * selectRow(class Fight fight, int side, int row)
{
	return select(fight, side, sideRow[side][row]);
}

static class Warrior * selectCol(class Fight fight, int side, int col)
{
	return select(fight, side, sideCol[side][col]);
}


class Warrior * selectColTargets(class Fight fight, int side, int col)
{
	class Warrior * ws = selectCol(fight, side, col);
	if (sizeof(ws) > 0) {
		return ws;
	}
	
	int c;
	for(col = 0; c < sizeof(sideCol[side]); c++) {
		if (c != col) {
			ws = selectCol(fight, side, c);
			if (sizeof(ws) > 0) {
				return ws;
			}

		}
	}

	return ({});
}

class Warrior * selectRowTargets(class Fight fight, int side, int row)
{
	class Warrior * ws = selectRow(fight, side, row);
	if (sizeof(ws) > 0) {
		return ws;
	}
	
	int r;
	for(r = 0; r < sizeof(sideRow[side]); r++) {
		if (r != row) {
			ws = selectRow(fight, side, r);
			if (sizeof(ws) > 0) {
				return ws;
			}
		}
	}

	return ({});
}

static int sortWarrior(class Warrior a, class Warrior b)
{
	return a->position > b->position ? -1 : 1;
}

class Warrior selectPositionTarget(class Fight fight, int side, int row)
{
	class Warrior * ws = selectRow(fight, side, row);
	if (sizeof(ws) > 0) {
		return sort_array(ws, (:sortWarrior:))[0];
	}
	
	int r;
	for(r = 0; r < sizeof(sideRow[side]); r++) {
		if (r != row) {
			ws = selectRow(fight, side, r);
			if (sizeof(ws) > 0) {
				return sort_array(ws, (:sortWarrior:))[0];
			}
		}
	}

	return 0;
}

