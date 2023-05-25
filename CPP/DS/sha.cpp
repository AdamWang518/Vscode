#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
const int BlockSize = 128; // Block Size 128Bytes
// const int HashSize = 64;   // HashValue 64Bytes
const uint64_t sha512_initial_hash[8] = {
    0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
    0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179};
// 初始hash value
const uint64_t sha512_round_constant[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
    0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
    0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
    0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
    0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
    0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
    0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
    0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
    0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
// hash round constants，每次hash需要運算80個rounds，這是其中所需的常數
uint64_t rightrotate(uint64_t value, int positions)
{
    return (value >> positions) | (value << (64 - positions));
}
uint8_t *PadInput(const uint8_t *inputBytes, size_t inputLength, size_t &paddedLength)
{
    size_t originalLength = inputLength;
    size_t paddingLength = (BlockSize - ((originalLength + 16) % BlockSize)); // 預留至少16Bytes(128bit)裝原始訊息長度，並計算剩餘可用作填充的空間

    paddedLength = originalLength + paddingLength + 16; // 保留最後用來裝原始訊息長度的16bytes
    uint8_t *paddedData = new uint8_t[paddedLength];

    memcpy(paddedData, inputBytes, originalLength); // 保留原始資訊
    paddedData[originalLength] = 0x80;              // 最後面填充1然後後面放0，1000 0000
    // 填充0x00直到最後的16個位元組之前
    // 順便填充最後的16~9位元組
    for (size_t i = originalLength + 1; i < paddedLength - 8; i++)
    {
        paddedData[i] = 0x00;
        // 由於使用uint8，因此每一位都是一個byte
    }

    uint64_t bitLength = originalLength * 8; // 算出原始長度有幾bit，存在最後16Byte中
    // 因為 inputLength（即原始消息的長度）被定義為 size_t 類型
    // 而用來讀取的String尺寸也有限(8byte/64bit)
    // 在這種情況下，原始消息的長度不可能超過64位可以表示的最大值
    // 所以長度只需填充最後的64bit，最後128~65可以保持為0
    for (int i = 7; i >= 0; i--)
    {
        paddedData[paddedLength - 8 + i] = (bitLength >> ((7 - i) * 8)) & 0xFF;
        //  依照big endian，最高位元放在最左側
        //  每個迴圈都向右移一個byte
        //  並使用位元運算符 & 配合遮罩 0xFF，將 bitLength 的位元內容截斷為 8 位元
        //  將結果存儲在 paddedData 的對應位元組中。直到最後
    }
    return paddedData;
    // 回傳填充好的陣列
}

inline uint64_t Ch(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (~x & z); }
inline uint64_t Maj(uint64_t x, uint64_t y, uint64_t z) { return (x & y) ^ (x & z) ^ (y & z); }
inline uint64_t Sigma0(uint64_t x) { return rightrotate(x, 28) ^ rightrotate(x, 34) ^ rightrotate(x, 39); }
inline uint64_t Sigma1(uint64_t x) { return rightrotate(x, 14) ^ rightrotate(x, 18) ^ rightrotate(x, 41); }
// 每round的運算中，會運用到幾個運算函數（Bitwise Operation Function）：
// Ch(x, y, z) = (x AND y) XOR (NOT x AND z)
// Maj(x, y, z) = (x AND y) XOR (x AND z) XOR (y AND z)
// Σ0(x) = ROTR^28(x) XOR ROTR^34(x) XOR ROTR^39(x)
// Σ1(x) = ROTR^14(x) XOR ROTR^18(x) XOR ROTR^41(x)
void shaCompress(uint64_t *hash, const uint8_t block[BlockSize], bool output)
{
    uint64_t W[80];
    // 每個word需要為64bit長，因此用uint64_t，每位有8byte
    for (int i = 0; i < 16; i++)
    {
        W[i] = 0;
        for (int j = 0; j < 8; j++)
        {
            W[i] |= static_cast<uint64_t>(block[i * 8 + j]) << (56 - j * 8);
            // 將當前的block的該字節(byte)轉換為64bit(8byte)格式儲存
        }
        // 設定前16(0~15)位訊息字
    }
    for (int i = 16; i < 80; i++)
    {
        uint64_t s0 = rightrotate(W[i - 15], 1) ^ rightrotate(W[i - 15], 8) ^ (W[i - 15] >> 7);
        uint64_t s1 = rightrotate(W[i - 2], 19) ^ rightrotate(W[i - 2], 61) ^ (W[i - 2] >> 6);
        W[i] = W[i - 16] + s0 + W[i - 7] + s1;
        // 以運算擴展出16~79位訊息字
    }
    uint64_t a = hash[0];
    uint64_t b = hash[1];
    uint64_t c = hash[2];
    uint64_t d = hash[3];
    uint64_t e = hash[4];
    uint64_t f = hash[5];
    uint64_t g = hash[6];
    uint64_t h = hash[7];
    // 主要循環，做80個round的運算
    for (int t = 0; t < 80; t++)
    {
        uint64_t T1 = h + Sigma1(e) + Ch(e, f, g) + sha512_round_constant[t] + W[t];
        uint64_t T2 = Sigma0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    // round完成，做addtion，更新hash
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
}
int main(int argc, char *argv[])
{
    string path = "D:\\Vscode\\CPP\\DS\\sha.txt";
    bool output = false;
    // string path = argv[1];
    // bool output;
    // if (strcmp(argv[2], "true") == 0 || strcmp(argv[2], "True") == 0 || strcmp(argv[2], "1") == 0)
    // {
    //     output = true;
    //     cout << "rounds output mode" << endl;
    // }
    // else if (strcmp(argv[2], "false") == 0 || strcmp(argv[2], "False") == 0 || strcmp(argv[2], "0") == 0)
    // {
    //     output = false;
    //     cout << "rounds hide mode" << endl;
    // }
    // else
    // {
    //     cout << "parameters error!" << endl;
    //     return 1;
    // }

    ifstream file(path);
    if (file.is_open())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str(); // read file
        uint8_t contentBytes[content.length()];
        memcpy(contentBytes, content.data(), content.length());
        size_t paddedLength = 0; // inintial paddedLength
        auto start_time = chrono::high_resolution_clock::now();
        uint8_t *paddedInput = PadInput(contentBytes, content.length(), paddedLength); // 以128Byte(1024bit)為單位計算填充量
        uint64_t hash[8];
        for (int i = 0; i < 8; i++)
        {
            hash[i] = sha512_initial_hash[i];
        }
        // 初始化hash value

        for (size_t i = 0; i < paddedLength; i += BlockSize)
        {
            uint8_t block[BlockSize];
            memcpy(block, &paddedInput[i], BlockSize);
            // 現在，block 陣列包含了當前的 128 byte 區塊
            // 接著對每個區塊執行 SHA-512 壓縮函數
            shaCompress(hash, block, output);
        }
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000000.0;
        // 紀錄整個計算所花費的時間
        if (duration > 0)
        {
            cout << "Performance: " << (paddedLength / duration) << " bytes/second" << endl;
        }
        else
        {
            cout << "The computation is too fast to measure the performance accurately." << endl;
        }
        // 輸出以(bytes/second)為單位的效能
        delete[] paddedInput;
        stringstream final_hash;
        for (int i = 0; i < 8; i++)
        {
            final_hash << setfill('0') << setw(16) << hex << hash[i];
        }
        cout << final_hash.str() << endl;
        file.close();
    }
    else
    {
        cout << "Failed to open the file." << std::endl;
    }
    return 0;
}
// reference
// https://en.wikipedia.org/wiki/SHA-2
// https://medium.com/@zaid960928/cryptography-explaining-sha-512-ad896365a0c1
