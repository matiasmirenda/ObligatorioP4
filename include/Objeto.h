#ifndef OBJETO_H
#define OBJETO_H

class Objeto
{
public:
    virtual ~Objeto();
    virtual long int getId() = 0;
};

#endif
