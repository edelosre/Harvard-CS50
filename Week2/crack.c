#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, string argv[])
{
    if (argc != 2 )
    {
        printf("Usage: ./crack hash");
        return 1;
    }
    
    //Get first two characters of encryption and store as hash
    char *ptr;
    char hash[3] = {argv[1][0], argv[1][1], '\0'}; 

    
    char password[6];
    int password_length = strlen(password);
    char *password_hash = crypt(password, hash);
    
    //Alphabet array organized by most common characters in english language (source: http://letterfrequency.org/)
    char *alphabet_array = "etaoinsrhldcumfpgwybvkxjqzETAOINSRHLDCUMFPGWYBVKXJQZ\0";

    //Begin timer
    clock_t begin = clock();
    
    //While hashes do not match and password length does not exceed 5
    while (strcmp(password_hash, argv[1]) != 0 && strlen(password) <= 5)
        
    {
      //Loop through all 1 char passwords
      for (int i = 0; i <= 51 && strcmp(password_hash, argv[1]) != 0; i++) 
      {
          password[password_length] = alphabet_array[i];
          password_hash = crypt(password, hash);
      }

      //Loop through all 2 char passwords
      if (strcmp(password_hash, argv[1]) != 0) 
      {
          for (int i = 0; i <= 51 && strcmp(password_hash, argv[1]) != 0; i++) 
          {
              password[password_length] = alphabet_array[i];
              password_hash = crypt(password, hash);
              
              for (int j = 0; j <= 51 && strcmp(password_hash, argv[1]) != 0; j++) 
              {
                  password[password_length + 1] = alphabet_array[j];
                  password_hash = crypt(password, hash);
              }               
          }
      }
        
      //Loop through all 3 char passwords
      if (strcmp(password_hash, argv[1]) != 0) 
      {
          for (int i = 0; i <= 51 && strcmp(password_hash, argv[1]) != 0; i++) 
          {
              password[password_length] = alphabet_array[i];
              password_hash = crypt(password, hash);
              
              for (int j = 0; j <= 51 && strcmp(password_hash, argv[1]) != 0; j++) 
              {
                  password[password_length + 1] = alphabet_array[j];
                  password_hash = crypt(password, hash);
                  
                  for (int k = 0; k <= 51 && strcmp(password_hash, argv[1]) != 0; k++) 
                  {
                      password[password_length + 2] = alphabet_array[k];
                      password_hash = crypt(password, hash);
                  } 
              }
          }
      }

      //Loop through all 4 char passwords
      if (strcmp(password_hash, argv[1]) != 0) 
      {
          for (int i = 0; i <= 51 && strcmp(password_hash, argv[1]) != 0; i++) 
          {
              password[password_length] = alphabet_array[i];
              password_hash = crypt(password, hash);
              
              for (int j = 0; j <= 51 && strcmp(password_hash, argv[1]) != 0; j++) 
              {
                  password[password_length + 1] = alphabet_array[j];
                  password_hash = crypt(password, hash);
                  
                  for (int k = 0; k <= 51 && strcmp(password_hash, argv[1]) != 0; k++) 
                  {
                      password[password_length + 2] = alphabet_array[k];
                      password_hash = crypt(password, hash);
                      
                      for (int l = 0; l <= 51 && strcmp(password_hash, argv[1]) != 0; l++) 
                      {
                          password[password_length + 3] = alphabet_array[l];
                          password_hash = crypt(password, hash);
                      }                       
                  } 
              }
          }
      }
        
      //Loop through all 5 char passwords
      if (strcmp(password_hash, argv[1]) != 0) 
      {
          for (int i = 0; i <= 51 && strcmp(password_hash, argv[1]) != 0; i++) 
          {
              password[password_length] = alphabet_array[i];
              password_hash = crypt(password, hash);
              
              for (int j = 0; j <= 51 && strcmp(password_hash, argv[1]) != 0; j++) 
              {
                  password[password_length + 1] = alphabet_array[j];
                  password_hash = crypt(password, hash);
                  
                  for (int k = 0; k <= 51 && strcmp(password_hash, argv[1]) != 0; k++) 
                  {
                      password[password_length + 2] = alphabet_array[k];
                      password_hash = crypt(password, hash);
                      
                      for (int l = 0; l <= 51 && strcmp(password_hash, argv[1]) != 0; l++) 
                      {
                          password[password_length + 3] = alphabet_array[l];
                          password_hash = crypt(password, hash);
                          
                          for (int m = 0; m <= 51 && strcmp(password_hash, argv[1]) != 0; m++) 
                          {
                              password[password_length + 4] = alphabet_array[m];
                              password_hash = crypt(password, hash);
                          }                           
                      }                       
                  } 
              }
          }
      }
        
        clock_t end = clock();
        double seconds = ((double)(end - begin)) / CLOCKS_PER_SEC;
        
        printf("Took %f seconds to crack password.\n", seconds);
        printf("%s\n", password);
        password_length++;
    }
    return 0;
}


