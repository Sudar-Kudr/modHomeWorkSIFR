/*  аргументы:
0 in.txt sifr.txt 5   //шифрование
1 sifr.txt out.txt 5  //расшифрование */
#include <iostream>
#include <string>
#include <ctime>
#include <bit>
#include <bitset>
#include <fstream>

using namespace std;
const int k = 7;

struct Block{
    unsigned short data;
};

Block& operator^=(Block& block1, Block& block2){
    block1.data ^= block2.data;
    return block1;
}

Block& operator>>=(Block& block, int n){
    block.data = (block.data >> n) | (block.data << (8*sizeof(Block) - n));
    return block;
}

Block& operator <<=(Block& block, int n){
    block.data = block.data << n | block.data >> (8 * sizeof(Block) - n);
    return block;
}

//шифр
string encrypt(string msg){
    if(msg.size() % sizeof(Block) != 0)
        msg += "#"; //заглушка
    
    int n = (int)msg.size() / sizeof(Block);
    Block* arr = new Block[n];
    memcpy(arr, msg.c_str(), n * sizeof(Block));
    
    Block* gamma = new Block[n];
    for(int i = 0; i < n; i++){
        gamma[i].data = rand() % 10000;
    }
    
    for(int i = 0; i < n; i++){
        arr[i] ^= gamma[i];
        arr[i] >>= k;
    }
    
    char* res = new char[n * sizeof(Block)];
    memcpy(res, arr, n * sizeof(Block));
    string resS(res);
    
    delete[] arr;
    delete[] res;
    delete[] gamma;
    return resS;
}

//расшифрование
string decrypt(string msg){
    int n = (int)msg.size() / sizeof(Block);
    Block* arr = new Block[n];
    memcpy(arr, msg.c_str(), n * sizeof(Block));
    
    Block* gamma = new Block[n];
    for(int i = 0; i < n; i++)
        gamma[i].data = rand() % 10000;
    
    for(int i = 0; i < n; i++){
        arr[i] <<= k;
        arr[i] ^= gamma[i];
    }
    char* res = new char[n * sizeof(Block)];
    memcpy(res, arr, n * sizeof(Block));
    string resS(res);
    size_t ind = resS.find("#");
    if (ind != -1)
        resS.replace(ind, ind + 1,"");
    
    delete[] arr;
    delete[] res;
    delete[] gamma;
    return resS;
}

/*  аргументы:
 0 in.txt sifr.txt 5   //шифрование
 1 sifr.txt out.txt 5  //расшифрование */
int main(int argc, char* argv[])
{
    if(argc < 5){
        cout << "Не хватает аргументов" << endl;
        return 1;
    }
    int mode = stoi(argv[1]);
    string fileNameFrom = argv[2];
    string fileNameTo =  argv[3];
    unsigned initNumber = stoi(argv[4]);

    string in;
    std::cout << "С файла выполнен\n";
    std::ifstream fromfile(fileNameFrom);
    if(!fromfile){
        std::cout << "File not found!" << std::endl;
        return 1;
    }
    std::getline(fromfile, in);
    fromfile.close();
    
    srand(initNumber);
    string out;
    if(mode == 0){
        out = encrypt(in);
    }
    else{
        out = decrypt(in);
    }
    std::cout << "Запись в файл выполнен\n";
    std::ofstream tofile(fileNameTo);
    tofile << out;
    tofile.close();
    
    return 0;
}
/*
 ФАЙЛ СОЗДАНИЕ
 
 std::cout << "СОЗДАНИЕ файла\n";
 std::ofstream Createfile("in.txt");
 Createfile << "Write here Random text Please";
 Createfile.close();
 
 */
