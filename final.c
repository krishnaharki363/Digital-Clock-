#include <stdio.h>      // For input/output functions like printf, scanf, fopen
#include <stdlib.h>     // For system() to clear screen based on OS
#include <time.h>       // For getting and formatting current time and date
#include <unistd.h>     // For sleep() function to pause program execution

#define TIME_BUFFER 50   // Define constant for time string buffer size
#define DATE_BUFFER 100  // Define constant for date string buffer size

// Function declarations
void fill_time(char*, int);                 // Fills time in buffer based on format
void fill_date(char*);                      // Fills date in buffer
int input_format();                         // Gets time format input from user
void clear_screen();                        // Clears the screen depending on OS
void log_to_file(const char*, const char*); // Logs date and time to file

int main() {
    char time[TIME_BUFFER], date[DATE_BUFFER];  // Arrays to store time and date
    int format = input_format();                // Ask user to choose time format

    while (1) {  // Infinite loop to update every second
        fill_time(time, format);     // Get current time and store in 'time'
        fill_date(date);             // Get current date and store in 'date'
        clear_screen();              // Clear the terminal/console screen

        printf("Current Time: %s\n", time);  // Print time to the screen
        printf("Date        : %s\n", date);  // Print date to the screen

        log_to_file(time, date);     // Save time and date to file (log)

        fflush(stdout);              // Force immediate output to screen
        sleep(1);                    // Wait for 1 second before updating again
    }

    return 0;  // Return success (never actually reached due to infinite loop)
}

// Function to clear the screen based on operating system
void clear_screen() {
    #ifdef _WIN32
        system("cls");   // For Windows systems
    #else
        system("clear"); // For Linux/Unix/Mac systems
    #endif  
}

// Function to take user input for time format (12 or 24 hour)
int input_format() {
    int format;  // Variable to store user's choice

    // Ask user to choose format
    printf("\nChoose the time format: ");
    printf("\n1. 24-Hour format");
    printf("\n2. 12-Hour format (default)");
    printf("\nMake a choice (1 or 2): ");

    // Read input and validate
    if (scanf("%d", &format) != 1) {
        printf("Invalid input. Defaulting to 12-hour format.\n");
        format = 2;
        while (getchar() != '\n');  // Clear invalid input from buffer
    }

    // If choice is not 1 or 2, default to 12-hour
    if (format != 1 && format != 2) {
        printf("Invalid choice. Defaulting to 12-hour format.\n");
        format = 2;
    }

    return format;  // Return valid format
}
// Function to fill date buffer with current date in readable format
void fill_date(char* buffer) {
    time_t raw_time;                // Variable to store raw time
    struct tm *current_time;       // Structure to hold converted local time

    time(&raw_time);               // Get current time in seconds since 1970
    current_time = localtime(&raw_time);  // Convert to local time format

    // Format date string: example "Tuesday, July 16, 2025"
    strftime(buffer, DATE_BUFFER, "%A, %B %d, %Y", current_time);
}

// Function to fill time buffer in 12-hour or 24-hour format
void fill_time(char* buffer, int format) {
    time_t raw_time;                // Variable to store raw time
    struct tm *current_time;       // Structure to hold local time

    time(&raw_time);               // Get current system time
    current_time = localtime(&raw_time);  // Convert to local time structure

    // Check user's format choice and format accordingly
    if (format == 1)
        strftime(buffer, TIME_BUFFER, "%H:%M:%S", current_time);     // 24-hour format
    else
        strftime(buffer, TIME_BUFFER, "%I:%M:%S %p", current_time);  // 12-hour format with AM/PM
}

// Function to log time and date to a file named "time_log.txt"
void log_to_file(const char* time, const char* date) {
    // Open file in append mode so previous data is not lost
    FILE *file = fopen("time_log.txt", "a");
    
    // If file couldn't be opened, show error
    if (file == NULL) {
        perror("Error opening log file");
        return;
    }

    // Write formatted time and date to file
    fprintf(file, "[%s] - %s\n", date, time);

    // Close the file after writing
    fclose(file);
}
