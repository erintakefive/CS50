# Final program used to ease into Python language.
# This was a real bugger to debug, but I learned 
# lots about how Python treats strings and lists,
# which are similar in structure to arrays, but 
# far, far more dynamic.

import cs50
import sys

# validate key
if len(sys.argv) != 2:
    print("usage: python vigenere.py k")
    exit(1)

# ensure key is alphabetical
if str.isalpha(sys.argv[1]) == False:
    print("usage: python vigenere.py k")
    exit(2)

# get plaintext from user
print("plaintext: ", end="")
plaintext = cs50.get_string()
print("ciphertext: ", end="")

# create alphabetical index for key
key = []
sys.argv[1] = str.lower(sys.argv[1])

for i in range(0, len(sys.argv[1])):
    key.append(ord(sys.argv[1][i]) - 97)

# create alphabetical index for plaintext, key
# tracking variable 'j' and ciphertext string
j = 0
ciphertext = []
alphatext = []

# encode plaintext and print new ciphertext
for i in range(0, len(plaintext)):
    
    if j % len(sys.argv[1]) == 0:
        j = 0
    
    if str.isupper(plaintext[i]) == True:
        alphatext.append(ord(plaintext[i]) - 65)
        ciphertext.append(chr(((alphatext[i] + key[j]) % 26) + 65))
        j += 1
        print("{}".format(ciphertext[i]), end="")
    
    if str.islower(plaintext[i]) == True:
        alphatext.append(ord(plaintext[i]) - 97)
        ciphertext.append(chr(((alphatext[i] + key[j]) % 26) + 97))
        j += 1
        print("{}".format(ciphertext[i]), end="")
    
    if str.isalpha(plaintext[i]) != True:
        alphatext.append(plaintext[i])
        ciphertext.append(plaintext[i])
        print("{}".format(ciphertext[i]), end="")

print("")
