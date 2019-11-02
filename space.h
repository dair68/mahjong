#ifndef SPACE_H
#define SPACE_H

#include <QDebug>

struct Space {
    int col;
    int row;
};

//checks if two spaces have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator==(const struct Space& spaceLeft, const struct Space& spaceRight);

//checks if two spaces do not have same row and column on grid
//@param spaceLeft - space on left side of operator
//@param spaceRight - space on right side of operator
bool operator!=(const struct Space& spaceLeft, const struct Space& spaceRight);

//outputs space in the format {col, row}
//@param os - some output stream
//@param space - space to be outputted
QDebug& operator<<(QDebug& debug, const struct Space& space);

#endif // SPACE_H
