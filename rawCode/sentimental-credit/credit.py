# TODO
import re

def main():
    card_number = getCreditCard()

    print(getCreditCardNameRegex(card_number))

    # print(getCreditCardName(card_number))


def getCreditCard():
    while True:
        try:
            card_number = int(input("Number: "))
        except:
            print("Please enter an int.")
            continue
        else:
            break

    return card_number


def getCreditCardNameRegex(card_number):
    list_card_num = [int(x) for x in str(card_number)]
    # trying reg ex here
    str_card_number = str(card_number)
    list_card_regex = [
        {"Card":"AMEX", "regex":["^(34|37)[0-9]{13}"]},
            # ^(34| 37)[0-9]{15} start(^) with 34 or 37 and then trailing [0-9] for 13 chars
        {"Card":"MASTERCARD", "regex":["^(51|52|53|54|55)[0-9]{14}"]},
        {"Card":"VISA", "regex":["^4[0-9]{12}", "^4[0-9]{15}"]},
    ]
    #visa_regex1 = "^(51|52|53|54|55)[0-9]{2}"
    #p = re.compile(visa_regex1)
    #print(p.match(str_card_number))
    #print("Reg ex end")
    for i in list_card_regex:
        for j in i["regex"]:
            p = re.compile(j)
            m = p.match(str_card_number)
            if m is not None:
                if luhnAlgo(list_card_num):
                    return i["Card"]

    return "INVALID"



def getCreditCardName(card_number):
    list_card_validation = [
        {"Card":"AMEX", "Length":[15], "Initial_digits":[34, 37]},
        {"Card":"MASTERCARD", "Length":[16], "Initial_digits":[51, 52, 53, 54, 55]},
        {"Card":"VISA", "Length":[13, 16], "Initial_digits":[4]}
    ]

    list_card_num = [int(x) for x in str(card_number)]

    # for each card's dict in the list
    for i in list_card_validation:
        # check if the given card_number matches any for this specific card's length
        for j in i["Length"]:
            # given card matches a length
            if len(list_card_num) == j:
                # check if the given card matches the initial digits of this card
                for k in i["Initial_digits"]:
                    slice_obj = slice(len(str(k)))  # get the number of digits of an initial digits
                    first_digits = "".join(map(str, list_card_num[slice_obj]))  # get the same num of digits from the list and
                                                                                # join them into a string
                    if first_digits == str(k):      # compare the strings
                        if luhnAlgo(list_card_num): # final check is Lunh algo
                            return i["Card"]


    return "INVALID"


def luhnAlgo(list_card_num):
    list_card_num.reverse() # reverse the card number due to the every second digit starts from the end
    toggle_even = False     # toggle even (for multiply then get sum of product) or odd where simply add to total
    total = 0;
    for i in list_card_num:
        if toggle_even:
            # multiply the number by 2, store each digit (as int) of the result seperately in a list, then sum
            total += sum([int(x) for x in str(i * 2)])
        else:
            total += i

        toggle_even = not toggle_even

    if (total % 10 == 0):   # check that last digit value
        return True
    else:
        return False

main()

# https://developer.paypal.com/api/nvp-soap/payflow/integration-guide/test-transactions/#standard-test-cards
# visa: 4111111111111111
# master: 5555555555554444
# amex: 378282246310005

# https://docs.python.org/3/howto/regex.html#regex-howto

# check50 cs50/problems/2023/x/sentimental/credit
#Results for cs50/problems/2023/x/sentimental/credit generated by check50 v3.3.8
#:) credit.py exists.
#:) identifies 378282246310005 as AMEX
#:) identifies 371449635398431 as AMEX
#:) identifies 5555555555554444 as MASTERCARD
#:) identifies 5105105105105100 as MASTERCARD
#:) identifies 4111111111111111 as VISA
#:) identifies 4012888888881881 as VISA
#:) identifies 4222222222222 as VISA
#:) identifies 1234567890 as INVALID
#:) identifies 369421438430814 as INVALID
#:) identifies 4062901840 as INVALID
#:) identifies 5673598276138003 as INVALID
#:) identifies 4111111111111113 as INVALID
#:) identifies 4222222222223 as INVALID