from cs50 import get_float

value = get_float("Change owed: ")

while value < 0:
    value = get_float("Change owed: ")

remaining_value = 0
num_quarters = value // 0.25
num_dimes = 0
num_nickels = 0
num_pennies = 0

if value - (num_quarters * 0.25) > 0:
    remaining_value = round(value - (num_quarters * 0.25), 2)
    num_dimes = remaining_value // 0.10

if remaining_value > 0:
    remaining_value = round(remaining_value - (num_dimes * 0.10), 2)
    num_nickels = remaining_value // 0.05

if remaining_value > 0:
    remaining_value = round(remaining_value - (num_nickels * 0.05), 2)
    num_pennies = remaining_value // 0.01


print(num_quarters + num_dimes + num_nickels + num_pennies)
#print(num_quarters)
#print(num_dimes)
#print(num_nickels)
#rint(num_pennies)