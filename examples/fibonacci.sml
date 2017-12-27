set a 0
set b 1
set c 0
set n 1
echo a
echo b
label loopstart
if n < 20
add c a b
echo c
set a b
set b c
add n n 1
goto loopstart
endif