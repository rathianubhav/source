# Source - a scripting language
Source is my hobby project to develop a simple yet efficient programming language with c like syntax and python like easiness

# To Build
- ## In Linux
    - ### Needed
        - g++
        - flex
        - bison
        - libr    (if releax os)
        - project (if releax os)

    - Execute make.sh
    - ``` $ bash make.sh ```
    - ``` $ ./build/sh ```


- ## In Windows
    - ### Needed
        - g++
        - flex
        - bison

    - i don't do coding here 😅


# To start

``` $ ./build/source ```

or

``` $ ./build/source test/01_hello.src ```


## Syntax

### assign
```
let a := 5;             /* integer   */
let b := 5.0;           /* float     */
let c := true           /* bool      */
let d := func arg { }   /* func      */
let e := cont { }       /* container */

```

### conditions
```

if a > 5 {
    println ("a is greater than 5");
} else {
    println ("a is smaller than 5");
}

```

### loops

```
for true {
    println("i am infinite");
    sleep(1);
}


for i in range 5 {
    println("i is ", 5);
}

```

### functions

```
let f := func a1, a2 {
    let x := 4;
    ret := a1 * a2 + 4;
}

let return_value := f(5, 2);


```

### container ( as the name suggest )
```
let c := cont {
    a : 10,
    b : 10.5,
    x : true,
    d : "hello World",
    
    e : func a1, a2 {
        ret := a1 + a2;
        }
};

println("c is ",c);

println("c -> a ", c->a);
println("c -> b ", c->b);
println("c -> x ", c->x);
println("c -> d ", c->d);
println("c -> e(2+5) ", c->e(2,5));
println("c -> f ", c->f);
println("a ", a);

```


### if execution for file then need to have a main function

```
let main := func args {

    /* do codding here */
}

```