// Become familiar wih C syntax
// Learn to debug buggy code

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string name, location;
    // Ask for your name and where live
    name = get_string("What is your name? ");
    location = get_string("Where do you live? ");

    // Say hello
    printf("Hello, %s, from %s!\n", name, location);
}

/*
You can check your code using check50, a program that CS50 will use to test your code when you submit,
by typing in the following at the $ prompt. But be sure to test it yourself as well!

check50 cs50/labs/2023/x/debug
Green smilies mean your program has passed a test! Red frownies will indicate your program output something unexpected.
Visit the URL that check50 outputs to see the input check50 handed to your program, what output it expected, and what output
your program actually gave.

--

To evaluate that the style of your code (indentations and spacing) is correct, type in the following at the $ prompt.

style50 debug.c
*/