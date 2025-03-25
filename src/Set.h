#ifndef DEBOUNCE_ME_SET_H
#define DEBOUNCE_ME_SET_H

#include "ArxContainer.h"

namespace arx {
    namespace stdx {

        template<typename T, size_t N = ARX_VECTOR_DEFAULT_SIZE>
        struct set : public vector<T, N> {

            set() : vector<T, N>() {}

            set(std::initializer_list<T> lst) : vector<T, N>() {
                for (const auto &item: lst) insert(item); // avoid duplicates
            }

            virtual void insert(const T &data) {
                // Check for existing element
                for (size_t i = 0; i < this->size(); ++i) {
                    if ((*this)[i] == data) {
                        return; // Element already exists
                    }
                }
                // Add new element if space available
                if (this->size() < N) this->push_back(data);
            }

            bool exist(const T &data) {
                for (size_t i = 0; i < this->size(); ++i) {
                    if ((*this)[i] == data) {
                        return true;
                    }
                }
                return false;
            }

            bool erase(const T &data) {
                // Find position using binary search
                size_t low = 0;
                size_t high = this->size();
                while (low < high) {
                    size_t mid = (low + high) / 2;
                    if ((*this)[mid] < data) {
                        low = mid + 1;
                    } else {
                        high = mid;
                    }
                }

                // Check if element exists
                if (low >= this->size() || (*this)[low] != data) {
                    return false;
                }

                // Shift elements left
                for (size_t i = low; i < this->size() - 1; ++i) {
                    (*this)[i] = (*this)[i + 1];
                }
                this->pop_back();
                return true;
            }

            friend bool operator==(const set &a, const set &b) {
                if (a.size() != b.size()) return false;
                for (size_t i = 0; i < a.size(); ++i) {
                    bool found = false;
                    for (size_t j = 0; j < b.size(); ++j) {
                        if (a[i] == b[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) return false;
                }
                return true;
            }

            friend bool operator<(const set &a, const set &b) {
                if (a.size() != b.size()) return a.size() < b.size();

                // If sizes are equal, compare elements
                for (size_t i = 0; i < a.size(); ++i) {
                    if (a[i] < b[i]) return true;
                    if (b[i] < a[i]) return false;
                }
                return false; // Sets are equal
            }

        private:
            using vector<T, N>::push_back;
            using vector<T, N>::pop_back;
            using vector<T, N>::front;
            using vector<T, N>::back;
            using vector<T, N>::emplace_back;
            using vector<T, N>::data;
            using vector<T, N>::erase;
            using vector<T, N>::shrink_to_fit;
            using vector<T, N>::resize;
            using vector<T, N>::reserve;
            using vector<T, N>::capacity;
        };
    } // namespace arx
} // namespace stdx

#endif //DEBOUNCE_ME_SET_H