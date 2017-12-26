int test(int a, int b)
{
    return a + b;
}

int main()
{
    test(5, 10);
    printf("%d\n", 7);
    int i = 0;
    while (i < 10)
        ++i;
flag:
    goto flag;
    return 0;
}