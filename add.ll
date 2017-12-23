target triple = "x86_64-unknown-linux-gnu"

declare i32 @printf(i8*, ...)

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00"

define i32 @xadd (i32, i32) {
    %t = add i32 %0, %1
    ret i32 %t
}

define i32 @main () {
    %a = alloca i32
    %c = call i32 @xadd(i32 5, i32 10)
    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 7)
    ret i32 0
}