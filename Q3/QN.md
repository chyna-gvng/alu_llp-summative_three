### Q3. Multi-Threading

Micro wave oven is one tools that we all often use, our interests in this project is to implement the program that manipulates the oven. Here are the functionalities that the oven implements:
- The oven needs to be closed with a start button pressed in order to have the oven working.
- For the oven to be working, the plate needs to be rotating, the lights needs to be on, and the timer needs to be counting down to zero (0)
- The oven will stop working when stop button is pressed or the oven is open.

Here are the triggers:
* 1 is used for opening the oven
* 0 is used closing opening the oven
* 2 is used to start the oven
* 3 is used to stop the oven
* The timer starts at at 60 seconds and lowest value is 0 seconds.
* 4 is used for rotating plate
* 5 is used for stopped plate
*6 is used for the lights off
* 7 is used for lights off

The program implemented will use multiple threads:
The thread that deals with open, the thread that closes the oven, the thread that rotates the plate, the thread that stops the plate from rotating, the thread that starts up the light, the thread that shuts down the light, the thread that counts down the timer from 60 second to 0 seconds (lowest)

#### Notes
- Assumption(BR): Oven is open

