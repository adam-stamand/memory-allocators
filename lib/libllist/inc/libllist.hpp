#pragma once

#include <iostream>
#include <status.hpp>

namespace llist{



template <class T>
class LList;

template <class T>
class LListNode
{

	friend class LList<T>;
public:
    LListNode():prev_(nullptr),next_(nullptr){};
    T data_;
    inline LListNode * next(){return next_;};
    inline LListNode * prev(){return prev_;};
private:
    LListNode * prev_;
    LListNode * next_;
};


template <class T>
class LList
{
public:

    LList();
    ~LList(){}
    

    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = LListNode<T>;
        using pointer           = LListNode<T>*;
        using reference         = LListNode<T>&;

        Iterator(pointer ptr) : ptr_(ptr) {}

        reference operator*() const {return *ptr_;}
        pointer operator->() {return ptr_;}
        Iterator& operator++() {ptr_=ptr_->next(); return *this;}
        Iterator operator++(int) {Iterator tmp = *this; ptr_=ptr_->next(); return tmp;}
        friend bool operator==(const Iterator& a, const Iterator& b) {return a.ptr_ == b.ptr_;}
        friend bool operator!=(const Iterator& a, const Iterator& b) {return a.ptr_ != b.ptr_;}

    private:
        pointer ptr_;
    };

    Iterator begin() {return Iterator(GetHead());}
    Iterator end()   {
        if (GetTail())
        {
            return Iterator(GetTail()->next());
        }else
        {
            return Iterator(GetTail());    
        }
    }

    typedef bool (*CompareDataFunction_t)(LListNode<T>*, T&);
    typedef bool (*CompareArgFunction_t)(LListNode<T>*, void*);
    typedef void (*PrintFunction_t)(LListNode<T>*);
    Status_t InsertHead(LListNode<T>*);
    Status_t InsertTail(LListNode<T>*);
    Status_t InsertNodeAfter(LListNode<T>*,LListNode<T>*);
    Status_t InsertNodeBefore(LListNode<T>*,LListNode<T>*);
    Status_t RemoveNode(LListNode<T>*);
    inline void RemoveHead()   {if (head_==nullptr) return; if (head_==tail_){head_=nullptr;tail_=nullptr;return;} head_->next_->prev_ = nullptr; head_ = head_->next_;}
    inline void RemoveTail()    {if (tail_==nullptr) return; if (head_==tail_){head_=nullptr;tail_=nullptr;return;} tail_->prev_->next_ = nullptr; tail_ = tail_->prev_;}
    Status_t SwapNode(LListNode<T>*,LListNode<T>*);
    inline LListNode<T>* GetHead(){return head_;}
    inline LListNode<T>* GetTail(){return tail_;}
    void Clear() {head_=nullptr; tail_=nullptr;};
    void PrintList(PrintFunction_t);
    void PrintList();
private:
    LListNode<T>* head_;
    LListNode<T>* tail_;
};







template <class T>
LList<T>::LList() : head_(nullptr), tail_(nullptr){}


template <class T>
Status_t LList<T>::InsertHead(LListNode<T>* node)
{
    if (node == nullptr)
    {
        return kStatusInvalidParam;
    }

    if (head_ == nullptr)
    {

        head_ = node;
        tail_ = node;
        head_->next_ = nullptr;
        head_->prev_ = nullptr;
    }
    else
    {
        node->next_ = head_;
        node->prev_ = nullptr;
        head_->prev_ = node;
        head_ = node;
    }
    
    return kStatusSuccess;
}
template <class T>
Status_t LList<T>::InsertTail(LListNode<T>* node)
{
    if (node == nullptr)
    {
        return kStatusInvalidParam;
    }

    if (tail_ == nullptr)
    {
        head_ = node;
        tail_ = node;
        head_->next_ = nullptr;
        head_->prev_ = nullptr;
    }
    else
    {
        node->next_ = nullptr;
        node->prev_ = tail_;
        tail_->next_ = node;
        tail_ = node;
    }
    
    return kStatusSuccess;
}

template <class T>
Status_t LList<T>::InsertNodeAfter(LListNode<T>* existing_node, LListNode<T>* new_node)
{
    Status_t status;

    if (existing_node == nullptr || new_node == nullptr)
    {
        return kStatusInvalidParam;
    }
    
    if (new_node == existing_node->next_)
    {
        return kStatusSuccess;
    }

    if (existing_node->next_ == nullptr)
    {
        status = InsertTail(new_node);
    }
    else
    {
        new_node->next_ = existing_node->next_;
        new_node->prev_ = existing_node;
        existing_node->next_ = new_node;
        status = kStatusSuccess;
    }
    
    return status;
}


template <class T>
Status_t LList<T>::InsertNodeBefore(LListNode<T>* existing_node, LListNode<T>* new_node)
{
    Status_t status;

    if (existing_node == nullptr || new_node == nullptr)
    {
        return kStatusInvalidParam;
    }

    if (new_node == existing_node->prev_)
    {
        return kStatusSuccess;
    }

    if (existing_node->prev_ == nullptr)
    {
        status = InsertHead(new_node);
    }
    else
    {
        new_node->prev_ = existing_node->prev_;
        new_node->next_ = existing_node;
        existing_node->prev_->next_ = new_node;
        existing_node->prev_ = new_node;
        status = kStatusSuccess;
    }
    
    return status;
}


template <class T>
Status_t LList<T>::SwapNode(LListNode<T>* existing_node, LListNode<T>* new_node)
{
    if (existing_node == nullptr || new_node == nullptr)
    {
        return kStatusInvalidParam;
    }

    if (new_node == existing_node->prev_)
    {
        return kStatusSuccess;
    }

    new_node->prev_ = existing_node->prev_;
    new_node->next_ = existing_node->next_;

    if (existing_node->next_)
    {
        existing_node->next_->prev_ = new_node;
    }else
    {
        tail_ = new_node;
    }
    
    if (existing_node->prev_)
    {
        existing_node->prev_->next_ = new_node;
    }else
    {
        head_ = new_node;
    }

    return kStatusSuccess;
}


template <class T>
Status_t LList<T>::RemoveNode(LListNode<T>* node)
{
    Status_t status = kStatusSuccess;

    if (node == nullptr)
    {
        return kStatusInvalidParam;
    }

    if (node == head_)
    {
        RemoveHead();
    }
    else if (node == tail_)
    {
        RemoveTail();
    }
    else if (node->prev_ == nullptr || node->next_ == nullptr)
    {
        status = kStatusInvalidParam;
    }
    else
    {
        node->prev_->next_ = node->next_;
        node->next_->prev_ = node->prev_;
        node->prev_ = nullptr;
        node->next_ = nullptr;
    }
    
    return status;
}


template <class T>
void LList<T>::PrintList(PrintFunction_t func)
{
    LListNode<T> * temp = head_;

    while (temp != nullptr)
    {
        func(temp);
        temp = temp->next_;
    }
    printf("\n");
}


template <class T>
void LList<T>::PrintList()
{
    LListNode<T> * temp = head_;
    
    while (temp != nullptr)
    {
        printf("Node:0x%lx prev:0x%lx next:0x%lx, data:0x%x\n", 
            (long unsigned int)temp, (long unsigned int)temp->prev_, (long unsigned int)temp->next_, (unsigned int)temp->data_);
        temp = temp->next_;
    }
}

}


