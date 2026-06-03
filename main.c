/*
===========================================================
                ADDRESS BOOK SYSTEM (C PROGRAM)
===========================================================

Name        : Rakshith C
Date        : 08-04-2026
Batch No    : 26001B

Description :
-------------
This project is an Address Book System implemented in C.
It allows users to manage contacts efficiently using a
menu-driven interface.

Features:
---------
1. Add Contact
   - Stores name, phone number, and email
   - Validates input fields
   - Prevents duplicate phone/email

2. List Contacts
   - Displays all contacts in tabular format

3. Search Contact
   - Search by name, phone, or email
   - Supports multiple matches
   - Allows filtered search within results

4. Edit Contact
   - Modify name, phone, or email
   - Ensures validation and uniqueness

5. Delete Contact
   - Delete contact with confirmation
   - Supports multiple match filtering
   - Option to delete multiple contacts

6. File Handling
   - Load contacts from file
   - Save contacts to file

7. Sorting
   - Contacts are sorted alphabetically (case-insensitive)
===========================================================
*/
#include <stdio.h>      // for input/output functions like printf, scanf
#include "contact.h"    // include your custom header (all functions & structures)

int main() {
    int choice;   // variable to store user menu choice

    load();       // load contacts from file into memory when program starts

    while(1){     // infinite loop → keeps program running until user exits
        menu();   // display menu options

        printf("Enter choice: ");

        // validate user input (must be integer)
        if(scanf("%d",&choice)!=1){
            error_msg("Invalid input.");   // show error if input is not number
            while(getchar()!='\n');        // clear invalid input from buffer
            continue;                     // restart loop
        }

        while(getchar()!='\n');   // clear leftover newline character

        // perform action based on user's choice
        switch(choice){

            case 1: 
                add_contact();   // call function to add new contact
                break;

            case 2: 
                list_contacts();   // display all contacts
                break;

            case 3: 
                search_contact();   // search for a contact
                break;

            case 4: 
                edit_contact();   // edit existing contact
                break;

            case 5: 
                delete_contact();   // delete a contact
                break;

            case 6: 
                save();   // save contacts to file (without exiting)
                break;

            case 7:
                save();   // save contacts before exiting
                printf("Saved & Exiting...\n");
                return 0;   // terminate program successfully

            case 8:
                printf("Exiting without saving...\n");
                return 0;   // exit program without saving

            default:
                error_msg("Invalid choice.");   // handle wrong menu option
        }
    }
}