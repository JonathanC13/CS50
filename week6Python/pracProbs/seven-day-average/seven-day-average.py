import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    # print(decoded_content)
    # eg content:
    # 2023-03-23,Ohio,39,3415254,42061
    # 2023-03-23,Oklahoma,40,1295832,16549

    file = decoded_content.splitlines() # creates a list where each elem is seperated based on default 'new line'
    # for i in range(10):
    #    print(file[len(file) - i - 1])

    reader = csv.DictReader(file)
    # Create an object that operates like a regular reader but maps the information in each row to a dict whose keys are
    #   given by the optional fieldnames parameter.
    # The fieldnames parameter is a sequence. If fieldnames is omitted, the values in the first row of file f will be
    #   used as the fieldnames. Regardless of how the fieldnames are determined, the dictionary preserves their original ordering.
    # Therefore the field names are:
    #   date,state,fips,cases,deaths

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # list all states
    print("State list:")
    for key, value in new_cases.items():
        print(key)
    print()

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    numOfDays = 14
    new_cases = {}
    previous_cases = {}

    # loop all elems in list
    for i in reader:
        state = i["state"]
        cases = int(i["cases"])
        # if 'state' does not exist as a key in dict 'new_cases' add in as a key and set value to an empty list
        if state not in new_cases:
            new_cases.update({state: []})

        if state not in previous_cases:
            previous_cases.update({state: 0})

        # if number of cases already in the 'states' list is greater than 'numOfDays,' remove the first elem
        if len(new_cases[state]) >= numOfDays:
            new_cases[state].pop(0)

        # add the 'cases' number to the end of the 'states' value list
        new_cases[state].append(int(cases - previous_cases[state]))

        # save the previous day's cases due to the 'cases' in the raw data is cumulative
        previous_cases.update({state: int(cases)})

    # print(new_cases)
    # without pre_cases
    # {'Washington': ['1928913', '1928913', '1928913', '1928913', '1928913', '1936946', '1936946',
    #   '1936946', '1936946', '1936946', '1936946', '1936946', '1940704', '1940704'],
    # 'Illinois': ['4090437', '4090437', '4090437', '4090501', '4090501', '4090542', '4099372',
    #   '4099400', '4099400', '4099400', '4099448', '4099448', '4099448', '4107931']
    # with pre_cases
    # {'Washington': [0, 0, 0, 0, 0, 8033, 0, 0, 0, 0, 0, 0, 3758, 0],
    # 'Illinois': [39, 0, 0, 64, 0, 41, 8830, 28, 0, 0, 48, 0, 0, 8483]

    return new_cases


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    # print(states)
    for i in states:
        firstWeekAvg = float(sum(new_cases[i][:7:]) / 7)
        secondWeekAvg = float(sum(new_cases[i][7::]) / 7)

        # print(f"first: {firstWeekAvg}")
        # print(f"second: {secondWeekAvg}")

        try:
            relativeChangePerc = float(((secondWeekAvg - firstWeekAvg) / firstWeekAvg) * 100)
            # print(f"perc: {relativeChangePerc}")
        except ZeroDivisionError:
            print("Zero division")
        else:
            if (relativeChangePerc >= 0):
                print(f"{i} had a 7-day average of {secondWeekAvg:.0f} and an increase of {relativeChangePerc:.0f}%.")
            else:
                print(f"{i} had a 7-day average of {secondWeekAvg:.0f} and an decrease of {relativeChangePerc * -1 :.0f}%.")


main()
