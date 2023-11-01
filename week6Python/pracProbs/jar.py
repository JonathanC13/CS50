import sys

class Jar:
    def __init__(self, capacity=12):
        self._size = 0

        while(True):
            try:
                jarCap = int(input("Max number of cookies in the jar? "))

                if (jarCap < 0):
                    raise ValueError
            except ValueError:
                print("Invalid input, please enter a positive int.")
                continue
            else:
                break

        # print(f"jarCap: {jarCap}")
        self._capacity = jarCap

    def __str__(self):
        return (" o" * self._size)

    def deposit(self, n):
        try:
            if (self._size + n > self._capacity):
                raise ValueError
        except ValueError:
            print("Deposit would over flow cookie jar!")
        else:
            self._size += n

    def withdraw(self, n):
        try:
            if (self._size - n < 0):
                raise ValueError
        except ValueError:
            print("Not enough cookies in the jar to withdraw that amount!")
        else:
            self._size -= n

    @property
    def capacity(self):
        return self._capacity

    @property
    def size(self):
        return self._size

def main():
    jar = Jar()

    print(str(jar.capacity))

    print(str(jar))

    jar.deposit(2)
    print(str(jar))

    jar.deposit(10)
    print(str(jar))


    jar.withdraw(1)
    print(str(jar))

    jar.withdraw(20)
    print(str(jar))


main()