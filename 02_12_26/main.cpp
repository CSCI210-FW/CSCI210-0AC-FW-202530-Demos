#include <iostream>
#include <string>
#include <sqlite3.h>
#include <limits>
#include <iomanip>

void resetStream();
void viewAssignmentsByProject(sqlite3 *db);
void selectEmployees(sqlite3 *db);

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
    viewAssignmentsByProject(db);
    query = "delete from employee where emp_lname = 'Brown' and emp_fname = 'Charlie'";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "There was an error with deleting charlie brown: " << sqlite3_errmsg(db) << std::endl;
        std::cout << query << std::endl;
    }
    std::string lname = "Brown";
    std::string fname = "Charlie";
    int job = 504;
    int years = 0;
    char formatDate[80];
    time_t currentDate = time(NULL);
    strftime(formatDate, 80, "%F", localtime(&currentDate));
    std::string hiredate(formatDate);
    query = "insert into employee(emp_lname, emp_fname, emp_hiredate, job_code, "
            "emp_years)\n"
            "values ('" +
            lname + "','" + fname + "', '" + hiredate + "', " + std::to_string(job) + ", " + std::to_string(years) + ")";

    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "There was an error with adding charlie brown: " << sqlite3_errmsg(db) << std::endl;
        std::cout << query << std::endl;
    }
    else
    {
        int emp_num = sqlite3_last_insert_rowid(db);
        std::cout << fname << " " << lname << " inserted into the database as employee number " << emp_num << std::endl;
    }
    selectEmployees(db);
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
    std::string query = "select proj_num, proj_name from ";
    query += "project";
    sqlite3_stmt *result;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &result, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_finalize(result);
        std::cout << "There was an error with the project query: " << sqlite3_errmsg(db) << std::endl;
        std::cout << query << std::endl;
        return;
    }
    int i = 1;
    int choice;
    std::cout << "Please choose the project: " << std::endl;
    rc = sqlite3_step(result);
    while (rc == SQLITE_ROW)
    {
        // row processing
        std::cout << i << ". " << sqlite3_column_text(result, 0);
        std::cout << " - " << sqlite3_column_text(result, 1) << std::endl;
        i++;
        rc = sqlite3_step(result);
    }
    std::cin >> choice;
    while (!std::cin || choice < 1 || choice >= i)
    {
        if (!std::cin)
        {
            resetStream();
        }
        std::cout << "That is not a valid choice! Try again! Please enter a number 1 - " << i - 1 << std::endl;
        std::cin >> choice;
    }
    sqlite3_reset(result);
    for (int j = 0; j < choice; j++)
    {
        sqlite3_step(result);
    }
    int proj_num = sqlite3_column_int(result, 0); // proj_num is the first column in the select
    sqlite3_finalize(result);
    query = "select assign_num as 'Assignment Num',assign_date as 'Date',"
            " proj_name as 'Project Name', emp_fname || ' ' || emp_lname as 'Employee',\n"
            "job_description as 'Job', assign_chg_hr as 'Charge/Hour', "
            " assign_hours as 'Hours', assign_charge as 'Total Charge'\n"
            "from assignment\n"
            "join employee on assignment.emp_num = employee.emp_num\n"
            "join job on job.job_code = assignment.assign_job\n"
            "join project on assignment.proj_num = project.proj_num\n"
            "where assignment.proj_num = @projnum";

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &result, NULL);
    if (rc != SQLITE_OK)
    {
        sqlite3_finalize(result);
        std::cout << "There was an error with the assignment query: " << sqlite3_errmsg(db) << std::endl
                  << std::endl;
        std::cout << query << std::endl;
        return;
    }
    rc = sqlite3_bind_int(result, sqlite3_bind_parameter_index(result, "@projnum"), proj_num);
    if (rc != SQLITE_OK)
    {
        sqlite3_finalize(result);
        std::cout << "There was an error with the bind assignment query: " << sqlite3_errmsg(db) << std::endl;
        std::cout << query << std::endl;
        return;
    }
    int columnCount = sqlite3_column_count(result);
    for (int i = 0; i < columnCount; i++)
    {
        std::cout << std::setw(20) << sqlite3_column_name(result, i);
    }
    std::cout << std::endl;
    while (sqlite3_step(result) == SQLITE_ROW)
    {
        for (int i = 0; i < columnCount; i++)
        {
            if (sqlite3_column_type(result, i) != SQLITE_NULL)
                std::cout << std::setw(20) << sqlite3_column_text(result, i);
            else
                std::cout << std::setw(20) << " ";
        }
        std::cout << std::endl;
    }
    /*
    for(rc = sqlite3_step(result); rc==SQLITE_ROW; rc = sqlite3_step(result))
    {
    }
    */
    /*
    while(sqlite3_step(result) == SQLITE_ROW)
    {
    }
    */
}

void selectEmployees(sqlite3 *db)
{
    std::string query = "select * from employee";
    sqlite3_stmt *result;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &result, NULL);
    int columnCount = sqlite3_column_count(result);
    for (rc = sqlite3_step(result); rc == SQLITE_ROW; rc = sqlite3_step(result))
    {
        for (int i = 0; i < columnCount; i++)
        {
            if (sqlite3_column_type(result, i) != SQLITE_NULL)
                std::cout << std::setw(20) << sqlite3_column_text(result, i);
            else
                std::cout << std::setw(20) << " ";
        }
        std::cout << std::endl;
    }
}
