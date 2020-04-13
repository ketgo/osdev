/**
 * Test to assert packed classes in GCC
 */

#include <assert.h>

struct UnPackedStruct
{
    unsigned char a;
    int b;
};

struct __attribute__((__packed__)) PackedStruct
{
    unsigned char a;
    int b;
};

class __attribute__((packed)) PackedClass
{
private:
    unsigned char a;
    int b;

public:
    void set_a(unsigned char uc) { a = uc; }
    unsigned char get_a() { return a; }
    void set_b(int i) { b = i; }
    int get_b() { return b; }
};

int main()
{
    PackedClass pc;
    pc.set_a('a');
    pc.set_b(10);

    assert(sizeof(UnPackedStruct) == 8);
    assert(sizeof(PackedStruct) == 5);
    assert(sizeof(PackedClass) == 5);
    assert(sizeof(pc) == 5);

    return 0;
}