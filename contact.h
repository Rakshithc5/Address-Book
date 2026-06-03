#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>   // standard input/output functions (printf, scanf, etc.)

#define MAX 100      // maximum number of contacts that can be stored
#define WIDTH 73     // width used for UI formatting (lines, headers, etc.)

// enum to represent function return status
typedef enum {
    FAILURE,   // indicates failure (usually 0)
    SUCCESS    // indicates success (usually 1)
} Status;

// structure to store a single contact
typedef struct {
    char name[50];    // contact name (max 49 chars + null)
    char phone[15];   // phone number (string to handle leading zeros)
    char email[50];   // email address
} Contact;

// external declaration of global contact list
extern Contact contacts[MAX];   // array to store all contacts

// external declaration of contact count
extern int count;   // number of contacts currently stored


// ---------- UI HELPER FUNCTIONS ----------

// prints a horizontal line (for UI formatting)
void line();

// prints centered text within WIDTH
void center(const char *text);

// prints formatted header with title
void header(const char *title);

// prints success message (formatted)
void success_msg(const char *msg);

// prints error message (formatted)
void error_msg(const char *msg);


// ---------- VALIDATION FUNCTIONS ----------

// validates name (format rules defined in implementation)
Status valid_name(char *name);

// validates phone number (length, digits, etc.)
Status valid_phone(char *p);

// validates email format
Status valid_email(char *e);

// checks if string is lowercase (used internally)
Status is_lowercase(char *str);


// ---------- DUPLICATE CHECK FUNCTIONS ----------

// checks if phone already exists (skip = index to ignore during edit)
Status phone_exists(char *p, int skip);

// checks if email already exists (skip = index to ignore during edit)
Status email_exists(char *e, int skip);


// ---------- CORE FUNCTIONALITY ----------

// sorts contacts (usually alphabetically by name)
void sort_contacts();

// loads contacts from file into memory
void load();

// saves contacts from memory to file
void save();

// finds contact based on mode (1=name, 2=phone, 3=email)
int find_contact(int mode, char *key);

// adds a new contact
void add_contact();

// displays all contacts
void list_contacts();

// searches for contacts
void search_contact();

// edits existing contact
void edit_contact();

// deletes a contact
void delete_contact();

// displays menu options
void menu();

#endif   // end of include guard