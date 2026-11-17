#ifndef STRING_H
#define STRING_H

class String
{
private:
    char* cadena;

public:
    String();
    // PRECONDICION:
    // 1- El puntero no es NULL y apunta a una cadena terminada en '\0'.
    String(const char*);
    String(const String&);
    ~String();
    String operator=(const String&);
    bool operator==(String);
    bool comienzaCon(String);
};

#endif
