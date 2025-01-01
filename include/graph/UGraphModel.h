/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        if (this->getVertexNode(from)==nullptr) throw VertexNotFoundException(this->vertex2str(from));
        if (this->getVertexNode(to)==nullptr) throw VertexNotFoundException(this->vertex2str(to));

        typename AbstractGraph<T>::VertexNode * fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode * toNode = this->getVertexNode(to);

        fromNode->connect(toNode, weight);
        toNode->connect(fromNode, weight);
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
        toNode->removeTo(fromNode);
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

    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        UGraphModel<T>* created = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i<nvertices; i++) created->add(vertices[i]);
        for (int i = 0; i<nedges; i++)
        {
            created->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return created;
    }
};

#endif /* UGRAPHMODEL_H */
