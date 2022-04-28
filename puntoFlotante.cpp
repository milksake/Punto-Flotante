//Alumna: Mariana Paula Caceres Urquizo 

#include <iostream>
#include <bitset>
#include <cmath>

//Helper functions
double pow2(int n)
{
    if (n >= 0)
    {
        int pow = 1;
        for (int j = 0; j < n; j++)
            pow *= 2;
        return pow;
    }
    else
        return 1 / pow2(-n);
}

std::string int_to_bits(unsigned int num)
{
    if (num == 0) return "0";
    char bit[2] = {'0', '1'};
    std::string bitsStr;
    for (; num != 1; num /= 2)
        bitsStr = bit[num % 2] + bitsStr;
    bitsStr = '1' + bitsStr;
    return bitsStr;
}

template <long long unsigned int n>
int to_biased_int(std::bitset<n> bits)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        if (bits[i])
            sum += pow2(i);
    int bias = pow2(n - 1) - 1;
    return sum - bias;
}

template <long long unsigned int n>
std::bitset<n> int_to_biased_bits(int num)
{
    int bias = pow2(n - 1) - 1;
    num += bias;
    char bit[2] = {'0', '1'};
    std::string bitsStr;
    for (; num != 1; num /= 2)
        bitsStr = bit[num % 2] + bitsStr;
    bitsStr = '1' + bitsStr;
    return std::bitset<n>(bitsStr);
}

template <long long unsigned int n>
double significant_to_double(std::bitset<n> bits)
{
    double sum = 0;
    for (int i = n; i >= 0; i--)
        if (bits[i])
        {
            int a = n - i;
            double b = pow2(-a);
            if (std::isinf(b)) break;
            sum += b;
        }
    return sum + 1;
}

//Float and double bit functions
std::bitset<32> float_to_bits(float f)
{
    //if (f == 0) return std::bitset<32>("00000000000000000000000000000000");
    char bitSign;
    if (f < 0)
    {
        bitSign = '1';
        f *= -1;
    }
    else
        bitSign = '0';
    //Separate
    int fInt = (int)f;
    float fFloat = f - fInt;
    //To bits
    auto bits1i = int_to_bits(fInt);
    std::string bits2f;
    for (int i = 0; i < 24 - bits1i.length(); i++)
    {
        fFloat *= 2;
        if (fFloat >= 1)
        {
            bits2f += '1';
            fFloat -= 1;
        }
        else
            bits2f += '0';
    }
    //Calculate exponent
    auto exponent = int_to_biased_bits<8>(bits1i.length() - 1);
    return std::bitset<32>(bitSign + exponent.to_string() + bits1i.substr(1, bits1i.length()) + bits2f);
}

float bits_to_float(std::bitset<32> bits)
{
    auto bitsStr = bits.to_string();
    //Biased exponent
    std::bitset<8> exponent(bitsStr.substr(1, 8));
    auto exponentInt = to_biased_int(exponent);
    //Significant
    std::bitset<23> significant(bitsStr.substr(9, 31));
    auto significantFloat = significant_to_double(significant);
    if (bits[31])
        return -1 * significantFloat * pow2(exponentInt);
    else
        return significantFloat * pow2(exponentInt);
}

std::bitset<64> double_to_bits(double d)
{
    char bitSign;
    if (d < 0)
    {
        bitSign = '1';
        d *= -1;
    }
    else
        bitSign = '0';
    //Separate
    int fInt = (int)d;
    double fDouble = d - fInt;
    //To bits
    auto bits1i = int_to_bits(fInt);
    std::string bits2f;
    for (int i = 0; i < 53 - bits1i.length(); i++)
    {
        fDouble *= 2;
        if (fDouble >= 1)
        {
            bits2f += '1';
            fDouble -= 1;
        }
        else
            bits2f += '0';
    }
    //Calculate exponent
    auto exponent = int_to_biased_bits<11>(bits1i.length() - 1);
    return std::bitset<64>(bitSign + exponent.to_string() + bits1i.substr(1, bits1i.length()) + bits2f);

}

double bits_to_double(std::bitset<64> bits)
{
    auto bitsStr = bits.to_string();
    //Biased exponent
    std::bitset<11> exponent(bitsStr.substr(1, 11));
    auto exponentInt = to_biased_int(exponent);
    //Significant
    std::bitset<52> significant(bitsStr.substr(12, 63));
    auto significantFloat = significant_to_double(significant);
    if (bits[63])
        return -1 * significantFloat * pow2(exponentInt);
    else
        return significantFloat * pow2(exponentInt);
}

std::bitset<64> floatBits_to_DoubleBits(std::bitset<32> bits)
{
    auto bitsStr = bits.to_string();
    auto exponent = bitsStr.substr(1, 8);
    auto newExponent = int_to_biased_bits<11>(to_biased_int(std::bitset<8>(exponent)));
    auto significant = bitsStr.substr(9, 31);
    auto newSignificant = significant += "00000000000000000000000000000";
    return std::bitset<64>(bitsStr[0] + newExponent.to_string() + newSignificant);
}

//Get the bits from a variable (float and double)
bool* get_bits(float f)
{
    int* q = (int*)&f;
    bool* p = new bool[32];
    for (int i = 31; i >= 0 ; i--)
    {
        p[i] = *q & 1;
        *q = *q >> 1;
    }
    return p;
}

bool* get_bits(double d)
{
    int* q = (int*)&d;
    bool* p = new bool[64];
    for (int i = 31; i >= 0 ; i--)
    {
        p[32 + i] = *q & 1;
        *q = *q >> 1;
    }
    q += 1;
    for (int i = 31; i >= 0 ; i--)
    {
        p[i] = *q & 1;
        *q = *q >> 1;
    }
    return p;
}

//Print bits (as a bitset and as a boolean array)
template<long long unsigned int n>
void print_bitset(std::bitset<n> bits)
{
    auto bitsStr = bits.to_string();
    if (n == 32)
        std::cout << bitsStr[0] << ' ' << bitsStr.substr(1, 8) << ' ' << bitsStr.substr(9, 31) << '\n';
    else if (n == 64)
        std::cout << bitsStr[0] << ' ' << bitsStr.substr(1, 11) << ' ' << bitsStr.substr(12, 63) << '\n';
}

void print_array(bool* p, bool isDouble)
{
    if (isDouble)
    {
        for (int i = 0; i < 64; i++)
        {
            std::cout << p[i];
            if (i == 0 || i == 11) std::cout << ' ';
        }
        std::cout << '\n';
    }
    else
    {
        for (int i = 0; i < 32; i++)
        {
            std::cout << p[i];
            if (i == 0 || i == 8) std::cout << ' ';
        }
        std::cout << '\n';
    }
}

int main()
{
    float value1;
    std::cout << "Introduce un float: ";
    std::cin >> value1;
    std::cout << "Bits:\nMi implementacion:  ";
    print_bitset(float_to_bits(value1));
    std::cout << "Bits en la memoria: ";
    print_array(get_bits(value1), 0);

    double value2;
    std::cout << "\nIntroduce un double: ";
    std::cin >> value2;
    std::cout << "Bits:\nMi implementacion:  ";
    print_bitset(double_to_bits(value2));
    std::cout << "Bits en la memoria: ";
    print_array(get_bits(value2), 1);

    double value3;
    std::cout << "\nIntroduce otro valor: ";
    std::cin >> value3;
    std::cout << "Float:  ";
    print_bitset(float_to_bits(value3));
    std::cout << "Double: ";
    print_bitset(double_to_bits(value3));

    float value4;
    std::cout << "\nIntroduce un ultimo valor: ";
    std::cin >> value4;
    std::cout << "Float:   ";
    auto bits = float_to_bits(value4);
    print_bitset(bits);
    std::cout << "Conversion a double:\n";
    std::cout << "Por mi:  ";
    print_bitset(floatBits_to_DoubleBits(bits));
    std::cout << "Por cpp: ";
    print_array(get_bits((double)value4), 1);

    return 0;
}