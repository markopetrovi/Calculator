#include "mainwindow.h"
#include <QString>
#include <boost/multiprecision/gmp.hpp>
#include <string>
#include <boost/exception/exception.hpp>
#include <stdexcept>
#include <string>

using namespace boost::multiprecision;

unsigned int max(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int max;
    if (a > b)
        max = a;
    else
        max = b;
    if (c > max)
        max = c;
    return max;
}
unsigned int min(unsigned int a, unsigned int b)
{
    if (a < b)
        return a;
    return b;
}

std::string transform_string(std::string scientific_number, unsigned int precision)
{
    int pos = scientific_number.find('e');
    if (pos == -1)
        return scientific_number;
    int zero_pos = pos + 1;
    if (scientific_number[pos+1] == '-')
        zero_pos++;
    int zero_num = std::stoi(scientific_number.substr(zero_pos));
    scientific_number.erase(pos);   // Erase exponent
    scientific_number.erase(1, 1);  // Erase .

    if (zero_pos == pos+2) {
        std::string zeros(zero_num, '0');
        zeros.insert(1, ".");
        std::string result = zeros + scientific_number;
        result.resize(precision+1);
        return result;
    }
    else {
        if (scientific_number.size() >= precision) {
            scientific_number.resize(precision);
            return scientific_number;
        }
        std::string zeros(min(precision - scientific_number.size(), zero_num), '0');
        return scientific_number + zeros;
    }
}

#define CreateMethod(name, opsign)                                                              \
QString name(const QString &prvi, const QString &drugi, unsigned int precision)                 \
{                                                                                               \
    try {                                                                                       \
        mpf_float::default_precision(max(prvi.size()*3/2, drugi.size()*3/2, precision*3/2));    \
        mpf_float a;                                                                            \
        mpf_float b;                                                                            \
        mpf_float res;                                                                          \
        a.assign(prvi.toStdString());                                                           \
        b.assign(drugi.toStdString());                                                          \
        res = a opsign b;                                                                       \
        return transform_string(res.str(precision), precision).c_str();                         \
    }                                                                                           \
    catch (const boost::wrapexcept<std::runtime_error> &e) {                                    \
        return e.what();                                                                        \
    }                                                                                           \
}

CreateMethod(saberi_vb, +)
CreateMethod(oduzmi_vb, -)
CreateMethod(pomnozi_vb, *)
CreateMethod(podeli_vb, /)
