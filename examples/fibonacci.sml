set a 0
set b 1
set c 0
set n 1
label loopstart
if n < 5
echo a
echo b
add c a b
set a b
set b c
add n n 1
goto loopstart
endif