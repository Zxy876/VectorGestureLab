#pragma once
#include <iostream>
#include <stdexcept>

template<typename T>
class MyVector {
private:
    T* data_;          // 指向动态数组的指针
    size_t size_;      // 当前元素个数
    size_t capacity_;  // 当前容量

public:
    // 构造函数
    MyVector() : data_(nullptr), size_(0), capacity_(0) {}

    // 析构函数
    ~MyVector() {
        delete[] data_;
    }

    // 返回当前元素数量
    size_t size() const { return size_; }

    // 返回容量
    size_t capacity() const { return capacity_; }

    // 下标访问
    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    // 自动扩容的 push_back
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    // 删除最后一个元素
    void pop_back() {
        if (size_ > 0) --size_;
    }

    // 调整容量
    void reserve(size_t newCap) {
        if (newCap <= capacity_) return;
        T* newData = new T[newCap];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCap;
    }

    // 清空所有元素
    void clear() {
        size_ = 0;
    }

    // 输出调试
    void print() const {
        std::cout << "[ ";
        for (size_t i = 0; i < size_; ++i)
            std::cout << data_[i] << " ";
        std::cout << "] (size=" << size_ << ", cap=" << capacity_ << ")\n";
    }
    // ========== 拷贝构造函数 ==========
MyVector(const MyVector& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    std::cout << "[Copy Constructor] created\n";
}

// ========== 拷贝赋值运算符 ==========
MyVector& operator=(const MyVector& rhs) {
    if (this == &rhs) return *this;  // 自赋值检测
    delete[] data_;

    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
    data_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = rhs.data_[i];
    }
    std::cout << "[Copy Assignment] executed\n";
    return *this;
}

// ========== 移动构造函数 ==========
MyVector(MyVector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    std::cout << "[Move Constructor] moved\n";
}

// ========== 移动赋值运算符 ==========
MyVector& operator=(MyVector&& rhs) noexcept {
    if (this == &rhs) return *this;
    delete[] data_;
    data_ = rhs.data_;
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
    std::cout << "[Move Assignment] executed\n";
    return *this;
}
    // ================== 迭代器实现 ==================
// ================== 迭代器实现 ==================
public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

    private:
        T* ptr_;

    public:
        explicit Iterator(T* ptr = nullptr) : ptr_(ptr) {}

        // 基本访问
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }

        // 前置/后置 ++ --
        Iterator& operator++() { ++ptr_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++ptr_; return tmp; }
        Iterator& operator--() { --ptr_; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --ptr_; return tmp; }

        // 算术运算（支持随机访问）
        Iterator operator+(difference_type n) const { return Iterator(ptr_ + n); }
        Iterator operator-(difference_type n) const { return Iterator(ptr_ - n); }
        difference_type operator-(const Iterator& other) const { return ptr_ - other.ptr_; }
        Iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        Iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

        // 比较运算
        bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const Iterator& other)  const { return ptr_ < other.ptr_; }
        bool operator>(const Iterator& other)  const { return ptr_ > other.ptr_; }
        bool operator<=(const Iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>=(const Iterator& other) const { return ptr_ >= other.ptr_; }
    };

    Iterator begin() { return Iterator(data_); }
    Iterator end()   { return Iterator(data_ + size_); }
    // const 版本（支持 const MyVector）
    class ConstIterator {
    private:
        const T* ptr_;
    public:
        explicit ConstIterator(const T* ptr = nullptr) : ptr_(ptr) {}
        const T& operator*() const { return *ptr_; }
        const T* operator->() const { return ptr_; }
        ConstIterator& operator++() { ++ptr_; return *this; }
        ConstIterator operator++(int) { ConstIterator tmp = *this; ++ptr_; return tmp; }
        bool operator==(const ConstIterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const ConstIterator& other) const { return ptr_ != other.ptr_; }
    };

    ConstIterator begin() const { return ConstIterator(data_); }
    ConstIterator end() const   { return ConstIterator(data_ + size_); }
    ConstIterator cbegin() const { return ConstIterator(data_); }
    ConstIterator cend() const   { return ConstIterator(data_ + size_); }
};