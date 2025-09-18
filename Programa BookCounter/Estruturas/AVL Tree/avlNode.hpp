#ifndef NODEAVL_H
#define NODEAVL_H
#include <utility>

template <typename K, typename V>
struct avlNode {
    
    std::pair<K,V> par;
    avlNode* right;
    avlNode* left;
    int height;

    avlNode(K key, V freq, avlNode* right = nullptr, avlNode* left = nullptr, int height = 0){
        par = std::make_pair(key, freq);
        this->right = right;
        this->left = left;
        this->height = height;
    }

};

#endif