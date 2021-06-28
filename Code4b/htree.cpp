#include "htree.h"

bool compare(const Node &a, const Node &b) {
    if (a.freq < b.freq)
        return 1;
    else if (a.freq > b.freq)
        return 0;
    else if (a.value < b.value)
        return 1;
    return 0;
}

void merge(Node &a, Node &b, Node &merged_node) {
    merged_node.value = a.value < b.value ? a.value : b.value;
    merged_node.freq = a.freq + b.freq;
    if(compare(a,b)) { // a smaller
        merged_node.p0 = &a;
        merged_node.p1 = &b;
    } else {
        merged_node.p0 = &b;
        merged_node.p1 = &a;
    }
}

Node buildTree(std::vector<Node> &histogram){  // build tree
    std::list<Node> filtered_hist;
    for(Node node : histogram) {
        if(node.freq != 0) {
            filtered_hist.push_front(node);
        }
    }
    while(filtered_hist.size() > 1) {
         Node *node1 = new Node();
         Node *node2 = new Node();
         Node *merged_node = new Node();
        //sort
        filtered_hist.sort(compare);
        // merged node
        *node1 = filtered_hist.front();
        filtered_hist.pop_front();
        *node2 = filtered_hist.front();
        filtered_hist.pop_front();
        merge(*node1, *node2, *merged_node);
        filtered_hist.push_front(*merged_node);
    }
    return filtered_hist.front();

}

void buildLookUpTable(Node &node, std::vector<int> prefix, std::vector< std::vector<int> > &lt) {
    // std::cout<<(*node).freq<<std::endl;
    if(node.p0 == NULL) {
        lt[node.value] = prefix;
    } else {
        // std::cout<<(*node).freq<<std::endl;
        std::vector<int> leftPrefix = prefix;
        leftPrefix.push_back(0);
        std::vector<int> rightPrefix = prefix;
        rightPrefix.push_back(1);
        buildLookUpTable(*(node.p0), leftPrefix, lt);
        buildLookUpTable(*(node.p1), rightPrefix, lt);
    }
}