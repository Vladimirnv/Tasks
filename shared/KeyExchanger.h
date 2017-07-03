#include <mpirxx.h>
#include <iostream>
#include <time.h>

#define TSIZE 2048

class KeyExchanger
{
public:
    KeyExchanger()
    {
        mpz_init(_A);
        mpz_init(_a);
        mpz_init(_g);
        mpz_init(_p);
        mpz_init(_K);
        gmp_randinit_default(rand);
        gmp_randseed_ui(rand, time(NULL));
        mpz_urandomb(_a, rand, 333);
        _base = 10;
    }

    void First()
    {
        mpz_urandomb(_g, rand, 4);
        mpz_add_ui(_g, _g, 3);
        mpz_urandomb(_p, rand, 1024);
    }

    void SetG(char* g)
    {
        mpz_init_set_str(_g, g, _base);
    }

    void SetP(char* p)
    {
        mpz_init_set_str(_p, p, _base);
    }

    char* GetG()
    {
        char tmp[TSIZE] = "";
        mpz_get_str(tmp, _base, _g);
        return tmp;
    }

    char* GetP()
    {
        char tmp[2048] = "";
        mpz_get_str(tmp, _base, _p);
        return tmp;
    }

    void GenPP()
    {
        mpz_powm(_A, _g, _a, _p);
    }

    void GenK(char B[TSIZE])
    {
        mpz_init_set_str(_g, B, _base);
        mpz_powm(_K, _g, _a, _p);
    }

    char* GetA()
    {
        char tmp[TSIZE] = "";
        mpz_get_str(tmp, _base, _A);
        return tmp;
    }

    char* GetKey()
    {
        char tmp[TSIZE] = "";
        mpz_get_str(tmp, _base, _K);
        return tmp;
    }

private:
    mpz_t _A;
    mpz_t _a;
    mpz_t _g;
    mpz_t _p;
    mpz_t _K;
    gmp_randstate_t rand;
    int _base;
};