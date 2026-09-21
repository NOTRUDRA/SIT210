import tkinter as tk
import RPi.GPIO as GPIO

# Set GPIO numbering to BCM mode
GPIO.setmode(GPIO.BCM)

# GPIO pins used for each room light
LIVING_ROOM = 18
BATHROOM = 27
CLOSET = 22

# Set the LED pins as output pins
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)

# Create PWM control for the living room LED
living_pwm = GPIO.PWM(LIVING_ROOM, 100)
living_pwm.start(0)


# Turn all room lights off
def turn_off_all():
    living_pwm.ChangeDutyCycle(0)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)


# Turn on the living room and use the current brightness value
def living_room_on():
    turn_off_all()
    living_pwm.ChangeDutyCycle(brightness.get())


# Turn on the bathroom and keep other lights off
def bathroom_on():
    turn_off_all()
    GPIO.output(BATHROOM, GPIO.HIGH)


# Turn on the closet and keep other lights off
def closet_on():
    turn_off_all()
    GPIO.output(CLOSET, GPIO.HIGH)


# Change living room brightness when the slider is moved
def change_brightness(value):
    if room.get() == "living":
        living_pwm.ChangeDutyCycle(float(value))


# Turn off the lights and safely close the program
def exit_program():
    turn_off_all()
    living_pwm.stop()
    GPIO.cleanup()
    window.destroy()


# Create the main GUI window
window = tk.Tk()
window.title("House Lights")

# Store the room selected by the user
room = tk.StringVar()

# Store the current living room brightness
brightness = tk.IntVar(value=100)


# Create the living room radio button
living_button = tk.Radiobutton(
    window,
    text="Living Room",
    variable=room,
    value="living",
    command=living_room_on
)

# Create the bathroom radio button
bathroom_button = tk.Radiobutton(
    window,
    text="Bathroom",
    variable=room,
    value="bathroom",
    command=bathroom_on
)

# Create the closet radio button
closet_button = tk.Radiobutton(
    window,
    text="Closet",
    variable=room,
    value="closet",
    command=closet_on
)

# Create the slider for living room brightness
brightness_slider = tk.Scale(
    window,
    from_=0,
    to=100,
    orient=tk.HORIZONTAL,
    label="Living Room Brightness",
    variable=brightness,
    command=change_brightness
)

# Create the exit button
exit_button = tk.Button(
    window,
    text="Exit",
    command=exit_program
)


# Add all controls to the window
living_button.pack()
bathroom_button.pack()
closet_button.pack()
brightness_slider.pack()
exit_button.pack()


# Start with all lights turned off
turn_off_all()

# Safely close the program when the window X is pressed
window.protocol("WM_DELETE_WINDOW", exit_program)

# Keep the GUI running and wait for user input
window.mainloop()
