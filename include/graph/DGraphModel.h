/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>
{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }
    
    void connect(T from, T to, float weight=0)
    {
        if (this->getVertexNode(from)==nullptr) throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to)==nullptr) throw VertexNotFoundException(this->vertex2str(to));

        typename AbstractGraph<T>::VertexNode * fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode * toNode = this->getVertexNode(to);

        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to)
    {
        if (this->getVertexNode(from)==nullptr) throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to)==nullptr) throw VertexNotFoundException(this->vertex2str(to));

        typename AbstractGraph<T>::VertexNode * fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode * toNode = this->getVertexNode(to);
        typename AbstractGraph<T>::Edge edge(fromNode, toNode, 0);

        if (!this->connected(from, to)) throw EdgeNotFoundException(this->edge2Str(edge));
        fromNode->removeTo(toNode);
    }

    void remove(T vertex)
    {
        if (this->getVertexNode(vertex)==nullptr) throw VertexNotFoundException(this->vertex2str(vertex));

        typename AbstractGraph<T>::VertexNode * fromNode = this->getVertexNode(vertex);

        for (auto v = this->nodeList.begin(); v!=this->nodeList.end(); v++)
        {
            typename AbstractGraph<T>::vertexEQFunc func = this->getVertexEQ();
            if (func!=nullptr)
            {
                if (!func(fromNode->getVertex(), (*v)->getVertex()))
                {
                    fromNode->removeTo(*v);
                    (*v)->removeTo(fromNode);
                }
            }
            else
            {
                if (fromNode->getVertex()!=(*v)->getVertex())
                {
                    fromNode->removeTo(*v);
                    (*v)->removeTo(fromNode);
                }
            }
        }

        this->nodeList.removeItem(fromNode);

        if (fromNode!=nullptr) delete fromNode;
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T> * edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&))
    {
        DGraphModel<T>* created = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i<nvertices; i++) created->add(vertices[i]);
        for (int i = 0; i<nedges; i++)
        {
            created->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return created;

        //TODO
    }
};

#endif /* DGRAPHMODEL_H */

