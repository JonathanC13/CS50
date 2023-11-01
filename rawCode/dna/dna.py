import csv
import sys


def main():

    # TODO: Check for command-line usage
    if (len(sys.argv) != 3):
        print("Usage: python dna.py [STR.csv] [seq.txt]")
        sys.exit(1)

    # TODO: Read database file into a variable
    # field names; name,AGATC,AATG,TATC
    list_people_dna = []
    try:
        with open(sys.argv[1], "r") as file:
            reader = csv.DictReader(file)
            for row in reader:
                list_people_dna.append(row)

            #get header fieldnames from DictReader and store in list
            headers = reader.fieldnames
    except Exception as error:
        print("Error handling csv: " + str(error))
        sys.exit(1)
    #print(list_people_dna)
    # TODO: Read DNA sequence file into a variable
    try:
        with open(sys.argv[2], "r") as file:
            dna_sequence = file.readline()
    except:
        print("Error handling txt")
        sys.exit(1)

    # TODO: Find longest match of each STR in DNA sequence
    dict_subseq_counts = {}
    for dna_STR in headers[1:]:
        dict_subseq_counts.update({dna_STR:int(longest_match(dna_sequence, dna_STR))})

    #print(dict_subseq_counts)

    # TODO: Check database for matching profiles
    match_count = 0
    match_goal = len(headers) - 1 # ignore 'name'
    curr_name = "No match"
    for row in list_people_dna:      # iterate each row of the DictReader
        match_count = 0
        for key, val in row.items():    # iterate each key:val pair in the row
            #print(key + " " + val)
            if key == "name":
                curr_name = val
                continue
            elif int(dict_subseq_counts[key]) == int(val):
                match_count += 1
            else:
                break   # if one doesn't match, don't bother continuing

        if (match_count != match_goal):
            curr_name = "No match"
        else:
            break

    print(f"{curr_name}")

    return 0


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

# python dna.py databases/large.csv sequences/5.txt
# python dna.py databases/large.csv sequences/19.txt
# check50 cs50/problems/2023/x/dna
# Results for cs50/problems/2023/x/dna generated by check50 v3.3.8
#:) dna.py exists
#:) correctly identifies sequences/1.txt
#:) correctly identifies sequences/2.txt
#:) correctly identifies sequences/3.txt
#:) correctly identifies sequences/4.txt
#:) correctly identifies sequences/5.txt
#:) correctly identifies sequences/6.txt
#:) correctly identifies sequences/7.txt
#:) correctly identifies sequences/8.txt
#:) correctly identifies sequences/9.txt
#:) correctly identifies sequences/10.txt
#:) correctly identifies sequences/11.txt
#:) correctly identifies sequences/12.txt
#:) correctly identifies sequences/13.txt
#:) correctly identifies sequences/14.txt
#:) correctly identifies sequences/15.txt
#:) correctly identifies sequences/16.txt
#:) correctly identifies sequences/17.txt
#:) correctly identifies sequences/18.txt
#:) correctly identifies sequences/19.txt
#:) correctly identifies sequences/20.txt