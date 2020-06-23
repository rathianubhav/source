; ModuleID = 'my cool jit'
source_filename = "my cool jit"

declare i32 @putch(i64)

define i64 @main(i32 %0) {
entry:
  %calltmp = call i32 @putch(i64 5)
  ret i64 5
}

