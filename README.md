# EDM Festival Schedule Optimizer
A C++ application that parses festival lineups, detects schedule conflicts, and calculates optimal viewing times using a greedy algorithm. 

## Features
* **File I/O:** Reads massive festival lineups dynamically from a `.csv` file.
* **Interval Scheduling:** Organizes sets chronologically using custom C++ sorting comparators.
* **Conflict Detection:** Identifies overlapping sets and calculates mathematically optimal split times (80% completion rule).
* **Travel Penalty Logic:** Factors in walking time between stages to generate realistic itineraries.
* **Midnight Crossover Math:** Converts 24-hour military time to base-60 minute integers to handle late-night sets without breaking the timeline.

## Technologies Used
* C++ 
* Standard Template Library (Vectors, Maps)
* File Stream (`<fstream>`)
