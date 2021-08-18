#include "chesspoint.h"

const QVector<ChessPoint> ChessPoint::CAMP_POINTS = {
    ChessPoint(2, 1), ChessPoint(2, 3),
    ChessPoint(3, 2), ChessPoint(4, 1),
    ChessPoint(4, 3), ChessPoint(7, 1),
    ChessPoint(7, 3), ChessPoint(8, 2),
    ChessPoint(9, 1), ChessPoint(9, 3)
};

const QVector<ChessPoint> ChessPoint::BREAK_POINTS = {
    ChessPoint(5, 1), ChessPoint(5, 3),
    ChessPoint(6, 1), ChessPoint(6, 3)
};

bool ChessPoint::isCamp() const {
    return CAMP_POINTS.contains(*this);
}

bool ChessPoint::isOnRailway() const {
    if (x() == 5 || x() == 6) { return true; }
    if (y() == 0 || y() == 4) {
        return x() != 0 && x() != 11;
    }

    return false;
}

bool ChessPoint::isAround(const ChessPoint &another) const {
    if (*this == another) { return false; }

    if (BREAK_POINTS.contains(*this) && BREAK_POINTS.contains(another)) {
        return false;
    }

    if (isCamp() || another.isCamp()) {
        return abs(another.x() - x()) <= 1 && abs(another.y() - y()) <= 1;
    }

    if (another.x() == x()) {
        return abs(another.y() - y()) == 1;
    }

    if (another.y() == y()) {
        return abs(another.x() - x()) == 1;
    }

    return false;
}