#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int contains_nonalpha(string word);
int shift (char letter);

int main(int argc, string argv[])
{
    
    //Exits if more than 2 arguments or cipher contains a number
    if (argc !=2 || contains_nonalpha(argv[1]) > 0)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    
    
    string plaintext = get_string("Enter your message: \n");
    
  
    //Create an array based on the amount of characters in original message
    char *ciphertext = (char *)malloc((strlen(plaintext) + 1) * sizeof(char));
    char cipher_letter;
       
    int cipher_loop = 0;
   
    //For each character in plaintext
    for (int i = 0; i < strlen(plaintext); i++)
    {

        if (cipher_loop > strlen(argv[1]) - 1)
        {
            cipher_loop = 0;
        }
        cipher_letter = shift(argv[1][cipher_loop]);
       
        //printf("cipher_loop is %i\n", cipher_loop);
        //printf("cipher_letter is %i\n", cipher_letter);
        cipher_loop++;

        
        //If cipher_letter is not within A-z then just equal to itself
        if (plaintext[i] < 65 || plaintext[i] > 122 ||
            (plaintext[i] > 90 && plaintext[i] < 97))
        {
            ciphertext[i] = plaintext[i];
            cipher_loop--;
            
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

int contains_nonalpha (string word)
{
    int nonalpha = 0;
    
    for(int i = 0; i < strlen(word); i++)
    {
        if (isdigit(word[i]) > 0) 
            nonalpha++; 
    }
    return nonalpha;
}

int shift (char letter)
{

    //A and a gives a value of 0
    letter = tolower(letter) - 0;
    letter = letter - 97;
    return letter;
}
