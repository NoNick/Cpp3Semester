#include <inttypes.h>
#include <assert.h>
#include <algorithm>
#include <string.h>

const int smallSize = 32;
struct container {
    uint32_t small[smallSize];
    std::vector <uint32_t> *big;
    size_t *cnt, size_;

    container() {
        size_ = 0;
    }

    container(container const& other) {
        load(other);
    }

    ~container() {
        deleteBig();
    }

    void load(container const &other){
        size_ = other.size_;
        if (size_ <= (unsigned)smallSize) {
            // possible overlap at self-assignment
            memmove(small, other.small, sizeof(uint32_t) * other.size_);
        }
        else {
            big = other.big;
            cnt = other.cnt;
            (*cnt)++;
        }
    }

    void fork() {
        if (size_ > (unsigned) smallSize && (*cnt) > 1) {
            big = new std::vector <uint32_t>(*big);
            (*cnt)--;
            cnt = new size_t(1);
        }
    }

    void deleteBig() {
        if (size_ > (unsigned) smallSize) {
            (*cnt)--;
            if (*cnt == 0) {
                delete cnt;
                delete big;
            }
        }
    }

    void createBig() {
        big = new std::vector <uint32_t>(small, small + size_);
        cnt = new size_t(1);
    }

    size_t size() const{
        return size_;
    }

    container& operator =(container const& other) {
        container old(*this);
        load(other);
        old.deleteBig();
        return *this;
    }

    uint32_t& operator [](size_t pos) {
        fork();
        if (size_ <= (unsigned) smallSize)
            return small[pos];
        else
            return big->operator [](pos);
    }

    uint32_t const& operator [](size_t pos) const {
        if (size_ <= (unsigned) smallSize)
            return small[pos];
        else
            return big->operator [](pos);
    }

    uint32_t& front() {
        if (size_ <= (unsigned) smallSize)
            return small[0];
        else
            return big->operator [](0);
    }

    uint32_t& back() {
        if (size_ <= (unsigned) smallSize)
            return small[size_ - 1];
        else
            return big->operator [](size_ - 1);
    }

    void push_back(uint32_t x) {
        fork();

        if (size_ + 1 > (unsigned) smallSize) {
            if (size_ == (unsigned) smallSize)
                createBig();
            big->push_back(x);
            size_++;
        }
        else {
            small[size_] = x;
            size_++;
        }
    }

    void resize(size_t newSize) {
        fork();

        // new is small object, old is big
        if (newSize <= (unsigned) smallSize && size_ > (unsigned) smallSize) {
            for (size_t i = 0; i < newSize; i++) {
                small[i] = big->operator [](i);
            }

            deleteBig();
            size_ = newSize;
        }
        // both objects are small
        else if (newSize <= (unsigned) smallSize) {
            for (size_t i = size_; i < newSize; i++) {
                small[i] = 0;
            }
            size_ = newSize;
        }
        // new object is big, the old one is small
        else if (newSize > (unsigned) smallSize && size_ <= (unsigned) smallSize) {
            createBig();
            size_ = newSize;
            big->resize(size_);
        }
        // both objects are big
        else {
            size_ = newSize;
            big->resize(size_);
        }
    }

    void setUint64Vector(std::vector <uint64_t> const& x) {
        resize(x.size());
        for (size_t i = 0; i < size_; i++) {
            operator [](i) = x[i];
        }
    }
};
