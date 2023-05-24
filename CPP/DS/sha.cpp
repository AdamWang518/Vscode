#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
const int BlockSize = 128; // Block Size 128Bytes
const int HashSize = 64;   // HashValue 64Bytes
const uint64_t sha512_initial_hash[8] = {
    0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
    0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179};
// 初始hash value
uint8_t *PadInput(const uint8_t *inputBytes, size_t inputLength, size_t &paddedLength)
{
    size_t originalLength = inputLength;
    size_t paddingLength = (BlockSize - ((originalLength + 16) % BlockSize)); // 預留至少16Bytes做填充用，好寫入1000 0000作為開始標誌

    paddedLength = originalLength + paddingLength + 16; // 保留最後用來裝原始訊息長度的16bytes
    uint8_t *paddedData = new uint8_t[paddedLength];

    memcpy(paddedData, inputBytes, originalLength); // 保留原始資訊
    paddedData[originalLength] = 0x80;              // 最後面填充1然後後面放0，1000 0000
    // 填充0x00直到最後的16個位元組之前
    for (size_t i = originalLength + 1; i < paddedLength - 16; i++)
    {
        paddedData[i] = 0x00;
    }

    uint64_t bitLength = originalLength * 8; // 算出原始長度有幾bit，存在最後16Byte中
    // 因為 inputLength（即原始消息的長度）被定義為 size_t 類型
    // 在這種情況下，原始消息的長度不可能超過64位可以表示的最大值
    // 所以長度只需填充最後的64bit，最後128~65可以保持為0
    for (int i = 7; i >= 0; i--)
    {
        paddedData[paddedLength - 8 + i] = (bitLength >> ((7 - i) * 8)) & 0xFF;
        // 依照big endian，最高位元放在最左側
        //  每個迴圈都向右移一個byte
        //  並使用位元運算符 & 配合遮罩 0xFF，將 bitLength 的位元內容截斷為 8 位元
        //  並將結果存儲在 paddedData 的對應位元組中。
        // 直到最後
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
        string content = buffer.str(); // read file
        uint8_t contentBytes[content.length()];
        memcpy(contentBytes, content.data(), content.length());
        size_t paddedLength = 0;                                                       // inintial paddedLength
        uint8_t *paddedInput = PadInput(contentBytes, content.length(), paddedLength); // 以128Byte為單位去切割並填充
        delete[] paddedInput;

        file.close();
    }
    else
    {
        cout << "Failed to open the file." << std::endl;
    }
    return 0;
}
