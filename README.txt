Movie Scanner App

Description: This program reads in a formatted CSV file that contains movie data. The movie name, year of release, languages, and rating are stored on each line and separated by commas. The CSV file is processed to create a new directory in the current directory. The new directory will contain a text file for each year a movie is released from the CSV data file. Formatting for the text file is "2018.txt".



How to run:
    1. Compile the code using the command in terminal: gcc --std=gnu99 -o movies JNguyen_A2_Movies.c
    2. Run the movies file with: ./movies

    Example run code:
    gcc --std=gnu99 -o movies JNguyen_A2_Movies.c
    ./movies