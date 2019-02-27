from cs50 import get_string
from sys import argv


def main():

    if (len(argv) != 2):
        print("Usage: python bleep.py dictionary")
        exit(1)

    with open(argv[1], 'r') as textfile:
        dictionary = textfile.read()
    print(dictionary)

    message = get_string("What message would you like to censor?\n")
    new_message = ""

    for word in message.split():
        if word.lower() in dictionary:
            word = len(word) * "*"

        if (len(new_message.split()) == 0):
            new_message = word
        else:
            new_message = new_message + " " + word

    print(new_message)


if __name__ == "__main__":
    main()
