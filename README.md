# game_update_broadcasting
A broadcasting app that handles game updates

PROBLEM 1:

Explanation: 

- This is a program that reads a JSON file containing different types of events and updates a friendship map and broadcast list accordingly. The main function reads the JSON file line by line and parses each line into a json object. It then checks the type of the event and calls the appropriate function to update the friendship map or broadcast list.
- The makeFriends function takes in a json object representing a "make_friends" event and a reference to the friendship map. It creates a symmetric relationship between the two users specified in the event by adding them to each other's lists in the map. If either user does not have an entry in the map, a new list is created for them.
- The deleteFriends function takes in a json object representing a "del_friends" event and a reference to the friendship map. It removes the symmetric relationship between the two users specified in the event by deleting them from each other's lists in the map. If either user's list becomes empty after the deletion, their entry is removed from the map.
- The broadcast function takes in a json object representing an "update" event, a reference to the broadcast list, and a reference to the friendship map. It creates a new json object representing a broadcast message and adds it to the broadcast list. The broadcast message includes the user who sent the update, the timestamp of the update, and the updated values. The function also checks if the user has any friends in the friendship map and, if so, prints the broadcast message. If the update has already been broadcasted for the same user, the updated values are not included in the broadcast message.
- The program makes use of the nlohmann::json library to parse and manipulate json objects. The map and list classes from the STL are also used to store the friendship relationships and broadcast messages.

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
