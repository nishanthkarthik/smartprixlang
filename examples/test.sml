set a 5
set b 10
set c 0

; test all combinations
add c 5 5
echo c

add c b 1
echo c

add c 1 b
echo c

add c a b
echo c

; test all ops
sub c b a
echo c

mul c b a
echo c

div c b a
echo c

; test if
set c 15
if c = 15
    echo a
endif

; test loop
;set c 0
;label flag
;if c < 5
;    echo c
;    add c 1 c
;    goto flag
;endif

exit