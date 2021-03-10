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


typedef struct Block
{
   uint8_t* data;
} block;
    
block* constructor() {
    block* newb = new block;
    newb->data = static_cast<uint8_t*>(malloc(12));
    return newb;
}

block& operator>>=(block& block, int k){
    uint8_t* help_data = static_cast<uint8_t*>(malloc(12));
    int j=0;
    for (int i = 12-k; i < 12; i++)
    {
        help_data[j] = block.data[i];
        j++;
    }
    for (int i = 0; i < 12-k; i++)
    {
        help_data[j] = block.data[i];
        j++;
        
    }
    free(block.data);
    block.data = help_data;
    return block;
}

block& operator <<=(block& block, int n){
    uint8_t* help_data = static_cast<uint8_t*>(malloc(12));
    int j=11;
    for (int i = k-1; i >= 0; i--)
    {
        help_data[j] = block.data[i];
        j--;
    }
    for (int i = 11; i >= k; i--)
    {
        help_data[j] = block.data[i];
        j--;
    }
    free(block.data);
    block.data = help_data;
    return block;
}

block& operator^=(block& block, string str){
    for (int i = 0; i < 12; ++i)
        block.data[i] = str[i];
    return block;
}
string& operator^=(string& str, block block){
    for (int i = 0; i < 12; ++i)
        str += block.data[i];
    return str;
}

//шифр
string encrypt(string msg){
    while(msg.size() % (sizeof(char) * 12) != 0) {
        msg += "#"; //заглушка
    }

    int n = static_cast<int>(msg.size()) / (sizeof(char) * 12);
    block arr[n];
    for (int i = 0; i < n; i++) {
        arr[i].data = static_cast<uint8_t*>(malloc(12));
    }
    for(int i = 0, j = 0; i < n; i++, j = j+12) {
        arr[i] ^= msg.substr(j,12);
    }
    
    block gamma;
    gamma.data = static_cast<uint8_t*>(malloc(12));
    for(int i = 0; i < n; i++){
         for(int j = 0; j < 12; j++) {
             gamma.data[j] = rand() % 80;
             arr[i].data[j] += gamma.data[j];
         }
        arr[i] >>= k;
    }

    string res = "";
    for (int i = 0; i < n; i++) {
        string h="";
        h ^= arr[i];
        std::string str(h);
        res += str;
    }
    string resS(res);
    for(int i = 0; i < n; i++) {
        free(arr[i].data);
    }
    return resS;
}

string decrypt(string msg){
    int n = static_cast<int>(msg.size()) / (sizeof(char) * 12);
    block arr[n];
    for (int i = 0; i < n; i++) {
        arr[i].data = static_cast<uint8_t*>(malloc(12));
    }
    for(int i = 0, j = 0; i < n; i++, j = j+12) {
        arr[i] ^= msg.substr(j,12);
    }
    
    block gamma;
    gamma.data = static_cast<uint8_t*>(malloc(12));
    for(int i = 0; i < n; i++){
        arr[i] <<= k;
         for(int j = 0; j < 12; j++) {
             gamma.data[j] = rand() % 80;
             arr[i].data[j] -= gamma.data[j];
         }
        
    }

    string res = "";
    for (int i = 0; i < n; i++) {
        string h="";
        h ^= arr[i];
        std::string str(h);
        res += str;
    }
    string resS(res);
    for(int i = 0; i < n; i++) {
        free(arr[i].data);
        
    }
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
    std::ofstream tofile(fileNameTo);
    while(!fromfile.eof()){
        std::getline(fromfile, in);
        
        srand(initNumber);
        string out;
        if(mode == 0){
            out = encrypt(in);
        }
        else{
            out = decrypt(in);
            size_t ind = out.find('#');
            if (ind != std::string::npos )
                out = out.substr(0, ind);
        }
        std::cout << "Запись в файл выполнен\n";
        tofile << out << "\n";
    }
    fromfile.close();
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
