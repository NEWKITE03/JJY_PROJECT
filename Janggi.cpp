#include "Janggi.h"
#include <algorithm>


JanggiPosition::JanggiPosition() :row(0), column(0) {};
JanggiPosition::JanggiPosition(int row, int column) :row(row), column(column) {}

JanggiPeice::JanggiPeice(Janggi* janggi, JanggiPosition pos, JanggiTeam team, JanggiPeiceType type) :janggi(janggi), pos(pos), team(team), type(type)
{
}

JanggiPeice::~JanggiPeice()
{
}

bool JanggiPosition::operator==(const JanggiPosition& pos) const {
	return pos.row == row && pos.column == column;
};
bool JanggiPosition::operator!=(const JanggiPosition& pos) const {
	return !(*this == pos);
};
JanggiPosition JanggiPosition::operator+(const JanggiPosition& pos) const {
	return { row + pos.row, column + pos.column };
};
JanggiPosition& JanggiPosition::operator+=(const JanggiPosition& pos) {
	row += pos.row;
	column += pos.column;
	return *this;
};


bool JanggiPeice::canPlace(JanggiPosition pos)
{
	return janggi->isValid(pos) && janggi->getPeice(pos) == nullptr;
}

bool JanggiPeice::canDestroy(JanggiPosition pos)
{
	JanggiPeice* peice = janggi->getPeice(pos);
	return  peice && peice->getTeam() != team;
}

bool JanggiPeice::canPlaceOrDestroy(JanggiPosition pos)
{
	return canPlace(pos) || canDestroy(pos);
}

JanggiTeam JanggiPeice::getTeam() const {
	return team;
}
JanggiPeiceType JanggiPeice::getType() const {
	return type;
}

std::vector<JanggiPosition> JanggiPeice::getMoves() {
	std::vector<JanggiPosition> moves;

	JanggiPosition directions[8] = { {0,1},{0,-1},{-1,0},{1,0}, {1,-1}, {1,1}, {-1,-1}, {-1,1} };

	switch (type)
	{
	case Gung:
	case Sa:
		for (int i = 0; i < ((janggi->isCastleCenter(pos, team) || janggi->isCastleVert(pos, team)) ? 8 : 4); i++)
		{
			JanggiPosition target = pos + directions[i];
			if (canPlaceOrDestroy(target) && janggi->isCastle(target, team))
				moves.push_back(target);
		}
		break;
		break;
	case Cha:
		for (int i = 0; i < ((janggi->isCastleCenter(pos) || janggi->isCastleVert(pos)) ? 8 : 4); i++)
		{
			JanggiPosition target = pos + directions[i];
			while (canPlace(target) && (i < 4 || janggi->isCastle(target)))
			{
				moves.push_back(target);
				target += directions[i];
			}
			if (canDestroy(target))
				moves.push_back(target);
		}
		break;
	case Po:
		for (int i = 0; i < 4; i++)
		{
			JanggiPosition target = pos + directions[i];
			while (canPlace(target)) {
				target += directions[i];
			}
			JanggiPeice* peice = janggi->getPeice(target);
			if (peice && peice->getType() != type)
			{
				while (target += directions[i], canPlace(target))
					moves.push_back(target);
				if (canDestroy(target) && janggi->getPeice(target) && janggi->getPeice(target)->getType() != type)
					moves.push_back(target);
			}
		}
		break;
	case Ma:
	case Sang:
		for (int i = 0; i < 4; i++)
		{
			for (auto& t : { -1, 1 })
			{
				JanggiPosition target = pos + directions[i];
				bool can_reach = true;

				for (int chk = 0; chk < (type == Ma ? 1 : 2); chk++)
				{
					if (!canPlace(target)) {
						can_reach = false;
						break;
					}
					target += directions[i];
					if (directions[i].row)
						target.column += t;
					else
						target.row += t;
				}
				if (can_reach && canPlaceOrDestroy(target))
					moves.push_back(target);
			}
		}
		break;
	case Byeong:
		for (int i = 0; i < ((janggi->isCastleCenter(pos) || janggi->isCastleVert(pos)) ? 8 : 4); i++)
		{
			if (directions[i].row < 0 && team == Red || directions[i].row > 0 && team == Blue) continue;

			JanggiPosition target = pos + directions[i];
			if (canPlaceOrDestroy(target) && (i < 4 || janggi->isCastle(target)))
				moves.push_back(target);
		}
		break;
	default:
		break;
	}

	return moves;
}

void JanggiPeice::move(JanggiPosition pos) {
	janggi->move(this->pos, pos);
	this->pos = pos;
}


Janggi::Janggi() {
	std::fill_n(&peices[0][0], GridRow * GridColumn, nullptr);
	reset();
}

Janggi::~Janggi() {
	std::for_each_n(&peices[0][0], GridRow * GridColumn, [](JanggiPeice* peice) {if (peice) delete peice; });
}

bool Janggi::isValid(JanggiPosition pos)
{
	return 0 <= pos.row && 0 <= pos.column && pos.row < GridRow&& pos.column < GridColumn;
}

bool Janggi::isCastle(JanggiPosition pos)
{
	return isCastle(pos, Red) || isCastle(pos, Blue);
}

bool Janggi::isCastle(JanggiPosition pos, JanggiTeam team)
{
	if (!isValid(pos)) return false;
	switch (team)
	{
	case Red:
		if (3 <= pos.column && pos.column <= 5 && pos.row <= 2)
			return true;
		break;
	case Blue:
		if (3 <= pos.column && pos.column <= 5 && pos.row >= 7)
			return true;
		break;
	}
	return false;
}


bool Janggi::isCastleCenter(JanggiPosition pos)
{
	return isCastleCenter(pos, Red) || isCastleCenter(pos, Blue);
}

bool Janggi::isCastleCenter(JanggiPosition pos, JanggiTeam team)
{
	if (!isValid(pos)) return false;
	switch (team)
	{
	case Red:
		if (4 == pos.column && pos.row == 1)
			return true;
		break;
	case Blue:
		if (4 == pos.column && pos.row == 8)
			return true;
		break;
	}
	return false;
}

bool Janggi::isCastleVert(JanggiPosition pos)
{
	return isCastleVert(pos, Red) || isCastleVert(pos, Blue);
}

bool Janggi::isCastleVert(JanggiPosition pos, JanggiTeam team)
{
	if (!isValid(pos)) return false;
	switch (team)
	{
	case Red:
		if ((3 == pos.column || pos.column == 5) && (pos.row == 2 || pos.row == 0))
			return true;
		break;
	case Blue:
		if ((3 == pos.column || pos.column == 5) && (pos.row == 7 || pos.row == 9))
			return true;
		break;
	}
	return false;
}

void Janggi::reset(int redToggle, int blueToggle)
{
	std::for_each_n(&peices[0][0], GridRow * GridColumn, [](JanggiPeice* peice) {if (peice) delete peice; });
	std::fill_n(&peices[0][0], GridRow * GridColumn, nullptr);

	peices[1][4] = new JanggiPeice(this, { 1,4 }, Red, Gung);
	peices[0][3] = new JanggiPeice(this, { 0,3 }, Red, Sa);
	peices[0][5] = new JanggiPeice(this, { 0,5 }, Red, Sa);
	peices[0][0] = new JanggiPeice(this, { 0,0 }, Red, Cha);
	peices[0][8] = new JanggiPeice(this, { 0,8 }, Red, Cha);

	if (redToggle & 1) {
		peices[0][1] = new JanggiPeice(this, { 0,1 }, Red, Ma);
		peices[0][2] = new JanggiPeice(this, { 0,2 }, Red, Sang);
	}
	else {
		peices[0][1] = new JanggiPeice(this, { 0,1 }, Red, Sang);
		peices[0][2] = new JanggiPeice(this, { 0,2 }, Red, Ma);
	}

	if (redToggle & 2) {
		peices[0][7] = new JanggiPeice(this, { 0,7 }, Red, Ma);
		peices[0][6] = new JanggiPeice(this, { 0,6 }, Red, Sang);
	}
	else {
		peices[0][7] = new JanggiPeice(this, { 0,7 }, Red, Sang);
		peices[0][6] = new JanggiPeice(this, { 0,6 }, Red, Ma);
	}
	peices[2][1] = new JanggiPeice(this, { 2,1 }, Red, Po);
	peices[2][7] = new JanggiPeice(this, { 2,7 }, Red, Po);
	peices[3][0] = new JanggiPeice(this, { 3,0 }, Red, Byeong);
	peices[3][2] = new JanggiPeice(this, { 3,2 }, Red, Byeong);
	peices[3][4] = new JanggiPeice(this, { 3,4 }, Red, Byeong);
	peices[3][6] = new JanggiPeice(this, { 3,6 }, Red, Byeong);
	peices[3][8] = new JanggiPeice(this, { 3,8 }, Red, Byeong);

	peices[8][4] = new JanggiPeice(this, { 8,4 }, Blue, Gung);
	peices[9][3] = new JanggiPeice(this, { 9,3 }, Blue, Sa);
	peices[9][5] = new JanggiPeice(this, { 9,5 }, Blue, Sa);
	peices[9][0] = new JanggiPeice(this, { 9,0 }, Blue, Cha);
	peices[9][8] = new JanggiPeice(this, { 9,8 }, Blue, Cha);

	if (blueToggle & 1) {
		peices[9][1] = new JanggiPeice(this, { 9,1 }, Blue, Ma);
		peices[9][2] = new JanggiPeice(this, { 9,2 }, Blue, Sang);
	}
	else {
		peices[9][1] = new JanggiPeice(this, { 9,1 }, Blue, Sang);
		peices[9][2] = new JanggiPeice(this, { 9,2 }, Blue, Ma);
	}

	if (blueToggle & 2) {
		peices[9][7] = new JanggiPeice(this, { 9,7 }, Blue, Ma);
		peices[9][6] = new JanggiPeice(this, { 9,6 }, Blue, Sang);
	}
	else {
		peices[9][7] = new JanggiPeice(this, { 9,7 }, Blue, Sang);
		peices[9][6] = new JanggiPeice(this, { 9,6 }, Blue, Ma);
	}

	peices[7][1] = new JanggiPeice(this, { 7,1 }, Blue, Po);
	peices[7][7] = new JanggiPeice(this, { 7,7 }, Blue, Po);
	peices[6][0] = new JanggiPeice(this, { 6,0 }, Blue, Byeong);
	peices[6][2] = new JanggiPeice(this, { 6,2 }, Blue, Byeong);
	peices[6][4] = new JanggiPeice(this, { 6,4 }, Blue, Byeong);
	peices[6][6] = new JanggiPeice(this, { 6,6 }, Blue, Byeong);
	peices[6][8] = new JanggiPeice(this, { 6,8 }, Blue, Byeong);
}

JanggiPeice* Janggi::getPeice(JanggiPosition pos) {
	if (isValid(pos))
		return peices[pos.row][pos.column];
	return nullptr;
}


void Janggi::move(JanggiPosition from, JanggiPosition to) {
	if (from != to && isValid(from) && isValid(to)) {
		if (peices[to.row][to.column]) delete peices[to.row][to.column];
		peices[to.row][to.column] = peices[from.row][from.column];
		peices[from.row][from.column] = nullptr;
	}
}