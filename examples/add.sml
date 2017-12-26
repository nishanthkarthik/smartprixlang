set a 5
set b 10
set c 0

; test 4 combinations
add c 5 5
echo c

add c b 1
echo c

add c 1 b
echo c

add c a b
echo c

; test 4 ops
sub c b a
echo c

mul c b a
echo c

div c b a
echo c

exit