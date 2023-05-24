#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
const int BlockSize = 128; // Block Size 128Bytes
const int HashSize = 64;   // HashValue 64Bytes
const uint64_t sha512_initial_hash[8] = {
    0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
    0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179};
uint8_t *PadInput(const uint8_t *inputBytes, size_t inputLength, size_t &paddedLength)
{
    size_t originalLength = inputLength;
    size_t paddingLength = (128 - ((originalLength + 16) % 128));

    paddedLength = originalLength + paddingLength + 16;
    uint8_t *paddedData = new uint8_t[paddedLength];

    memcpy(paddedData, inputBytes, originalLength);
    paddedData[originalLength] = 0x80;

    uint64_t bitLength = originalLength * 8;
    for (int i = 0; i < 8; i++)
    {
        paddedData[paddedLength - 8 + i] = (bitLength >> ((7 - i) * 8)) & 0xFF;
    }

    return paddedData;
}
int main(int argc, char *argv[])
{
    string path = "D:\\Vscode\\CPP\\DS\\sha.txt";
    // string path = argv[1];

    ifstream file(path);
    if (file.is_open())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        cout << "\nText file content:\n"
             << content << endl;
        uint8_t contentBytes[content.length()];
        memcpy(contentBytes, content.data(), content.length());
        size_t paddedLength = 0;
        // for (int i = 0; i < content.length(); i++)
        // {
        //     cout << hex << static_cast<int>(contentBytes[i]) << " ";
        // }
        uint8_t *paddedInput = PadInput(contentBytes, content.length(), paddedLength);
        delete[] paddedInput;
        // padd

        file.close();
    }
    else
    {
        cout << "Failed to open the file." << std::endl;
    }
    return 0;
}
