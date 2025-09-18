#ifndef RBNODE_H
#define RBNODE_H
#include <utility>

    constexpr bool RED = true;
    constexpr bool BLACK = false;

template <typename K, typename V>
struct rbNode {

    std::pair<K,V> par;
    bool color;
    rbNode<K,V>* right;
    rbNode<K,V>*left;
    rbNode<K,V>* p;

    rbNode(K key, V value, bool color = RED, rbNode<K,V>* right = nullptr, rbNode<K,V>* left = nullptr, rbNode<K,V>* p = nullptr){
        par = std::make_pair(key, value);
        this->color = color;
        this->right = right;
        this->left = left;
        this->p = p;
    }
};

#endif