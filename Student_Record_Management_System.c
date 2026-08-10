#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define FILE_NAME "students.dat"

// ANSI Color Codes for Terminal Visuals
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"

// Structure to define Student Record
typedef struct {
    char id[30];
    char name[100];
    char department[50];
    float cgpa;
    char phone[20];
} Student;

// Utility functions
void clearScreen() {
    system("cls");
}

void printHeader() {
    clearScreen();
    printf("\n");
    printf(COLOR_CYAN    "                  ================================================================\n" COLOR_RESET);
    printf(COLOR_CYAN    "                  ||                                                            ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||               #####  ######  #     #  #####                ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||              #     # #     # ##   ## #     #               ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||              #       #     # # # # # #                     ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||               #####  ######  #  #  #  #####                ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||                    # #   #   #     #       #               ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||              #     # #    #  #     # #     #               ||\n" COLOR_RESET);
    printf(COLOR_YELLOW "                  ||               #####  #     # #     #  #####                ||\n" COLOR_RESET);
    printf(COLOR_CYAN    "                  ||                                                            ||\n" COLOR_RESET);
    printf(COLOR_WHITE   "                  ||              STUDENT RECORD MANAGEMENT SYSTEM              ||\n" COLOR_RESET);
    printf(COLOR_CYAN    "                  ||                                                            ||\n" COLOR_RESET);
    printf(COLOR_CYAN    "                  ================================================================\n\n" COLOR_RESET);
}

// Function to check if ID already exists
int idExists(const char *checkID) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return 0;

    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, checkID) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// 1. Add New Record
void addRecord() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === ADD NEW STUDENT RECORD ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] Error opening file!\n" COLOR_RESET);
        system("pause");
        return;
    }

    Student s;
    printf(COLOR_YELLOW "                                  Enter Student ID (e.g., XXXX-X-XX-XXX): " COLOR_RESET);
    fgets(s.id, sizeof(s.id), stdin);
    s.id[strcspn(s.id, "\n")] = 0;

    if (idExists(s.id)) {
        printf(COLOR_RED "\n                                  [!] Student ID %s already exists! Operation cancelled.\n" COLOR_RESET, s.id);
        fclose(fp);
        system("pause");
        return;
    }

    printf(COLOR_YELLOW "                                  Enter Student Name: " COLOR_RESET);
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf(COLOR_YELLOW "                                  Enter Department: " COLOR_RESET);
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = 0;

    printf(COLOR_YELLOW "                                  Enter CGPA: " COLOR_RESET);
    scanf("%f", &s.cgpa);
    getchar();

    printf(COLOR_YELLOW "                                  Enter Phone Number: " COLOR_RESET);
    fgets(s.phone, sizeof(s.phone), stdin);
    s.phone[strcspn(s.phone, "\n")] = 0;

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf(COLOR_GREEN "\n                                  [+] Student Record Added Successfully!\n" COLOR_RESET);
    system("pause");
}

// 2. Display All Records
void displayAllRecords() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === ALL STUDENT RECORDS ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] No records found or database file missing!\n" COLOR_RESET);
        system("pause");
        return;
    }

    Student s;
    int count = 0;

    printf(COLOR_CYAN "%-20s | %-28s | %-12s | %-6s | %-15s\n" COLOR_RESET, "ID", "Name", "Department", "CGPA", "Phone");
    printf(COLOR_WHITE "----------------------------------------------------------------------------------------\n" COLOR_RESET);

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-20s | %-28s | %-12s | %-6.2f | %-15s\n", s.id, s.name, s.department, s.cgpa, s.phone);
        count++;
    }

    printf(COLOR_WHITE "----------------------------------------------------------------------------------------\n" COLOR_RESET);
    printf(COLOR_GREEN "Total Records Displayed: %d\n\n" COLOR_RESET, count);

    fclose(fp);
    system("pause");
}

// 3. Search Record by ID
void searchRecord() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === SEARCH STUDENT RECORD ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] Database file not found!\n" COLOR_RESET);
        system("pause");
        return;
    }

    char searchID[30];
    printf(COLOR_YELLOW "                                  Enter Student ID to Search: " COLOR_RESET);
    fgets(searchID, sizeof(searchID), stdin);
    searchID[strcspn(searchID, "\n")] = 0;

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, searchID) == 0) {
            printf(COLOR_GREEN "\n                                  [+] Record Found:\n" COLOR_RESET);
            printf(COLOR_WHITE "                                  ID         : %s\n", s.id);
            printf("                                  Name       : %s\n", s.name);
            printf("                                  Department : %s\n", s.department);
            printf("                                  CGPA       : %.2f\n", s.cgpa);
            printf("                                  Phone      : %s\n\n" COLOR_RESET, s.phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED "\n                                  [!] Student Record with ID %s not found!\n" COLOR_RESET, searchID);
    }

    fclose(fp);
    system("pause");
}

// 4. Update Record
void updateRecord() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === UPDATE STUDENT RECORD ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb+");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] Database file not found!\n" COLOR_RESET);
        system("pause");
        return;
    }

    char targetID[30];
    printf(COLOR_YELLOW "                                  Enter Student ID to Update: " COLOR_RESET);
    fgets(targetID, sizeof(targetID), stdin);
    targetID[strcspn(targetID, "\n")] = 0;

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, targetID) == 0) {
            found = 1;
            printf(COLOR_GREEN "\n                                  Record Found! Enter New Details:\n\n" COLOR_RESET);

            printf(COLOR_YELLOW "                                  Enter New Name: " COLOR_RESET);
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf(COLOR_YELLOW "                                  Enter New Department: " COLOR_RESET);
            fgets(s.department, sizeof(s.department), stdin);
            s.department[strcspn(s.department, "\n")] = 0;

            printf(COLOR_YELLOW "                                  Enter New CGPA: " COLOR_RESET);
            scanf("%f", &s.cgpa);
            getchar();

            printf(COLOR_YELLOW "                                  Enter New Phone Number: " COLOR_RESET);
            fgets(s.phone, sizeof(s.phone), stdin);
            s.phone[strcspn(s.phone, "\n")] = 0;

            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            printf(COLOR_GREEN "\n                                  [+] Student Record Updated Successfully!\n" COLOR_RESET);
            break;
        }
    }

    if (!found) {
        printf(COLOR_RED "\n                                  [!] Student ID %s not found!\n" COLOR_RESET, targetID);
    }

    fclose(fp);
    system("pause");
}

// 5. Delete Record
void deleteRecord() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === DELETE STUDENT RECORD ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] Database file not found!\n" COLOR_RESET);
        system("pause");
        return;
    }

    char targetID[30];
    printf(COLOR_YELLOW "                                  Enter Student ID to Delete: " COLOR_RESET);
    fgets(targetID, sizeof(targetID), stdin);
    targetID[strcspn(targetID, "\n")] = 0;

    FILE *tempFP = fopen("temp.dat", "wb");
    if (tempFP == NULL) {
        printf(COLOR_RED "                                  [!] Error creating temporary file!\n" COLOR_RESET);
        fclose(fp);
        system("pause");
        return;
    }

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.id, targetID) == 0) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, tempFP);
        }
    }

    fclose(fp);
    fclose(tempFP);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found) {
        printf(COLOR_GREEN "\n                                  [+] Record Deleted Successfully!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "\n                                  [!] Student ID %s not found!\n" COLOR_RESET, targetID);
    }

    system("pause");
}

// 6. Sort Records
void sortRecords() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === SORT STUDENT RECORDS ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf(COLOR_RED "                                  [!] Database file not found!\n" COLOR_RESET);
        system("pause");
        return;
    }

    Student students[500];
    int count = 0;

    while (fread(&students[count], sizeof(Student), 1, fp) == 1) {
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf(COLOR_YELLOW "                                  No records available to sort.\n" COLOR_RESET);
        system("pause");
        return;
    }

    int choice;
    printf(COLOR_YELLOW "                                  [1] Sort by ID (Ascending)\n" COLOR_RESET);
    printf(COLOR_YELLOW "                                  [2] Sort by CGPA (Descending)\n" COLOR_RESET);
    printf(COLOR_YELLOW "                                  Select Option (1-2): " COLOR_RESET);
    scanf("%d", &choice);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int condition = 0;
            if (choice == 1) {
                condition = strcmp(students[j].id, students[j + 1].id) > 0;
            } else if (choice == 2) {
                condition = students[j].cgpa < students[j + 1].cgpa;
            }

            if (condition) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf(COLOR_GREEN "\nSorted Results:\n\n" COLOR_RESET);
    printf(COLOR_CYAN "%-20s | %-28s | %-12s | %-6s | %-15s\n" COLOR_RESET, "ID", "Name", "Department", "CGPA", "Phone");
    printf(COLOR_WHITE "----------------------------------------------------------------------------------------\n" COLOR_RESET);

    for (int i = 0; i < count; i++) {
        printf("%-20s | %-28s | %-12s | %-6.2f | %-15s\n",
               students[i].id, students[i].name, students[i].department, students[i].cgpa, students[i].phone);
    }
    printf(COLOR_WHITE "----------------------------------------------------------------------------------------\n\n" COLOR_RESET);

    system("pause");
}

// 7. Count Total Records
void countTotalRecords() {
    printHeader();
    printf(COLOR_MAGENTA "                                  === TOTAL RECORDS COUNT ===\n\n" COLOR_RESET);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf(COLOR_GREEN "                                  Total Registered Students: 0\n\n" COLOR_RESET);
        system("pause");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    int totalCount = fileSize / sizeof(Student);
    fclose(fp);

    printf(COLOR_GREEN "                                  Total Registered Students: %d\n\n" COLOR_RESET, totalCount);
    system("pause");
}

// Main Menu System
int main() {
    int choice;

    while (1) {
        printHeader();
        printf(COLOR_WHITE "                  --------------------- MAIN MENU OPTIONS ---------------------\n\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [1] Add a New Record\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [2] Display All Records\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [3] Search for a Record\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [4] Update a Record\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [5] Delete a Record\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [6] Sort Records\n" COLOR_RESET);
        printf(COLOR_CYAN  "                                      [7] Count Total Records\n" COLOR_RESET);
        printf(COLOR_RED   "                                      [8] Exit Program\n\n" COLOR_RESET);
        printf(COLOR_WHITE "                  -------------------------------------------------------------\n\n" COLOR_RESET);
        printf(COLOR_YELLOW "                                      Enter Your Choice (1-8): " COLOR_RESET);

        if (scanf("%d", &choice) != 1) {
            fflush(stdin);
            continue;
        }
        getchar();

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayAllRecords(); break;
            case 3: searchRecord(); break;
            case 4: updateRecord(); break;
            case 5: deleteRecord(); break;
            case 6: sortRecords(); break;
            case 7: countTotalRecords(); break;
            case 8:
                printf(COLOR_GREEN "\n                                  Exiting Program... Thank you!\n" COLOR_RESET);
                exit(0);
            default:
                printf(COLOR_RED "\n                                  [!] Invalid choice! Please select 1 to 8.\n" COLOR_RESET);
                system("pause");
        }
    }

    return 0;
}
