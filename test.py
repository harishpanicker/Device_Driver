
m tkinter import *
import tkinter as tk
from tkinter import messagebox
import subprocess

def connect_device():
    the_Checkbutton1 = Radiobutton(root, text="Not Connected", value=0, variable=0)
    the_Checkbutton1.grid(column=4, row=1)

    status = subprocess.call("adb wait-for-device && adb root && adb remount",timeout=10, shell=True)
    print(status)
    if(status == 0):
         the_Checkbutton1 =  Radiobutton(root, text="Connected", value=1, variable=1)
         the_Checkbutton1.grid(column=4, row=1)


def save_log_info():
    if (var5.get() == 1):
        print("var log", var5.get())

    else:
        print("var log", var5.get())
    if(var6.get()==1):
        print("adb log",var6.get())
        subprocess.call("start cmd.exe /c \"adb logcat >/tmp/adb.txt\"", shell=True)
    else:
        print("adb log",var6.get())

    if(var7.get()==1):
        print("dmesg log",var7.get())
        subprocess.call("start cmd.exe /c \"dmesg >/tmp/dmesg.txt\"", shell=True)
    else:
        print("dmesg log",var7.get())

    if (var8.get() == 1):
        print("all log", var8.get())
        subprocess.call("start cmd.exe /c \"adb logcat >/tmp/adb.txt\"", shell=True)
        subprocess.call("start cmd.exe /c \"dmesg >/tmp/dmesg.txt\"", shell=True)
    else:
        print("all log", var8.get())


# create root window
root = Tk()

# root window title and dimension
root.title("Log Tool")
root.geometry('550x200')



# adding a label to the root window
lbl = Label(root, text="Log Tool", bg="lightblue")
lbl.grid(column=6, row=0)

the_lb1 = Label(root, text="Adb enable")
the_lb1.grid(column=0, row=1)

button_device = Button(root, text="connect",command=connect_device)
button_device.grid(column=5, row=1)


the_lbl = tk.Label(root, text="Log collections")
the_lbl.grid(column=0, row=3)


var5 = IntVar()
the_Checkbutton5=Checkbutton(root, text="varlog", variable=var5)
the_Checkbutton5.grid(column=3, row=3)

var6 = IntVar()
the_Checkbutton6=Checkbutton(root, text="adblog", variable=var6)
the_Checkbutton6.grid(column=5, row=3)

var7 = IntVar()
the_Checkbutton7=Checkbutton(root, text="dmesg", variable=var7)
the_Checkbutton7.grid(column=7, row=3)

var8 = IntVar()
the_Checkbutton8=Checkbutton(root, text="all logs", variable=var8)
the_Checkbutton8.grid(column=11, row=3)

button1 = Button(root, text="Run",command=save_log_info)
button1.grid(column=15, row=3)

button2 = Button(root, text="stop",command=save_log_info)
button2.grid(column=18, row=3)


root.mainloop()
