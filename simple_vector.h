#pragma once

#include <cstddef> // for size_t
#include <algorithm>
#include <cassert>
#include <initializer_list>

#include "array_ptr.h"

struct ReserveProxyObj {
    explicit ReserveProxyObj(size_t capacity_to_reserve)
            : capacity(capacity_to_reserve) {}

    size_t capacity;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
        : items_(size > 0 ? ArrayPtr<Type>(size) : ArrayPtr<Type>()),
          size_(size),
          capacity_(size) {
        std::fill(items_.Get(), items_.Get() + size_, Type{});
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value)
        : items_(ArrayPtr<Type>(size)),
          size_(size),
          capacity_(size) {
        std::fill(items_.Get(), items_.Get() + size_, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
        : items_(init.size() > 0 ? ArrayPtr<Type>(init.size()) : ArrayPtr<Type>()),
          size_(init.size()),
          capacity_(init.size()) {
        std::copy(init.begin(), init.end(), items_.Get());
    }

    // Конструктор копирования
    SimpleVector(const SimpleVector& other)
        : items_(other.size_ > 0 ? ArrayPtr<Type>(other.size_) : ArrayPtr<Type>()),
          size_(other.size_),
          capacity_(other.capacity_) {
        std::copy(other.items_.Get(), other.items_.Get() + size_, items_.Get());
    }

    // Конструктор перемещения
    SimpleVector(SimpleVector&& other) noexcept
        : items_(std::move(other.items_)) {
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }

    // Конструктор, принимающий ReserveProxyObj
    explicit SimpleVector(ReserveProxyObj reserve)
            : items_(reserve.capacity ? new Type[reserve.capacity] : nullptr)
            , size_(0)
            , capacity_(reserve.capacity) {
    }


    // oператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            SimpleVector temp(rhs);
            swap(temp);
        }
        return *this;
    }

    // оператор присваивания перемещением
    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this != &rhs) {
            items_.swap(rhs.items_);
            size_ = std::exchange(rhs.size_, 0);
            capacity_ = std::exchange(rhs.capacity_, 0);
        }
        return *this;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < size_ && "Index out of range");
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < size_ && "Index out of range");
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size < size_) {
            size_ = new_size;
        } else if (new_size <= capacity_) {
            std::fill(items_.Get() + size_, items_.Get() + new_size, Type{});
            size_ = new_size;
        } else {
            ArrayPtr<Type> new_data(new_size);
            std::move(items_.Get(), items_.Get() + size_, new_data.Get());
            std::fill(new_data.Get() + size_, new_data.Get() + new_size, Type{});
            items_.swap(new_data);
            capacity_ = new_size;
            size_ = new_size;
        }
    }


    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> new_data(new_capacity);
            std::move(items_.Get(), items_.Get() + size_, new_data.Get());
            items_.swap(new_data);
            capacity_ = new_capacity;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return items_.Get() + size_;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(size_ > 0 && "Cannot pop from an empty vector");
        --size_;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& value) {
        if (size_ >= capacity_) {
            capacity_ = std::max(capacity_ * 2, size_t(1));
            ArrayPtr<Type> new_data(capacity_);
            std::move(items_.Get(), items_.Get() + size_, new_data.Get());
            items_.swap(new_data);
        }
        items_[size_] = value;
        ++size_;
    }

    void PushBack(Type&& value) {
        if (size_ >= capacity_) {
            Reserve(std::max(capacity_ * 2, size_t(1)));
        }
        items_[size_] = std::move(value);
        ++size_;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, Type value) {
        assert(pos >= begin() && pos <= end() && "Position out of range");

        size_t index = pos - begin();
        if (size_ >= capacity_) {
            Reserve(std::max(capacity_ * 2, size_t(1)));
        } else {
            std::move_backward(items_.Get() + index, items_.Get() + size_, items_.Get() + size_ + 1);
        }

        items_[index] = std::move(value);
        ++size_;
        return items_.Get() + index;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos < end() && "Position out of range");

        size_t index = pos - begin();
        std::move(items_.Get() + index + 1, items_.Get() + size_, items_.Get() + index);
        --size_;
        return items_.Get() + index;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        using std::swap;
        items_.swap(other.items_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

private:
    ArrayPtr<Type> items_;

    size_t size_{0};
    size_t capacity_{0};
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() &&
           std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return  std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                         rhs.cbegin(), rhs.cend());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}