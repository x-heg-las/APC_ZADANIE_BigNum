#pragma once

// here you can include whatever you want :)
#include <string>
#include <stdint.h>
#include <algorithm>
#include <sstream>
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
        
        if (num.front() == '-')
        {
            this->positive = false;
            num = num.substr(1);
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
                this->number.push_back(str.at(position) - '0');
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
        trimTrailingZero(this->number);
        negativeZeroCheck(*this);
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
        negativeZeroCheck(copy);
        return copy;
    }

    // binary arithmetics operators
    BigNum& operator+=(const BigNum& rhs)
    {
        std::pair<std::vector<uint8_t>, bool> result;
        std::cout << "Addition of : " << *this << " and " << rhs << " = ";
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
            BigNum unsignedCopy(*this);
            unsignedCopy.positive = true;

            result = subtract(rhs, unsignedCopy);
            this->positive = !result.second;
        }
        else if (this->positive && (!rhs.positive))
        {
            //right is negative
            BigNum unsignedCopy(*this);
            unsignedCopy.positive = true;

            result = subtract(unsignedCopy, rhs);
            this->positive = !result.second;
        }
        BigNum::trimTrailingZero(result.first);
       
        this->number = result.first;

        std::cout << *this << std::endl;
        return *this;
    }

    BigNum& operator-=(const BigNum& rhs)
    {
        std::pair<std::vector<uint8_t>, bool> result;
        std::cout << "Subtraction of : " << *this << " and " << rhs << " = ";
        if (this->positive && rhs.positive)
        {
            //both are positive
            result = subtract(*this, rhs);
            this->positive = !result.second;
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
            result = add(rhs, *this);
            this->positive = result.second;
        }
        else if (this->positive && (!rhs.positive))
        {
            //right is negative
            result = add(*this, rhs);
            this->positive = !result.second;
        }
        
        this->number = result.first;
        negativeZeroCheck(*this);
        std::cout << *this << std::endl;
        return *this;
    }

    BigNum& operator*=(const BigNum& rhs)
    {
        std::vector<uint8_t> result;
        uint32_t carry = 0;
        
        std::cout << "Multiplication of : " << *this << " and " << rhs << " = ";

        for (uint64_t multiplicand = 0; multiplicand < rhs.number.size(); multiplicand++)
        {
            for (uint64_t multiplicator = 0; multiplicator < this->number.size(); multiplicator++)
            {
                uint32_t digits = (this->number.at(multiplicator) * rhs.number.at(multiplicand)) + carry;
                if (multiplicand + multiplicator >= result.size())
                {
                    result.push_back(digits % 10);
                }
                else
                {
                    result.at(multiplicand + multiplicator) += digits % 10;
                }
                carry = digits / base;
                if (carry > 0)
                {
                    result.push_back(carry % 10);
                    carry = 0;
                }
            }
        }
        this->positive = this->positive && rhs.positive;
        if ((!this->positive) && (!rhs.positive))
        {
            this->positive = true;
        }

        for (uint64_t iter = 0; iter < result.size(); iter++)
        {
            if (result.at(iter) >= 10)
            {
                carry = result.at(iter) / 10;
                result.at(iter) = result.at(iter) % 10;
                if (iter + 1 < result.size())
                {
                    result.at(iter + 1) += static_cast<uint8_t>(carry);
                }
                else
                {
                    result.push_back(static_cast<uint8_t>(carry));
                }
                carry = 0;
            }
        }

        BigNum::trimTrailingZero(result);
        this->number = result;
        negativeZeroCheck(*this);
        std::cout << *this << std::endl;
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
    static void trimTrailingZero(std::vector<uint8_t>& data);
    friend void negativeZeroCheck(BigNum& member);
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

void negativeZeroCheck(BigNum& member)
{
    if (member.number.size() == 1 && member.number.front() == 0 && !member.positive)
    {
        member.positive = true;
    }
}

void BigNum::trimTrailingZero(std::vector<uint8_t>& data)
{
    int64_t cutPoint = -1;

    //skip first number
    for(uint64_t position = 1; position < data.size(); position++)
    {
        if (data.at(position) == 0 && (cutPoint == -1))
        {
            cutPoint = position;
        }
        else if(data.at(position) != 0)
        {
            cutPoint = -1;
        }
    }

    if (cutPoint != -1)
    {
        data.erase(data.begin() + cutPoint, data.end());
    }
}

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
    else if (lhs.number.size() > rhs.number.size() || (lhs.positive && !rhs.positive))
    {
        return false;
    }
    int64_t position = rhs.number.size() - 1;
    while (rhs.number.at(position) == lhs.number.at(position))
    {
        position--;
    }

    if (lhs.number.at(position) < rhs.number.at(position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator>(const BigNum& lhs, const BigNum& rhs)
{
    if ((lhs.positive && !rhs.positive) || lhs.number.size() > rhs.number.size())
    {
        return true;
    }
    else if (lhs.number.size() < rhs.number.size() || (!lhs.positive && rhs.positive))
    {
        return false;
    }

    int64_t position = lhs.number.size() - 1;

    while (lhs.number.at(position) == rhs.number.at(position))
    {
        position--;
    }
    
    if (lhs.number.at(position) > rhs.number.at(position))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator<=(const BigNum& lhs, const BigNum& rhs)
{
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const BigNum& lhs, const BigNum& rhs)
{
    return lhs > rhs || lhs == rhs;
}


std::ostream& operator<<(std::ostream& lhs, const BigNum& rhs)
{
    auto iter = rhs.number.rbegin();
    
    if (!rhs.positive)// && (rhs.number.front() != 0))
    {
        lhs << '-';
    }

    std::stringstream stream;
    while (iter != rhs.number.rend())
    {
        char number = (*iter + '0');
        stream << number;
        iter++;
    }
    lhs << stream.rdbuf();
    return lhs;
}


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
    BigNum::trimTrailingZero(result);
    return std::make_pair(result, reverse);
}

std::pair<std::vector<uint8_t>, bool> add(const BigNum& lhs, const BigNum& rhs)
{
    uint8_t carry = 0;
    std::vector<uint8_t> result;
    for (uint64_t position = 0; (position < lhs.number.size()) || (position < rhs.number.size()); position++)
    {
        int8_t partialSum = 0;
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
    if (carry > 0)
    {
        result.push_back(carry);
    }

    BigNum::trimTrailingZero(result);
    return std::make_pair(result, false);
}
