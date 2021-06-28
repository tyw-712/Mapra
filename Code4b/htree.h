#ifndef HTREE_H // GreyScale.h nicht doppelt benutzen
#define HTREE_H
#include <vector>
#include <iostream>
#include <list>

class Node {
    public:
        int value; // Grauwert 
        u_int32_t freq; // Haeufigkeit 
        Node *p0;
        Node *p1; // linker/rechter 
};
Node buildTree(std::vector<Node> &histogram);
bool compare(const Node &, const Node &);
void merge(Node &a, Node &b, Node &);
void buildLookUpTable(Node &node, std::vector<int> prefix, std::vector< std::vector<int> > &lt);
#endif