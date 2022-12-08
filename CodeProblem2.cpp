//  Created by Melih Kurtaran on 11.11.2022.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <pthread.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;
#define MAX_THREADS 40 //define any number of threads
pthread_mutex_t locked;

struct thread_param
{
    int t_id;
    char t_FileName[25];
};

// Global Variables
json g_lastUpdateJSON;
json g_UserValue;
json g_timeJSON;

/* update JSONs are processed */
void * receiveUpdates(void *tParam)
{
    struct thread_param * mData;
    mData = (struct thread_param*) tParam;
    string line;
    //N is how many line will be processed by each thread
    double N = 1000000 / MAX_THREADS; 
    int i, initPos = mData->t_id * N ;
    json j;
    ifstream myFile(mData->t_FileName);
    if(myFile.is_open())
    {
        for(int j=0;j<initPos;j++)
            getline(myFile,line);
        i=0;
        while(myFile.good() && i < N)
        {
            i++;
            getline(myFile, line);
            j = json::parse(line);

            pthread_mutex_lock(&locked); //for the clean printing
            if(i==1)
                cout << "Thread " << mData->t_id << " stated on line " << mData->t_id*N+i << endl;
            else if(i==N)
                cout << "Thread " << mData->t_id << " finished on line " << mData->t_id*N+i << endl;
            pthread_mutex_unlock(&locked); 

            // substr to get rid of extra quotation marks ""
            string user = j["user"].dump().substr(1,j["user"].dump().length()-2);
            json emptyJSON;

            pthread_mutex_lock(&locked); //global variables accessed
            for (auto it = j["values"].begin(); it != j["values"].end(); ++it)
            {
                // create the key as user name and the value, corresponding to the timestamp
                string key = j["user"].dump() + it.key();
                if(!g_timeJSON.contains(key) || (stof(g_timeJSON.at(key).dump()) < stof(j["timestamp"].dump())))
                    g_timeJSON[key] = j["timestamp"];
                else
                    continue;
                g_UserValue[user][it.key()] = it.value(); // value set for each user
            }
            g_lastUpdateJSON[user] = g_UserValue.at(user);
            pthread_mutex_unlock(&locked); 

        }
        myFile.close();
    }
    else{
        cout << "ERROR: Cannot open file for reading" << endl;
    }
    pthread_exit(NULL);

}

int main(int argc, char* argv[])
{
    pthread_t threads[MAX_THREADS];
    struct thread_param mTD[MAX_THREADS];
    int rc;

    //mutex needs to be initialized
    if (pthread_mutex_init(&locked, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    for(int i=0;i<MAX_THREADS;i++)
    {
        mTD[i].t_id = i;
        memset(mTD[i].t_FileName,'\0',sizeof(mTD[i].t_FileName));
        memcpy(mTD[i].t_FileName, argv[1], strlen(argv[1]));
        rc = pthread_create(&threads[i],NULL,receiveUpdates,(void*) &mTD[i]);
        if (rc)
        {
            cout << "ERROR: Unable to create thread " << rc << endl;
            exit(-1);
        }
    }
    for(int i=0;i<MAX_THREADS;i++)
        pthread_join(threads[i], NULL);
    
    pthread_mutex_destroy(&locked);
    ofstream output("output.json");
    cout << "Pretty JSON output:" << endl;
    cout << setw(4) << g_lastUpdateJSON << endl;
    // cout << setw(4) << g_timeJSON << endl;
    output << setw(4) << g_lastUpdateJSON << endl;
    output.close();
    
    return 0;
}
