#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    
    //Program must be ran with 2 arguments
    if (argc !=2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    
    //Convert argument to int
    char *ptr;
    int cipher_key = strtol(argv[1], &ptr, 10);
    
    string plaintext = get_string("Enter your message: \n");
    
    //Create an array based on the amount of characters in original message
    char ciphertext [strlen(plaintext)];
    char cipher_letter;
    
    //For each character in plaintext
    for(int i = 0; i < strlen(plaintext); i++)
    {
        //printf("%i\n", plaintext[i]);
        cipher_letter = cipher_key % 26;
        
        //If cipher_letter is not within A-z then just equal to itself
        if (plaintext[i] < 65 || plaintext[i] > 122 ||
            (plaintext[i] > 90 && plaintext[i] < 97))
        {
            ciphertext[i] = plaintext[i];
        }
        //If its a lowercase
        else if (islower(plaintext[i]))
        {
            //If overflows outside of lowercase bounds then subtract 26 to rotate
            ciphertext[i] = (plaintext[i] + cipher_letter) > 122 ?
                (plaintext[i] + cipher_letter) - 26 : (plaintext[i] + cipher_letter);
        }
        //If its uppercase
        else
        {
            //If overflows outside of uppercase bounds then subtract 26 to rotate
            ciphertext[i] = (plaintext[i] + cipher_letter) > 90 ?
                (plaintext[i] + cipher_letter) - 26 : (plaintext[i] + cipher_letter);
        }

       
    }

     printf("ciphertext: %s\n", ciphertext);
}
