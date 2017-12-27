int test(int a, int b)
{
    return a + b;
}

int main()
{
    int c = 2;
    if (c > 0) {
        test(c, c);
        c = 5;
        test(c, c);
    } else {
        test(c, c);
        c = c + 10;
        test(c, c);
    }
    return 0;
}