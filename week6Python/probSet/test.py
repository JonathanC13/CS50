i = 5

list_test = [1, 2, 3, 8]
slice_obj = slice(i)
print(list_test[slice_obj])





res_list = []
total = 0
for i in list_test:
    total += sum([int(x) for x in str(i * 2)])

print(str(total))



from tkinter import *

window = tk()


food = ['pizza', 'burger', 'hotdog']

for i in food:
    radio_button = Radiobutton(window,
                            text = i)
    radio_button.pack()

window.mainloop()