/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"


template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0)
    {
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    
    DLinkedList<T> sort(int mode=0, bool sorted=true)
    {
        if (mode == 0) return dfsSort(sorted);
        if (mode == 1) return bfsSort(sorted);

        return DLinkedList<T>();
    }

    DLinkedList<T> bfsSort(bool sorted=true)
    { 
        DLinkedList<T> result;

        DLinkedListSE<T> vertices = DLinkedListSE<T>(this->graph->vertices());
        xMap<T,int> in_degree = vertex2inDegree(this->hash_code);
        DLinkedList<T> zero_in = listOfZeroInDegrees();
    
        if (sorted)
        {
            vertices.sort();
            // zero_in.sort();
        }

        Queue<T> queue;

        // initialize
        for (auto it = vertices.begin(); it!=vertices.end(); it++)
        {
            if (zero_in.contains(*it))
            {
                queue.push(*it);
            }
        }

        while (!queue.empty())
        {
            T got = queue.pop();
            result.add(got);
            DLinkedListSE<T> got_out = DLinkedListSE<T>(this->graph->getOutwardEdges(got));

            if (got_out.empty()) continue;

            if (sorted)
            {
                got_out.sort();
            }

            for (auto it = got_out.begin(); it!=got_out.end(); it++)
            {
                in_degree.get(*it)--;
                if (in_degree.get(*it)==0) 
                {
                    queue.push(*it); 
                }
            }
        }

        return result;
    }

    DLinkedList<T> dfsSort(bool sorted=true)
    {
        DLinkedList<T> result;

        DLinkedListSE<T> vertices = DLinkedListSE<T>(this->graph->vertices());
        // xMap<T, bool> mask(this->hash_code);
        xMap<T,int> out_degree = vertex2outDegree(this->hash_code);
        DLinkedList<T> zero_out = listOfZeroOutDegrees();
    
        if (sorted)
        {
            vertices.sort();
        }

        Stack<T> st;

        // initialize
        for (auto it = vertices.begin(); it!=vertices.end(); it++)
        {
            if (zero_out.contains(*it))
            {
                st.push(*it);
            }
        }

        while (!st.empty())
        {
            T got = st.pop();
            result.add(0, got);
            DLinkedListSE<T> got_in = DLinkedListSE<T>(this->graph->getInwardEdges(got));

            if (got_in.empty()) continue;

            if (sorted)
            {
                got_in.sort();
            }

            for (auto it = got_in.begin(); it!=got_in.end(); it++)
            {
                out_degree.get(*it)--;
                if (out_degree.get(*it)==0) 
                {
                    st.push(*it); 
                }
            }
        }

        // for (auto it = vertices.begin(); it!=vertices.end(); it++)
        // {
        //     mask.put(*it, false);
        // }

        // for (auto it = vertices.bbegin(); it!=vertices.bend(); it--)
        // {
        //     if (mask.get(*it)==false)
        //     {
        //         dfs_help(mask, st, *it, sorted);
        //     }
        // }

        // while (!st.empty())
        // {
        //     result.add(st.pop());
        // }

        return result;
    }

protected:

    // void dfs_help(xMap<T, bool> & mask, Stack<T> & stack, T vertex, bool sorted)
    // {
    //     mask.get(vertex) = true;
    //     DLinkedListSE<T> got_out = DLinkedListSE<T>(this->graph->getOutwardEdges(vertex));

    //     if (!got_out.empty())
    //     {
    //         if (sorted)
    //         {
    //             got_out.sort();
    //         }

    //         for (auto it = got_out.bbegin(); it!=got_out.bend(); it--)
    //         {
    //             if (mask.get(*it)==false)
    //             {
    //                 dfs_help(mask, stack, *it, sorted);
    //             }
    //         }
    //     }

    //     stack.push(vertex);   
    // }

    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int))
    {
        xMap<T, int> map(hash);
        for (auto it = this->graph->begin(); it != this->graph->end(); it++)
        {
            map.put((*it), this->graph->inDegree(*it));
        }

        return map;
    }

    xMap<T, int> vertex2outDegree(int (*hash)(T&, int))
    {
        xMap<T, int> map(hash);
        for (auto it = this->graph->begin(); it != this->graph->end(); it++)
        {
            map.put((*it), this->graph->outDegree(*it));
        }

        return map;
    }

    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> list;
        for (auto it = this->graph->begin(); it != this->graph->end(); it++)
        {
            if (this->graph->inDegree(*it)==0)
            {
                list.add(*it);
            }
        }

        return list;
    }

    DLinkedList<T> listOfZeroOutDegrees()
    {
        DLinkedList<T> list;
        for (auto it = this->graph->begin(); it != this->graph->end(); it++)
        {
            if (this->graph->outDegree(*it)==0)
            {
                list.add(*it);
            }
        }

        return list;
    }

}; //TopoSorter

template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */

