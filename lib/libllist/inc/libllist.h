#pragma once

#include <iostream>
#include <status.h>

namespace llist{



template <class T>
class LList;

template <class T>
class LListNode
{

	friend class LList<T>;
public:
    LListNode(){};
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
    inline LListNode<T>* GetHead(){return head_;}
    inline LListNode<T>* GetTail(){return tail_;}
    void Clear() {head_=nullptr; tail_=nullptr;};
    LListNode<T>* FindNode(CompareDataFunction_t,T&);
    LListNode<T>* FindNode(CompareArgFunction_t,void*);
    void PrintList(PrintFunction_t);
    
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
        existing_node->prev_ = new_node;
        status = kStatusSuccess;
    }
    
    return status;
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
LListNode<T>* LList<T>::FindNode(CompareDataFunction_t func, T& data)
{
    LListNode<T>* temp = head_;
    
    while (temp != nullptr)
    {
        if (func(temp, data))
        {
            return temp;
        }

        temp = temp->next_;
    }
    return nullptr;
}

template <class T>
LListNode<T>* LList<T>::FindNode(CompareArgFunction_t func, void * arg)
{
    LListNode<T>* temp = head_;
    
    while (temp != nullptr)
    {
        if (func(temp, arg))
        {
            return temp;
        }

        temp = temp->next_;
    }
    return nullptr;
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

}


