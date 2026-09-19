import tkinter as tk
import RPi.GPIO as GPIO

# Set GPIO numbering mode
GPIO.setmode(GPIO.BCM)

# Set GPIO pins
LIVING_ROOM = 17
BATHROOM = 27
CLOSET = 22

# Set LED pins as outputs
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)


# Turn all LEDs off
def turn_off_all():
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)


# Turn on living room LED
def living_room_on():
    turn_off_all()
    GPIO.output(LIVING_ROOM, GPIO.HIGH)


# Turn on bathroom LED
def bathroom_on():
    turn_off_all()
    GPIO.output(BATHROOM, GPIO.HIGH)


# Turn on closet LED
def closet_on():
    turn_off_all()
    GPIO.output(CLOSET, GPIO.HIGH)


# Close the program safely
def exit_program():
    turn_off_all()
    GPIO.cleanup()
    window.destroy()


# Create the main window
window = tk.Tk()
window.title("House Lights")

# Store the selected radio button
room = tk.StringVar()

# Create living room radio button
living_button = tk.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="living",
    command=living_room_on
)

# Create bathroom radio button
bathroom_button = tk.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="bathroom",
    command=bathroom_on
)

# Create closet radio button
closet_button = tk.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="closet",
    command=closet_on
)

# Create exit button
exit_button = tk.Button(
    window,
    text="Exit",
    command=exit_program
)

# Show the living room button
living_button.pack()

# Show the bathroom button
bathroom_button.pack()

# Show the closet button
closet_button.pack()

# Show the exit button
exit_button.pack()

# Start with all LEDs off
turn_off_all()

# Close safely using the window X
window.protocol("WM_DELETE_WINDOW", exit_program)

# Start the GUI
window.mainloop()
