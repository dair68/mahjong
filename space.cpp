#include "space.h"

bool operator==(const struct Space& spaceLeft, const struct Space& spaceRight) {
    return spaceLeft.col == spaceRight.col && spaceLeft.row == spaceRight.row;
}

bool operator!=(const struct Space& spaceLeft, const struct Space& spaceRight) {
    return spaceLeft.col != spaceRight.col || spaceLeft.row != spaceRight.row;
}

QDebug& operator<<(QDebug& debug, const struct Space& space) {
    debug << "{" << space.col << "," << space.row << "}";
    return debug;
}
