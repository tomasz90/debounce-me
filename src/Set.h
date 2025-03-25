#ifndef DEBOUNCE_ME_SET_H
#define DEBOUNCE_ME_SET_H

#include "ArxContainer.h"

namespace arx {
    namespace stdx {

        template<typename T, size_t N = ARX_VECTOR_DEFAULT_SIZE>
        struct set : public vector<T, N> {
            set() : vector<T, N>() {}

            set(std::initializer_list<T> lst) : vector<T, N>() {
                for (const auto& item : lst) {
                    insert(item); // avoid duplicates
                }
            }

            // Updated insert() with duplicate checking
            virtual void insert(const T& data) {
                // Check for existing element
                for (size_t i = 0; i < this->size(); ++i) {
                    if ((*this)[i] == data) {
                        return; // Element already exists
                    }
                }

                // Add new element if space available
                if (this->size() < N) {
                    this->push_back(data);
                }
            }

            friend bool operator==(const set& a, const set& b) {
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

            friend bool operator<(const set& a, const set& b) {
                // Create sorted temporary copies
                T a_sorted[N], b_sorted[N];
                size_t a_size = 0, b_size = 0;

                // Copy elements
                for (const auto& item : a) a_sorted[a_size++] = item;
                for (const auto& item : b) b_sorted[b_size++] = item;

                // Bubble sort (no std::sort)
                for (size_t i = 0; i < a_size; ++i) {
                    for (size_t j = 0; j < a_size-1; ++j) {
                        if (a_sorted[j] > a_sorted[j+1]) {
                            T temp = a_sorted[j];
                            a_sorted[j] = a_sorted[j+1];
                            a_sorted[j+1] = temp;
                        }
                    }
                }

                for (size_t i = 0; i < b_size; ++i) {
                    for (size_t j = 0; j < b_size-1; ++j) {
                        if (b_sorted[j] > b_sorted[j+1]) {
                            T temp = b_sorted[j];
                            b_sorted[j] = b_sorted[j+1];
                            b_sorted[j+1] = temp;
                        }
                    }
                }

                // Lexicographical comparison
                size_t min_size = a_size < b_size ? a_size : b_size;
                for (size_t i = 0; i < min_size; ++i) {
                    if (a_sorted[i] < b_sorted[i]) return true;
                    if (b_sorted[i] < a_sorted[i]) return false;
                }
                return a_size < b_size;
            }

            bool isEmpty() const {
                return empty();
            }

        private:
            using vector<T, N>::empty;
            using vector<T, N>::push_back;
            using vector<T, N>::pop_back;
            using vector<T, N>::begin;
            using vector<T, N>::back;
            using vector<T, N>::emplace_back;
            using vector<T, N>::data;
            using vector<T, N>::front;
            using vector<T, N>::end;
            using vector<T, N>::erase;
            using vector<T, N>::shrink_to_fit;
            using vector<T, N>::resize;
            using vector<T, N>::reserve;
            using vector<T, N>::capacity;
        };
    } // namespace arx
} // namespace stdx

#endif //DEBOUNCE_ME_SET_H