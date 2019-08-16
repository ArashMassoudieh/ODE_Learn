#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <Expression.h>


using namespace std;

class Object
{
    public:
        Object();
        virtual ~Object();
        Object(const Object& other);
        Object& operator=(const Object& other);
        string GetName() {return name;}
        void SetName(const string &_name) {name = _name; }
        double GetValue(const Expression::timing &tmg=Expression::timing::present)
        {
            if (tmg==Expression::timing::past)
                return value_past;
            else
                return value_current;
        }
        void SetValue(const double &val, const Expression::timing &tmg=Expression::timing::present)
        {
            if (tmg==Expression::timing::past)
                value_past = val;
            else
                value_current = val;
        }
    protected:

    private:
        string name;
        double value_past;
        double value_current;
};

#endif // OBJECT_H
