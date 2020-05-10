//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_ROWBOUNDS_HPP
#define MAPPER_ROWBOUNDS_HPP

/**
 * 相当于limit
 */
class RowBounds {
private:
    int offset = 0;
    int limit = 0;

public:
    int getOffset() const {
        return offset;
    }

    int getLimit() const {
        return limit;
    }

public:
    RowBounds() {
        this->limit = 2147483647;
    }

    RowBounds(int offset, int limit) {
        this->offset = offset;
        this->limit = limit;
    }

};

#endif //MAPPER_ROWBOUNDS_HPP
