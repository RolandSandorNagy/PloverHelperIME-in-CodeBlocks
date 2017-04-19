#include "Suggestion.h"

Suggestion::Suggestion(int m, std::wstring wt)
{
    multiplicity = m;
    wtext = wt;
}

Suggestion::Suggestion()
{

}

Suggestion::~Suggestion()
{

}

int Suggestion::getMultiplicity()
{
    return multiplicity;
}

std::wstring Suggestion::getWText()
{
    return wtext;
}

void Suggestion::setMultiplicity(int m)
{
    multiplicity = m;
}

void Suggestion::setWText(std::wstring wt)
{
    wtext = wt;
}

