# Andersen flow-insensitive pointer analysis
Andersen Analysis covering four basic statements

p=&a

p=q   (copy)

*p=q  (store)

p=*q  (load)


Compile & Run

g++ Andersen.cpp

./a.out sample.txt x.dot

