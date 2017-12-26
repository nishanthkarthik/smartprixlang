int test(int a, int b)
{
    return a + b;
}

int main()
{
    int c = 2;
    if (c > 0) {
        c = 5;
    } else {
        c = c + 10;
    }
    return 0;
}