#include "mainwindow.h"
#include <QString>
#include <regex>

int singleDigitToArabic(const QChar &RomanDigit)
{
    if (RomanDigit == 'I')
        return 1;
    if (RomanDigit == 'V')
        return 5;
    if (RomanDigit == 'X')
        return 10;
    if (RomanDigit == 'L')
        return 50;
    if (RomanDigit == 'C')
        return 100;
    if (RomanDigit == 'D')
        return 500;
    if (RomanDigit == 'M')
        return 1000;
    return -1;
}

bool isValidRoman(const QString &RomanNumber)
{
    const std::regex pattern("^M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$");
    if (RomanNumber.isEmpty())
        return false;
    return std::regex_match(RomanNumber.toStdString(), pattern);
}

int toArabic(const QString &RomanNumber)
{
    if (!isValidRoman(RomanNumber))
        return -1;

    int number = 0, prev = 1000;
    for (const QChar &ch : RomanNumber) {
        int digit = singleDigitToArabic(ch);
        if (digit > prev)
            number += digit - (2*prev);
        else
            number += digit;
        prev = digit;
    }
    return number;
}
// 1404 = MCDIV

QString toRoman(int ArabicNumber)
{
    QString RomanNumber = "";
    if (ArabicNumber > 3999)
        return QStringLiteral("Broj je <b>preveliki</b> da bi se zapisao rimskim ciframa");
    if (ArabicNumber < 1)
        return QStringLiteral("Broj je <b>premali</b> da bi se zapisao rimskim ciframa");

    while (ArabicNumber >= 1000) {
        RomanNumber += "M";
        ArabicNumber -= 1000;
    }
    if (ArabicNumber >= 900) {
        RomanNumber += "CM";
        ArabicNumber -= 900;
    }
    if (ArabicNumber >= 500) {
        RomanNumber += "D";
        ArabicNumber -= 500;
    }
    if (ArabicNumber >= 400) {
        RomanNumber += "CD";
        ArabicNumber -= 400;
    }
    while (ArabicNumber >= 100) {
        RomanNumber += "C";
        ArabicNumber -= 100;
    }
    if (ArabicNumber >= 90) {
        RomanNumber += "XC";
        ArabicNumber -= 90;
    }
    if (ArabicNumber >= 50) {
        RomanNumber += "L";
        ArabicNumber -= 50;
    }
    if (ArabicNumber >= 40) {
        RomanNumber += "XL";
        ArabicNumber -= 40;
    }
    while (ArabicNumber >= 10) {
        RomanNumber += "X";
        ArabicNumber -= 10;
    }
    if (ArabicNumber >= 9) {
        RomanNumber += "IX";
        ArabicNumber -= 9;
    }
    if (ArabicNumber >= 5) {
        RomanNumber += "V";
        ArabicNumber -= 5;
    }
    if (ArabicNumber >= 4) {
        RomanNumber += "IV";
        ArabicNumber -= 4;
    }
    while (ArabicNumber >= 1) {
        RomanNumber += "I";
        ArabicNumber--;
    }

    return RomanNumber;
}
