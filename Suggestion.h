#ifndef SUGGESTION_H_INCLUDED
#define SUGGESTION_H_INCLUDED

#include "includes.h"


class Suggestion
{

public:  /* members */


private: /* members */
    int multiplicity;
    std::wstring wtext;


public:  /* methods */
    Suggestion(int, std::wstring);
    Suggestion();
    ~Suggestion();
    int getMultiplicity();
    std::wstring getWText();
    void setMultiplicity(int);
    void setWText(std::wstring);


private: /* methods */

};

struct suggestion_compare_operator
{
    inline bool operator() (Suggestion& sug1, Suggestion& sug2)
    {
        return (sug1.getMultiplicity() < sug2.getMultiplicity());
    }
};


#endif // SUGGESTION_H_INCLUDED
