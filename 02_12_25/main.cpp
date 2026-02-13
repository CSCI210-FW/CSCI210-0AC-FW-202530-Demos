#include <iostream>
#include <string>
#include <sqlite3.h>
#include <limits>

void resetStream();
void viewAssignmentsByProject(sqlite3 *db);

int main()
{
    sqlite3 *db;
    int rc; // return code
    std::string query;
    rc = sqlite3_open_v2("ConstructCo.db", &db, SQLITE_OPEN_READWRITE, NULL);
    // rc = sqlite3_open("SaleCo.db", &db); don't use this. it will create an empty database if the file doesn't exist
    if (rc != SQLITE_OK)
    {
        std::cout << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return 0;
}

void resetStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void viewAssignmentsByProject(sqlite3 *db)
{
}
