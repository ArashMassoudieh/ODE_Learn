#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <Expression.h>

class System;

using namespace std;

class Object
{
    public:
        Object();
        virtual ~Object();
        Object(const Object& other);
        Object& operator=(const Object& other);
        string GetName() const {return name;}
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
            else if (tmg==Expression::timing::present)
                value_current = val;
            else if (tmg==Expression::timing::both)
            {
                value_past = val;
                value_current = val;
            }
        }
        void SetParent(System *system);
        System* Parent() {return parent;}
    protected:

    private:
        string name;
        double value_past;
        double value_current;
        System *parent;
};

#endif // OBJECT_H
