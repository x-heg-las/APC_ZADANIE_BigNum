#pragma once

// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <algorithm>
#include <exception>
#include <utility>
#include <vector>
#include <ranges>
#include <limits>

#define SUPPORT_DIVISION 0 // define as 1 when you have implemented the division
#define SUPPORT_IFSTREAM 0 // define as 1 when you have implemented the input >>

// if you do not plan to implement bonus, just delete those lines
class BigNum final
{
public:
    // constructors
    BigNum()
    {
        this->number.push_back(0);
    }

    BigNum(int64_t n)
    {
        std::string num = std::to_string(n);
        if (n < 0)
        {
            this->positive = false;
        }
        
        while (!num.empty())
        {
            this->number.push_back(num.back() - '0');
            num.pop_back();
        }
    }

    explicit BigNum(const std::string& str)
    {
        uint64_t position = 0;
        bool skipFrontZero = true;

        if (str.front() == '-')
        {
            this->positive = false;
            position = 1;
        }
        else if (str.front() == '+')
        {
            position = 1;
        }

        for (; position < str.size(); position++)
        {
            if (str.at(position) == '0' && skipFrontZero)
            {
                continue;
            }

            skipFrontZero = false;
            if (std::isdigit(str.at(position)))
            {
                this->number.push_back(str.at(position) - '0');// skontroluj implicitnu konveziu 
            }
            else
            {
                throw std::exception("Wrong number format");
            }
        }
        std::ranges::reverse(this->number);
        if (str.size() == 0)
        {
            number.push_back(0);
        }

        //std::string numberString = str;
        //size_t substringLength = std::to_string(base).size();
        //std::string resultString;
        //auto substr = numberString.substr(0, substringLength);
        //numberString = numberString.substr(substringLength);
        //std::ranges::reverse(numberString);

        //try
        //{
        //    while (!numberString.empty())
        //    {
        //        uint64_t partialResult = std::stoull(substr);
        //        uint64_t result = partialResult >> 30;

        //        if (!resultString.empty() || (result != 0))
        //        {
        //            resultString.append(std::to_string(result));
        //        }
        //        uint64_t remainder = partialResult - (result * base); // skontroluj overflow 
        //        substr = std::to_string(remainder);
        //        substr.push_back(numberString.back());
        //        numberString.pop_back();
        //    }

        //    uint64_t partialResult = std::stoull(substr);
        //    uint64_t result = partialResult >> 30;
        //    resultString.append(std::to_string(result));
        //    uint64_t remainder = partialResult - (result * base);

        //    std::cout << resultString << std::endl;
        //    std::cout << numberString;
        //}
        //catch (std::out_of_range& a)
        //{
        //    std::cerr << a.what() << std::endl;

        //}
        //catch (std::invalid_argument)
        //{
        //    std::cerr << "Invalid argument" << std::endl;
        //    throw std::exception("Wrong number format");
        //}
        
    }

    // copy
    BigNum(const BigNum& other) = default;
    BigNum& operator=(const BigNum& rhs)
    {
        this->positive = rhs.positive;
        this->number = rhs.number;
    }

    // unary operators
    const BigNum& operator+() const
    {
        return *this;
    }

    BigNum operator-() const
    {
        BigNum copy(*this);
        copy.positive = !this->positive;
        return copy;
    }

    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs)
    {
        std::pair<std::vector<uint8_t>, bool> result;

        if (this->positive && rhs.positive)
        {
            //both are positive
            result = add(*this, rhs);
        }
        else if ((!this->positive) && (!rhs.positive))
        {
            //both are negative
            result = add(*this, rhs);
            this->positive = false;
        }
        else if((!this->positive) && rhs.positive)
        {
            //left is negative
            result = subtract(rhs, *this);
            this->positive = !result.second;
        }
        else if (this->positive && (!rhs.positive))
        {
            //right is negative
            result = subtract(*this, rhs);
            this->positive = !result.second;
        }
        this->number = result.first;
        return *this;
    }

    BigNum& operator-=(const BigNum& rhs)
    {
        std::pair<std::vector<uint8_t>, bool> result;

        if (this->positive && rhs.positive)
        {
            //both are positive
            result = subtract(*this, rhs);
        }
        else if ((!this->positive) && (!rhs.positive))
        {
            //both are negative
            result = subtract(*this, rhs);
            this->positive = result.second;
        }
        else if ((!this->positive) && rhs.positive)
        {
            //left is negative
            result = subtract(rhs, *this);
            this->positive = !result.second;
        }
        else if (this->positive && (!rhs.positive))
        {
            //right is negative
            result = subtract(*this, rhs);
            this->positive = !result.second;
        }

        this->number = result.first;
        return *this;
    }

    BigNum& operator*=(const BigNum& rhs)
    {
        std::vector<uint8_t> result;
        uint32_t carry = 0;
        uint64_t multiplicator = 0;

        for (uint64_t multiplicand = 0; multiplicand < rhs.number.size(); multiplicand++)
        {
            for (; multiplicator < this->number.size(); multiplicator++)
            {
                uint32_t digits = (this->number.at(multiplicator) * rhs.number.at(multiplicand)) + carry;
                if (multiplicand + multiplicator >= result.size())
                {
                    result.push_back(digits % 10);
                }
                else
                {
                    result.at(multiplicand + multiplicator) +=  digits % 10;
                }
                carry = digits / base;
            }
            multiplicator = 0;
        }
        this->positive = this->positive && rhs.positive;
        if ((!this->positive) && (!rhs.positive))
        {
            this->positive = true;
        }
        this->number = result;
        return *this;
    }

#if SUPPORT_DIVISION == 1
    BigNum& operator/=(const BigNum& rhs); // bonus
    BigNum& operator%=(const BigNum& rhs); // bonus
#endif

private:

    bool positive = true;
    const uint32_t base = 10;
    std::vector<uint8_t> number;

    //functions//
    friend std::pair<std::vector<uint8_t>, bool> subtract(const BigNum& lhs, const BigNum& rhs);
    friend std::pair<std::vector<uint8_t>, bool> add(const BigNum& lhs, const BigNum& rhs);
    friend std::vector<uint8_t> multiply(const BigNum& lhs, const BigNum& rhs);
    friend bool operator<(const BigNum& lhs, const BigNum& rhs);
    friend bool operator>(const BigNum& lhs, const BigNum& rhs);
    friend std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);
    friend bool operator==(const BigNum& lhs, const BigNum& rhs);

    bool isPositive()
    {
        return this->positive;
    }

    std::vector<uint8_t> getNumber()
    {
        return this->number;
    }
};

BigNum operator+(BigNum lhs, const BigNum& rhs)
{
    return lhs += rhs;
}

BigNum operator-(BigNum lhs, const BigNum& rhs)
{
    return lhs -= rhs;
}
BigNum operator*(BigNum lhs, const BigNum& rhs)
{
    return lhs *= rhs;
}

#if SUPPORT_DIVISION == 1
BigNum operator/(BigNum lhs, const BigNum& rhs); // bonus
BigNum operator%(BigNum lhs, const BigNum& rhs); // bonus
#endif

bool operator==(const BigNum& lhs, const BigNum& rhs)
{
    return ((lhs.number == rhs.number) && (lhs.positive == rhs.positive));
}

bool operator!=(const BigNum& lhs, const BigNum& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const BigNum& lhs, const BigNum& rhs)
{
    if ((!lhs.positive && rhs.positive) || lhs.number.size() < rhs.number.size())
    {
        return true;
    }
    else if (lhs.number.size() > rhs.number.size())
    {
        return false;
    }

    for (int64_t position = rhs.number.size() - 1; 0 <= position; position--)
    {
        if (lhs.number.at(position) < rhs.number.at(position))
        {
            return true;
        }
    }
    return false;
}

bool operator>(const BigNum& lhs, const BigNum& rhs)
{
    if ((lhs.positive && !rhs.positive) || lhs.number.size() > rhs.number.size())
    {
        return true;
    }
    else if (lhs.number.size() < rhs.number.size())
    {
        return false;
    }

    for (int64_t position = lhs.number.size() - 1; 0 <= position; position--)
    {
        if (lhs.number.at(position) > rhs.number.at(position))
        {
            return true;
        }
    }
    return false;
}

bool operator<=(const BigNum& lhs, const BigNum& rhs)
{
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const BigNum& lhs, const BigNum& rhs)
{
    return lhs > rhs || lhs == rhs;
}


std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs);


#if SUPPORT_IFSTREAM == 1
std::istream& operator>>(std::istream& lhs, BigNum& rhs); // bonus
#endif



std::pair<std::vector<uint8_t>, bool> subtract(const BigNum& lhs, const BigNum& rhs)
{
    std::vector<uint8_t> result;
    int8_t subresult = 0;
    uint8_t carry = 0;
    bool reverse = lhs < rhs;

    for (uint64_t position = 0; position < lhs.number.size() || position < rhs.number.size(); position++)
    {
        uint8_t lhsNum = 0, rhsNum = 0;
        if (position < rhs.number.size())
        {
            rhsNum = rhs.number.at(position);
        }
        if (position < lhs.number.size())
        {
            lhsNum = lhs.number.at(position);
        }

        if (reverse)
        {
            subresult = rhsNum - (lhsNum + carry);
        }
        else
        {
            subresult = lhsNum - (rhsNum + carry);
        }

        carry = 0;
        if (subresult < 0)
        {
            result.push_back(10 + subresult);
            carry = 1;
        }
        else
        {
            result.push_back(subresult);
        }
    }
    return std::make_pair(result, reverse);
}

std::pair<std::vector<uint8_t>, bool> add(const BigNum& lhs, const BigNum& rhs)
{
    uint8_t carry = 0;
    std::vector<uint8_t> result;
    for (uint64_t position = 0; (position < lhs.number.size()) || (position < rhs.number.size()); position++)
    {
        uint8_t partialSum = 0;
        if (position < lhs.number.size())
        {
            partialSum += lhs.number.at(position);
        }
        if (position < rhs.number.size())
        {
            partialSum += rhs.number.at(position);
        }
        partialSum += carry;
        if (partialSum >= 10)
        {
            carry = 1;
            partialSum -= 10;
        }
        else
        {
            carry = 0;
        }

        result.push_back(partialSum);
        partialSum = 0;
        
    }
    return std::make_pair(result, false);
}
