//  Created by Melih Kurtaran on 11.11.2022.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

/* creates a symmetric relationship in the map */
void makeFriends(const json & j, map<json, list<json>> & fMap)
{
    json user1 = j["user1"];
    json user2 = j["user2"];
    //friendship relation is symmetric
    if(fMap.count(user1) == 0) {
        list<json> v = {user2};
        fMap[user1] = v;
    } else 
        fMap[user1].push_back(user2);

    if (fMap.count(user2) == 0) {
        list<json> v = {user1};
        fMap[user2] = v;
    } else 
        fMap[user2].push_back(user1);
        
}

/* deletes the symmetric relationship in the map */
void deleteFriends(const json & j, map<json, list<json>> & fMap)
{
    json user1 = j["user1"];
    json user2 = j["user2"];
    fMap[user1].remove(user2);
    fMap[user2].remove(user1); 

    //if no friend for a user, it is not stored in the map
    if(fMap[user1].empty())
        fMap.erase(user1);
    if(fMap[user2].empty())
        fMap.erase(user2);
}

/* broadcasting the update depending on the conditions */
void broadcast(const json & j,vector<json> & brs, const map<json, list<json>> & fMap)
{
    json user = j["user"];
    long timeStamp = stol(j["timestamp"].dump());
    json values = j["values"];
    json broadcast;
    if (fMap.count(user)) 
        broadcast["broadcast"] = fMap.at(user);
    broadcast["user"] = user;
    broadcast["timestamp"] = timeStamp;

    //checking if already broadcasted before or not
    for(int i=0;i<brs.size();i++)
    {
        if(brs[i]["timestamp"] < timeStamp || brs[i]["user"] != user)
            continue; //no check if timeStamp past or another user's broadcast
        for (auto& v : values.items())
        {
            //if already broadcasted, dont again
            if(brs[i]["values"].contains(v.key()))
            {
                values.erase(v.key());
                break;
            }
        }
    }
    broadcast["values"] = values;
    brs.push_back(broadcast);
    if(!values.empty() && fMap.count(user))
    //if no friend then no broadcasting
    {
        cout << broadcast << endl;
    }
}

int main(int argc, char* argv[])
{
    //Using hashmap for players who have friends
    //This way O(1) time user access
    map<json, list<json>> friendshipMap;
    vector<json> broadcasts;

    // read the txt and make it a JSON
    ifstream readFromFile(argv[1]);

    if (readFromFile.is_open()) {
        string line;
        vector<json> jsons;
        json j;

        while(getline(readFromFile,line))
        {
            j = json::parse(line);

            if(j["type"].dump() == "\"make_friends\"")
                makeFriends(j,friendshipMap);
            else if(j["type"].dump() == "\"del_friends\"")
                deleteFriends(j,friendshipMap);
            else if(j["type"].dump() == "\"update\"")
                broadcast(j,broadcasts,friendshipMap); //if there is update->broadcast

            jsons.push_back(j);
        }
        readFromFile.close();
    }
    else {
        cout << "unable to open file";
    }
    return 0;
}
