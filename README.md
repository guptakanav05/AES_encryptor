# AES_encryptor
There are 4 main components of this project:

1.Vigenere Encryption

2.Vigenere Decryption

3.AES Encryption

4.AES Decryption

The user can implement any one of the above encryptions directly from the terminal for which he/shse needs to run all the .c files simultaneously and then follow the following command:

                    ./<exe name>  -e -c v -k KEY -s "STRING" or -f <filename.txt>


-e is for encryption  

-d for decryption.

-c is to indicate the cipher used v is for vigenere and aes for AES.

-k stands for key which the user would provide(THE KEY SHOULD ONLY CONTAIN LETTERS FOR vigenere and should be 16 byte long in case of AES).

-s is the string provided by the user that is to be encrypted or decrypted .

-f if the data is in a file .

For Command line inputs I used a new C command-agrv/agrc which enables us to take multiple iputs right from the CLI
I added several if else checks for the cases where the above syntactical requirement was not followed


**Vigenere cipher:**

The logic is simple:
take the inputs -- strings and key put them in arrays 
now we need to shift each letter of the string with the corresponding key in the key array.
If len(key)>=len(string)-->proceed normally else repeat the key 
placed checks for non alphabetic char in key.
In addition to string and key I added an argument direction which can take 2 values 1,-1 depending upon encryption or decryption.


**AES Encrytpion**

We start by defining a 1d array for the substitution box 

(simple rule arr[i][j]==arr[i*16+j])

also for Rcon vectors used in Key expansion 

Defined function for converting a byte to hex

Understood the steps(I didnt code this part on my own as the math was tricky although got a hang of the entre process of GF operations involved) for **key expansion** that is break the given 16 byte key into 4 words (4 byte each)--followed by rotation,substitution,xor with corresponding rcon and then finally using xor to find the next set of words.

Coming back to AES 

Firsly **Padding** is done (if number of bytes<16 then the remaining bytes are filled with the same number)
after that **ECB** that is dividing the text into 16 block states

4 major operations:


1.Substitution using the S box

2.shifting rows to increase interdependency(diffusion)

3.Mixing columns-->I didnt code this part on my own as the math was tricky although got a hang of the entre process of GF operations involved

4.XOR with Roundkey

The entire process is done 10 times
skipping mixingcol in the last round and at last the state is finally written into an array in hex format.



