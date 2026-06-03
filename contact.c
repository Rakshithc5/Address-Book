#include <stdio.h>      // Standard input-output functions (printf, scanf)
#include <string.h>     // String handling functions (strcpy, strcmp, etc.)
#include <ctype.h>      // Character handling functions (isdigit, toupper, etc.)
#include <strings.h>    // Additional string functions (like strcasecmp)
#include "contact.h"    // User-defined header file for Contact structure and constants

// Array to store contacts (size defined by MAX in contact.h)
Contact contacts[MAX];

// Variable to keep track of number of contacts stored
int count = 0;

// Function to print a horizontal line using '=' characters
void line() {
    // Loop runs WIDTH times to print '=' repeatedly
    for(int i = 0; i < WIDTH; i++) 
        printf("=");   // Print '=' without newline
    
    printf("\n");      // Move to next line after printing
}

// Function to print text centered within WIDTH
void center(const char *text) {
    int len = strlen(text);              // Get length of the input text
    
    // Calculate padding (spaces before text to center it)
    int pad = (WIDTH - len) / 2;
    
    // Print leading spaces
    for(int i = 0; i < pad; i++) 
        printf(" ");
    
    printf("%s\n", text);               // Print the actual text followed by newline
}

// Function to print a formatted header with title
void header(const char *title) {
    line();            // Print top border line
    center(title);     // Print centered title
    line();            // Print bottom border line
}

// Function to display a success message in green color
void success_msg(const char *msg) {
    // \033[32m -> sets text color to green
    // \033[0m  -> resets color back to default
    printf("\033[32m[SUCCESS] %s\033[0m\n", msg);
}

// Function to display an error message in red color
void error_msg(const char *msg) {
    // \033[31m -> sets text color to red
    // \033[0m  -> resets color back to default
    printf("\033[31m[ERROR] %s\033[0m\n", msg);
}

// Function to validate a name
Status valid_name(char *name) {
    // Loop through each character in the name string
    for(int i = 0; name[i]; i++) {
        // Check if character is NOT a space AND NOT an alphabet
        if(name[i] != ' ' && !isalpha(name[i])) {
            // Print error message if invalid character found
            error_msg("Name must contain only letters and space.");
            return FAILURE;   // Return failure status
        }
    }
    return SUCCESS;   // Return success if all characters are valid
}

// Function to check if all alphabet characters in string are lowercase
Status is_lowercase(char *str) {
    // Loop through each character in the string
    for(int i = 0; str[i]; i++) {
        // If character is alphabet but NOT lowercase
        if(isalpha(str[i]) && !islower(str[i])) {
            // Print error message
            error_msg("Email must contain only lowercase letters.");
            return FAILURE;   // Return failure status
        }
    }
    return SUCCESS;   // Return success if all alphabet characters are lowercase
}
// Function to validate a phone number
Status valid_phone(char *p) {
    // Loop through each character of the phone string
    for(int i = 0; p[i]; i++) {
        // Check if character is NOT a digit
        if(!isdigit(p[i])) {
            // Print error message if non-digit is found
            error_msg("Phone must contain only digits.");
            return FAILURE;   // Return failure status
        }
    }

    // Check if length of phone number is exactly 10 digits
    if(strlen(p) != 10) {
        // Print error message if length is not 10
        error_msg("Phone must be exactly 10 digits.");
        return FAILURE;   // Return failure status
    }

    // Check if first digit is between '6' and '9'
    if(p[0] < '6' || p[0] > '9') {
        // Print error message if starting digit is invalid
        error_msg("Phone must start with 6–9.");
        return FAILURE;   // Return failure status
    }

    return SUCCESS;   // Return success if all validations pass
}
// Function to validate an email address
Status valid_email(char *e) {
    // Find first occurrence of '@'
    char *at = strchr(e, '@');
    
    // Find occurrence of ".com"
    char *dot = strstr(e, ".com");

    int count_at = 0;   // Counter to count number of '@' symbols

    // Check if '@' is present
    if(!at) {
        error_msg("Email must contain '@'.");
        return FAILURE;
    }

    // Count how many '@' are present
    for(int i = 0; e[i]; i++) {
        if(e[i] == '@') 
            count_at++;
    }

    // Ensure only one '@' exists
    if(count_at != 1) {
        error_msg("Email must contain only one @.");
        return FAILURE;
    }

    // Check if '@' is at the beginning
    if(at == e) {
        error_msg("Invalid format.");
        return FAILURE;
    }

    // Check if ".com" exists and is exactly at the end
    if(!dot || strcmp(dot, ".com") != 0) {
        error_msg("Email must end with '.com'.");
        return FAILURE;
    }

    // Ensure '@' appears before ".com"
    if(at > dot) {
        error_msg("Invalid email format.");
        return FAILURE;
    }

    // Ensure there is at least one character between '@' and ".com"
    if(at + 1 == dot) {
        error_msg("Email must contain domain name.");
        return FAILURE;
    }

    // Validate domain name (characters between '@' and ".com")
    char *ptr = at + 1;
    while(ptr < dot) {
        // Check if each character is alphabet only
        if(!isalpha(*ptr)) {
            error_msg("Domain must contain only alphabets.");
            return FAILURE;
        }
        ptr++;
    }

    // Check if entire email string has only lowercase alphabets (for letters)
    if(is_lowercase(e) == FAILURE)
        return FAILURE;

    return SUCCESS;   // Return success if all validations pass
}


// Function to check if a phone number already exists in contacts
Status phone_exists(char *p, int skip) {
    // Loop through all stored contacts
    for(int i = 0; i < count; i++)
        // Check if index is not the skipped one AND phone matches
        if(i != skip && strcmp(contacts[i].phone, p) == 0)
            return SUCCESS;   // Phone already exists
    
    return FAILURE;   // Phone does not exist
}

// Function to check if an email already exists in contacts
Status email_exists(char *e, int skip) {
    // Loop through all stored contacts
    for(int i = 0; i < count; i++)
        // Check if index is not the skipped one AND email matches
        if(i != skip && strcmp(contacts[i].email, e) == 0)
            return SUCCESS;   // Email already exists
    
    return FAILURE;   // Email does not exist
}


// Function to sort contacts alphabetically by name
void sort_contacts() {
    Contact temp;   // Temporary variable for swapping contacts

    // Outer loop to traverse each contact
    for(int i = 0; i < count - 1; i++) {
        
        // Inner loop to compare current contact with remaining contacts
        for(int j = i + 1; j < count; j++) {
            
            // Compare names ignoring case (strcasecmp)
            if(strcasecmp(contacts[i].name, contacts[j].name) > 0) {
                
                // Swap contacts if they are in wrong order
                temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }
}


// Function to load contacts from file into memory
void load() {
    // Open file in read mode
    FILE *fp = fopen("contacts.txt", "r");

    // Check if file opening failed
    if(!fp) {
        error_msg("File open failed.");
        return;
    }

    // Read data until MAX limit or file ends
    while(count < MAX &&
          // Read name, phone, email separated by ';'
          fscanf(fp, " %[^;];%[^;];%[^\n]",
                 contacts[count].name,
                 contacts[count].phone,
                 contacts[count].email) == 3) {
        
        count++;   // Increment contact count after successful read
    }

    fclose(fp);   // Close file after reading

    sort_contacts();   // Sort contacts after loading
}

// Function to save contacts from memory to file
void save() {
    // Open file in write mode (overwrites existing data)
    FILE *fp = fopen("contacts.txt", "w");

    // Check if file opening failed
    if(!fp) {
        error_msg("File open failed.");
        return;
    }
    if(count==0){
        success_msg("There no Contacts to save.");
        return;
    }

    // Write all contacts into file
    for(int i = 0; i < count; i++)
        fprintf(fp, "%s;%s;%s\n",
                contacts[i].name,
                contacts[i].phone,
                contacts[i].email);

    fclose(fp);   // Close file after writing

    success_msg("Contacts saved successfully.");   // Show success message
}


// Function to find a contact based on search mode and key
int find_contact(int mode, char *key) {
    // Loop through all contacts
    for(int i = 0; i < count; i++) {
        
        // Mode 1 → Search by name
        if(mode == 1 && strcmp(contacts[i].name, key) == 0) 
            return i;   // Return index if match found
        
        // Mode 2 → Search by phone
        if(mode == 2 && strcmp(contacts[i].phone, key) == 0) 
            return i;
        
        // Mode 3 → Search by email
        if(mode == 3 && strcmp(contacts[i].email, key) == 0) 
            return i;
    }

    return -1;   // Return -1 if no match found
}

// Function to find a contact within a filtered list of indices
int find_in_filtered(int indices[], int size, int mode, char *key) {
    // Loop through filtered indices array
    for(int i = 0; i < size; i++) {
        int idx = indices[i];   // Get actual index from contacts array

        // Mode 2 → Search by phone in filtered list
        if(mode == 2 && strcmp(contacts[idx].phone, key) == 0)
            return idx;   // Return actual index
        
        // Mode 3 → Search by email in filtered list
        if(mode == 3 && strcmp(contacts[idx].email, key) == 0)
            return idx;
    }

    return -1;   // Return -1 if not found
}

// Function to add a new contact
void add_contact() {
    char ch;   // Variable to store user's choice (y/n)

    do {
        // Check if contact list is full
        if(count >= MAX) {
            error_msg("Contact list full.");
            return;
        }

        header("ADD NEW CONTACT");   // Display header

        // Input and validate name
        while(1) {
            printf("Name  : ");
            scanf(" %[^\n]", contacts[count].name);   // Read full name (including spaces)

            // Validate name
            if(valid_name(contacts[count].name) == SUCCESS)
                break;   // Exit loop if valid
        }

        // Input and validate phone number
        while(1) {
            printf("Phone : ");
            scanf("%14s", contacts[count].phone);   // Read phone (max 14 chars for safety)

            // Validate phone format
            if(valid_phone(contacts[count].phone) == FAILURE) 
                continue;

            // Check for duplicate phone
            if(phone_exists(contacts[count].phone, -1) == SUCCESS) {
                error_msg("Phone already exists.");
                continue;
            }
            break;   // Exit loop if valid and unique
        }

        // Input and validate email
        while(1) {
            printf("Email : ");
            scanf("%49s", contacts[count].email);   // Read email (max 49 chars)

            // Validate email format
            if(valid_email(contacts[count].email) == FAILURE) 
                continue;

            // Check for duplicate email
            if(email_exists(contacts[count].email, -1) == SUCCESS) {
                error_msg("Email already exists.");
                continue;
            }
            break;   // Exit loop if valid and unique
        }

        count++;              // Increase contact count
        sort_contacts();     // Sort contacts after adding
        success_msg("Contact added.");   // Show success message

        // Ask user if they want to add another contact
        printf("Add another? (y/n): ");
        scanf(" %c", &ch);

    } while(ch == 'y' || ch == 'Y');   // Repeat if user enters 'y' or 'Y'
}
// Function to display all contacts in a formatted table
void list_contacts() {
    header("CONTACT LIST");   // Display header title

    // Check if no contacts are available
    if(count == 0) {
        printf("No contacts available.\n");
        return;   // Exit function if list is empty
    }

    line();   // Print top border line

    // Print table column headers with formatting
    printf("| %-3s | %-20s | %-12s | %-25s |\n",
           "No", "Name", "Phone", "Email");

    line();   // Print separator line

    // Loop through all contacts and print each one
    for(int i = 0; i < count; i++) {
        printf("| %-3d | %-20s | %-12s | %-25s |\n",
               i + 1,                  // Serial number (1-based index)
               contacts[i].name,      // Contact name
               contacts[i].phone,     // Contact phone
               contacts[i].email);    // Contact email
    }

    line();   // Print bottom border line
}

// Function to search for a contact using different criteria
void search_contact() {
    int ch;             // Stores user choice (search mode)
    char key[50];       // Stores input value (name / phone / email)

    // Infinite loop to keep showing search menu until user chooses "Back"
    while(1) {
        header("SEARCH CONTACT");   // Print formatted header
        printf("Search by :\n");
        // Display menu options
        printf("1. Name\n2. Phone\n3. Email\n4. Back\nChoice: ");

        // Validate numeric input (avoid crash on invalid input like characters)
        if(scanf("%d", &ch) != 1) {
            error_msg("Invalid input");
            while(getchar() != '\n');   // Clear invalid input from buffer
            continue;                  // Restart loop
        }

        while(getchar() != '\n');   // Clear leftover newline from buffer

        // Option 4 → Exit search menu
        if(ch == 4) return;

        // -------------------- SEARCH BY NAME --------------------
        if(ch == 1) {

            // Take name input and validate
            while(1) {
                printf("Enter Name: ");
                scanf(" %[^\n]", key);   // Read full string including spaces

                if(valid_name(key) == SUCCESS) 
                    break;   // Exit loop if valid
            }

            int indices[MAX];   // Array to store indices of matching contacts
            int found = 0;      // Counter for number of matches

            // Loop through all contacts to find matching names
            for(int i = 0; i < count; i++) {
                if(strcmp(contacts[i].name, key) == 0) {
                    indices[found++] = i;   // Store matching index
                }
            }

            // If no match found
            if(found == 0) {
                error_msg("Contact not found.");
                continue;   // Restart menu
            }

            // If exactly one match found → directly display
            if(found == 1) {
                int idx = indices[0];   // Get the only matching index

                success_msg("Contact found.");
                printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                       contacts[idx].name,
                       contacts[idx].phone,
                       contacts[idx].email);
                continue;   // Return to menu
            }

            // If multiple matches found → display as table
            line();   // Print table border

            printf("| %-3s | %-20s | %-12s | %-25s |\n",
                   "No", "Name", "Phone", "Email");

            line();

            // Print all matched contacts
            for(int i = 0; i < found; i++) {
                int idx = indices[i];   // Actual index in contacts array

                printf("| %-3d | %-20s | %-12s | %-25s |\n",
                       i + 1,                         // Display number (1-based)
                       contacts[idx].name,
                       contacts[idx].phone,
                       contacts[idx].email);
            }

            line();   // Bottom border

            // Ask user if they want to refine search further
            int opt;
            printf("\nSearch within results using Phone or Email:\n2.Phone\n3.Email\nChoice: ");

            // Validate numeric input again
            if(scanf("%d", &opt) != 1) {
                error_msg("Invalid input");
                while(getchar() != '\n');   // Clear buffer
                continue;
            }

            while(getchar() != '\n');   // Clear newline

            char sub[50];   // Secondary key for filtering

            // -------- Filter by Phone --------
            if(opt == 2) {
                while(1) {
                    printf("Enter Phone: ");
                    scanf("%s", sub);
                     while(getchar()!='\n');
                    if(valid_phone(sub) == SUCCESS) 
                        break;
                }
            }
            // -------- Filter by Email --------
            else if(opt == 3) {
                while(1) {
                    printf("Enter Email: ");
                    scanf("%s", sub);
                    while(getchar()!='\n');
                    if(valid_email(sub) == SUCCESS) 
                        break;
                }
            }
            // Invalid option
            else {
                error_msg("Invalid choice.");
                continue;
            }

            while(1) {   // ADDED LOOP

    int idx = find_in_filtered(indices, found, opt, sub);// find specific contact in filtered list

    if(idx == -1) {
        error_msg("Contact not found in filtered results.\n");
        error_msg("Try again!.\n");
        //  Ask again
        if(opt == 2) {
            while(1){
                printf("Enter Phone: ");
                scanf("%s", sub);
                while(getchar()!='\n');
                if(valid_phone(sub)==SUCCESS) break;
            }
        }
        else if(opt == 3) {
            while(1){
                printf("Enter Email: ");
                scanf("%s", sub);
                while(getchar()!='\n');
                if(valid_email(sub)==SUCCESS) break;
            }
        }

        continue;   //  retry
    } 
    else {
        success_msg("Contact found.");
        printf("Name  : %s\nPhone : %s\nEmail : %s\n",
               contacts[idx].name,
               contacts[idx].phone,
               contacts[idx].email);
        break;   //  exit loop
    }
}

continue;  // Return to main search menu
        }

        // -------------------- SEARCH BY PHONE --------------------
        else if(ch == 2) {
            while(1) {
                printf("Enter Phone: ");
                scanf("%s", key);

                if(valid_phone(key) == SUCCESS) 
                    break;
            }
        }

        // -------------------- SEARCH BY EMAIL --------------------
        else if(ch == 3) {
            while(1) {
                printf("Enter Email: ");
                scanf("%s", key);

                if(valid_email(key) == SUCCESS) 
                    break;
            }
        }

        // Invalid menu choice
        else {
            error_msg("Invalid choice.");
            continue;
        }

        // Perform direct search (for phone/email cases)
        int idx = find_contact(ch, key);

        // If not found
        if(idx == -1) {
            error_msg("Contact not found.");
        } 
        else {
            // If found → display details
            success_msg("Contact found.");
            printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                   contacts[idx].name,
                   contacts[idx].phone,
                   contacts[idx].email);
        }
    }
}


void edit_contact() {
    int ch,opt;                // ch = main choice, opt = edit option
    char key[50];              // key for searching

    while(1){                  // infinite loop for menu
        header("EDIT CONTACT");   // display header
        printf("Search by :\n");
        printf("1. Name\n2. Phone\n3. Email\n4. Back\nChoice: ");
        if(scanf("%d",&ch)!=1){   // input validation
            error_msg("Invalid input");
            while(getchar()!='\n');  // clear buffer
            continue;
        }
        while(getchar()!='\n');  // clear buffer after input

        if(ch==4) return;       // exit function

        if(ch==1){              // search by name

            while(1){
                printf("Enter Name: ");
                scanf(" %[^\n]",key);   // read full name with spaces
                if(valid_name(key)==SUCCESS) break;   // validate name
            }
            while(getchar()!='\n');  // clear buffer

            int indices[MAX], found=0;   // store matching indices

            for(int i=0;i<count;i++){    // loop through contacts
                if(strcmp(contacts[i].name,key)==0){  // match name
                    indices[found++] = i;   // store index
                }
            }

            if(found==0){   // no match
                error_msg("Contact not found.");
                continue;
            }

            if(found == 1){   // exactly one match
                int idx = indices[0];   // get index

                success_msg("Contact found.");
                printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                       contacts[idx].name,
                       contacts[idx].phone,
                       contacts[idx].email);

                printf("\nWhat you want to edit:\n");
                printf("1.Name\n2.Phone\n3.Email\n4.All\nChoice: ");
                scanf("%d",&opt);   // choose field to edit
                while(getchar()!='\n');

                if(opt==1||opt==4){   // edit name
                    while(1){
                        printf("New Name: ");
                        scanf(" %[^\n]",contacts[idx].name);
                        if(valid_name(contacts[idx].name)==SUCCESS)
                            break;
                    }
                    while(getchar()!='\n');
                }

                if(opt==2||opt==4){   // edit phone
                    while(1){
                        printf("New Phone: ");
                        scanf("%s",contacts[idx].phone);
                        while(getchar()!='\n');

                        if(valid_phone(contacts[idx].phone)==FAILURE) continue;

                        if(phone_exists(contacts[idx].phone,idx)==SUCCESS){
                            error_msg("Phone already exists.");
                            continue;
                        }
                        break;
                    }
                }

                if(opt==3||opt==4){   // edit email
                    while(1){
                        printf("New Email: ");
                        scanf("%s",contacts[idx].email);
                        while(getchar()!='\n');

                        if(valid_email(contacts[idx].email)==FAILURE) continue;

                        if(email_exists(contacts[idx].email,idx)==SUCCESS){
                            error_msg("Email already exists.");
                            continue;
                        }
                        break;
                    }
                }

                sort_contacts();   // sort after update
                success_msg("Contact updated.");

                printf("\nPress ENTER...");
                getchar();   // wait for user

                continue;
            }

            line();   // table header line
            printf("| %-3s | %-20s | %-12s | %-25s |\n",
                   "No","Name","Phone","Email");
            line();

            for(int i=0;i<found;i++){   // display all matches
                int idx = indices[i];
                printf("| %-3d | %-20s | %-12s | %-25s |\n",
                       i+1,
                       contacts[idx].name,
                       contacts[idx].phone,
                       contacts[idx].email);
            }
            line();

            int opt2;
            printf("\nSearch within results using Phone or Email:\n2.Phone\n3.Email\nChoice: ");
            if(scanf("%d",&opt2)!=1){   // validate input
                error_msg("Invalid input");
                while(getchar()!='\n');
                continue;
            }
            while(getchar()!='\n');

            char sub[50];   // sub key

            if(opt2==2){   // filter by phone
                while(1){
                    printf("Enter Phone: ");
                    scanf("%s",sub);
                    while(getchar()!='\n');
                    if(valid_phone(sub)==SUCCESS) break;
                }
            }
            else if(opt2==3){   // filter by email
                while(1){
                    printf("Enter Email: ");
                    scanf("%s",sub);
                    while(getchar()!='\n');
                    if(valid_email(sub)==SUCCESS) break;
                }
            }
            else{
                error_msg("Invalid choice.");
                continue;
            }

            int idx;

while(1){   // ADDED LOOP

    idx = find_in_filtered(indices,found,opt2,sub);// find specific contact in filtered list

    if(idx==-1){
        error_msg("Contact not found in filtered results.\n");
        error_msg("Try again!.\n");
        //  ask again
        if(opt2==2){
            while(1){
                printf("Enter Phone: ");
                scanf("%s",sub);
                while(getchar()!='\n');
                if(valid_phone(sub)==SUCCESS) break;
            }
        }
        else if(opt2==3){
            while(1){
                printf("Enter Email: ");
                scanf("%s",sub);
                while(getchar()!='\n');
                if(valid_email(sub)==SUCCESS) break;
            }
        }

        continue;   // retry
    }

    break;   // correct input → exit loop
}
            success_msg("Contact found.\n");
            printf("\nCurrent Data:\n");
            printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                   contacts[idx].name,
                   contacts[idx].phone,
                   contacts[idx].email);

            printf("\nWhat you want to edit:\n");
            printf("1.Name\n2.Phone\n3.Email\n4.All\nChoice: ");
            scanf("%d",&opt);
            while(getchar()!='\n');

            if(opt==1||opt==4){   // edit name
                while(1){
                    printf("New Name: ");
                    scanf(" %[^\n]",contacts[idx].name);
                    if(valid_name(contacts[idx].name)==SUCCESS)
                        break;
                }
                while(getchar()!='\n');
            }

            if(opt==2||opt==4){   // edit phone
                while(1){
                    printf("New Phone: ");
                    scanf("%s",contacts[idx].phone);
                    while(getchar()!='\n');

                    if(valid_phone(contacts[idx].phone)==FAILURE) continue;

                    if(phone_exists(contacts[idx].phone,idx)==SUCCESS){
                        error_msg("Phone already exists.");
                        continue;
                    }
                    break;
                }
            }

            if(opt==3||opt==4){   // edit email
                while(1){
                    printf("New Email: ");
                    scanf("%s",contacts[idx].email);
                    while(getchar()!='\n');

                    if(valid_email(contacts[idx].email)==FAILURE) continue;

                    if(email_exists(contacts[idx].email,idx)==SUCCESS){
                        error_msg("Email already exists.");
                        continue;
                    }
                    break;
                }
            }

            sort_contacts();   // sort updated list
            success_msg("Contact updated.");

            printf("\nPress ENTER...");
            getchar();
        }

        else if(ch==2 || ch==3){   // search by phone or email

            if(ch==2){   // phone
                while(1){
                    printf("Enter Phone: ");
                    scanf("%s",key);
                    while(getchar()!='\n');
                    if(valid_phone(key)==SUCCESS) break;
                }
            }
            else{   // email
                while(1){
                    printf("Enter Email: ");
                    scanf("%s",key);
                    while(getchar()!='\n');
                    if(valid_email(key)==SUCCESS) break;
                }
            }

            int idx = find_contact(ch,key);   // find contact

            if(idx==-1){
                error_msg("Contact not found.");
                continue;
            }
            success_msg("Contact found.");
            printf("\nCurrent Data:\n");//printing the found contact
            printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                   contacts[idx].name,
                   contacts[idx].phone,
                   contacts[idx].email);
            
            printf("\nWhat you want to edit:\n");       
            printf("\n1.Name\n2.Phone\n3.Email\n4.All\nChoice: ");
            scanf("%d",&opt);
            while(getchar()!='\n');

            if(opt==1||opt==4){   // edit name
                while(1){
                    printf("New Name: ");
                    scanf(" %[^\n]",contacts[idx].name);
                    if(valid_name(contacts[idx].name)==SUCCESS)//checking valid or not
                        break;
                }
                while(getchar()!='\n');
            }

            if(opt==2||opt==4){   // edit phone
                while(1){
                    printf("New Phone: ");
                    scanf("%s",contacts[idx].phone);
                    while(getchar()!='\n');

                    if(valid_phone(contacts[idx].phone)==FAILURE) continue;//checking valid or not

                    if(phone_exists(contacts[idx].phone,idx)==SUCCESS){//checking phone alredy exits or not
                        error_msg("Phone already exists.");
                        continue;
                    }
                    break;
                }
            }

            if(opt==3||opt==4){   // edit email
                while(1){
                    printf("New Email: ");
                    scanf("%s",contacts[idx].email);
                    while(getchar()!='\n');

                    if(valid_email(contacts[idx].email)==FAILURE) continue;//chacking valid or not

                    if(email_exists(contacts[idx].email,idx)==SUCCESS){//checking already exits
                        error_msg("Email already exists.");
                        continue;
                    }
                    break;
                }
            }

            sort_contacts();   // sort after editing
            success_msg("Contact updated.");

            printf("\nPress ENTER...");
            getchar();
        }

        else{
            error_msg("Invalid choice.");   // invalid menu option
        }
    }
}
void delete_contact() {
    int ch;                     // stores user menu choice (Name / Phone / Email / Back)
    char key[50];               // input key for searching (name/phone/email)

    while(1){                   // infinite loop to repeatedly show delete menu
        header("DELETE CONTACT");   // display header title
        
        printf("Search by :\n");
        // menu options for deleting contact
        printf("1. Name\n2. Phone\n3. Email\n4. Back\nChoice: ");

        // validate integer input
        if(scanf("%d",&ch)!=1){
            error_msg("Invalid input");   // show error if input is not integer
            while(getchar()!='\n');       // clear invalid input from buffer
            continue;                    // restart loop
        }
        while(getchar()!='\n');           // clear leftover newline

        if(ch==4) return;                 // exit function if user selects Back

        int idx = -1;                    // index of contact to delete (initialized as not found)

    
        if(ch==1){                       // SEARCH BY NAME

            // take valid name input
            while(1){
                printf("Enter Name: ");
                scanf(" %[^\n]",key);    // read full name including spaces
                if(valid_name(key)==SUCCESS) break;  // validate name
            }
            while(getchar()!='\n');      // clear buffer

            int indices[MAX], found=0;   // array to store matched indices

            // search all contacts for matching name
            for(int i=0;i<count;i++){
                if(strcmp(contacts[i].name,key)==0){
                    indices[found++] = i;   // store index if match found
                }
            }

            if(found==0){   // no contact found
                error_msg("Contact not found.");
                continue;
            }

            // CASE: ONLY ONE MATCH FOUND
            if(found == 1){
                idx = indices[0];   // directly select that contact

                success_msg("Contact found.");

                // display full contact details
                printf("Name  : %s\nPhone : %s\nEmail : %s\n",
                       contacts[idx].name,
                       contacts[idx].phone,
                       contacts[idx].email);

                // show selected contact again clearly
                printf("\nSelected Contact:\n");
                printf("Name  : %s\n", contacts[idx].name);
                printf("Phone : %s\n", contacts[idx].phone);
                printf("Email : %s\n", contacts[idx].email);

                char confirm;   // confirmation input

                // ask user confirmation before deleting
                printf("\nAre you sure you want to delete? (y/n): ");
                scanf(" %c",&confirm);
                while(getchar()!='\n');   // clear buffer

                // if user confirms deletion
                if(confirm=='y' || confirm=='Y'){

                    // shift all elements left to overwrite deleted contact
                    for(int i=idx;i<count-1;i++)
                        contacts[i] = contacts[i+1];

                    count--;   // reduce total contact count
                    success_msg("Contact deleted successfully.");
                }
                else{
                    printf("[INFO] Deletion cancelled.\n");   // user cancelled
                }

                printf("\nPress ENTER to continue...");
                getchar();

char again;
printf("\nDo you want to delete another contact? (y/n): ");
scanf(" %c",&again);
while(getchar()!='\n');

if(again=='y'||again=='Y')
    continue;
else
    return;     // exit after deletion
            }

            // CASE: MULTIPLE MATCHES FOUND → DISPLAY TABLE
            line();   // print separator line
            printf("| %-3s | %-20s | %-12s | %-25s |\n",
                   "No","Name","Phone","Email");
            line();

            // display all matched contacts
            for(int i=0;i<found;i++){
                int id = indices[i];
                printf("| %-3d | %-20s | %-12s | %-25s |\n",
                       i+1,
                       contacts[id].name,
                       contacts[id].phone,
                       contacts[id].email);
            }
            line();

            int opt;

            // ask user to further filter results
            printf("\nSearch within results using Phone or Email:\n2.Phone\n3.Email\nChoice: ");
            if(scanf("%d",&opt)!=1){
                error_msg("Invalid input");
                while(getchar()!='\n');
                continue;
            }
            while(getchar()!='\n');

            char sub[50];   // sub key for filtering

            if(opt==2){   // filter by phone
                while(1){
                    printf("Enter Phone: ");
                    scanf("%s",sub);
                    while(getchar()!='\n');
                    if(valid_phone(sub)==SUCCESS) break;
                }
            }
            else if(opt==3){   // filter by email
                while(1){
                    printf("Enter Email: ");
                    scanf("%s",sub);
                    while(getchar()!='\n');
                    if(valid_email(sub)==SUCCESS) break;
                }
            }
            else{
                error_msg("Invalid choice.");
                continue;
            }

            // find specific contact in filtered list
            while(1){

    idx = find_in_filtered(indices,found,opt,sub);

    if(idx==-1){
        error_msg("Contact not found in filtered results.\n");
        error_msg("Try again!.\n");
        if(opt==2){
            while(1){
                printf("Enter Phone: ");
                scanf("%s",sub);
                while(getchar()!='\n');
                if(valid_phone(sub)==SUCCESS) break;
            }
        }
        else if(opt==3){
            while(1){
                printf("Enter Email: ");
                scanf("%s",sub);
                while(getchar()!='\n');
                if(valid_email(sub)==SUCCESS) break;
            }
        }

        continue;   // retry
    }

    break;   // success
}
        }

        else if(ch==2){   // SEARCH BY PHONE
            while(1){
                printf("Enter Phone: ");
                scanf("%s",key);
                while(getchar()!='\n');
                if(valid_phone(key)==SUCCESS) break;
            }

            idx = find_contact(2,key);   // find contact using phone
        }

        else if(ch==3){   // SEARCH BY EMAIL
            while(1){
                printf("Enter Email: ");
                scanf("%s",key);
                while(getchar()!='\n');
                if(valid_email(key)==SUCCESS) break;
            }

            idx = find_contact(3,key);   // find contact using email
        }

        else{
            error_msg("Invalid choice.");   // invalid menu option
            continue;
        }

        // if contact not found
        if(idx==-1){
            error_msg("Contact not found.");
            continue;
        }

        // display selected contact details
        printf("\nSelected Contact:\n");
        printf("Name  : %s\n", contacts[idx].name);
        printf("Phone : %s\n", contacts[idx].phone);
        printf("Email : %s\n", contacts[idx].email);

        char confirm;

        // confirmation before deletion
        printf("\nAre you sure you want to delete? (y/n): ");
        scanf(" %c",&confirm);
        while(getchar()!='\n');

        if(confirm=='y' || confirm=='Y'){
            // shift contacts to remove selected one
            for(int i=idx;i<count-1;i++)
                contacts[i] = contacts[i+1];

            count--;   // decrease total count
            success_msg("Contact deleted successfully.");
        }
        else{
            printf("[INFO] Deletion cancelled.\n");
        }

        printf("\nPress ENTER to continue...");
getchar();

char again;
printf("\nDo you want to delete another contact? (y/n): ");
scanf(" %c",&again);
while(getchar()!='\n');

if(again=='y'||again=='Y')
    continue;
else
    return;  // exit after deletion attempt
    }
}

/* ---------------- MENU ---------------- */

void menu() {
    header("ADDRESS BOOK SYSTEM");   // display the main title/header of the program

    // display menu options for user
    printf("1. Add Contact\n");            // option to add a new contact
    printf("2. List Contacts\n");          // option to display all saved contacts
    printf("3. Search Contact\n");         // option to search a contact (by name/phone/email)
    printf("4. Edit Contact\n");           // option to modify existing contact details
    printf("5. Delete Contact\n");         // option to remove a contact
    printf("6. Save Contacts\n");          // option to save contacts to file without exiting
    printf("7. Save and Exit\n");          // option to save contacts and then exit program
    printf("8. Exit Without Saving\n");    // option to exit program without saving changes

    line();   // print a separator line for better UI formatting
}