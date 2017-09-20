# Original program that prints a mario pyramid
# based on user inputted number. used as a te-
# -st writing session for new language Python.

import cs50

# prepare n
n = 0

# get an integer between 1 and 23 from user
while n < 1 or n > 23:
    
    print("Height: ", end="")
    n = cs50.get_int()
    
    if n > 0 and n < 24:
        break

# draw pyramid of size n
for i in range(0, n):
    
    for j in range(0, n - i - 1):
        print(" ", end="")
    
    for k in range(0, i + 1):
        print("#", end="")
    
    print("  ", end="")
    
    for l in range(0, i + 1):
        print("#", end="")
    
    print("")
