/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T>{
public:
    
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ)
    {
    };
    
    DLinkedListSE(const DLinkedList<T>& list)
    {
        this->copyFrom(list);
    }
    
    typename DLinkedList<T>::Node* mergeLists(typename DLinkedList<T>::Node* a,typename DLinkedList<T>::Node* b, int (*comparator)(T&,T&)=0) 
    {
        typename DLinkedList<T>::Node dummy;  // Temporary dummy node to start the merged list
        typename DLinkedList<T>::Node* last = &dummy;  // Pointer to the last node in the merged list

        while (a!=nullptr && (b!=nullptr && b!=this->tail)) 
        {
            if (comparator!=nullptr)
            {
                if (comparator(a->data, b->data)==-1) 
                {
                    last->next = a;
                    a->prev = last;
                    a = a->next;
                } else 
                {
                    last->next = b;
                    b->prev = last;
                    b = b->next;
                }
            }
            else
            {
                if (a->data < b->data) 
                {
                    last->next = a;
                    a->prev = last;
                    a = a->next;
                } else 
                {
                    last->next = b;
                    b->prev = last;
                    b = b->next;
                }
            }
            
            last = last->next;
        }

        last->next = a!=nullptr && (b==nullptr || b==this->tail) ? a : b;
        if (last->next!=nullptr) 
        {
            last->next->prev = last;
        }

        return dummy.next;
    }

    // Sort and unsorted list given its head pointer
    typename DLinkedList<T>::Node* mergeSortList(typename DLinkedList<T>::Node* head, int size, int (*comparator)(T&,T&)=0) 
    {
        if (size <= 1) {
            head->prev = nullptr;
            head->next = nullptr;
            return head;
        }

        // Split
        // cout << size << endl;
        int mid = size%2==0 ? size/2 : size/2+1;
        typename DLinkedList<T>::Node* pivot = head;
        for (int i = 1; i < mid; i++) 
        {
            pivot = pivot->next;
        }

        typename DLinkedList<T>::Node* rhs = pivot->next;
        pivot->next = nullptr;
        rhs->prev = nullptr;

        typename DLinkedList<T>::Node* temp = head;

        typename DLinkedList<T>::Node * left = mergeSortList(head, mid, comparator);
        typename DLinkedList<T>::Node * right = mergeSortList(rhs, size-mid, comparator);
        return mergeLists(left, right, comparator);
    }

    void sort(int (*comparator)(T&,T&)=0)
    {
        this->head->next = mergeSortList(this->head->next, this->size(), comparator);
        reconnect();
    };

    void reconnect() 
    {
        typename DLinkedList<T>::Node* current = this->head->next;
        if (current == nullptr) 
        {
            this->head->next = this->tail;
            this->tail->prev = this->head;
            return;
        }

        current->prev = this->head;
        while (current->next) {
            current = current->next;
        }
        current->next = this->tail;
        this->tail->prev = current;
    }
    
protected:
    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */

