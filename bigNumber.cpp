#include "mainwindow.h"
#include <QString>
#include <boost/multiprecision/gmp.hpp>
#include <string>
#include <boost/exception/exception.hpp>
#include <stdexcept>

using namespace boost::multiprecision;

#define CreateMethod(name, opsign)                                                      \
QString name(const QString &prvi, const QString &drugi, unsigned int precision)         \
{                                                                                       \
    try {                                                                               \
        mpf_float a;                                                                    \
        mpf_float b;                                                                    \
        mpf_float res;                                                                  \
        res.precision(precision);                                                       \
        a.precision(prvi.size());                                                       \
        b.precision(drugi.size());                                                      \
        a.assign(prvi.toStdString());                                                   \
        b.assign(drugi.toStdString());                                                  \
        res = a opsign b;                                                               \
        return res.str(precision).c_str();                                              \
    }                                                                                   \
    catch (const boost::wrapexcept<std::runtime_error> &e) {                            \
        return e.what();                                                                \
    }                                                                                   \
}

CreateMethod(saberi_vb, +)
CreateMethod(oduzmi_vb, -)
CreateMethod(pomnozi_vb, *)
CreateMethod(podeli_vb, /)
