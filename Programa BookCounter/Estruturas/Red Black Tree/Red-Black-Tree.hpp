#ifndef RBTREE_H
#define RBTREE_H
#include "rbNode.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

template <typename K, typename V>
class rbTree {
private:
    rbNode<K,V>* root;
    rbNode<K,V>* NIL;
    size_t comps;
    size_t size;
    int rots;


    //==================Funções básicas da estrutura==================\\

    //Função que insere *iterativamente* um nó na árvore.
    //Essa função recebe a raiz da árvore por referência e a cópia de um nó Z.
    void _insert (rbNode<K,V>*& root, rbNode<K,V>* z) {
        
        
        rbNode<K,V>* x = root;
        rbNode<K,V>* y = NIL;

        //Enquanto X não for um nó NIL.
        while(x != NIL){
            y = x;
            
            //Caso chave igual.
            if (z->par.first == x->par.first){
                this->comps++;
                delete z;
                return;
            }
            
            
            if (z->par.first < x->par.first){
                this->comps++;
                x = x->left;

            
            } else if (z->par.first > x->par.first){
                this->comps++;
                x = x->right;
            }
            
        }


        //Inserção da chave
        //Ajusta o pai do nó

        z->p = y;

        //Árvore vazia
        if (y == NIL){
            this->comps++;
            root = z;
        //Nó na esquerda
        } else if (z->par.first < y->par.first) {
            this->comps++;
            y->left = z;
        //Nó na direita
        } else {
            y->right = z;
        }

        //Reajusta os ponteiros 
        z->left = NIL;
        z->right = NIL;
        //z->color = BLACK;

        size++;

        //Chama a função de conserto para inserções
        insertFixUp(root, z);

    }
    
    //Função que remove um item da árvore baseado na chave enviada.
    //Essa função recebe a raiz da árvore por referência e a cópia de um nó Z.
    void _remove(rbNode<K,V>*& root, rbNode<K,V>* z){
        rbNode<K,V>* y;

        //Árvore ou subárvore vazia.
        if (z->left == NIL || z->right == NIL){
            y = z;
        //Procura o mínimo da árvore para substituir e remover sem degenerar a árvore.
        } else {
            y = _minimum(z->right);
        }

        //Navega pela árvore para encontrar o fim da árvore;
        if(y->left != NIL){
            z = y->left;
        } else {
            z = y->right;
        }

        //Troca o pai de Z com o pai de Y.
        z->p = y->p;

        //Se acabar sendo NIL, a árvore está vazia, então z se torna raiz.
        if (y->p == NIL){
            root = z;
        //Se não, joga o nó na ponta da ponta de y.
        } else {
            if (y == y->p->left){
                y->p->left = z;
            } else {
                y->p->right = z;
            }
        }

        //Troca as chaves caso não sejam iguais
        if (y != z){
            z->par.first = y->par.first;
            z->par.second = y->par.second;
        } 

        //Se o nó acabar ficando chamando preto, bem, hora de chamar reforços.
        if (y->color == BLACK){
            deleteFixUp(root, z);
        }

        size--;
        delete y;
    }
    
    //Função que pesquisa um valor na estrutura e retorna VERDADEIRO se ele estiver
    //E FALSO se não.
    bool _search(rbNode<K,V>* node, K key) {
        if (node == NIL) return false;
        if (key < node->par.first) return _search(node->left, key);
        if (key > node->par.first) return _search(node->right, key);
        return true;
    }

    /// @brief Função que limpa toda a árvore recursivamente.
    /// @param node A árvore a ser limpa.
    void _clear(rbNode<K,V>* node) {
        if (node == NIL) return;

        _clear(node->left);
        _clear(node->right);

        delete node;
    }  

    /// @brief Função que procura uma chave na árvore e RETORNA ela como uma referência.
    /// @param node 
    /// @param key 
    /// @return 
    V& _searchKey(rbNode<K,V>* node, const K& key) {
        if (node == NIL) {
            throw std::out_of_range("Chave não encontrada na árvore.");
        }

        if (key < node->par.first) {
            return _searchKey(node->left, key);
        } else if (key > node->par.first) {
            return _searchKey(node->right, key);
        } else {
            return node->par.second;  // Chave encontrada
        }
    }

    //==================Funções auxiliares==================\\ 
    
    //Funções de rotação e conserto:

    void leftRotate(rbNode<K,V>*& root, rbNode<K,V>* x) {
        rots++;
        rbNode<K,V>* y = x->right;        
        x->right = y->left;              

        if (y->left != NIL) {
            this->comps++;    
            y->left->p = x;              
        }

        y->p = x->p;                     

        if (x->p == NIL) {   
            this->comps++;      
            root = y;                    
        } else if (x == x->p->left) {  
            this->comps++;  
            x->p->left = y;              
        } else {                       
            x->p->right = y;             
        }

        y->left = x;                     
        x->p = y;                        
    }

    void rightRotate(rbNode<K,V>*& root, rbNode<K,V>* x) {
        rots++;
        rbNode<K,V>* y = x->left;
        x->left = y->right;

        if (y->right != NIL) {
            this->comps++;
            y->right->p = x;
        }

        y->p = x->p;

        if (x->p == NIL) {
            this->comps++;
            root = y;
        } else if (x == x->p->right) {
            this->comps++;
            x->p->right = y;
        } else {
            this->comps++;
            x->p->left = y;
        }

        y->right = x;
        x->p = y;
    }

    /// @brief Função de conserto da inserção.
    /// @param root 
    /// @param z 
    void insertFixUp(rbNode<K,V>*& root, rbNode<K,V>* z) {        
        rbNode<K,V>* y;

        while(z->p->color == RED){
            if(z->p == z->p->p->left){
                this->comps++;
                y = z->p->p->right;
                if(y != NIL && y->color == RED){
                    this->comps++;
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right){
                        this->comps++;
                        z = z->p;
                        leftRotate(root, z);
                    }
                    z->p->color = BLACK;
                    z->p->p->color = RED;
                    rightRotate(root, z->p->p);
                }
            } else {
                    y = z->p->p->left;

                if(y->color == RED) {
                    this->comps++;
                    z->p->color = BLACK;
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left){
                        this->comps++;
                        z = z->p;
                        rightRotate(root, z);
                    }

                z->p->color = BLACK;
                z->p->p->color = RED;
                leftRotate(root, z->p->p);
                }
            }

    }
        root->color = BLACK;
    }


    /// @brief Função de conserto da remoção.
    /// @param root 
    /// @param z 
    void deleteFixUp(rbNode<K,V>*& root, rbNode<K,V>* z) {
        rbNode<K,V>* w;
        
        while(z != root && z->color == BLACK){
            if(z == z->p->left){
                w = z->p->right;
                
                //CASE 1:
                if (w->color == RED){
                    w->color = BLACK;
                    z->p->color = RED;

                    comps++;
                    leftRotate(root, z->p);
                    w = z->p->right;
                }

                //CASE 2:
                if (w->left->color == BLACK && w->right->color == BLACK){
                    comps++;
                    w->color = RED;
                    z = z->p;
                } else {
                //CASE 3:
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;

                    comps++;
                    rightRotate(root, w);
                    w = z->p->right;
                }
                //CASE 4:
                w->color = z->p->color;
                z->p->color = BLACK;
                w->right->color = BLACK;

                leftRotate(root, z->p);
                z = root;
                }
            } else {
                //Lado oposto.
                w = z->p->left;
                
                //CASE 1:
                if (w->color == RED){
                    w->color = BLACK;
                    z->p->color = RED;
                    
                    comps++;
                    rightRotate(root, z->p);
                    w = z->p->left;
                }

                //CASE 2:
                if (w->right->color == BLACK && w->left->color == BLACK){
                    comps++;
                    w->color = RED;
                    z = z->p;
                } else {
                //CASE 3:
                if (w->left->color == BLACK) {
                    comps++;
                    w->right->color = BLACK;
                    w->color = RED;
                    
                    leftRotate(root, w);
                    w = z->p->left;
                }
                //CASE 4:
                w->color = z->p->color;
                z->p->color = BLACK;
                w->left->color = BLACK;
                
                rightRotate(root, z->p);
                z = root;
                }
            }
        }
        z->color = BLACK;
    }

    //================== Funções de Impressão ==================\\

    //Retorna um vetor organizado em Ordem Simétrica.
    void _inOrder(rbNode<K,V>* node, std::vector<std::pair<K,V>>& vec) const {
        if (node == NIL) return;
        _inOrder(node->left, vec);
        vec.push_back(node->par);
        _inOrder(node->right, vec);
    }

    //==================Funções de Mínimo e Máximo==================\\

    //Retorna a maior palavra da árvore
    rbNode<K,V>* _maximum(rbNode<K,V>* z){
        rbNode<K,V>* current = z;
        
        while (current->left != NIL){
            current = current->left;
        }
        
        return current;
    }

    //Retorna a menor palavra da árvore
    rbNode<K,V>* _minimum(rbNode<K,V>* z){
        rbNode<K,V>* current = z;

        while (current->right != NIL){
            current = current->right;
        }

        return current;
    }


public:

    //==================Construtor e Destrutor==================\\

    //Construtor, cria a árvore rubro-negra com um nó NIL (um nó que aponta apenas para si mesmo),
    //Inicializa a raíz e os atributos da classe como size e Comps.
    rbTree(){
        NIL = new rbNode<K,V>(K(), V(), BLACK, NIL, NIL, NIL);
        root = NIL;
        
        comps = 0;
        size = 0;
        rots = 0;
    }

    //Destrutor, no fim da vida da classe, chama a função de limpar a classe e destrói a classe.
    ~rbTree(){
        _clear(root);
    }
    
    //==================Funções básicas==================\\
    
    void insert(K key, V value){   
        rbNode<K,V>* z = new rbNode<K,V>(key, value, RED, NIL, NIL, NIL);
        _insert(root, z);
    }

    bool search(K key) {
        return _search(root, key);
    }

    void remove(K key){
        rbNode<K,V>* p = root;
        
        //Verifica se a chave existe mesmo, se sim, envia para a função de remove.
        while( p != NIL && p->par.first != key ){
            if (key < p->par.first){
                p = p->left;
            } else {
                p = p->right;
            }
        }

        if(p != NIL){
            _remove(root, p);
        }

    }
    
    //==================Funções Adicionais==================\\
    
    size_t getComps() const {
        return comps;
    }

    int getRotations() const {
        return rots;
    }

    size_t getSize() const {
        return size;
    }

    //Função de limpeza da classe, retira todos os nós, libera a memória e faz a raíz voltar ao seu estado primitivo,
    //Assim como as comparações.
    void clear(){
        _clear(root);
        root = NIL;
        
        comps = 0;
        size = 0;
        rots = 0;
    }

    //Função que retorna um vetor com os itens da árvore ordenados da palavra com maior frequência para a menor frequência.
    std::vector<std::pair<K,V>> sortByFrequency() const {
        std::vector<std::pair<K,V>> result;
        _inOrder(root, result);
        
        std::sort(result.begin(), result.end(), [](const std::pair<K,V>& a, const std::pair<K,V>& b) {
            return a.second > b.second;
        });

        return result;
    }

    //Função que retorna a maior palavra da classe ou a menor palavra da classe baseado no booleano enviado.
    std::pair<K,V> getMaxOrMin(bool order){
        if (order == true){
            rbNode<K,V>* aux = _maximum(root);
            std::cout << "Maior palavra: " << aux->par.first << std::endl;

            return aux->par;
        } else {
            rbNode<K,V>* aux = _minimum(root);
            std::cout << "Menor palavra: " << aux->par.first << std::endl;

            return aux->par;
        }
    }

    //Sobrecarga do operador colchetes, acessa o nódulo da árvore com a chave e retorna o valor dela.
    V& operator[](const K& key){
        rbNode<K,V>* node = root;

        while(node != NIL){

            if (key < node->par.first){
                node = node->left;
            } else if (key > node->par.first){
                node = node->right;
            } else {
                return node->par.second;
            }
        }

        V defaultValue{};
        rbNode<K,V>* z = new rbNode<K,V>(key, defaultValue, RED, NIL, NIL, NIL);
        _insert(root, z);
        return z->par.second;
    }

    const V& operator[](const K& key) const {
        return _searchKey(root, key);
    }

};


#endif