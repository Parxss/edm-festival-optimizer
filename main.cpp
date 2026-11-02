#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // Needed for the sort() function

using namespace std;

// --- 1. DATA STRUCTURES ---
struct Performance {
    string artist;
    string stage;
    int startTime;
    int endTime;
};

// --- 2. HELPER FUNCTIONS ---

// Tells C++ how to sort our custom objects (chronologically by start time)
bool compareByStartTime(const Performance& a, const Performance& b) {
    return a.startTime < b.startTime;
}

// Converts military time (2130) into pure minutes (1290) for accurate math
int timeToMins(int hhmm) {
    int hours = hhmm / 100;
    int minutes = hhmm % 100;
    return (hours * 60) + minutes;
}

// Converts pure minutes (1290) back into military time (2130) for the UI
int minsToTime(int totalMins) {
    int hours = totalMins / 60;
    int minutes = totalMins % 60;
    return (hours * 100) + minutes;
}


// --- 3. MAIN PROGRAM ---
int main() {
    vector<Performance> lineup;
    
    // IMPORTANT: Make sure this path matches exactly where your lineup.csv is saved!
    // Remember to use double backslashes \\ for Windows paths.
    ifstream file("lineup.csv");
    
    if (!file.is_open()) {
        cout << "Error: Could not open lineup.csv! Check your file path." << endl;
        return 1; 
    }

    string line;
    // Read the CSV file line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string artist, stage, startStr, endStr;

        // Parse the data using commas as the delimiter
        getline(ss, artist, ',');
        getline(ss, stage, ',');
        getline(ss, startStr, ',');
        getline(ss, endStr, ',');

        // Create a Performance object and store it
        Performance p;
        p.artist = artist;
        p.stage = stage;
        p.startTime = stoi(startStr); 
        p.endTime = stoi(endStr);

        lineup.push_back(p);
    }
    file.close();

    // Sort the lineup chronologically before we try to find overlaps
    sort(lineup.begin(), lineup.end(), compareByStartTime);

    cout << "--- Festival Route Optimizer ---" << endl;
    cout << "Loaded " << lineup.size() << " artists.\n" << endl;

    if (lineup.empty()) {
        return 1;
    }

    // --- 4. THE OPTIMIZER ALGORITHM ---
    
    int walkTimeMins = 10; // Adjust this depending on the festival size
    Performance currentSet = lineup[0];
    
    cout << "✅ Attend: " << currentSet.artist << " (" << currentSet.startTime << " - " << currentSet.endTime << ")" << endl;

    for (int i = 1; i < lineup.size(); i++) {
        
        int currentEndMins = timeToMins(currentSet.endTime);
        int nextStartMins = timeToMins(lineup[i].startTime);

        // Calculate actual arrival time including the walk
        int arrivalTimeMins = currentEndMins + walkTimeMins;

        // NO CONFLICT: We arrive before or exactly when the next set starts
        if (nextStartMins >= arrivalTimeMins) {
            currentSet = lineup[i];
            cout << "✅ Attend: " << currentSet.artist << " (" << currentSet.startTime << " - " << currentSet.endTime << ")" << endl;
        } 
        // CONFLICT DETECTED: Calculate optimal split
        else {
            cout << "\n⚠️ CONFLICT: " << currentSet.artist << " overlaps with " << lineup[i].artist << endl;

            int startA = timeToMins(currentSet.startTime);
            int startB = timeToMins(lineup[i].startTime);
            int endB = timeToMins(lineup[i].endTime);

            int durationA = currentEndMins - startA;
            int durationB = endB - startB;

            // Find the exact minute to leave Set A (after 80%)
            int leaveTimeMins = startA + (durationA * 0.80);
            
            // Find the latest arrival for Set B (to catch 80%)
            int latestArrivalBMins = endB - (durationB * 0.80);

            // Factor the walk into our plan
            int actualArrivalAtB = leaveTimeMins + walkTimeMins;

            if (actualArrivalAtB <= latestArrivalBMins) {
                cout << "   -> PLAN: Leave " << currentSet.artist << " at " << minsToTime(leaveTimeMins) 
                     << " (10m walk), arriving at " << minsToTime(actualArrivalAtB) 
                     << " to catch the last 80% of " << lineup[i].artist << "." << endl;
            } else {
                cout << "   -> ALERT: Cannot see 80% of both sets with a " << walkTimeMins << "-minute walk." << endl;
                cout << "      If you leave at " << minsToTime(leaveTimeMins) 
                     << ", you arrive at " << minsToTime(actualArrivalAtB) 
                     << " and miss too much of " << lineup[i].artist << "." << endl;
            }
        }
    }

    return 0;
}