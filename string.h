#pragma once

#include <cstring>
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace tinystl {

class string
{
public:
    using data_pointer = char*;
    using const_data_pointer = const char*;
    static const size_t s_min_cap;
private:
    data_pointer data_;
    size_t size_;
    size_t capacity_;
private:
    //重新分配内存
    void reallocData(size_t newCap)
    {
        if (newCap > capacity_)
        {
            //申请新的内存并拷贝旧的内存中的元素
            data_pointer newMem = new char[newCap + 1];
            std::memcpy(newMem, data_, size_ + 1);
            //释放旧的内存
            delete[] data_;
            //更新
            capacity_ = newCap;
            data_ = newMem;
        }
    }

public:
    //构造函数
    string() 
        : data_(new char[s_min_cap + 1]),
          size_(0),
          capacity_(s_min_cap) 
    {
        data_[0] = '\0';
    }
    //从C风格字符串构造
    string(const char* str)
    {
        if (str == nullptr)
        {
            throw std::invalid_argument("null pointer");
        }
        //用 strlen + memcpy 构造对象
        size_t len = strlen(str);
        size_t cap = std::max(len, s_min_cap); 
        data_ = new char[cap + 1];
        std::memcpy(data_, str, len + 1);
        //更新
        size_ = len;
        capacity_ = cap;
    }
    //二进制安全的构造函数
    string(void* bytes, size_t len)
        : size_(len)
    {
        if (bytes == nullptr)
        {
            throw std::invalid_argument("null pointer");
        }
        size_t cap = std::max(len, s_min_cap);
        data_ = new char[cap + 1];
        std::memcpy(data_, bytes, len);
        data_[len] = '\0';
        capacity_ = cap;
    }

    //拷贝构造函数
    string(const string& other) 
        : size_(other.size_),
          capacity_(other.capacity_)
    {
        data_ = new char[other.capacity_ + 1];
        std::memcpy(data_, other.data_, other.size_ + 1);
    }
    //移动构造函数
    string(string&& other) noexcept
        : size_(other.size_),
          capacity_(other.capacity_),
          data_(other.data_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    //析构函数
    ~string()
    {
        delete[] data_;
    }

    //赋值运算符重载
    string& operator=(const string& other)
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = new char[other.capacity_ + 1];
            std::memcpy(data_, other.data_, other.size_ + 1);
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }
    //移动赋值
    string& operator=(string&& other) noexcept
    {
        if (this != &other)
        {
            //释放旧的内存，避免内存泄漏
            delete[] data_;
            //移动赋值
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    //输出运算符重载
    friend std::ostream& operator<<(std::ostream& os, const string& s)
    {
        os.write(s.data_, s.size_);
        return os;

    }
    
    //预先分配内存
    void reserve(size_t new_cap)
    {
        if (new_cap > capacity_)
        {
            reallocData(new_cap);
        }
    }

    //append() 函数
    string& append(const_data_pointer str, size_t len)
    {
        if (str == nullptr)
        {
            throw std::invalid_argument("null pointer");
        }

        if (size_ + len >= capacity_)
        {
            reserve((size_ + len) * 2);
        }

        std::memcpy(data_ + size_, str, len + 1);
        size_ += len;
        return *this;
    }
    string& append(const_data_pointer str)
    {
        if (str == nullptr)
        {
            throw std::invalid_argument("null pointer");
        }
        return append(str, strlen(str));
    }
    string& append(const string& s)
    {
        return append(s.data_);
    }
    //返回信息
    size_t capacity() const noexcept {return capacity_;}
    size_t size() const noexcept {return size_;}
    const_data_pointer c_str() const noexcept {return data_;}
    bool empty() const noexcept {return size_ == 0;}
};


} // namespace tinystl

const size_t tinystl::string::s_min_cap = 15;