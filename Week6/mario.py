from cs50 import get_int

pyramid_height = get_int("What should the height of the pyramid be?\n")

while pyramid_height > 8 or pyramid_height < 1:
    pyramid_height = get_int("What should the height of the pyramid be?\n")

pyramid_width = 1
padding = pyramid_height - 1


for i in range(pyramid_height):

    print(" " * padding, end = "")
    print("#" * pyramid_width, end = "")
    print("  ", end = "")
    print("#" * pyramid_width, end = "")
    print()

    pyramid_width += 1
    padding -= 1


