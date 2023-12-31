# TODO

import cs50
import re


def main():
    text = cs50.get_string("Text: ")

    # any sequence of characters separated by spaces should count as a word, and that any occurrence of a
    #   period, exclamation point, or question mark indicates the end of a sentence.
    regex_sentence = "[\.|\?|!]"    # match if end with [. or ? or !]
                                                    # [\.|\?|!](?=[\s|a-zA-Z]+)
                                                    # match is: [. or ? or !]
                                                    # followed by look ahead 'space' or letter char at least 1 time (+)
    list_sentences = re.split(regex_sentence, text)[:-1:] # split but remove that elem, which is a blank
    num_sentences = len(list_sentences)
                                    # Text: w. W?w!
                                    # ['w', ' W', 'w', '']
    #print(list_sentences)
    #print(num_sentences)

    regex_words = "[\s|,]+"   # match if at least 1 space
    list_words = []
    for sent in list_sentences:
        list_words += re.split(regex_words, sent.strip().strip(","))   # strip to remove leading and trailiing whitespace

    num_words = len(list_words)
    #print(list_words)
    #print(len(list_words))

    regex_letters = "[a-z|A-Z]+"    # You may assume that a letter is any lowercase character from
                                    # a to z or any uppercase character from A to Z,
    num_letters = 0
    #p = re.compile(regex_letters)
    for words in list_words:
        #m = p.match(words)
        #print(m.end())
        #num_letters += int(m.end())

        for letter in words:
            if letter.isalpha():
                num_letters += 1

    #print(count_letters)

    #print(num_sentences)
    #print(num_words)
    #print(num_letters)

    # Coleman-Liau formula
    L = (num_letters / num_words) * 100 # L is the average number of letters per 100 words in the text
    S = (num_sentences / num_words) * 100   # S is the average number of sentences per 100 words in the text.
    result = round(0.0588 * L - 0.296 * S - 15.8)
    #print(result)
    if result < 1:
        print("Before Grade 1")
    elif result > 16:
        print("Grade 16+")
    else:
        print(f"Grade {int(result)}")



main()
# check50 cs50/problems/2023/x/sentimental/readability

# Results for cs50/problems/2023/x/sentimental/readability generated by check50 v3.3.8
#:) readability.py exists.
#:) handles single sentence with multiple words
#:) handles punctuation within a single sentence
#:) handles more complex single sentence
#:) handles multiple sentences
#:) handles multiple more complex sentences
#:) handles longer passages
#:) handles questions in passage
#:) handles reading level before Grade 1
#:) handles reading level at Grade 16+