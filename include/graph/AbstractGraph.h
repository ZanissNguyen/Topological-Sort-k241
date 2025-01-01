/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include <iomanip>
#include "graph/IGraph.h"
#include <string>
#include <sstream>
using namespace std;


template<class T>
class AbstractGraph: public IGraph<T>{
public:
    class Edge; //forward declaration
    class VertexNode; //forward declaration
    class Iterator; //forward declaration
    
private:
// protected:
public:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode*> nodeList; 
    
    //Function pointers:
    bool (*vertexEQ)(T&, T&); //to compare two vertices
    string (*vertex2str)(T&); //to obtain string representation of vertices
public:
    VertexNode* getVertexNode(T& vertex)
    {
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while(it != nodeList.end()){
            VertexNode* node = *it;
            if(vertexEQ(node->vertex, vertex)) return node;
            it++;
        }
        return 0;
    }

    string vertex2Str(VertexNode& node){
        return vertex2str(node.vertex);
    }

    string vertexp2Str(VertexNode*& node){
        return vertex2str(node->vertex);
    }

    string edge2Str(Edge& edge){
        stringstream os;
        os << "E("
                << vertex2str(edge.from->vertex)
                << ","
                << vertex2str(edge.to->vertex)
                << ")";
        return os.str();
    }
    
    AbstractGraph(
            bool (*vertexEQ)(T&, T&)=0, 
            string (*vertex2str)(T&)=0){
        
        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph()
    {
        // cout << "Ay yo ??" << endl;
        clear();
    }
    
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);

    vertexEQFunc getVertexEQ(){
        return this->vertexEQ;
    }

    vertex2strFunc getVertex2Str(){
        return this->vertex2str;
    }
    
//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight=0) =0;
    virtual void disconnect(T from, T to) =0;
    virtual void remove(T vertex) =0;
    
    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) 
    {
        if (getVertexNode(vertex)!=nullptr) return;
        else
        {
            VertexNode * v = new VertexNode(vertex, this->vertexEQ, this->vertex2str);
            nodeList.add(v);
        }
    }

    virtual bool contains(T vertex)
    {
        return getVertexNode(vertex)!=nullptr;
    }

    virtual float weight(T from, T to)
    {
        if (getVertexNode(from)==nullptr) throw VertexNotFoundException(vertex2str(from));
        if (getVertexNode(to)==nullptr) throw VertexNotFoundException(vertex2str(to));

        VertexNode * fromNode = getVertexNode(from);
        VertexNode * toNode = getVertexNode(to);
        Edge temp = Edge(fromNode, toNode, 0);

        if (fromNode->getEdge(toNode)==nullptr) throw EdgeNotFoundException(edge2Str(temp));
        else return fromNode->getEdge(toNode)->weight;

        return 0.0;
    }

    virtual DLinkedList<T> getOutwardEdges(T from)
    {
        if (getVertexNode(from)==nullptr) throw VertexNotFoundException(vertex2str(from));
        
        VertexNode * fromNode = getVertexNode(from);

        return fromNode->getOutwardEdges();
    }
    
    virtual DLinkedList<T> getInwardEdges(T to)
    {
        if (getVertexNode(to)==nullptr) throw VertexNotFoundException(vertex2str(to));

        VertexNode * toNode = getVertexNode(to);
        DLinkedList<T> result(nullptr, vertexEQ);

        for (auto ve = this->nodeList.begin(); ve!=this->nodeList.end(); ve++)
        {
            VertexNode * v = *ve;

            if (v->getEdge(toNode)!=nullptr) result.add(v->vertex);
        }

        return result;
    }
    
    virtual int size() 
    {
        return this->nodeList.size();
    }

    virtual bool empty()
    {
        return this->nodeList.size()==0;
    }

    virtual void clear()
    {
        int nodeList_size = this->nodeList.size();
        for (int i = 0; i<nodeList_size; i++)
        {
            VertexNode * v = this->nodeList.get(i);
        
            int adList_size = v->adList.size();
            for (int j = 0; j<adList_size; j++)
            {
                Edge * e = v->adList.removeAt(0);
                if (e!=nullptr) delete e;
            }
        }

        for (int i = 0; i<nodeList_size; i++)
        {
            VertexNode * v = this->nodeList.removeAt(0);
            if (v!=nullptr) delete v;
        }
    }

    virtual int inDegree(T vertex)
    {
        if (getVertexNode(vertex)==nullptr) throw VertexNotFoundException(vertex2str(vertex));
        
        VertexNode * fromNode = getVertexNode(vertex);

        return fromNode->inDegree();
    }

    virtual int outDegree(T vertex)
    {
        if (getVertexNode(vertex)==nullptr) throw VertexNotFoundException(vertex2str(vertex));
        
        VertexNode * node = getVertexNode(vertex);

        return node->outDegree();
    }
    
    virtual DLinkedList<T> vertices()
    {
        DLinkedList<T> result(nullptr, vertexEQ);

        for (auto it = this->nodeList.begin(); it!=this->nodeList.end(); it++)
        {
            result.add((*it)->vertex);
        }

        return result;
    }

    virtual bool connected(T from, T to)
    {
        if (getVertexNode(from)==nullptr) throw VertexNotFoundException(vertex2str(from));
        if (getVertexNode(to)==nullptr) throw VertexNotFoundException(vertex2str(to));

        VertexNode * fromNode = getVertexNode(from);
        VertexNode * toNode = getVertexNode(to);

        if (fromNode->getEdge(toNode)==nullptr) return false;
        else return true;

        return false;
    }

    void println()
    {
        cout << this->toString() << endl;
    }

    virtual string toString(){
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
//BEGIN of VertexNode    
    class VertexNode{
    private:
        template<class U>
        friend class UGraphModel; //UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ)(T&, T&);
        string (*vertex2str)(T&);
        
    public:
        VertexNode():adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){}
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&))
            :adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }

        T& getVertex(){
            return vertex;
        }

        void connect(VertexNode* to, float weight=0)
        {
            // edge exist
            if (getEdge(to)!=nullptr)
            {
                Edge * e = getEdge(to);
                e->weight = weight;
            }
            else // edge doesn't exist
            {
                Edge * e = new Edge(this, to, weight);
                this->outDegree_++;
                to->inDegree_++;
                this->adList.add(e);
            }
        }

        DLinkedList<T> getOutwardEdges()
        {
            DLinkedList<T> result(nullptr, vertexEQ);

            for (auto it = this->adList.begin(); it!=this->adList.end(); it++)
            {
                result.add((*it)->to->vertex);
            }

            return result;
        }

        Edge* getEdge(VertexNode* to)
        {
            for (auto it = this->adList.begin(); it!=this->adList.end(); it++)
            {
                if (vertexEQ!=nullptr)
                {
                    if (vertexEQ((*it)->to->vertex, to->vertex))
                    {
                        return *it;
                    }
                }
                else
                {
                    if ((*it)->to->vertex == to->vertex)
                    {
                        return *it;
                    }
                }
            }

            return nullptr;
        }

        bool equals(VertexNode* node)
        {
            if (vertexEQ!=nullptr)
            {
                return vertexEQ(node->vertex, this->vertex);
            }
            else
            {
                return node->vertex==this->vertex;
            }

            return false;
        }
        
        void removeTo(VertexNode* to)
        {
            if (getEdge(to)!=nullptr)
            {
                Edge * e = getEdge(to);
                this->adList.removeItem(e);

                this->outDegree_--;
                to->inDegree_--;

                if (e!=nullptr) delete e;
            } 
        }

        int inDegree()
        {
            return inDegree_;
        }

        int outDegree()
        {
            return outDegree_;
        }

        string toString(){
            stringstream os;
            os << "V("
                    << this->vertex << ", "
                    << "in: " << this->inDegree_ << ", "
                    << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge{
    private:
        VertexNode* from;
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight=0){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        
        bool equals(Edge* edge)
        {
            vertexEQFunc func = edge->from->vertexEQ;

            if (func!=nullptr)
            {
                return func(edge->from->vertex, this->from->vertex) 
                        && func(edge->to->vertex, this->to->vertex); 
            }
            else
            {
                return edge->from->vertex==this->from->vertex
                        && edge->to->vertex==this->to->vertex; 
            }
        }

        T getFrom()
        {
            return this->from->getVertex();
        }

        T getTo()
        {
            return this->to->getVertex();
        }

        static bool edgeEQ(Edge*& edge1, Edge*& edge2){
            return edge1->equals(edge2);
        }
        string toString(){
            stringstream os;
            os << "E("
                    << this->from->vertex
                    << ","
                    << this->to->vertex
                    << ","
                    << this->weight
                    << ")";
            return os.str();
        }
    };
 //END of Edge
    

//BEGIN of Iterator
public:
    class Iterator{
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true){
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            }
            else{
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator& operator=(const Iterator& iterator){
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*(){
            return (*nodeIt)->vertex;
        }
        
        bool operator!=(const Iterator& iterator){
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            nodeIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
//END of Iterator
};

#endif /* ABSTRACTGRAPH_H */

