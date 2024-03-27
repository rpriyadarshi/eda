syms a b x y

c   = sym('sqrt(a^2+b^2)')

txy = [1 0 0; 0 1 0; x y 1]
mx  = [-1 0 0; 0 1 0; 0 0 1]
my  = [1 0 0; 0 -1 0; 0 0 1]
rab = [a/c b/c 0; -b/c a/c 0; 0 0 1]

a =  0; b =  0;
a =  0; b =  1;
a =  1; b =  0;
a =  1; b =  1;

a =  0; b = -1;
a = -1; b =  0;
a = -1; b = -1;

a =  1; b = -1;
a = -1; b =  1;

a = 0.23; b = 29; x = 36; y = -50;
r = txy*my*rab*mx
eval(r)

