set a 5
set b 10
set c 20

label flag

if c < 100
    add c c 10
    echo c
    goto flag
endif
