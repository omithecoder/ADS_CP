#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 100
#define MAX_FRIENDS 100
#define MAX_USERS 100



void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// structure
int num=1;

// User structure


typedef struct User {
    char name[MAX_STRING_LEN];
    char email[MAX_STRING_LEN];
    char Bio[MAX_STRING_LEN];
    char gender[MAX_STRING_LEN];
    char dob[MAX_STRING_LEN];
    char location[MAX_STRING_LEN];
    char post[MAX_STRING_LEN];
    // Add more fields as needed
} User;



// Graph node structure
typedef struct UserNode {
    User userData;
    struct UserNode* friends[MAX_USERS]; // Array to store pointers to friends
    int numFriends;  // Number of friends for efficient traversal
    struct UserNode* next;
} UserNode;

// Array to store the head of each linked list (one linked list per user)
UserNode* userList[MAX_USERS] = {NULL};


int authenticate(const char *username, const char *password)
{
    FILE *file = fopen("user.txt", "r");

    if (file == NULL)
    {
        // File doesn't exist, so authentication fails
        return 0;
    }

    char storedUsername[MAX_STRING_LEN];
    char storedPassword[MAX_STRING_LEN];

    // Loop through the file to find the matching username and password
    while (fscanf(file, "%s %s", storedUsername, storedPassword) == 2)
    {
        // Compare the entered username and password with the ones in the file
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
        {
            fclose(file);
            return 1; // Authentication successful
        }
    }

    fclose(file);

    return 0; // Authentication fails
}




void storeCredentials(const char *username, const char *password)
{
    FILE *file = fopen("user.txt", "a");

    if (file == NULL)
    {
        // Error opening the file
        printf("Error opening file for writing.\n");
        return;
    }

    // Write username and password to the file
    fprintf(file, "%s %s\n", username, password);

    fclose(file);
}

int isUsernameExists(const char *username)
{
    FILE *file = fopen("user.txt", "r");

    if (file == NULL)
    {
        // File doesn't exist, so username doesn't exist
        return 0;
    }

    char storedUsername[MAX_STRING_LEN];

    // Loop through the file to find the matching username
    while (fscanf(file, "%s", storedUsername) == 1)
    {
        // Compare the entered username with the one in the file
        if (strcmp(username, storedUsername) == 0)
        {
            fclose(file);
            return 1; // Username exists
        }
    }

    fclose(file);

    return 0; // Username doesn't exist
}


// graph
UserNode* createUserNode(const char* name, const char* mail,const char* bio,const char* gen,const char* dob,const char* location) {
    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    if (newNode != NULL) {
        strncpy(newNode->userData.name, name, MAX_STRING_LEN);
        strncpy(newNode->userData.email,mail, MAX_STRING_LEN);
        strncpy(newNode->userData.Bio, bio, MAX_STRING_LEN);
        strncpy(newNode->userData.gender, gen, MAX_STRING_LEN);
        strncpy(newNode->userData.dob, dob, MAX_STRING_LEN);
        strncpy(newNode->userData.location, location, MAX_STRING_LEN);
        strcpy(newNode->userData.post,"\0");
        newNode->numFriends = 0;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to add a user to the graph
void addUserToGraph(const char* name, const char* mail,const char* bio,const char* gen,const char* dob,const char* location) {
    int index = num ++;

    UserNode* newNode = createUserNode(name,mail,bio,gen,dob,location);

    // Insert at the beginning of the linked list
    newNode->next = userList[index];
    userList[index] = newNode;
}

// Function to find a user by name
UserNode* findUserByName(const char* name) {
    for (int i = 0; i < MAX_USERS; ++i) {
        UserNode* current = userList[i];
        while (current != NULL) {
            if (strcmp(current->userData.name, name) == 0) {
                return current;  // User found
            }
            current = current->next;
        }
    }
    return NULL;  // User not found
}

// Function to add a friend
void addFriend(const char* userName1, const char* userName2) {
    int a;
    UserNode* User = findUserByName(userName1);
    UserNode* user2 = findUserByName(userName2);

    if (User != NULL && user2 != NULL) {
        // Both users exist, add friendship
        if (User->numFriends < MAX_USERS && user2->numFriends < MAX_USERS) {
            User->friends[User->numFriends++] = user2;
            user2->friends[user2->numFriends++] = User;
            printf("\n\t\t\t\t\t\tFriendship added between %s and %s.\n", userName1, userName2);
        } else {
            printf("\n\t\t\t\t\t\tFriendship not added. Maximum number of friends reached for one or both users.\n");
        }
    } else {
        printf("\n\t\t\t\t\t\tOne or both users not found. Please check the names and try again.\n");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
}

// Function to display friends of a user
void displayFriends(const char* userName) {
    int a;
    UserNode* user = findUserByName(userName);

    if (user != NULL) {
        printf("\n\t\t\t\t\t\tFriends of %s are:\n", userName);
        for (int i = 0; i < user->numFriends; ++i) {
            printf("\n\t\t\t\t\t\t%s\n", user->friends[i]->userData.name);
        }
    } else {
        printf("\n\t\t\t\t\t\tUser not found. Please check the name and try again.\n");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
}

// Function to display all users in the graph
void displayUsers() {
    int a;
    printf("\n\t\t\t\t\t\tDisplaying all users:\n");
    for (int i = 0; i < MAX_USERS; ++i) {
        UserNode* current = userList[i];
        while (current != NULL) {
            printf("\n\t\t\t\t\t\tName: %s\n\t\t\t\t\t\t Bio: %s\n\t\t\t\t\t\t Email: %s\n\t\t\t\t\t\tgender: %s\n\t\t\t\t\t\tDate of Birth: %s\n\t\t\t\t\t\tLocation: %s\n  ", current->userData.name, current->userData.Bio,current->userData.email,current->userData.gender,current->userData.dob,current->userData.location);
            printf("\n\t\t\t\t\t\t______________________POST_______________________\n");
            printf("\n\t\t\t\t\t\t%s",current->userData.post);

            current = current->next;
        }
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
    
}

// Function to check if a user is already a friend
int isFriend(UserNode* user, UserNode* potentialFriend) {
    for (int i = 0; i < user->numFriends; ++i) {
        if (user->friends[i] == potentialFriend) {
            return 1;  // Already a friend
        }
    }
    return 0;  // Not a friend
}


//function to recommend friends
void recommendFriends(const char* userName) {
    int a;
    UserNode* user = findUserByName(userName);

    if (user != NULL) {
        printf("\n\t\t\t\t\t\tRecommended friends for %s are:\n", userName);

        // Iterate through user's friends
        for (int i = 0; i < user->numFriends; ++i) {
            UserNode* friendUser = user->friends[i];

            // Iterate through friend's friends
            for (int j = 0; j < friendUser->numFriends; ++j) {
                UserNode* friendOfFriend = friendUser->friends[j];

                // Check if the friend of a friend is not already a friend of the user
                if (!isFriend(user, friendOfFriend) && friendOfFriend != user) {
                    printf("\n\t\t\t\t\t\t%s\n", friendOfFriend->userData.name);
                }
            }
        }
    } else {
        printf("\n\t\t\t\t\t\tUser not found. Please check the name and try again.\n");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
}

//function to add post 
void addpost(char *name)
{
    int a;
    char post[100];
    UserNode* user = findUserByName(name);
    if(user!=NULL)
    {
        printf("\n\t\t\t\t\t\tEnter the post : ");
        printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t    ");
        printf("\n||\t\t\t\t\t\t|  Enter Your new Post here     |                                              |          \t\t\t\t    ");
        printf("\n||\t\t\t\t\t\t|                               |                                              |          \t\t\t\t    ");
        printf("\n||\t\t\t\t\t\t|                               |                                              |          \t\t\t\t    ");
        printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t    ");
        gotoxy(83,11);
        scanf("\t%[^\n]", post);
        strcat(post,"\n\t\t\t\t\t\t");
        strcat(user->userData.post,post);
        printf("\n\n\n\t\t\t\t\t\tPost Added Successfully!");
        
    }
    else
    {
        printf("\n\t\t\t\t\t\tNo user Found");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
}

void Myprofile(const char *u)
{
    int a;
    UserNode* user = findUserByName(u);
    if(user!=NULL)
    {
        printf("\t\t\t\t\t\t---------------------------MY Profile---------------------------\n\n");
    printf("\n||\t\t\t\t\t\t\t\t\t           .@@@@@@@.       ");    
    printf("\n||\t\t\t\t\t\t\t\t\t          @@@@@@@@@@@      ");    
    printf("\n||\t\t\t\t\t\t\t\t\t         +@@@@@@@@@@@*     ");    
    printf("\n||\t\t\t\t\t\t\t\t\t         @@@@@@@@@@@@@     ");             
    printf("\n||\t\t\t\t\t\t\t\t\t          @@@@@@@@@@@      ");    
    printf("\n||\t\t\t\t\t\t\t\t\t            @@@@@@@        ");    
    printf("\n||\t\t\t\t\t\t\t\t\t                           ");    
    printf("\n||\t\t\t\t\t\t\t\t\t       #@@@@@@@#@@@@@@@#   ");    
    printf("\n||\t\t\t\t\t\t\t\t\t     @@@@@@@@@@@@@@@@@@@@@ ");      
    printf("\n||\t\t\t\t\t\t\t\t\t    @@@@@@@@@@@@@@@@@@@@@@@");    
    printf("\n||\t\t\t\t\t\t\t\t\t    @@@@@@@@@@@@@@@@@@@@@@@");    
    printf("\n||\t\t\t\t\t\t\t\t\t    *@@@@@@@@@@@@@@@@@@@@@*");    
    printf("\n||\t\t\t\t\t\t\t\t\t       .#@@@@@@@@@@@@@@.   ");
    printf("\n\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your Full Name        |  %s                                            |          \t\t\t\t ",user->userData.name);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your Email Address    |  %s                                            |          \t\t\t\t ",user->userData.email);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your location/Country |  %s                                            |          \t\t\t\t ",user->userData.location);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your Date of Birth    |  %s                                            |          \t\t\t\t ",user->userData.dob);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your profile Bio      |  %s                                            |          \t\t\t\t ",user->userData.Bio);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t|  Enter Your gender           |  %s                                            |          \t\t\t\t ",user->userData.gender);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);

}

void explore(const char* usname)
{
    int a;
    UserNode* user = findUserByName(usname);
    if(user!=NULL)
    {
        printf("\n||\t\t\t\t\t\t\t\t\t           .@@@@@@@.       ");    
    printf("\n||\t\t\t\t\t\t\t\t\t          @@@@@@@@@@@      ");    
    printf("\n||\t\t\t\t\t\t\t\t\t         +@@@@@@@@@@@*     ");    
    printf("\n||\t\t\t\t\t\t\t\t\t         @@@@@@@@@@@@@     ");             
    printf("\n||\t\t\t\t\t\t\t\t\t          @@@@@@@@@@@      ");    
    printf("\n||\t\t\t\t\t\t\t\t\t            @@@@@@@        ");    
    printf("\n||\t\t\t\t\t\t\t\t\t                           ");    
    printf("\n||\t\t\t\t\t\t\t\t\t       #@@@@@@@#@@@@@@@#   ");    
    printf("\n||\t\t\t\t\t\t\t\t\t     @@@@@@@@@@@@@@@@@@@@@ ");      
    printf("\n||\t\t\t\t\t\t\t\t\t    @@@@@@@@@@@@@@@@@@@@@@@");    
    printf("\n||\t\t\t\t\t\t\t\t\t    @@@@@@@@@@@@@@@@@@@@@@@");    
    printf("\n||\t\t\t\t\t\t\t\t\t    *@@@@@@@@@@@@@@@@@@@@@*");    
    printf("\n||\t\t\t\t\t\t\t\t\t       .#@@@@@@@@@@@@@@.   ");
    printf("\n\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           Full Name        |  %s                                        \t          \t\t\t\t ",user->userData.name);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           Email Address    |  %s                                        \t          \t\t\t\t ",user->userData.email);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           location/Country |  %s                                        \t          \t\t\t\t ",user->userData.location);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           Date of Birth    |  %s                                        \t          \t\t\t\t ",user->userData.dob);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           profile Bio      |  %s                                        \t          \t\t\t\t ",user->userData.Bio);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           gender           |  %s                                        \t          \t\t\t\t ",user->userData.gender);
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t           post                                                                        \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t                    %s                                                                 \t\t\t\t ",user->userData.post);
    printf("\n||\t\t\t\t\t\t                                                                                       \t\t\t\t ");
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t ");
    }
    printf("\n\t\t\t\t\t\tPress '1' to Continue!");
    scanf("%d",&a);
    }


























void intropage()
{
    gotoxy(5,6);
printf("\n\t               @@@@@   ");         
printf("\n\t       @@@@@ @@@  @@@@ ");         
printf("\n\t     @@@   @@@      @@@");         
printf("\n\t   @@@   @@@ @@     @@@");         
printf("\n\t @@@    @@   @@    @@@ ");         
printf("\n\t@@@     @@ @@@   @@@   ");         
printf("\n\t@@@      @@@   @@@     ");         
printf("\n\t @@@@  @@@  @@@@       ");         
printf("\n\t   @@@@@               ");
    gotoxy(10,3);
        printf("\n==========================================================================================================================================================");
    printf("\n||\t\t\t\t                                                                                                        \t\t||");
    printf("\n||\t\t\t\t                                                                                                        \t\t||");
    printf("\n||\t\t\t\t                                                                                                        \t\t||");
    printf("\n||\t\t\t\t                                                                              :@@:                  @@@@\t\t||");       
    printf("\n||\t\t\t\t                                                                         =@@@@=                 @@@@    \t\t||");   
    printf("\n||\t\t\t\t                                                                          *@@*                  @@@@    \t\t||");   
    printf("\n||\t\t\t\t                                .*@@@@*.      .+@@@@#-        .*@@@@#-    ###*     +@@@@*.*###. @@@@    \t\t||");   
    printf("\n||\t\t\t\t                               @@@@@@@@@@   :@@@@@@@@@@#    -@@@@@@@@@@+ .@@@@   #@@@@@@@@@@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                              :@@@@. ++    :@@@@#:.-@@@@@  =@@@@*:.+@@*. .@@@@  @@@@@-  =@@@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                               @@@@@@@@+   @@@@-     @@@@- @@@@-         .@@@@ :@@@@      @@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                                 :#@@@@@@. @@@@+     @@@@: @@@@=         .@@@@ :@@@@     .@@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                              .#@@#--@@@@=  @@@@@@*@@@@@*  :@@@@@@#@@@@+ .@@@@  +@@@@@+*@@@@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                              .@@@@@@@@@#    *@@@@@@@@@-     #@@@@@@@@@: .@@@@   -@@@@@@@@@@@@: @@@@    \t\t||");   
    printf("\n||\t\t\t\t                                 :=++=:        .-++=:          .=++=:     ::::      -++=. ::::  ::::    \t\t||");   
    printf("\n||\t\t\t\t                                                                                                   .    \t\t||");   
    printf("\n||\t\t\t\t                                                                                                @@@@    \t\t||");   
    printf("\n||\t\t\t\t        :*#+:          -*#+.       ...  =**-     ...  .+#+.         -*#+.          .=**=      ..@@@@..  \t\t||");   
    printf("\n||\t\t\t\t     *@@@@@@@@@=    #@@@@@@@@@-   @@@@@@@@@@@@. =@@@@@@@@@@@=    #@@@@@@@@@:    .@@@@@@@@@@  :@@@@@@@@* \t\t||");   
    printf("\n||\t\t\t\t   .@@@@@*+#@@@+  .@@@@@+=#@@@@#  @@@@@=:-@@@@@ =@@@@@-:*@@@@= .@@@@*::-@@@@+  +@@@@@++@@@@: .@@@@@@@@= \t\t||");   
    printf("\n||\t\t\t\t   +@@@#          #@@@+     @@@@- @@@@-   :@@@@ =@@@@    +@@@+ *@@@@#####@@@@..@@@@.            @@@@    \t\t||");   
    printf("\n||\t\t\t\t   +@@@*          @@@@=     #@@@= @@@@:   .@@@@ =@@@#    +@@@+ #@@@@@@@@@@@@@..@@@@.            @@@@    \t\t||");   
    printf("\n||\t\t\t\t   .@@@@@:.-@@@:  :@@@@#:.-@@@@@  @@@@:   .@@@@ =@@@#    +@@@+ :@@@@-   :+.    #@@@@+.:*@@=     @@@@=.  \t\t||");   
    printf("\n||\t\t\t\t     @@@@@@@@@@#   .@@@@@@@@@@#   @@@@:   .@@@@ =@@@#    +@@@+  .@@@@@@@@@@@.   =@@@@@@@@@@:    #@@@@@* \t\t||");   
    printf("\n||\t\t\t\t      -@@@@#:        -@@@@*:     =***.    ***+ :***=    -***-     -@@@@@=        .+@@@@=         -*##   \t\t||");


}
void printGetStartedButton()
{
    gotoxy(-50,6);
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t\t\t\t======================================                                                          ||");
    printf("\n||\t\t\t\t\t\t\t||            Press  Enter          ||                                                          ||");
    printf("\n||\t\t\t\t\t\t\t======================================                                                          ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n||\t\t\t\t                                                                                                    \t\t        ||");
    printf("\n==========================================================================================================================================================");
}
int displayauthScreen()
{
    int a;
    COORD coord;
    coord.X = 10; // column
    coord.Y = 3;  // row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("\n==============================================================================================================================================================");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");

printf("\n||\t\t\t\t\t ____             _       _  ____                            _          \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t/ ___|  ___   ___(_) __ _| |/ ___|___  _ __  _ __   ___  ___| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t\\___ \\ / _ \\ / __| |/ _` | | |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __|        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t ___) | (_) | (__| | (_| | | |__| (_) | | | | | | |  __/ (__| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t|____/ \\___/ \\___|_|\\__,_|_|\\____\\___/|_| |_|_| |_|\\___|\\___|\\__|       \t\t\t\t\t     ||");
                                                                  

    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||-----------------------------------------------------------------------------------------------------------------------------------------------------------||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    COORD coord1;
    coord.X = 15; // column
    coord.Y = 9;  // row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord1);
    printf("\n||                           @@@@@@@@@                         \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                         @@@@@@@@@@@@@                       \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                        @@@@@@@@@@@@@@@                      \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                       @@@@@@@@@@@@@@@@@                     \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                        @@@@@@@@@@@@@@@                      ==================================\t\t\t\t\t\t             ||");
    printf("\n||                          @@@@@@@@@@@                        || (press 1)  Log In            ||\t\t\t\t\t\t             ||");
    printf("\n||                            @@@@@@@                          ==================================\t\t\t\t\t\t             ||");
    printf("\n||                       @@@@@@@   @@@@@@@                     || (press 2)  Sign up           ||\t\t\t\t\t\t             ||");
    printf("\n||                   @@@@@@@@@@@@@@@@@@@@@@@@@                 ==================================\t\t\t\t\t\t             ||");
    printf("\n||                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@               || (press 3)  Exit              ||\t\t\t\t\t\t             ||");
    printf("\n||                @@@@@@@@@@@@       @@@@@@@@@@@@              ==================================\t\t\t\t\t\t             ||");
    printf("\n||                @@@@@@@@@  @@@@@@@@@  @@@@@@@@@              \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                @@@@@@  @@@@@ @@@ @@@@@  @@@@@@              \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                @@@@@  @@@@@@ @ @ @@@@@@  @@@@@              \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                  @@@  @@@@@       @@@@@  @@@                \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                       @@@@@   @   @@@@@                     \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                       @@@@@   @   @@@@@                     \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                        @@@@@@@@@@@@@@@                      \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||                           @@@@@@@@@                         \t\t\t\t\t\t\t\t\t\t\t             ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n==============================================================================================================================================================");
    fflush(stdin);
    scanf("%d", &a);
    return a;
}

char *displayLoginScreen()
{
   
    char username[100];
    char password[100];
    int i=8000;
    char userid[100];
    char* str = (char*)malloc(strlen("NuLL") + 1);
    strcpy(str, "NuLL");
    COORD coord;
    coord.X = 10; // column
    coord.Y = 3;  // row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("\n==============================================================================================================================================================");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");

printf("\n||\t\t\t\t\t ____             _       _  ____                            _          \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t/ ___|  ___   ___(_) __ _| |/ ___|___  _ __  _ __   ___  ___| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t\\___ \\ / _ \\ / __| |/ _` | | |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __|        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t ___) | (_) | (__| | (_| | | |__| (_) | | | | | | |  __/ (__| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t|____/ \\___/ \\___|_|\\__,_|_|\\____\\___/|_| |_|_| |_|\\___|\\___|\\__|       \t\t\t\t\t     ||");
                                                                  

    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||-----------------------------------------------------------------------------------------------------------------------------------------------------------||");
    printf("\n||\t\t\t                          Please Enter Your Credentials to Login!                                   \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Username (ID)   |                                               |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Password        |                                               |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n==============================================================================================================================================================");

    gotoxy(65, 18);
    scanf(" %[^\n]", username); // Assuming you have a 'username' field in your user struct
    gotoxy(65, 20);
    scanf(" %[^\n]", password); // Assuming you have a 'password' field in your user struct
    char* u = (char*)malloc(strlen(username) + 1);
    strcpy(u,username); 
    COORD coord2, coord1;
    coord2.X = 70; // column
    coord2.Y = 25; // row
    coord1.X = 70; // column
    coord1.Y = 25; // row

    gotoxy(65, 24);
    printf("\n\t\t\t\t\t\t\tAuthenticating");

    while (1)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord1);
        for (int i = 0; i < 3; i++)
        {
            printf(".");
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord2);
        for (int j = 2; j >= 0; j--)
        {
            printf(" ");
        }
        i--;
        if (i == 0)
        {
            break;
        }
        
    }
    if (authenticate(username, password))
        {
            gotoxy(65, 27);
            printf("\n\t\t\t\t\t\tAuthentication Successful!");
            printf("%s",u);
            Sleep(2000);
            return u;
        }
        else
        {
            gotoxy(65, 27);
            printf("\n\t\t\t\t\t\tAuthentication Failed. Please check your credentials.");
            Sleep(3000);
            return str;
        }
        
        
}

void createidpass()
{
    char username[100];
    char password[100];
    int i = 8000;
    char confpass[100];
    User u1;
    COORD coord;
    coord.X = 10; // column
    coord.Y = 3;  // row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("\n==============================================================================================================================================================");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");

printf("\n||\t\t\t\t\t ____             _       _  ____                            _          \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t/ ___|  ___   ___(_) __ _| |/ ___|___  _ __  _ __   ___  ___| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t\\___ \\ / _ \\ / __| |/ _` | | |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __|        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t ___) | (_) | (__| | (_| | | |__| (_) | | | | | | |  __/ (__| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t|____/ \\___/ \\___|_|\\__,_|_|\\____\\___/|_| |_|_| |_|\\___|\\___|\\__|       \t\t\t\t\t     ||");
                                                                  

    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||-----------------------------------------------------------------------------------------------------------------------------------------------------------||");
    printf("\n||\t\t\t                          One Step To GO! Create Credentials for Login!                             \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Username (ID)   |                                               |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Set Your Password          |                                               |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Confirm Password           |                                               |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n=============================================================================================================================================================");
    // Move the cursor to a new position
l:
    gotoxy(65, 18);
    scanf(" %[^\n]", username); // Assuming you have a 'username' field in your user struct
k:
    gotoxy(65, 20);
    scanf(" %[^\n]", password); // Assuming you have a 'password' field in your user struct
    gotoxy(65, 22);
    scanf(" %[^\n]", confpass); // Assuming you have a 'password' field in your user struct

    if (isUsernameExists(username))
    {
        gotoxy(65, 27);
        printf("\n\t\t\t\t\t\tUsername already exists. Please choose a different username.");
        goto l;
    }
    if (strcmp(password, confpass))
    {
        gotoxy(65, 27);
        printf("\n\t\t\t\t\t\tConfirm Password and set password not matching");
        goto k;
    }

    COORD coord2, coord1;
    coord2.X = 67; // column
    coord2.Y = 25; // row
    coord1.X = 67; // column
    coord1.Y = 25; // row

    gotoxy(65, 24);
    printf("\n\t\t\t\t\t\t\tProcessing");

    while (1)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord1);
        for (int i = 0; i < 3; i++)
        {
            printf(".");
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord2);
        for (int j = 2; j >= 0; j--)
        {
            printf(" ");
        }
        i--;
        if (i == 0)
        {
            break;
        }
    }

     storeCredentials(username, password);
    gotoxy(65, 27);
    printf("\n\t\t\t\t\t\t                                              ");
    gotoxy(65, 27);
    printf("\n\t\t\t\t\t\tLogin password created Successfully!Node created Successfully!");
    Sleep(3000);
}

void registration()
{
        // int i = getCurrentSerialNumber();
    char name[MAX_STRING_LEN];
    char bio[MAX_STRING_LEN];
    char email[MAX_STRING_LEN];
    char gender[MAX_STRING_LEN];
    char dob[MAX_STRING_LEN];
    char location[MAX_STRING_LEN];
    COORD coord;
    coord.X = 10; // column
    coord.Y = 3;  // row
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("\n==============================================================================================================================================================");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");

printf("\n||\t\t\t\t\t ____             _       _  ____                            _          \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t/ ___|  ___   ___(_) __ _| |/ ___|___  _ __  _ __   ___  ___| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t\\___ \\ / _ \\ / __| |/ _` | | |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __|        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t ___) | (_) | (__| | (_| | | |__| (_) | | | | | | |  __/ (__| |_        \t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t|____/ \\___/ \\___|_|\\__,_|_|\\____\\___/|_| |_|_| |_|\\___|\\___|\\__|       \t\t\t\t\t     ||");
                                                                  

    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||-----------------------------------------------------------------------------------------------------------------------------------------------------------||");
    printf("\n||\t\t\t                                    PROFILE CREATION                                                \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Full Name        |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Email Address    |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your location/Country |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your Date of Birth    |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your profile Bio      |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t|  Enter Your gender           |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n||\t\t\t                                                                                                    \t\t\t\t     ||");
    printf("\n==============================================================================================================================================================");
    // Move the cursor to a new position
    gotoxy(65, 18);
    scanf(" %[^\n]", name); // Read until newline
    gotoxy(65, 20);
    scanf(" %[^\n]", email);
    // Check if the roll number already exists
    gotoxy(65, 22);
    scanf(" %[^\n]", location);
    gotoxy(65, 24);
    scanf(" %[^\n]", dob);
    gotoxy(65, 26);
    scanf(" %[^\n]", bio);
    gotoxy(65, 28);
    scanf(" %[^\n]", gender);
    addUserToGraph(name,bio,email,gender,dob,location);

}
// sartj

void image()
{
    printf("\n\t\t\t\t\t\t                                                      #@@@@@@@@                     ");       
printf("\n\t\t\t\t\t\t                                                   @@@@@@@@@@@@@@@-                 ");       
printf("\n\t\t\t\t\t\t                                                 @@@@=  .**-   :@@@@:               ");       
printf("\n\t\t\t\t\t\t                                               :@@@:   @@@@@@-    @@@#              ");       
printf("\n\t\t\t\t\t\t                                               @@@    -@@@@@@#     @@@*             ");       
printf("\n\t\t\t\t\t\t                                              @@@      @@@@@@       @@@             ");       
printf("\n\t\t\t\t\t\t                                              @@@        .:         @@@             ");       
printf("\n\t\t\t\t\t\t                    -*****=.                  @@@     =@@@@@@#      @@@             ");       
printf("\n\t\t\t\t\t\t               -@@@@@@@@@@@@@@@*              +@@    @@@@@@@@@@:    @@@             ");       
printf("\n\t\t\t\t\t\t             @@@@@@@@@@#@@@@@@@@@@:          .@@@@  #@@@@@@@@@@@   @@@-             ");       
printf("\n\t\t\t\t\t\t           @@@@@@-    ::      @@@@@@      +@@@@@@@@# +@@@@@@@@@@ =@@@-              ");       
printf("\n\t\t\t\t\t\t         :@@@@@    #@@@@@@*     +@@@@@ @@@@@@@@@:@@@@@:  -=-.  #@@@@                ");       
printf("\n\t\t\t\t\t\t        =@@@@     #@@@@@@@@@      @@@@@@@@@@#      +@@@@@@@@@@@@@@                  ");       
printf("\n\t\t\t\t\t\t       .@@@@      @@@@@@@@@@       @@@@@@+             *@@@@@@.                     ");       
printf("\n\t\t\t\t\t\t       @@@@-       @@@@@@@@         @@@@                                            ");       
printf("\n\t\t\t\t\t\t       @@@@         :@@@#:          @@@@                  -*@@@@@@@#+.              ");       
printf("\n\t\t\t\t\t\t       @@@@          -++-           +@@@              @@@@@@@@@@@@@@@@@@@+          ");       
printf("\n\t\t\t\t\t\t       @@@@       @@@@@@@@@@:       @@@@           #@@@@@@@@@@@@@@@@@@@@@@@@        ");       
printf("\n\t\t\t\t\t\t       @@@@-    @@@@@@@@@@@@@@.    .@@@@         @@@@@@@@@           -@@@@@@@@:     ");       
printf("\n\t\t\t\t\t\t       .@@@@   #@@@@@@@@@@@@@@@    @@@@-       =@@@@@@#    .@@@@@@-      @@@@@@@    ");       
printf("\n\t\t\t\t\t\t        =@@@@  #@@@@@@@@@@@@@@@   @@@@@@@-    @@@@@@-     @@@@@@@@@@       @@@@@@   ");       
printf("\n\t\t\t\t\t\t         :@@@@@  #@@@@@@@@@@@@  -@@@@@@@@@@@@@@@@@@      #@@@@@@@@@@@       -@@@@@  ");       
printf("\n\t\t\t\t\t\t           @@@@@@    =#@#+    @@@@@@  #@@@@@@@@@@@       @@@@@@@@@@@@         @@@@@ ");       
printf("\n\t\t\t\t\t\t            .@@@@@@@@@*+*#@@@@@@@@-       -@@@@@@        :@@@@@@@@@@          -@@@@#");       
printf("\n\t\t\t\t\t\t               =@@@@@@@@@@@@@@@+           +@@@@-          @@@@@@@@            @@@@@");       
printf("\n\t\t\t\t\t\t                    .+#@#*:                +@@@@:                              *@@@@");       
printf("\n\t\t\t\t\t\t                                           +@@@@:           #@@@@@.            +@@@@");       
printf("\n\t\t\t\t\t\t                                           +@@@@-       -@@@@@@@@@@@@+         @@@@@");       
printf("\n\t\t\t\t\t\t                                           .@@@@#      @@@@@@@@@@@@@@@@        @@@@@");       
printf("\n\t\t\t\t\t\t                                            @@@@@     @@@@@@@@@@@@@@@@@@      @@@@@-");       
printf("\n\t\t\t\t\t\t                                             @@@@@   @@@@@@@@@@@@@@@@@@@@    @@@@@@ ");       
printf("\n\t\t\t\t\t\t                                              @@@@@*  @@@@@@@@@@@@@@@@@@@   @@@@@@  ");       
printf("\n\t\t\t\t\t\t                                               @@@@@@:  =@@@@@@@@@@@@@@.  #@@@@@*   ");       
printf("\n\t\t\t\t\t\t                                                #@@@@@@@    .=#@@*-    :@@@@@@@.    ");       
printf("\n\t\t\t\t\t\t                                                  @@@@@@@@@@=      *@@@@@@@@@:      ");       
printf("\n\t\t\t\t\t\t                                                    +@@@@@@@@@@@@@@@@@@@@@@         ");       
printf("\n\t\t\t\t\t\t                                                       :#@@@@@@@@@@@@@@+            ");
}
void homepagegui(const char* client)
{
     printf("\n\n");
    printf("===============================================================================================================================================================================");
    printf("\n||\t\t\t\t\t|\t                                                                                        \t||   Welcome %s\t     ||",client);
    printf("\n||\t\t\t\t        |------------------------------------------------------------------------------------------------------------------------------------||");
printf("\n||              .....      \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||");     
printf("\n||         .-*@@@@@@@@@**-.     \t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||       .+@@@@@@@@@@@@@@@+.   \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||      -@@@@@@@+-.:+*@@@@@@-  \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||     -@@@@@@*.      *@@@@@@- \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||     @@@@@@@*       *@@@@@@@ \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||    -@@@@@@@@+.   .+@@@@@@@@-\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||    :@@@@@@@@@**+**@@@@@@@@@:\t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||     @@@@@@@         @@@@@@@ \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||     :@@@@:           :@@@@: \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||      :@@@#***********#@@@:  \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||        +@@@@@@@@@@@@@@@+    \t\t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||"); 
printf("\n||         :+#@@@@@@@@@#+:       \t|\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     ||");
printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tHomepage (press 1)\t\t|\t\t                                                                                              \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tAdd Post (press 2)\t\t|\t\t                                                                                              \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tExplore (press 3)\t\t|\t\t                                                                               \t\t\t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                      \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                      \t\t\t     ||");
    printf("\n||\tAdd Friends (press 4)\t\t|\t\t                                                                                              \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                      \t\t\t     ||");
    printf("\n||\tFriend List (press 5)\t\t|\t\t                                                                                              \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tFriend Recommendation (press 6) |\t\t\t\t                                                                                     \t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tMy Profile (press 7)\t\t|\t\t                                                                                              \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
     printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\tLogout (press 8)\t\t|\t\t                                                                            \t\t\t\t             ||");
    printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("\n||\t\t\t\t\t|\t                                                                                                  \t\t\t     ||");
    printf("===============================================================================================================================================================================");
}

void Homepage(char *client) {
int choice;

  char friend2[MAX_STRING_LEN];
fflush(stdin);
fflush(stdout);
 do {
    homepagegui(client);
    gotoxy(6,6);
    image();
    scanf("%d",&choice);
    clearScreen();
    

    


        switch (choice) {
            case 1:
            homepagegui(client);
            gotoxy(10,8);
                displayUsers();
                clearScreen();
                break;

            case 2:
            homepagegui(client);
            gotoxy(10,8);
            addpost(client);
            clearScreen();
                break;
            case 3:
            homepagegui(client);
            gotoxy(10,8);
            printf("\n\t\t\t\t\t\tEnter name of User : ");
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t\t\t|  Enter name of user           |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t     ||");
    gotoxy(83,11);
            scanf("\t\t%s",friend2);
                 explore(friend2);
                 clearScreen();
                break;
            case 4:
            homepagegui(client);
            gotoxy(10,8);
            printf("\n\t\t\t\t\t\tEnter name of User : ");
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t     ||");
    printf("\n||\t\t\t\t\t\t|  Enter name of user           |                                              |          \t\t\t\t     ||");
    printf("\n||\t\t\t\t\t\t===============================================================================          \t\t\t\t     ||");
            gotoxy(83,11);
            scanf("\t\t%s",friend2);
                 addFriend(client,friend2 );
                 clearScreen();
                break;
            case 5:
            homepagegui(client);
            gotoxy(10,8);
                displayFriends(client);
                clearScreen();
                break;
            case 6:
            homepagegui(client);
            gotoxy(10,8);
                recommendFriends(client);
                clearScreen();
                break;
            case 7:
            homepagegui(client);
            gotoxy(10,8);
                Myprofile(client);
                clearScreen();
                break;
            case 8:
            homepagegui(client);
                printf("Successfully Logout!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

}




int main()
{
    char user[100];
    int b=1;
    int ch;
    intropage();
    printGetStartedButton();
    getchar(); // Wait for Enter key press
    clearScreen();
    while(b)
    {
    
    ch = displayauthScreen();
    
   
    switch(ch)
    {
        case 1:
        clearScreen();
        strcpy(user, displayLoginScreen());
        if(strcmp(user,"NuLL"))
        {
            clearScreen();
            Homepage(user);
            clearScreen();
        }
        clearScreen();
        break;

        case 2:
        clearScreen();
        registration();
        clearScreen();
        createidpass();
        clearScreen();
        break;

        case 3:
        printf("\nYou are Exiting.....");
        for (int i = 0; i < MAX_USERS; ++i) {
        UserNode* current = userList[i];
        while (current != NULL) {
            UserNode* next = current->next;
            free(current);
            current = next;
        }
    }
        b=0;
        break;

        default:
        printf("\nInvalide Input!Please Enter Again!");
    }
    }
    return 0;


}