#include <stdio.h>
#include <cs50.h>
#include <math.h>

void valid_credit_card_checker (long creditcard_number);
int checksum (long number);

int main (void)
{
    long creditcard_number = get_long("Number: ");
    valid_credit_card_checker(creditcard_number);
    
}

//Function for checking if credit card is valid
void valid_credit_card_checker (long creditcard_number)
{
    //Gets length of digits in number
    long num_digits = floor(log10(labs(creditcard_number))) + 1;
    
    //First check if card has valid checksum
    if (checksum(creditcard_number) % 10 != 0) {
        printf("INVALID\n");
    }
    //Amex cards are 15 digits, and only start with 34 or 37
    else if (num_digits == 15 && ((creditcard_number / pow(10,13) >= 34 && creditcard_number / pow(10,13) < 35)
                             || (creditcard_number / pow(10,13) >= 37 && creditcard_number / pow(10,13) < 38))) {
        printf("AMEX\n");
    }
    //MasterCard are 16 digits and start with 51,52,53,54, or 55
    else if (num_digits == 16 && (creditcard_number/ pow(10,14)>= 51
                                  && creditcard_number / pow(10,14) < 56))
    {
        printf("MASTERCARD\n");
    }
    //Visa are 13 or 16 digits and only start with 4
    else if ((num_digits == 13 && creditcard_number/ pow(10,12) >= 4 && creditcard_number / pow(10,12) < 5) || (num_digits == 16 
                                                                                                               && creditcard_number/ pow(10,15) >= 4
                                                                                                               && creditcard_number/ pow(10,15) < 5))
    {
        printf("VISA\n");
    }
    //Case for when the card has a valid checksum, but is not an AMEX, MasterCard, or Visa
    else {
        printf("INVALID\n");
    }
}

//Function for getting checksum
int checksum (long number)
{
    int i = 1;
    int checksum = 0;
    int extracted_number;
    int second_extracted_number = 0; 
    
    //Loop through all digits
    while(number)
    {
        //Store each digit into variable
        extracted_number = number % 10;
        number = number / 10;

        //Every other digit is multiplied by 2
        if (i % 2 == 0) {
            extracted_number *= 2;
            //In the case that 5 and above get multiplied by 2, we need to parse it again
            if (extracted_number >= 10) {
                second_extracted_number = extracted_number % 10;
                extracted_number =  extracted_number / 10;
            }
        }

        checksum = checksum + extracted_number + second_extracted_number;
        second_extracted_number = 0;
        
        //printf("Checksum is: %i\n", checksum);
        i++;
        
    }
    return checksum;
}


