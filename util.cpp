#include "util.h"

QString remove_yo(QString text)
{
    QString res = "";

    for(auto ch: text)
    {
        if(QString(ch) == "ё")
            res += "е";
        else res += ch;
    }
    return res;
}
