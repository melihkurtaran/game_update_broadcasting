# game_update_broadcasting
A broadcasting app that handles game updates

PROBLEM 1:

To run the test case use command "./problem1 inputName.txt".

You can review the implementation in codeProblem1.cpp file.

new testcases written to see if the system works correctly for edge cases.

---------------------------------------------------------------------------------------

input4: this test case checks if friendship deleted, then the broadcast stoppping or not.

input5: more complicated test of if it update with future broadcast will be broadcasted again or not.

inpput6: testing making/deleting friends works correctly or not.

input7: more complex example of testing if update of a value for a user affects other users with same value.

---------------------------------------------------------------------------------------

PROBLEM 2:

To run the test case use command "./problem2 inputProblem2.txt". 

It will both print the results and create a pretty JSON file as a result named as "output.json".

You can review the implementation in codeProblem2.cpp file.

Explanation: It is difficult to read a txt file in a parallized way since all threads actually starts in the
beginning of file, they need to be moved further lines to make them work in parallel. The system would be
more efficient if data would be flowing continuously rather than a static txt file. However, a thread-safe
concurrent program implemented by considering using mutexes during the access of global variables. Number of
threads can be defined on top of the code, the default value is set to 40.
