#include <cstddef>

namespace tinystl {

template <typename T>
class vector
{
public:

    //无参构造函数
    vector()
        : element_(nullptr), capacity_(0), size_(0)
    {}

    //拷贝构造函数
    vector(const vector& other) 
        : capacity_(other.capacity_), size_(other.size_)
    {
        element_ = new T[capacity_];
        std::copy(other.element_, other.element_ + size_, element_);
    }
    
    //赋值运算符重载
    vector& operator=(const vector& other)
    {
        if (this == &other) //防止自赋值
        {
            return *this;
        }
        //释放旧的内存
        delete[] element_;
        //申请新的内存
        element_ = new T[other.capacity_];
        //复制other的数据
        std::copy(other.element_, other.element_ + other.size_, element_);

        capacity_ = other.capacity_;
        size_ = other.size_;
        return *this; //返回自身的引用，支持连续赋值操作
    }

    //扩容操作
    void reserve(size_t newCapacity)
    {
        if (newCapacity > capacity_)
        {
            //申请新的内存
            T* newElement = new T[newCapacity];
            //复制旧的内存中的数据
            std::copy(element_, element_ + size_, newElement);
            //释放旧的内存
            delete[] element_;
            //更新
            element_ = newElement;
            capacity_ = newCapacity;
        }
    }

    //在末尾添加一个元素
    void push_back(const T& val)
    {
        //判断内存是否够用
        if (size_ == capacity_)
        {
            //内存不足，扩容
            size_t newCapacity = capacity_ == 0 ? 1 : 2 * capacity_;
            reserve(newCapacity);
        }
        //内存够用，移动指针，添加元素
        element_[size_++] = val;
    }

    //将末尾元素删除
    void pop_back()
    {
        //判断vector是否为空
        if (size_ == 0)
        {
            return;
        }
        //不为空，移动指针，删除元素
        --size_;
    }

    //在指定位置插入元素
    void insert(size_t pos, const T& val)
    {
        //检查下标是否越界
        if (pos < 0 || pos > size_)
        {
            throw std::out_of_range("Index out of range");
        }
        //判断内存是否够用
        if (size_ == capacity_)
        {
            size_t newCapacity = capacity_ == 0 ? 1 : 2 * capacity_;
            reserve(newCapacity);
        }
        //将pos及以后的元素后移一位
        for (size_t i = size_; i > pos; --i)
        {
            element_[i] = element_[i - 1];
        }
        //将元素插入位置pos
        element_[pos] = val;
        ++size_;
    }

    //重载[]运算符
    T& operator[](size_t index)
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range("Index out of range");
        }
        return element_[index];
    }
    const T& operator[](size_t index) const 
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range("Index out of range");
        }
        return element_[index];
    }

    //调用对象的析构函数，清空vector
    void clear()
    {
        size_ = 0;
    }

    //获取vector的内存大小与元素个数
    size_t capacity() const
    {
        return capacity_;
    }
    size_t size() const 
    {
        return size_;
    }

    //迭代器
    T* begin()
    {
        return element_;
    }
    const T* begin() const 
    {
        return element_;
    }
    T* end()
    {
        return element_ + size_;
    }
    const T* end() const 
    {
        return element_ + size_;
    }

    //打印vector的所有元素
    void printElements()
    {
        if (size_ == 0)
        {
            std::cout << "empty";
        }
        else 
        {
            for (size_t i = 0; i < size_; ++i)
            {
                std::cout << element_[i] << " ";
            }
        }
    }

private:
    T*     element_;     // 指向首元素的指针
    size_t capacity_;    // vector的内存大小
    size_t size_;        // vector中元素的个数

};

} // namespace tinystl