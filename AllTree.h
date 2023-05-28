//
// Created by marek on 28.05.2023.
//

#ifndef GIPF_ALLTREE_H
#define GIPF_ALLTREE_H

#include <vector>

template <typename T>
class AllTree {
public:
    T data;
    AllTree* parent;
    std::vector<AllTree*> children;

    explicit AllTree(T data);
    AllTree(AllTree* parent, T data);
    ~AllTree();

    AllTree<T> addChild(AllTree* child);
    AllTree<T> addChild(T child_data);
    //void removeChild(AllTree* child);
    //void removeChild(T data);

    AllTree<T>* getChild(int index);
    std::vector<AllTree<T> *>::iterator getChildrenIterator();
    std::vector<AllTree<T> *> getChildren() { return children; }

    bool isRoot() { return parent == nullptr; }
    bool isLeaf() { return children.empty(); }
    int getDepth() {
        if (isRoot()) {
            return 0;
        } else {
            return parent->getDepth() + 1;
        }
    }
    int getNumberOfChildren() { return children.size(); }
};

template<typename T>
std::vector<AllTree<T> *>::iterator AllTree<T>::getChildrenIterator() {
    return std::vector<AllTree<T> *>::iterator();
}

template<typename T>
AllTree<T> *AllTree<T>::getChild(int index) {
    if (index < children.size()) {
        return children[index];
    } else {
        return nullptr;
    }
}

template<typename T>
AllTree<T>::~AllTree() {
    for (auto* child : children) {
        delete child;
    }
    children.clear();

}

template<typename T>
AllTree<T>::AllTree(T data) {
    this->parent = nullptr;
    this->data = data;
    this->children = std::vector<AllTree*>();

}

template<typename T>
AllTree<T>::AllTree(AllTree *parent, T data) {
    this->parent = parent;
    this->data = data;
    this->children = std::vector<AllTree*>();

}

template<typename T>
AllTree<T> AllTree<T>::addChild(T child_data) {
    auto* child = new AllTree(this, child_data);
    children.push_back(child);
    return *child;
}

template<typename T>
AllTree<T> AllTree<T>::addChild(AllTree *child) {
    children.push_back(child);
    child->parent = this;
    return *child;
}


#endif //GIPF_ALLTREE_H
