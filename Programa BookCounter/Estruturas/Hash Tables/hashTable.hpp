#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <list>
#include <vector>
#include <utility>
#include <functional>
#include <exception>

/*Classe que implementa uma hashtable com tratamento de colisão por
endereçamento aberto*/ 
template < typename Key, typename Value,  typename Hash = std::hash<Key> >
class hashTable {
private:
    //Preferi usar um ENUM para essa classe por gostar mais da claridade, diferente de usar um char.
    enum Status {EMPTY, DELETED, OCCUPIED};

    //Struct dentro da classe para evitar criar fora da classe e ter que chamar como biblioteca.
    //Recebe valores genéricos como Key e Value e um ENUM do status.
    struct Entry {
        Key key;
        Value value;
        Status status;

        //Construtor padrão, deixa o slot apenas Empty.
        Entry() : status(EMPTY) {}

        //Construtor composto com chave e valor.
        Entry(Key key, Value value, Status status = OCCUPIED){
            this->key = key;
            this->value = value;
            this->status = status; 
        }
    
    };

    //Atributos principais: 
    //Vetor da tabela, capacidade atual, tamanho atual (conteúdo) e a função de hashing.
    std::vector<Entry> table;
    size_t capacity;
    size_t currentSize;
    size_t comps;
    int rehashCount;
    Hash hashFunction;

    //Função que acha e retorna o slot livre para a inserção do item baseado na chave e no índice.
    //Usando módulo, conseguimos delimitar o domínio para dentro do vetor, evitando overflow.
    size_t probe(const Key& key, size_t i) const {
        return (hashFunction(key) + i) % capacity;
    }

    size_t get_next_prime(size_t x) {
        if(x <= 2) return 3;
        x = (x % 2 == 0) ? x + 1 : x;
        bool not_prime = true;
        while(not_prime) {
            not_prime = false;
            for(int i = 3; i <= sqrt(x); i+=2) {
                if(x % i == 0) {
                    comps++;
                    not_prime = true;
                    break;
                }
            }
            x += 2;
        }
        return x - 2;
    }

    void rehash(){
        size_t newCap = get_next_prime(capacity*2);
        resize(newCap);
    }

public:

    //Construtor da classe:
    hashTable(size_t cap = 101){
        capacity = cap;
        currentSize = 0;
        
        table.resize(cap);
        comps = 0;
        rehashCount = 0;
    }

    //Destrutor da classe:
    ~hashTable(){
        capacity = 0;
        currentSize = 0;
        comps = 0;
        table.clear();
    }

    //Função de inserção, recebe uma referência da chave e do valor a serem colocados.
    bool insert(const Key& key, const Value& value){
        
        if (static_cast<double>(currentSize)/capacity >= 0.7){
            comps++;
            rehashCount++;
            rehash();
        }

        for (size_t i = 0; i < capacity; i++){
            //Ajusta o índice inicial para a região que a chave vai estar.
            //Isso é feito dentro do for para tentar várias vezes e tentar achar uma região apropriada.
            size_t index = probe(key, i);

            //Caso ideal. Existe um espaço (seja virgem ou deletado) livre na tabela,
            //Então insere a chave e o valor dentro da estrutura.
            if(table[index].status == EMPTY || table[index].status == DELETED){
                comps++;
                table[index].key = key;
                table[index].value = value;
                table[index].status = OCCUPIED;
                currentSize++;
                return true;

            //Chave igual e status ocupado? A chave já existe na tabela, então apenas atualiza o valor com o novo.
            } else if (table[index].status == OCCUPIED && table[index].key == key){
                comps++;
                table[index].value = value;
                return true;
            }
        }

        return false;
    }

    //Função que procura a existência do item, retornando ele como uma referência.
    Value& search(const Key& key) {
        for (size_t i = 0; i < capacity; i++){
            size_t index = probe(key, i);
            
            if (table[index].status == EMPTY){
                comps++;
                break;
            } else if (table[index].status == OCCUPIED && table[index].key == key){
                comps++;
                return table[index].value;  
            }
        }

        throw std::out_of_range("Chave não encontrada na tabela");
    }

    bool contains(const Key& key) {
        try {
            search(key);
            return true;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    //Função que "remove" o item baseado na chave enviada.
    bool remove(const Key& key){
        for (size_t i = 0; i < capacity; i++){
            size_t index = probe(key, i);

            if(table[index].status == EMPTY){
                comps++;
                return false;
            }

            if(table[index].status == OCCUPIED && table[index].key == key){
                comps++;
                table[index].status = DELETED;
                currentSize--;
                return true;
            }
        }
        
        return false;
    }

    //Função que redimensiona parcialmente baseado em um valor passado por parâmetro pelo usuário.
    void resize(size_t newCapacity) {

        if(capacity > newCapacity){
            comps++;
            return;
        }
        
        std::vector<Entry> oldtable = table;
        
        table.clear();
        table.resize(newCapacity);
        capacity = newCapacity;
        currentSize = 0;

        
        for(const auto& entry : oldtable){
            if (entry.status == OCCUPIED){
                comps++;
                insert(entry.key, entry.value);
            }
        }
    }

    //Função que retorna um vetor com os valores ordenados da maior a menor frequência.
    std::vector<std::pair<Key, Value>> sortByFrequency() {
        std::vector<std::pair<Key, Value>> itens;

        for (const auto& it : table ){
            if (it.status == OCCUPIED){
                comps++;
                itens.emplace_back(it.key, it.value);
            }
        }

        std::sort(itens.begin(), itens.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
        });

        return itens;
    }

    bool isEmpty(){
        return currentSize == 0;
    }

    size_t getSize() const {
        return currentSize;
    }

    int getRehashAmmount() const {
        return rehashCount;
    }

    size_t getComps() const {
        return comps;
    }

    Value& operator[](const Key& key){
        try {
            return search(key);
        } catch (std::out_of_range&){
            Value defaultValue{};
            insert(key, defaultValue);
            return search(key);
        }
    }

    const Value& operator[](const Key& key) const {
        return search(key);
    }

};

#endif