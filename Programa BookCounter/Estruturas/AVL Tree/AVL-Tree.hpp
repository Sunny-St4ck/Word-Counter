#ifndef AVLTREE_H
#define AVLTREE_H
#include "avlNode.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>

template <typename K, typename V>
class avlTree {
private:
    avlNode<K,V>* root;
    int size;
    size_t comps;
    int rots;

    //Função básica que insere um valor e uma chave na árvore
    avlNode<K,V>* _insert(avlNode<K,V>* node, K key, V value) {
        //Árvore vazia, cria um nó.
        if (node == nullptr){
            comps++;
            size++;
            return new avlNode<K,V>(key, value);
        }

        if (key == node->par.first){
            comps++;
            return node;
        }

        //Procura a chave dentro da estrutura, caso encontre incrementa a quantidade;
        if (key < node->par.first) {
            comps++;
            node->left = _insert(node->left, key, value);
        } else if (key > node->par.first){
            comps++;
            node->right = _insert(node->right, key, value);
        } 
        
        node = _fixUpNode(node, key);
        return node;
    }

    avlNode<K,V>* _remove(avlNode<K,V>* node, K key){
        if( node == nullptr ) // node nao encontrado
            return nullptr;
        
        if( key < node->par.first){
            node->left = _remove (node->left, key);
        }
        else if(key > node->par.first){
            node -> right = _remove (node->right , key);
        }

        else if(node->right == nullptr ) { // sem filho direito
            avlNode<K,V>* child = node -> left ;
            delete node;
            size--;
            return child;
        } else { // tem filho direito : troca pelo sucessor
            node->right = _removeSuccessor(node,node->right );
        }

        // Atualiza a altura do node e regula o node
        node = _fixUpDeletion(node);
        return node;
    }
    
    //Função privada que realiza a busca do item recursivamente.
    bool _contains(avlNode<K,V>* node, K key){
        
        if (node == nullptr){
            return false;
        }     

        if (key < node->par.first){
            return _contains(node->left, key);
        } else if (key > node->par.first){
            return _contains(node->right, key);
        }

        return true;
    }
    
    //Função que recebe o nó e sua chave e procura se é necessário fazer rotações na árvore baseado no balanço dele.
    avlNode<K,V>* _fixUpNode(avlNode<K,V>* p, K key){

        p->height = 1 + std::max(_getHeight(p->left), _getHeight(p->right));

        
        int bal = _getBalance(p);
        //Árvore desbalanceada para a direita, rotação simples.
        if (bal < -1 && key < p->left->par.first){
            rots++;
            comps++;
            return _rightRotation(p);
        } 
        //Árvore desbalanceada para a direita, rotação dupla.
        else if (bal < -1 && key > p->left->par.first){
            rots += 2;
            comps++;
            p->left = _leftRotation(p->left);
            return _rightRotation(p);
        }

        //Caso simétrico para a esquerda, rotação simples.
        if (bal > 1 && key > p->right->par.first){
            rots++;
            comps++;
            return _leftRotation(p);
        } 
        //Rotação dupla para a esquerda.
        else if (bal > 1 && key < p->right->par.first){
            rots += 2;
            comps++;
            p->right = _rightRotation(p->right);
            return _leftRotation(p);
        }

        //Retorna o nó modificado. "P" também pode ser enviado como referência, nesse caso, o retorno não é necessário.
        return p;
    }
    
    //Função que recebe a raíz da árvore e o nó a ser removido.
    avlNode<K,V>* _removeSuccessor(avlNode<K,V>* root, avlNode<K,V>* node){
        //Procura o menor valor da sub-árvore esquerda para substituir o nó. 
        if (node->left != nullptr){
            node->left = _removeSuccessor(root, node->left);
        
        //Uma vez achado, trocamos as chaves.
        } else {
            root->par.first = node->par.first ;
            
            //Reconectamos à sub-árvore da direita do node, apagamos o nó, decrementamos e retornamos a nova sub-árvore.
            avlNode<K,V>* aux = node->right ;
            delete node;
            size--;

            return aux;
        }
        
        //Verifica se a remoção foi feita corretamente e se há necessidade de reajustar a árvore.
        node = _fixUpDeletion(node);
        return node;
    }

    //Função privada que realiza o conserto do nó enviado, retorna a árvore consertada após a remoção.
    //Faz o mesmo que o fixUpNode, porém agora usando os balanços ao invés do valor das chaves.
    avlNode<K,V>* _fixUpDeletion(avlNode<K,V>* p){
        
        int bal = _getBalance(p);
        if (bal > 1 && _getBalance(p->right) >= 0){
            rots++;
            return _leftRotation(p);
        } 
        else if (bal > 1 && _getBalance(p->right) < 0){
            rots += 2;
            p->right = _rightRotation(p->right);
            return _leftRotation(p);
        }

        if (bal < -1 && _getBalance(p->left) <= 0){
            rots++;
            return _rightRotation(p);
        } 
        else if (bal < -1 && _getBalance(p->left) > 0){
            rots += 2;
            p->left = _leftRotation(p->left);
            return _rightRotation(p);
        }

        p->height = 1 + std::max(_getHeight(p->left), _getHeight(p->right)); 

        return p;
    }
    
    //Rotação à esquerda, recebe o nó que precisa ser rotacionado e retorna o nó consertado.
    avlNode<K,V>* _leftRotation(avlNode<K,V>* node){
        //Cria duas variáveis "bolha" que recebem o nó a direita dele e o nó à direita desse.
        avlNode<K,V>* y = node->right;
        avlNode<K,V>* T2 = y->left;

        //Faz o nó à esquerda receber o nó a ser rotacionado, depois o nó a direita recebe a esquerda novamente.
        //Quase fazendo a operação inversa da operação acima. 
        y->left = node;
        node->right = T2;
        
        //Recalcula as alturas e retorna a nova subárvore.
        node->height = 1 + std::max(_getHeight(node->left), _getHeight(node->right));
        y->height = 1 + std::max(_getHeight(y->left), _getHeight(y->right));

        return y;
    }

    //Rotação à direita, essa função é simétrica à função acima.
    avlNode<K,V>* _rightRotation(avlNode<K,V>* node){
        avlNode<K,V>* y = node->left;
        avlNode<K,V>* T2 = y->right;

        y->right = node;
        node->left = T2;

        node->height = 1 + std::max(_getHeight(node->left), _getHeight(node->right));
        y->height = 1 + std::max(_getHeight(y->left), _getHeight(y->right));

        return y;
    }

    //Função que encontra o valor associado à uma chave e retorna uma referência para ela.
    V& _searchKey(avlNode<K,V>* node, const K& key) {
        if (node == nullptr) {
            throw std::out_of_range("Chave não encontrada na árvore.");
        }

        if (key < node->par.first) {
            return _searchKey(node->left, key);
        } else if (key > node->par.first) {
            return _searchKey(node->right, key);
        } else {
            return node->par.second;  // Chave encontrada.
        }
        
    }

    //Retorna à altura do nó, não precisava de uma função para isso, porém a orientação a objetos pede.
    int _getHeight(avlNode<K,V>* node){
        if (node == nullptr){
            comps++;
            return -1;
        } else {
            return node->height;
        }
    }

    //Retorna o balanço do nó.
    int _getBalance(avlNode<K,V>* Node){
        return (_getHeight(Node->right) - _getHeight(Node->left));
    }

    //Função que preenche um vetor de pares com o valor do nó de cada nódulo da árvore.
    //Essa função percorre a árvore em ordem Simétrica.
    void _inOrder(avlNode<K,V>* node, std::vector<std::pair<K, V>>& vec) const{
        if (node != nullptr){
            _inOrder(node->left, vec);
            vec.push_back(node->par);
            _inOrder(node->right, vec);
        }
    }

public:

    //Construtor, inicia toda a classe.
    avlTree() {
        root = nullptr;
        comps = 0;
        size = 0;
        rots = 0;
    }

    //Destrutor, chama a função de apagar a árvore.
    ~avlTree() {
        _deleteTree(root);
    }

    void insert(K key, V value){
        root = _insert(root, key, value);
    }

    void remove(K key){
       root = _remove(root, key);
    }

    bool contains(K key){
        return _contains(root, key);
    }

    //Retorna o tamanho atual da árvore.
    int getSize(){
        return size;
    }

    //Retorna a altura total da árvore.
    int getTreeHeight(){
        if (root != nullptr){
            return root->height;
        }
    }

    size_t getComps() const {
        return comps;
    }

    int getRotations() const {
        return rots;
    }
    
    //Função que retorna um vetor com os itens ordenados com o item da maior frequência à menor frequência.
    std::vector<std::pair<K,V>> sortByFrequency() const {
        std::vector<std::pair<K,V>> vec;

        _inOrder(root, vec);
        std::sort(vec.begin(), vec.end(), [] (const std::pair<K, V>& a, const std::pair<K, V>& b) {
            return a.second > b.second;
        });
        return vec;
    }

    /// @brief Função operador colchetes, permite acessar os valores armazenados no endereço da chave enviada, 
    /// caso ela não exista, insere dentro da árvore e retorna esse valor recém-colocado.
    /// @param key 
    /// @return Uma referência ao valor.
    V& operator[](const K& key){
        try {
            return _searchKey(root, key);
        } catch (const std::out_of_range&){
            V defaultValue{};
            insert(key, defaultValue);
            return _searchKey(root, key);
        }
    }
 
    const V& operator[](const K& key) const {
        return _searchKey(root, key);
    }

private:

    //Função que apaga a árvore (ou uma sub-árvore) de maneira recursiva.
    void _deleteTree(avlNode<K,V>* node) {
        if (node) {
            _deleteTree(node->left);
            _deleteTree(node->right);
            delete node;
        }
    }
};

#endif