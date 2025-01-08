#ifndef _ARRAYVECTOR_HPP_
#define _ARRAYVECTOR_HPP_

#include <initializer_list>
#include <vector>
#include <optional>
#include <functional>
#include <algorithm>
#include <numeric>

namespace app_helpers::array_helper
{
    template <typename T>
    class ArrayVector
    {
    private:
        std::vector<T> arr;
        // int size, capacity;
        // void extend()
        // {
        //     const int newCapacity = this->capacity ? this->capacity * 2 : 1;
        //     T *tempArr = new T[newCapacity];
        //     for (int i = 0; i < this->capacity; i++)
        //     {
        //         tempArr[i] = this->arr[i];
        //     }
        //     delete[] arr;
        //     this->arr = tempArr;
        //     this->capacity = newCapacity;
        // };
        // void rightshift(const int stopAt, const int steps)
        // {
        //     const int newCapacity = this->capacity ? this->capacity * 2 : 1;
        //     T *tempArr = new T[newCapacity];
        //     for (int i = 0; i < this->capacity; i++)
        //     {
        //         tempArr[i] = this->arr[i];
        //     }
        //     delete[] arr;
        //     this->arr = tempArr;
        //     this->capacity = newCapacity;
        // };
        // void leftshift(const int startAt, const int steps)
        // {
        //     for (int i = startAt; i < this->size - steps; i++)
        //     {
        //         this->arr[i] = this->arr[i + steps];
        //     }
        // };
        // void quickSort(const int start, const int end, const int order)
        // {
        //     if (end > start)
        //     {
        //         const int pivotIndex = this->partition(start, end, order);
        //         this->quickSort(start, pivotIndex - 1, order);
        //         this->quickSort(pivotIndex + 1, end, order);
        //     }
        // };
        // int partition(const int start, const int end, const int order)
        // {
        //     int j = start - 1;
        //     int i = start;
        //     int pivotIndex = end;
        //     for (; i < pivotIndex; i++)
        //     {
        //         if (order >= 0 ? this->arr[i] <= this->arr[pivotIndex] : this->arr[i] > this->arr[pivotIndex])
        //         {
        //             this->swap(this->arr[i], this->arr[++j]);
        //         }
        //     }
        //     this->swap(this->arr[++j], this->arr[pivotIndex]);
        //     return j;
        // };
        // void swap(T &a, T &b)
        // {
        //     const T temp = a;
        //     a = b;
        //     b = temp;
        // };

    public:
        // Array(const int capacity = 10)
        // {
        //     auto sizeT = capacity < 0 ? capacity * -1 : capacity;
        //     this->capacity = sizeT;
        //     this->size = 0;
        //     this->arr = new T[sizeT];
        // };
        ArrayVector(const ArrayVector &arr)
        {
            this->arr = arr.arr;
        };
        // Constructor nhận một danh sách khởi tạo
        ArrayVector(std::initializer_list<T> init) : arr(init)
        {
        }
        // Array(const T *arr, const int size)
        // {
        //     this->capacity = size;
        //     this->size = size;
        //     this->arr = new T[this->size];
        //     for (int i = 0; i < size; i++)
        //     {
        //         this->arr[i] = arr[i];
        //     }
        // };
        ArrayVector()
        {
            this->arr = std::vector<T>();
        }
        ArrayVector(const std::vector<T> arr, const int size)
        {
            //     this->capacity = size;
            //     this->size = size;

            this->arr = arr;
            // for (int i = 0; i < size; i++)
            // {
            //     this->arr[i] = arr[i];
            // }
        }
        // Constructor mới nhận std::array
        template <std::size_t N>
        ArrayVector(const std::array<T, N> &stdArr)
        {

            for (int i = 0; i < stdArr.size(); ++i)
            {
                this->arr.push_back(stdArr[i]);
            }
        }
        ~ArrayVector()
        {
            this->arr.clear();
        };

        T operator[](int index)
        {
            index = index < 0 ? index % this->arr.size() * -1 : index % this->arr.size();
            return this->arr[index];
        };
        std::vector<T> getVector()
        {
            return this->arr;
        }
        void setVector(const std::vector<T> &arr)
        {
            this->arr = arr;
        }
        // T peak(int index)
        // {
        //     return this->operator[](index);
        // };

        // int length()
        // {
        //     return this->size;
        // };

        // Array<T> &pop()
        // {
        //     this->splice(this->size - 1, 1, {});
        //     return *this;
        // };
        // Array<T> &shift()
        // {
        //     return this->splice(0, 1, {});
        // };
        // Array<T> &splice(const int startingIndex, const int noItemsToDelete, std::initializer_list<T> items)
        // {
        //     const int itemsSize = items.size();
        //     if (startingIndex < 0 ||
        //         startingIndex > this->size ||
        //         noItemsToDelete < 0 ||
        //         noItemsToDelete > this->size - startingIndex)
        //     {
        //         return *this;
        //     }
        //     while (itemsSize > (this->capacity + noItemsToDelete) - this->size)
        //     {
        //         this->extend();
        //     }
        //     if (noItemsToDelete < itemsSize)
        //     {
        //         this->rightshift(startingIndex + noItemsToDelete, itemsSize - noItemsToDelete);
        //         this->size += itemsSize - noItemsToDelete;
        //     }
        //     else if (noItemsToDelete > itemsSize)
        //     {
        //         this->leftshift(startingIndex + itemsSize, noItemsToDelete - itemsSize);
        //         this->size -= noItemsToDelete - itemsSize;
        //     }
        //     T const *itemsPtr = items.begin();
        //     for (int i = startingIndex, j = 0; i < itemsSize + startingIndex; i++, j++)
        //     {
        //         this->arr[i] = itemsPtr[j];
        //     }
        //     return *this;
        // };
        // Array<T> &push(const T item)
        // {
        //     return this->splice(this->size, 0, {item});
        // }

        // Array<T> &unshift(const T item)
        // {
        //     return this->splice(0, 0, {item});
        // };

        // bool sort(const int start, const int end, const int order = 1)
        // {
        //     if (start < 0 || start > this->size - 1 || end < 0 || end > this->size - 1)
        //     {
        //         return false;
        //     }
        //     this->quickSort(start, end, order);
        //     return true;
        // };
        // bool sort(const int order = 1)
        // {
        //     return this->sort(0, this->length() - 1, order);
        // };

        // Array<T> &copy(const Array &arr)
        // {
        //     this->size = arr.size;
        //     for (int i = 0; i < arr.size; i++)
        //     {
        //         this->splice(i, 1, {arr.arr[i]});
        //     }
        //     return *this;
        // };
        // Array<T> &concat(const Array &arr)
        // {
        //     for (int i = 0; i < arr.size; i++)
        //     {
        //         this->splice(this->size, 0, {arr.arr[i]});
        //     }
        //     return *this;
        // };
        // Array<T> &concat(std::initializer_list<T> arr)
        // {
        //     this->splice(this->size, 0, arr);
        //     return *this;
        // };

        // bool includes(const T item)
        // {
        //     for (int i = 0; i < this->size; i++)
        //     {
        //         if (this->arr[i] == item)
        //         {
        //             return true;
        //         }
        //     }
        //     return false;
        // };
        // int indexOf(const T item)
        // {
        //     for (int i = 0; i < this->size; i++)
        //     {
        //         if (this->arr[i] == item)
        //         {
        //             return i;
        //         }
        //     }
        //     return -1;
        // };
        // int lastIndexOf(const T item)
        // {
        //     for (int i = this->size - 1; i >= 0; i--)
        //     {
        //         if (this->arr[i] == item)
        //         {
        //             return i;
        //         }
        //     }
        //     return -1;
        // };

        // T *begin()
        // {
        //     return this->arr;
        // };
        // T *end()
        // {
        //     return this->arr + this->size - 1;
        // };

        // void forEach(void (*callback)(T item, int index))
        // {
        //     for (int i = 0; i < this->size; i++)
        //     {
        //         callback(this->arr[i], i);
        //     }
        // };

        ArrayVector<T> map(std::function<T(T, int)> func)
        {
            ArrayVector<T> arr;

            std::vector<int> indexArr;
            for (int i = 0; i < this->arr.size(); i++)
            {
                indexArr.push_back(i);
            };

            std::vector<T> arrResult;
            std::transform(this->arr.begin(), this->arr.end(), indexArr.begin(), std::back_inserter(arrResult), func);
            arr.setVector(arrResult);
            return arr;
        };
        ArrayVector<T> filter(std::function<bool(T)> func)
        {

            auto arrVector = this->arr;

            // auto excludeItems = std::remove_if(arrVector.begin(), arrVector.end(), func);

            // arrVector.erase(excludeItems, arrVector.end());

            // Array<T> ret(arrVector, arrVector.size());
            std::vector<T> result = {};
            std::copy_if(arrVector.begin(), arrVector.end(), std::back_inserter(result), func);
            ArrayVector<T> ret(result, result.size());

            return ret;
        };
        // T reduce(T (*callback)(T acc, T item, int index), const T initialiValue)
        // {
        //     T accumulator = initialiValue;
        //     for (int i = 0; i < this->size; i++)
        //     {
        //         accumulator = callback(accumulator, this->arr[i], i);
        //     }
        //     return accumulator;
        // };
        // T reduce(T (*callback)(T acc, T item, int index))
        // {
        //     return this->reduce(callback, this->arr[0]);
        // };
        std::optional<T> find(std::function<bool(T)> func)
        {
            auto findedItem = std::find_if(this->arr.begin(), this->arr.end(), func);

            if (findedItem == this->arr.end())
                return std::nullopt;

            return std::make_optional(*findedItem);
        };
        int findIndex(std::function<bool(T)> func)
        {
            auto findedItem = std::find_if(this->arr.begin(), this->arr.end(), func);

            if (findedItem == this->arr.end())
                return -1;

            return (findedItem - this->arr.begin());
        };
        std::string join(std::string separator)
        {
            // Join vector elements with a separator
            std::string result = std::accumulate(this->arr.begin() + 1, this->arr.end(), this->arr[0],
                                                 [&separator](const std::string &a, const std::string &b)
                                                 {
                                                     return a + separator + b;
                                                 });
            return result;
        };
        bool includes(const T &value)
        {
            return std::find(this->arr.begin(), this->arr.end(), value) != this->arr.end();
        }
    };
}

#endif
