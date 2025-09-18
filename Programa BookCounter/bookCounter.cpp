#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <utility>

#include "Estruturas/AVL Tree/AVL-Tree.hpp"
#include "Estruturas/Hash Tables/ChainedHashTable.hpp"
#include "Estruturas/Hash Tables/hashTable.hpp"
#include "Estruturas/Red Black Tree/Red-Black-Tree.hpp"

using namespace std::chrono;


std::string limparPalavra(const std::string& word) {
    std::string limpa;
    for (char c : word) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            limpa += std::tolower(c);   
        }
    }

    return limpa;

}

template<typename DS>
auto Count(std::ifstream& readArchive, DS& dataStructure) {
    
    std::string line;
    while (std::getline(readArchive, line)) {
        std::stringstream ss(line);
        std::string word;
    
        while (ss >> word) {
            std::string clear = limparPalavra(word);
            if (clear.empty()) continue;
            
            dataStructure[clear]++;
        }
    }

    return dataStructure.sortByFrequency();

}

int main(int argc, char* argv[]) {

    if (argc == 2 && std::string(argv[1]) == "--help" || std::string(argv[1]) == "--h") {
       std::cout << "Uso: ./bookcount [arquivo de entrada] [estrutura] [arquivo de saída (opcional)]\n";
        std::cout << "Exemplo: ./bookcount Livro1.txt avl resultado.txt\n\n";
        std::cout << "Estruturas suportadas:\n";
        std::cout << "  avl  - Árvore AVL\n";
        std::cout << "  rbt  - Árvore Rubro-Negra\n";
        std::cout << "  cht  - Hash Table com encadeamento\n";
        std::cout << "  ht   - Hash Table com endereçamento aberto\n";
        return 0;
    }
    
    if (argc < 3 || argc > 4) {
        std::cerr << "Verifique se seu comando está no formato: ./bookcount [entrada] [estrutura] [saida (opcional)]" << std::endl;
        std::cerr << "./bookcount [entrada] [estrutura] [saida (opcional)]\n";
        std::cerr << "Use ./bookcount --help para mais informações.\n";
        return 1;
    }

    std::string inModel = "Arquivos para leitura/Entrada/";
    inModel += argv[1];

    std::ifstream archive(inModel);
    if (!archive.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de leitura." << std::endl;
        return 1;
    }
    
    
    std::string outModel = "Arquivos para leitura/Saida/";
    if (argc == 4){
        outModel += argv[3];
    } else {
        outModel += "resultado.txt";
    }
    
    std::ofstream exit(outModel);
    
    if (!exit.is_open()){
        std::cout << "Erro ao abrir o arquivo de saída" << std::endl;
        return 1;
    }
    
    std::string structure = argv[2];
    std::vector<std::pair<std::string, int>> freq;
    size_t comps;
    size_t totalWords;
    auto start = high_resolution_clock::now();

    exit << "Todas as palavras do livro: \n" << std::endl;

    if (structure == "avl") {
        avlTree<std::string, int> avl;
        freq = Count(archive, avl);
        
        comps = avl.getComps();
        totalWords = avl.getSize();
        exit << "Quantidade de rotações feitas: " << avl.getRotations() << std::endl;

    } else if (structure == "rbt") {
        rbTree<std::string, int> rbt;
        freq = Count(archive, rbt);
        comps = rbt.getComps();
        totalWords = rbt.getSize();

        exit << "Quantidade de rotações feitas: " << rbt.getRotations() << std::endl;

    } else if (structure == "cht") {
        ChainedHashTable<std::string, int> cht;
        freq = Count(archive, cht);
        comps = cht.getComps();
        totalWords = cht.getSize();

        exit << "Quantidade de Rehashes realizados: " << cht.getRehashAmmount() << std::endl; 

    } else if (structure == "ht") {
        hashTable<std::string, int> ht;
        freq = Count(archive, ht);
        comps = ht.getComps();
        totalWords = ht.getSize();

        exit << "Quantidade de rehashes realizados: " << ht.getRehashAmmount() << std::endl;

    } else {
        std::cerr << "Estrutura desconhecida. Use: avl | rbt | cht | ht";
        return 1;
    }

    archive.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    
    exit << "Comparações feitas: " << comps << std::endl;
    exit << "Tempo utilizado: " << duration.count() << " microsegundos." << std::endl;
    exit << "Palavras distintas: " << totalWords << std::endl;
    

    for (size_t i = 0; i < freq.size(); ++i) {
        exit << "---------------------------------------" << std::endl;
        exit << "|"<< i+1 << "º" <<"             "<< freq[i].first << " => " << freq[i].second <<'\n';
        exit << "---------------------------------------" << std::endl;
    }

    exit.close();

    std::cout << "Execução concluída com zero erros. " << std::endl;

    return 0;
}