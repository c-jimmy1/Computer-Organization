ORG 100              / Multiplication by addition
    Load    B            / Load the multiplier into AC
    Store     Ctr        / Store the number of loop iterations in loop control variable
Loop, Load    Prod       / Load the product into AC
    Add       A          / Add the multiplicand
    Store     Prod       / Store the new product
    Load      Ctr        / Load the loop control variable
    Add       Neg1       / Decrement the loop control variable by one
    Store     Ctr        / Store the new value of the loop control variable
    Skipcond  400        / If control variable = 0, skip next instruction to terminate the loop
    Jump      Loop       / Otherwise, go to Loop
    Halt                 / Terminate program
A,    Dec     5          / The multiplicand
B,    Dec     7          / The multiplier
Prod, Dec     0          / The product
Ctr,  Hex     0          / The loop control variable
Neg1, Dec     -1         / Used to increment and decrement by 1
