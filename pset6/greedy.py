# Program used, again, for practicing Python
# syntax and conventions.

import cs50

# prepare coins & other variables
q, d, n, p = 25, 10, 5, 1
qn, dn, nn, pn = 0, 0, 0, 0
money = 0.0

# prompt user for input
while money <= 0:
    
    print("O hai! How much change is owed?")
    money = cs50.get_float()
    
    if money > 0:
        break

# now work out amount of coins owed
money = int(money * 100)

while money >= q:
    money -= q
    qn += 1

while money >= d:
    money -= d
    dn += 1

while money >= n:
    money -= n
    nn += 1

while money >= p:
    money -= p
    pn += 1

print("Coins used: {}".format(qn + dn + nn + pn))
