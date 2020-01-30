#include <stdio.h>

void func(int nb, void (*f)(int))
{
    for (int i = 0; i < nb; i++) f(i);
}

int main()
{
    func(4, ({
        void callback(int v) { printf("%d\n", v); }
        callback;
    }));
}