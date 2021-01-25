// Name: Josh Nguyen
// Date: 1/25/2021
// Class: CS 344
// Description: This program searches for and processes CSV files containing movie data.

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 100
#define PREFIX "movies_"

char* new_directory_name() {
    // Generates new directory name string with random number
    char *newDirName = malloc(40);
    srandom(time(NULL));  // seed random() with current time
    long randNum = random() % 100000;
    char randNumString[10];

    strcpy(newDirName, "nguyjosh.movies.");

    sprintf(randNumString, "%d", randNum);
    strcat(newDirName, randNumString);

    return newDirName;
}

void parse_file(char file[80]) {
    // Processes the designated file
    char *newDirName = new_directory_name();
    char newDir[40];
    char *saveptr = NULL;
    FILE *movieData = fopen(file, "r");
    FILE *fp;
    size_t lineSize = 100;
    char *line = malloc(sizeof(char) * lineSize);
    double nread;
    int count = -1;

    strcpy(newDir, "./");
    strcat(newDir, newDirName);  // make actual new directory path name
    mkdir(newDir, 0750);
    printf("Created directory with name %s \n", newDirName);

    if (movieData == NULL){
        fprintf(stderr, "Error reading movide data csv file.\n");
        exit(1);
    }

    while ((nread = getline(&line, &lineSize, movieData)) != -1) {
        if (count == -1) {
            count++;
            continue;
        }
        char *token = strtok_r(line, ",", &saveptr);
        char newFile[90];

        // save name of movie
        char *movieName = calloc(strlen(token)+1, sizeof(char));
        strcpy(movieName, token);

        // get year of movie
        token = strtok_r(NULL, ",", &saveptr);
        char *movieYear = malloc(sizeof(char) * 10);
        strcpy(movieYear, token);

        // open or create "YYYY.txt" file
        strcpy(newFile, newDirName);
        strcat(newFile, "/");
        strcat(newFile, movieYear);
        strcat(newFile, ".txt");
        fp = fopen(newFile, "a+");

        // write movie to newFile
        fprintf(fp, "%s\n", movieName);

        fclose(fp);
        free(movieName);
        free(movieYear);
    }

    fclose(movieData);
    free(line);
    free(newDirName);
}

void largest_file() {
    // Find then process the largest csv file that starts with the name "movies_"
    // A lot of this code was sourced from the Directory module repl.it
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    struct stat dirStat;
    double biggestSize = 0;
    char *biggestFilename;

    // Search for biggest "movies_" file
    while((aDir = readdir(currDir)) != NULL){
        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) {
            stat(aDir->d_name, &dirStat);

            // Keep track of largest file
            if(dirStat.st_size > biggestSize) {
                biggestSize = dirStat.st_size;
                biggestFilename = aDir->d_name;
            }
        } 
    }

    printf("Now processing the chosen file named %s \n", biggestFilename);
    parse_file(biggestFilename);
    closedir(currDir);
}

void smallest_file() {
    // Find and process the smallest csv file that starts with the name "movie_"
    // A lot of this code was sourced from the Directory module repl.it
    DIR* currDir = opendir("."); 
    struct dirent *aDir;
    struct stat dirStat;
    int smallestSize = INT_MAX;  // set equal to largest int value
    char *smallestFilename;

    // Search for smallest "movies_" file
    while((aDir = readdir(currDir)) != NULL){
        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) {
            stat(aDir->d_name, &dirStat);

            // Keep track of smallest file
            if(dirStat.st_size < smallestSize) {
                smallestSize = dirStat.st_size;
                smallestFilename = aDir->d_name;
            }
        }  
    }

    printf("Now processing the chosen file named %s \n", smallestFilename);
    parse_file(smallestFilename);
    closedir(currDir);
}

void specify_file() {
    // Find and process the specified csv file
    DIR* currDir = opendir("."); 
    struct dirent *aDir;
    struct stat dirStat;
    char filename[50];
    int fileFound = 1;  // set to 0 if file found

    printf("Enter the complete file name: ");
    scanf("%s", &filename);

    // Search for filename in directory
    while((aDir = readdir(currDir)) != NULL){
        if(strcmp(filename, aDir->d_name) == 0) {
            stat(aDir->d_name, &dirStat);
            fileFound = 0;
            break;        
        }
    }
    if (fileFound == 0) {
        printf("Now processing the file named %s \n", filename);
        parse_file(filename);
    } else {
        printf("The file named %s was not found. Try again.\n", filename);
    }
    
    closedir(currDir);
}

void select_processing() {
    // Allow user to select a file processing option
    int processingChoice = 0;

    printf("\nWhich file do you want to process?\n" 
    "Enter 1 to pick the largest file\n" 
    "Enter 2 to pick the smallest file\n"
    "Enter 3 to specify the name of a file\n"
    "Enter a choice from 1 to 3: ");
    scanf("%i", &processingChoice);

    switch(processingChoice) {
        case 1:
            largest_file();
            break;
        case 2:
            smallest_file();
            break;
        case 3:
            specify_file();
            break;
    }
    
}

int main(void) {
    int userChoice = 0;


    while (userChoice != 2) {
        printf("\n1. Select file to process \n2. Exit the program \nEnter a choice 1 or 2: ");
        scanf("%i", &userChoice);

        if (userChoice == 1) {
            select_processing();
        } else if (userChoice != 2) {
            printf("ERROR: Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
