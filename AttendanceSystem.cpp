







#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include<fstream>
#include <chrono>
#include <iomanip>
#include<sstream>
using namespace std;
class Leave;
class Attendance;
class AttendanceSystem;
// Employee class


std::tm stringToTm(const std::string& dateStr) {
   std::tm tm = {};
   std::istringstream ss(dateStr);
   ss >> std::get_time(&tm, "%Y-%m-%d");
   return tm;
}

std::string tmToString(const std::tm& tm) {
   std::ostringstream ss;
   ss << std::put_time(&tm, "%Y-%m-%d");
   return ss.str();
}


class Employee {
protected:
   int ID;
   string name;
   int casualLeavesRemaining;
   int earnedLeavesRemaining;
   Employee* supervisor;
   Employee* director;
   vector<Leave*> leaveHistory;
   vector<Attendance> attendanceHistory;
   // vector<string> leaveHistory;
   vector<string> outstandingLeaves; // To track rejected leaves
public:
   Employee(int id, string name, Employee* spvr)
       : ID(id), name(name), supervisor(spvr), casualLeavesRemaining(15), earnedLeavesRemaining(21) {}

   Employee(int id, string name)
       : ID(id), name(name), casualLeavesRemaining(15), earnedLeavesRemaining(21) {}

   int getId()
   { 
       return ID;
   }
   string getName()
   { 
       return name;
   }
   int getCasualLeavesRemaining() 
   {
       return casualLeavesRemaining; 
   }
   int getEarnedLeavesRemaining() 
   { 
       return earnedLeavesRemaining;
   }
   void reduceCasualLeaves(int days) 
   {
       casualLeavesRemaining -= days; 
   }
   void reduceEarnedLeaves(int days)
   { 
       earnedLeavesRemaining -= days;
   }

   void setSupervisor(Employee* sup) 
   { 
       supervisor = sup;
   }
   void setDirector(Employee* dir) 
   { 
       director = dir; 
   }

   Employee* getSupervisor()
   { 
       return supervisor; 
   }
   Employee* getDirector()
   {
       return director;
   }

   // Methods to handle Outstanding Leaves
   void addOutstandingLeave(string leaveDetails) 
   {
       outstandingLeaves.push_back(leaveDetails);
   }

   void showOutstandingLeaves() 
   {
       cout << "Outstanding Leaves for Employee: " << name << endl;
       if (outstandingLeaves.empty()) {
           cout << "No outstanding leaves for " << name << endl;
       }
       else {
           for (auto& leave : outstandingLeaves) {
               cout << leave << endl;
           }
       }
   }

   // adding attendance
   void addAttendance(Attendance& attendance)
   {
       attendanceHistory.push_back(attendance);
   }

   void addLeave(Leave* leave)
   {
       leaveHistory.push_back(leave);
   }


   //employe can see his/her attendance

   void seeAttendance();

   void ApplyLeave(AttendanceSystem* s1);
   void approveLeave(Leave& leave);

   void fileApplication(AttendanceSystem& s1);

   void updateLeaveCounts(int empID, int newCasualLeaves, int newEarnedLeaves) {
       // Construct the filename based on empID
       stringstream filenameStream;
       filenameStream << "emp" << empID << ".txt";
       string filename = filenameStream.str();

       ifstream inputFile(filename); // Open the file in read mode
       if (!inputFile) {
           cerr << "Error: Unable to open the file " << filename << ".\n";
           return;
       }

       stringstream buffer;
       string line;
       string updatedContent;
       int lineCount = 0;

       // Read the file line by line and update the necessary lines
       while (getline(inputFile, line))
       {
           lineCount++;
           if (lineCount == 3) { // Update Casual Leaves Remaining
               updatedContent += "Casual Leaves Remaining: " + to_string(newCasualLeaves) + "\n";
           }
           else if (lineCount == 4) { // Update Earned Leaves Remaining
               updatedContent += "Earned Leaves Remaining: " + to_string(newEarnedLeaves) + "\n";
           }
           else {
               updatedContent += line + "\n";
           }
       }
       inputFile.close();

       // Write the updated content back to the file
       ofstream outputFile(filename, ios::trunc); // Open in truncate mode to overwrite the file
       if (!outputFile) {
           cerr << "Error: Unable to write to the file " << filename << ".\n";
           return;
       }

       outputFile << updatedContent; // Write the updated content
       outputFile.close();

       cout << "Leave balances updated successfully for Employee ID: " << empID << "!\n";
   }
};

// Guard class
class Guard {
private:
   int ID;
   string name;

public:
   Guard(int id, string name) : ID(id), name(name) {}

   int getId()
   {
       return ID;
   }
   string getName()
   { 
       return name;
   }

   // Method to mark attendance
   void markAttendance(class AttendanceSystem& s1, int empID, int hrs, string date);
};

// Leave base class (Abstract)
class Leave {
protected:
   Employee* employee;
   string leaveFromDate;
   string leaveToDate;
   string reason;
   string leaveAddress;
   string applicationDate;
   string status;
   string leaveType;
   bool isApproved;
   bool isRejected;
   string Approval_Rejection_date;
   int days;
public:
   Leave(Employee& emp, string type, string from, string to, string reason, string leaveAddress, string s, int d)
       : employee(&emp), leaveType(type), leaveFromDate(from), leaveToDate(to), reason(reason),
       leaveAddress(leaveAddress), isApproved(false), isRejected(false), status(s), days(d) {
       // Set application date as current date
       time_t now = time(0);
       struct tm timeinfo;
       localtime_s(&timeinfo, &now);
       char dateStr[20];
       strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &timeinfo);
       applicationDate = string(dateStr);
   }

   virtual void approveLeave() = 0;
   virtual void rejectLeave() = 0;
   virtual void showLeaveDetails() {
       cout << "Leave for " << employee->getName() << ": " << reason << endl;
       cout << "From: " << leaveFromDate << " To: " << leaveToDate << endl;
       cout << "Leave Type: " << typeid(*this).name() << endl;
       cout << "Application Date: " << applicationDate << endl;
       cout << "Approval Status: " << (isApproved ? "Approved" : isRejected ? "Rejected" : "Not Approved") << endl;
   }

   string getFromDate()
   {
       return leaveFromDate;
   }

   void set_Approval_Rejection_Date(string d)
   {
       Approval_Rejection_date = d;
   }

   string getToDate()
   {
       return leaveToDate;
   }

   string getType()
   {
       return leaveType;
   }

   string getStatus()
   {
       return status;
   }

   void setStatus(string s)
   {
       status = s;
   }

   string getReason()
   {
       return reason;
   }

   int getDays()
   {
       return days;
   }

   bool getApprovalStatus() const 
   { 
       return isApproved;
   }
   Employee* getEmp()
   {
       return employee;
   }

   bool getRejectionStatus() const
   {
       return isRejected;
   }
};


// Derived class CasualLeave
class CasualLeave : public Leave {
   int days;
public:
   CasualLeave(Employee& emp, string type, string from, string to, string reason, string leaveAddress, int d, string status)
       : Leave(emp, "casual", from, to, reason, leaveAddress, status, d), days(d) {}

   void approveLeave() override {
       if (days > 4) {
           cout << "Casual Leave cannot exceed 4 consecutive days. Leave rejected." << endl;
           rejectLeave();
           return;
       }

       // Supervisor Approval
       cout << "Supervisor Approval Required for " << employee->getName() << "'s Casual Leave request." << endl;
       // Assume supervisor decision is 1 (approve), in a real-world scenario, we would ask for input.
       cout << "Supervisor, enter 1 to approve or 0 to reject: ";
       int supervisorDecision;
       cin >> supervisorDecision;

       if (supervisorDecision == 1) {
           if (employee->getCasualLeavesRemaining() >= days) {
               isApproved = true;
               employee->reduceCasualLeaves(days);
               cout << "Casual Leave approved for " << employee->getName() << ". Remaining casual leaves: "
                   << employee->getCasualLeavesRemaining() << endl;
           }
           else {
               cout << "Not enough Casual Leaves for " << employee->getName() << endl;
           }
       }
       else {
           rejectLeave();
       }
   }

   void rejectLeave() override {
       isRejected = true;
       status = "rejected";
       employee->addOutstandingLeave("Casual Leave (Rejected) from " + leaveFromDate + " to " + leaveToDate);
       cout << "Casual Leave rejected for " << employee->getName() << endl;
   }
};

// Derived class EarnedLeave
class EarnedLeave : public Leave {
   int days;
public:
   EarnedLeave(Employee& emp, string type, string from, string to, string reason, string leaveAddress, int d, string status)
       : Leave(emp, "earned", from, to, reason, leaveAddress, status, d), days(d) {}

   void approveLeave() override {
       Employee* supervisor = this->getEmp()->getSupervisor();
       supervisor->approveLeave(*this);

   }

   void rejectLeave() override {
       isRejected = true;
       status = "rejected";
       employee->addOutstandingLeave("Earned Leave (Rejected) from " + leaveFromDate + " to " + leaveToDate);
       cout << "Earned Leave rejected for " << employee->getName() << endl;
   }
};

// Derived class UnpaidLeave
class UnpaidLeave : public Leave {
   int days;
public:
   UnpaidLeave(Employee& emp, string type, string from, string to, string reason, string leaveAddress, int d, string status)
       : Leave(emp, "Unpaid", from, to, reason, leaveAddress, status, d), days(d) {}

   void approveLeave() override {
       Employee* supervisor = this->getEmp()->getSupervisor();
       supervisor->approveLeave(*this);
   }

   void rejectLeave() override {
       status = "rejected";
       isRejected = true;
       employee->addOutstandingLeave("Unpaid Leave (Rejected) from " + leaveFromDate + " to " + leaveToDate);
       cout << "Unpaid Leave rejected for " << employee->getName() << endl;
   }
};

class OfficialLeave : public Leave {
   int days;
public:
   OfficialLeave(Employee& emp, string type, string from, string to, string reason, string leaveAddress, int d, string status)
       : Leave(emp, "official", from, to, reason, leaveAddress, status, d), days(d) {}

   void approveLeave() override {
       isApproved = true;
       cout << "Official Leave approved for " << employee->getName() << ". Leave is automatically approved." << endl;
   }

   void rejectLeave() override {
       cout << "Official Leave cannot be rejected, it is automatically approved." << endl;
   }
};

// Attendance class to store attendance information
class Attendance {
private:
   Employee* employee;  // Pointer to Employee object
   Guard* guard;       // Pointer to Guard object
   string date;        // Date of the attendance
   int hrs;            // Hours worked by the employee

public:
   Attendance(Employee& emp, Guard& g, string date, int hrs)
       : employee(&emp), guard(&g), date(date), hrs(hrs) {}

   Employee* getEmployee() { return employee; }
   Guard* getGuard() { return guard; }
   int getHrs() { return hrs; }
   string getDate() { return date; }

   void getAttendance_detail()
   {
       cout << guard->getName() << " " << date << "  " << hrs << endl;
   }

};

// Attendance system to store and manage attendance records
class AttendanceSystem {
private:
   vector<Attendance> attendanceRecords;
   vector<Leave*> leaveRecords;
   unordered_map<int, Employee*> employees; // Map to track registered employees
   unordered_map<int, Guard*> guards;

public:
   // Method to register an employee in the attendance system
   void registerEmployee(Employee& emp) {
       if (employees.find(emp.getId()) == employees.end()) {
           employees[emp.getId()] = &emp;
           cout << "Employee " << emp.getName() << " added to the attendance system." << endl;
       }
       else
       {
           cout << "Employee " << emp.getName() << " is already registered in the attendance system." << endl;
       }
   }

   void registerGuards(Guard & guard)
   {
       if (guards.find(guard.getId()) == guards.end()) {
           guards[guard.getId()] = &guard;
           cout << "Guard " << guard.getName() << " added to the attendance system." << endl;
       }
       else
       {
           cout << "Guard " << guard.getName() << " is already registered in the attendance system." << endl;
       }
   }

   Employee* getEmployeeById(int empId) {
       auto it = employees.find(empId);
       if (it != employees.end()) {
           return it->second; // Return pointer to the Employee
       }
       else {
           return nullptr; // Return nullptr if the ID is not found
       }
   }


   Guard getGuardByID(int gID)
   {
       auto it = guards.find(gID);
       if (it != guards.end()) {
           return *it->second; 
       }
   }

   unordered_map<int, Employee*> getEmployees() {
       return employees;
   }

   unordered_map<int, Guard*> getGuards() {
       return guards;
   }


   // Method to mark attendance for an employee
   void markAttendance(int empID, Guard& g1, string& date, int hrs) {


       // Find employee by ID in the unordered_map
       auto it = employees.find(empID);

       if (it != employees.end()) {
           // If found, print employee details
           Employee* emp = it->second;
           std::cout << "Employee ID: " << emp->getId() << ", Name: " << emp->getName() << std::endl;
           Attendance attendance(*emp, g1, date, hrs);
           emp->addAttendance(attendance);
           attendanceRecords.push_back(attendance);
       }
       else {
           // If not found
           std::cout << "Employee with ID " << empID << " not found." << std::endl;
       }

   }


   // Method to show employee attendance details
   void showEmpAttendance(int empID) {


       auto it = employees.find(empID);

       if (it != employees.end()) {
           // If found, print employee details
           Employee* emp = it->second;
           std::cout << "Employee ID: " << emp->getId() << ", Name: " << emp->getName() << std::endl;



           // Create a filename based on employee's name and ID
           stringstream filename;
           filename << emp->getName() << "_" << emp->getId() << "_attendance_detail.txt";  // e.g., "Mohsin_1_attendance_detail.txt"

           // Open the file for writing
           ofstream outFile(filename.str());

           if (!outFile) {
               cerr << "Error: Could not open file for writing attendance details." << endl;
               return;
           }

           outFile << "Attendance details for Employee: " << emp->getName() << " (" << emp->getId() << ")\n";
           outFile << "----------------------------------------------------\n";

           // Check if the employee has attendance records
           if (attendanceRecords.empty()) {
               outFile << "No attendance records found for " << emp->getName() << ".\n";
           }
           else {
               // Loop through the attendance history and write to file
               for (Attendance record : attendanceRecords)
               {
                   if (record.getEmployee()->getId() == empID)
                   {
                       outFile << "Date: " << record.getDate() << ", Hours Worked: " << record.getHrs() << "\n";
                   }
               }
           }

           // Close the file after writing
           outFile.close();

           cout << "Attendance details for " << emp->getName() << " (" << emp->getId() << ") have been written to the file: "
               << filename.str() << endl;
       }

       else {
           // If not found
           std::cout << "Employee with ID " << empID << " not found." << std::endl;
       }

   }

   void addLeave(Leave& l)
   {
       leaveRecords.push_back(&l);
   }

   void showOutstandingLeaves()
   {

       // Create a filename for outstanding leave records
       stringstream filename;
       filename << "Outstanding_Leaves.txt";  // Name of the file: "Outstanding_Leaves.txt"

       // Open the file for writing
       ofstream outFile(filename.str());

       if (!outFile) {
           cerr << "Error: Could not open file for writing leave details." << endl;
           return;
       }

       outFile << "Outstanding Leave details for Employees:\n";
       outFile << "----------------------------------------------------\n";

       bool hasLeaveRecords = false;

       // Loop through all the leave records and check for rejected leaves
       for (auto& leaveRecord : leaveRecords) {
           if (leaveRecord->getStatus() == "rejected" || leaveRecord->getStatus() == "pending") {
               // Write leave details to the file
               outFile << "Employee: " << leaveRecord->getEmp()->getName()
                   << " (ID: " << leaveRecord->getEmp()->getId() << ")\n";
               outFile << "Leave from: " << leaveRecord->getFromDate()
                   << " To: " << leaveRecord->getToDate() << "\n";
               outFile << "Reason: " << leaveRecord->getReason() << "\n";
               outFile << "Status: " << leaveRecord->getStatus() << "\n";
               outFile << "----------------------------\n";
               hasLeaveRecords = true;
           }
       }

       // If no rejected leave records found, mention it in the file
       if (!hasLeaveRecords) {
           outFile << "No outstanding leave records found.\n";
       }

       // Close the file after writing
       outFile.close();

       cout << "Outstanding leave details have been written to the file: " << filename.str() << endl;
   }

   void showEmployeesBelowAttendancePercentage(int threshold, int month, int year)
   {
       bool found = false;
       stringstream filename;
       filename << "attendance_below_" << threshold << ".txt";  // Example: attendance_below_80.txt
       ofstream outFile(filename.str());  // Create or open file with the dynamic name

       for (auto it = employees.begin(); it != employees.end(); ++it)
       {
           cout << "Key: " << it->first << ", Employee ID: " << it->second->getId() << ", Name: " << it->second->getName() << endl;
           int totalHoursWorked = 0;
           int totalWorkingDaysInMonth = 0;

           // Check attendance records for this employee
           for (auto& record : attendanceRecords) {
               if (record.getEmployee() == it->second)
               {
                   string date = record.getDate();
                   stringstream ss(date);
                   int recordYear, recordMonth, recordDay;
                   char delimiter;

                   ss >> recordYear >> delimiter >> recordMonth >> delimiter >> recordDay;

                   // Match year and month
                   if (recordYear == year && recordMonth == month) {
                       totalHoursWorked += record.getHrs();
                       totalWorkingDaysInMonth++;
                   }
               }
           }

           // Calculate attendance percentage
           int expectedWorkingHours = 8 * 20; // Assuming 160 hours/month
           int percentageWorked = totalHoursWorked == 0 ? 0 : (totalHoursWorked * 100) / expectedWorkingHours;

           // Check if the employee's attendance is below the threshold
           if (percentageWorked < threshold) {
               found = true;
               cout << "Employee: " << it->second->getName() << " (" << it->second->getId() << ") worked "
                   << percentageWorked << "% of the expected hours in " << month << "/" << year << ".\n";

               outFile << "Employee: " << it->second->getName() << " (" << it->second->getId() << ") worked "
                   << percentageWorked << "% of the expected hours in " << month << "/" << year << ".\n";
           }
       }

       if (!found) {
           cout << "There is no employee whose attendance is less than " << threshold << "%.\n";
           outFile << "There is no employee whose attendance is less than " << threshold << "%.\n";
       }

       outFile.close(); // Close the file after writing
   }


   void showEmpLeave(Employee& emp)
   {
       // Create a filename based on employee's name and ID
       stringstream filename;
       filename << emp.getName() << "_" << emp.getId() << "_leave.txt";  // e.g., "Mohsin_1_leave.txt"

       // Open the file for writing
       ofstream outFile(filename.str());

       if (!outFile) {
           cerr << "Error: Could not open file for writing leave details." << endl;
           return;
       }

       outFile << "Leave details for Employee: " << emp.getName() << " (" << emp.getId() << ")\n";
       outFile << "----------------------------------------------------\n";

       bool hasLeaveRecords = false;

       // Loop through the leave history of the employee and write to file
       for (auto& leaveRecord : leaveRecords) {
           if (leaveRecord->getEmp()->getId() == emp.getId()) {
               // Write leave details to the file
               outFile << "Leave from: " << leaveRecord->getFromDate() << " To: " << leaveRecord->getToDate() << "\n";
               outFile << "Reason: " << leaveRecord->getReason() << "\n";
               outFile << "Status: " << leaveRecord->getStatus() << "\n";
               outFile << "----------------------------\n";
               hasLeaveRecords = true;
           }
       }

       // If no leave records found, mention it in the file
       if (!hasLeaveRecords) {
           outFile << "No leave records found for " << emp.getName() << ".\n";
       }

       // Close the file after writing
       outFile.close();

       cout << "Leave details for " << emp.getName() << " (" << emp.getId() << ") have been written to the file: "
           << filename.str() << endl;
   }

   Employee* ApplyLeave(int empID)
   {
       auto it = employees.find(empID);

       if (it != employees.end()) {
           // If found, print employee details
           Employee* emp = it->second;
           return emp;
       }
       else
       {
           // If not found
           std::cout << "Employee with ID " << empID << " not found." << std::endl;
           return NULL;
       }
   }

   Leave* getLeaveByEmployeeId(int empId) {
       for (Leave* leave : leaveRecords) {
           if (leave->getEmp()->getId() == empId) { // Assuming Leave class has getEmployeeId()
               return leave;
           }
       }
       return nullptr; // Return nullptr if no matching leave record is found
   }

   void fileApplication(int empID)
   {
       auto it = employees.find(empID);

       if (it != employees.end()) {
           // If found, print employee details
           Employee* emp = it->second;
           emp->fileApplication(*this);
           stringstream filename;
           filename << "emp" << empID << ".txt"; // File for employee

           ofstream empFile(filename.str(), ios::app); // Open in append mode
           if (!empFile) {
               cerr << "Error opening file for Employee ID: " << empID << endl;
               return;
           }

           Leave* lv = getLeaveByEmployeeId(empID);
           empFile << "Leave Application details: " << lv->getType();
           empFile << " Leave " << lv->getStatus() << " " << lv->getReason();
           empFile << " " << lv->getFromDate() << " to " << lv->getToDate() << "\n";
           empFile.close();
       }
       else
       {
           // If not found
           std::cout << "Employee with ID " << empID << " not found." << std::endl;
       }
   }

   void ApplyLeave(int empID, Leave& leave);
};


void Guard::markAttendance(AttendanceSystem& s1, int empID, int hrs, string date) {
   // Mark attendance in the AttendanceSystem
   s1.markAttendance(empID, *this, date, hrs);

   // Open or create the file named empID.txt in append mode
   stringstream filename;
   filename << "emp" << empID << ".txt"; // File named as empID.txt

   ofstream empFile(filename.str(), ios::app); // Open in append mode
   if (!empFile) {
       cerr << "Error opening file for Employee ID: " << empID << endl;
       return;
   }

   // Append the attendance details to the file
   empFile << "Attendance Record: ";
   empFile << "Date: " << date << ", Hours Worked: " << hrs << endl;
   empFile.close();

   cout << "Attendance for Employee ID " << empID << " on " << date
       << " with " << hrs << " hours worked has been added to the file." << endl;
}

void Employee::approveLeave(Leave& leave)
{

   cout << "Leave detailes are:\n Number of days:" <<leave.getDays() <<" Reason :" << leave.getReason() <<"Leaving Address: "<< leave.getType();

   // Supervisor Approval
   Employee* employee = leave.getEmp();
   string date;
   cout << "Supervisor Approval Required for " << employee->getName() << "'s " << leave.getType() << " leave request." << endl;
   cout << "Enter date of approval or rejection (YYYY-MM-DD): ";
   cin >> date;
   leave.set_Approval_Rejection_Date(date);
   // Assume supervisor decision is 1 (approve), in a real-world scenario, we would ask for input.
   cout << "Supervisor, enter 1 to approve or 0 to reject: ";
   int supervisorDecision;
   cin >> supervisorDecision;

   if (supervisorDecision == 1)
   {
       if (leave.getType() == "earned")
       {
           // Director Approval
           cout << "Director Approval Required for " << employee->getName() << "'s for " << leave.getType() << " Leave request." << endl;
           // Assume director decision is 1 (approve), in a real-world scenario, we would ask for input.
           cout << "Director, enter 1 to approve or 0 to reject: ";
           int directorDecision;
           cin >> directorDecision;

           if (directorDecision == 1)
           {
               leave.setStatus("accepted");

               if (leave.getType() == "earned")
               {
                   employee->reduceEarnedLeaves(leave.getDays());

               }
               if (leave.getType() == "casual")
               {
                   employee->reduceCasualLeaves(leave.getDays());
               }

               cout << leave.getType() << " Leave approved for " << employee->getName() << endl;
               int empID = leave.getEmp()->getId();

               int newCasualLeaves = leave.getEmp()->getCasualLeavesRemaining();
               int newEarnedLeaves = leave.getEmp()->getEarnedLeavesRemaining();
               updateLeaveCounts(empID, newCasualLeaves, newEarnedLeaves);
           }
           else
           {
               leave.rejectLeave();
               leave.setStatus("rejected");
           }
       }
       else
       {
           // all other leave types
           leave.setStatus("accepted");
           if (leave.getType() == "casual")
           {
               employee->reduceCasualLeaves(leave.getDays());
           }

           cout << leave.getType() << " Leave approved for " << employee->getName() << endl;
           int empID = leave.getEmp()->getId();

           int newCasualLeaves = leave.getEmp()->getCasualLeavesRemaining();
           int newEarnedLeaves = leave.getEmp()->getEarnedLeavesRemaining();
           updateLeaveCounts(empID, newCasualLeaves, newEarnedLeaves);
       }

   }
   else
   {
       leave.rejectLeave();
       employee->reduceEarnedLeaves(leave.getDays());
       leave.setStatus("rejected");
   }
}


void Employee::seeAttendance()
{
   cout << this->getName() << "(" << this->getId() << ")" << "attendance detail is:";
   for (Attendance record : attendanceHistory)
   {
       record.getAttendance_detail();
   }
}

void Employee::fileApplication(AttendanceSystem & s1)
{
   for (Leave* leave : leaveHistory) {
       if (leave->getType() == "casual" && leave->getStatus() == "pending")
       {
           this->getSupervisor()->approveLeave(*leave);
       }


       string status = leave->getStatus();
       string start = leave->getFromDate();
       string end = leave->getToDate();
       if (status == "accepted")
       {
          std::tm startTm = stringToTm(start);
          std::tm endTm = stringToTm(end);
          while (std::difftime(mktime(&startTm), mktime(&endTm)) <= 0)
          {
           // Convert the date to string and call markAttendance
           std::string currentDate = tmToString(startTm);
           //  g1.markAttendance(attendanceSystem, empID, 8, currentDate);

           // Increment the date by one day
           startTm.tm_mday++;
           mktime(&startTm); // Normalize the tm structure after incrementing day

           Guard g1 =s1.getGuardByID(1);

           g1.markAttendance(s1,this->getId(),8,currentDate);

          }
       }

   }
}


void AttendanceSystem::ApplyLeave(int empID, Leave& leave) {
   // Check if the employee exists
   auto it = employees.find(empID);
   if (it != employees.end())
   {
       Employee* emp = it->second;
       // Ask the employee to apply for leave

       // Now, write the leave details to the employee's file
       stringstream filename;
       filename << "emp" << empID << ".txt"; // File for employee

       ofstream empFile(filename.str(), ios::app); // Open in append mode
       if (!empFile) {
           cerr << "Error opening file for Employee ID: " << empID << endl;
           return;
       }

       emp->addLeave(&leave);
       this->addLeave(leave);

       Leave* leavee = &leave;

       // Write the leave details into the employee's file
       empFile << leavee->getFromDate() << ": " << "Leave Applied: " << leavee->getType() << ": " << leavee->getStatus() << endl;


       cout << "Leave details for Employee ID " << empID << " have been added to the file." << endl;
       if (leavee->getType() == "earned" || leavee->getType() == "unpaid")
       {
           leavee->approveLeave(); // This will handle supervisor and director approval
           // Write the leave approval / rejection details details into the employee's file
           empFile << "Leave Application details: " << leavee->getType() << " Leave " << leavee->getStatus() << " " << leavee->getReason() << " " << leavee->getFromDate() << " to " << leavee->getToDate() << endl;
           empFile.close();
       }
   }
   else {
       cout << "Employee with ID " << empID << " not found!" << endl;
   }
}


// Button Class
class Button {
public:
   sf::RectangleShape shape;
   sf::Text label;

   Button(float x, float y, float width, float height, const std::string& text, sf::Font& font, sf::Color color) {
       shape.setPosition(x, y);
       shape.setSize(sf::Vector2f(width, height));
       shape.setFillColor(color);

       label.setFont(font);
       label.setString(text);
       label.setCharacterSize(20);
       label.setFillColor(sf::Color::White);
       label.setPosition(
           x + (width / 2) - (label.getLocalBounds().width / 2),
           y + (height / 2) - (label.getLocalBounds().height / 2)
       );
   }

   // Draw the button
   void draw(sf::RenderWindow& window) {
       window.draw(shape);
       window.draw(label);
   }

   // Check if button is clicked
   bool isClicked(sf::Vector2i mousePos) {
       return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
   }
};

// Function to calculate the difference in days between two dates
int calculateDaysDifference(const string& from, const string& to) {
   struct tm startDate = {}, endDate = {};
   istringstream ssFrom(from), ssTo(to);

   ssFrom >> get_time(&startDate, "%Y-%m-%d");
   ssTo >> get_time(&endDate, "%Y-%m-%d");

   if (ssFrom.fail() || ssTo.fail()) {
       return -1; // Invalid date format
   }

   time_t start = mktime(&startDate);
   time_t end = mktime(&endDate);

   if (start == -1 || end == -1) {
       return -1; // Error in converting to time_t
   }

   return static_cast<int>(difftime(end, start) / (60 * 60 * 24));
}

// Function to validate date format
bool isValidDateFormat(const string& date) {
   struct tm testDate = {};
   istringstream ss(date);
   ss >> get_time(&testDate, "%Y-%m-%d");
   return !ss.fail();
}


int main()
{

   AttendanceSystem attendanceSystem;

   sf::RenderWindow window(sf::VideoMode(800, 600), "Attendance Management System");
   sf::Font font;
   if (!font.loadFromFile("arial.ttf")) {
       std::cerr << "Error loading font\n";
       return -1;
   }

   // Create guards
   Guard g1(1, "Bashir");
   Guard g2(2, "Khalil");


   // Read the Supervisor.txt file
   ifstream sfile("Supervisor.txt");
   if (!sfile) {
       cerr << "Error opening Employee.txt file!" << endl;
       return 1;
   }

   string sline;
   getline(sfile, sline); // Skip the header line

   while (getline(sfile, sline))
   {
       stringstream ss(sline);
       int id;
       string name;

       ss >> id >> name; // Parse ID and Name
       Employee* emp = new Employee(id, name); // Dynamically create an Employee object

       // Register the employee in the AttendanceSystem
       attendanceSystem.registerEmployee(*emp);

       // Create a new file for the employee
       stringstream filename;
       filename << "spvr" << id << ".txt"; // File name as empId.txt

       ofstream empFile(filename.str());
       if (!empFile) {
           cerr << "Error creating file for Employee ID: " << id << endl;
       }
       else {
           // Write employee details into the file
           empFile << "Employee ID: " << id << endl;
           empFile << "Name: " << name << endl;
           empFile << "Casual Leaves Remaining: " << emp->getCasualLeavesRemaining() << endl;
           empFile << "Earned Leaves Remaining: " << emp->getEarnedLeavesRemaining() << endl;
           empFile.close();
           cout << "File created for Supervisor ID " << id << " (" << name << ")." << endl;
       }
   }

   sfile.close();

   // Read the Employee.txt file
   ifstream efile("Employee.txt");
   if (!efile) {
       cerr << "Error opening Employee.txt file!" << endl;
       return 1;
   }

   string eline;
   getline(efile, eline); // Skip the header line
   while (getline(efile, eline)) {
       stringstream ss(eline);
       int id;
       string name;
       int supervisorID;

       ss >> id >> name >> supervisorID;

       unordered_map<int, Employee*> super = attendanceSystem.getEmployees();
       auto it = super.find(supervisorID);
       if (it != super.end()) {
           // If found, print employee details
           Employee* supervisor = it->second;
           ss >> id >> name; // Parse ID and Name
           Employee* emp = new Employee(id, name, supervisor); // Dynamically create an Employee object

           // Register the employee in the AttendanceSystem
           attendanceSystem.registerEmployee(*emp);

           // Create a new file for the employee
           stringstream filename;
           filename << "emp" << id << ".txt"; // File name as empId.txt

           ofstream empFile(filename.str());
           if (!empFile) {
               cerr << "Error creating file for Employee ID: " << id << endl;
           }
           else {
               // Write employee details into the file
               empFile << "Employee ID: " << id << endl;
               empFile << "Name: " << name << endl;
               empFile << "Casual Leaves Remaining: " << emp->getCasualLeavesRemaining() << endl;
               empFile << "Earned Leaves Remaining: " << emp->getEarnedLeavesRemaining() << endl;
               empFile.close();
               cout << "File created for Employee ID " << id << " (" << name << ")." << endl;
           }
       }
   }
   efile.close();


   //reading the Guard.txt file

   ifstream gfile("Guard.txt");
   if (!gfile) {
       cerr << "Error opening Employee.txt file!" << endl;
       return 1;
   }

   string gline;
   getline(gfile, gline); // Skip the header line

   while (getline(gfile,gline))
   {
       stringstream ss(gline);
       int id;
       string name;

       ss >> id >> name; // Parse ID and Name
       Guard* g1 = new Guard(id, name); // Dynamically create an Employee object

       // Register the employee in the AttendanceSystem
       attendanceSystem.registerGuards(*g1);
   }

   sfile.close();

   // Create Buttons
   Button markAttendanceButton(100, 160, 200, 50, "Mark Attendance", font, sf::Color::Blue);
   Button viewAttendanceButton(100, 220, 200, 50, "View Attendance", font, sf::Color::Blue);
   Button viewAttendanceLess80Button(100, 280, 210, 50, "View Attendance < *%", font, sf::Color::Blue);
   Button ApplyLeaveButton(100, 340, 200, 50, "Apply Leave", font, sf::Color::Blue);
   Button ApplyCasualLeave(400, 100, 200, 50, "Apply Casual Leave", font, sf::Color::Blue);
   Button ApplyEarnedLeave(400, 160, 200, 50, "Apply Earned Leave", font, sf::Color::Blue);
   Button ApplyUnpaidLeave(400, 220, 200, 50, "Apply Unpaid Leave", font, sf::Color::Blue);
   Button ApplyOfficialLeave(400, 280, 200, 50, "Apply Official Leave", font, sf::Color::Blue);
   Button OutstandingLeaves(400, 340, 200, 50, "Outstanding Leave", font, sf::Color::Blue);
   Button FileApplication(400, 440, 200, 50, "File Application Leave", font, sf::Color::Blue);

   while (window.isOpen()) {
       sf::Event event;
       while (window.pollEvent(event))
       {
           if (event.type == sf::Event::Closed)
               window.close();

           // Button Click Detection
           if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
               sf::Vector2i mousePos = sf::Mouse::getPosition(window);


               // Mark Attendance
               if (markAttendanceButton.isClicked(mousePos))
               {
                   int gID, empID;
                   string date;
                   int hrs;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   cout << "Enter Guard ID: ";
                   cin >> gID;
                   cout << "Enter Date: ";
                   cin >> date;
                   cout << "Enter Hours Worked: ";
                   cin >> hrs;

                   Guard g1 = attendanceSystem.getGuardByID(gID);

                   g1.markAttendance(attendanceSystem, empID, hrs, date);
               }

               if (viewAttendanceButton.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   attendanceSystem.showEmpAttendance(empID);
               }

               if (ApplyCasualLeave.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   string reason, address;
                   int days;
                   cout << "Enter the number of days you want to take leave for (Note: days must not be greater than 4): ";
                   cin >> days;
                   Employee* emp = attendanceSystem.getEmployeeById(empID);
                   while ((emp->getCasualLeavesRemaining() < days) || (days > 4 || days < 1)) {
                       if (days > 4 || days < 1) {
                           cout << "Invalid number of days. Please enter a value between 1 and 4: ";
                           cin >> days;
                       }
                       if (emp->getCasualLeavesRemaining() < days) {
                           cout << "Employee has only " << emp->getCasualLeavesRemaining() << " Casual Leaves remaining." << endl;
                           cout << "Please enter a value less than " << emp->getCasualLeavesRemaining() << ": ";
                           cin >> days;
                       }
                   }

                   string from, to;
               STARTDATEINPUT:
                   cout << "Enter the Starting Date (YYYY-MM-DD): ";
                   cin >> from;

                   while (!isValidDateFormat(from)) {
                       cout << "Invalid start date format. Please enter a valid date (YYYY-MM-DD): ";
                       cin >> from;
                   }

                   cout << "Enter the Ending Date (YYYY-MM-DD): ";
                   cin >> to;

                   while (!isValidDateFormat(to)) {
                       cout << "Invalid end date format. Please enter a valid date (YYYY-MM-DD): ";
                       cin >> to;
                   }

                   int diff = calculateDaysDifference(from, to);
                   if (diff != days - 1) {
                       cout << "Error: The difference between start and end date does not match the requested number of leave days.\n";
                       goto STARTDATEINPUT;
                   }

                   cin.ignore();
                   cout << "Enter Reason for Leave: ";
                   getline(cin, reason);
                   cout << "Enter Leaving Address: ";
                   getline(cin, address);
                   Employee* emp1 = attendanceSystem.ApplyLeave(empID);
                   Leave* cl1 = new CasualLeave(*emp1, "casual", from, to, reason, address, days, "pending");
                   attendanceSystem.ApplyLeave(empID, *cl1);
               }

               if (ApplyEarnedLeave.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   string from, to, reason, address;
                   int days;
                   cout << "Enter the number of days you want to take leave for (Note: days must be greater than 4): ";
                   cin >> days;
                   Employee* emp = attendanceSystem.getEmployeeById(empID);
                   while ((emp->getEarnedLeavesRemaining() < days) || (days <= 4 || days < 1)) {
                       if (days <= 4 || days < 1) {
                           cout << "Invalid number of days. Please enter a value greater than 4: ";
                           cin >> days;
                       }
                       if (emp->getEarnedLeavesRemaining() < days) {
                           cout << "Employee has only " << emp->getEarnedLeavesRemaining() << " Earned Leaves remaining." << endl;
                           cout << "Please enter a value less than " << emp->getEarnedLeavesRemaining() << ": ";
                           cin >> days;
                       }
                   }


               STARTDATEINPUTS:
                   cout << "Enter the Starting Date (YYYY-MM-DD): ";
                   cin >> from;

                   while (!isValidDateFormat(from)) {
                       cout << "Invalid start date format. Please enter a valid date (YYYY-MM-DD): ";
                       cin >> from;
                   }

                   cout << "Enter the Ending Date (YYYY-MM-DD): ";
                   cin >> to;

                   while (!isValidDateFormat(to)) {
                       cout << "Invalid end date format. Please enter a valid date (YYYY-MM-DD): ";
                       cin >> to;
                   }

                   int diff = calculateDaysDifference(from, to);
                   if (diff != days - 1) {
                       cout << "Error: The difference between start and end date does not match the requested number of leave days.\n";
                       goto STARTDATEINPUTS;
                   }
                   cin.ignore();
                   cout << "Enter Reason for Leave: ";
                   getline(cin, reason);
                   cout << "Enter Leaving Address: ";
                   getline(cin, address);
                   Employee* emp2 = attendanceSystem.ApplyLeave(empID);
                   Leave* el1 = new EarnedLeave(*emp2, "earned", from, to, reason, address, days, "pending");
                   attendanceSystem.ApplyLeave(empID, *el1);
                
                   string status = el1->getStatus();
                   string start = el1->getFromDate();
                   string end = el1->getToDate();
                   if (status == "accepted")
                   {
                       std::tm startTm = stringToTm(start);
                       std::tm endTm = stringToTm(end);
                       while (std::difftime(mktime(&startTm), mktime(&endTm)) <= 0) {
                           // Convert the date to string and call markAttendance
                           std::string currentDate = tmToString(startTm);
                           g1.markAttendance(attendanceSystem, empID, 8, currentDate);

                           // Increment the date by one day
                           startTm.tm_mday++;
                           mktime(&startTm); // Normalize the tm structure after incrementing day
                       }
                   }
               }

               if (ApplyUnpaidLeave.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   string from, to, reason, address;
                   int days;
                   cout << "Enter number of days you want to take leave for (Note days must be less than = 4): ";
                   cin >> days;
                   cout << "Enter the Starting Date (YYYY-MM-DD): ";
                   cin >> from;
                   cout << "Enter the Ending Date (YYYY-MM-DD): ";
                   cin >> to;
                   cin.ignore();
                   cout << "Enter Reason for Leave: ";
                   getline(cin, reason);
                   cout << "Enter Leaving Address: ";
                   getline(cin, address);

                   Employee* emp3 = attendanceSystem.ApplyLeave(empID);
                   Leave* ul1 = new UnpaidLeave(*emp3, "unpaid", from, to, reason, address, days, "pending");
                   attendanceSystem.ApplyLeave(empID, *ul1);
               }

               if (ApplyOfficialLeave.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   string from, to, reason, address;
                   int days;
                   cout << "Enter number of days you want to take leave for: (Note days must be less than = 4): ";
                   cin >> days;
                   cout << "Enter the Starting Date (YYYY-MM-DD): ";
                   cin >> from;
                   cout << "Enter the Ending Date (YYYY-MM-DD): ";
                   cin >> to;
                   cin.ignore();
                   cout << "Enter Reason for Leave: ";
                   getline(cin, reason);
                   cout << "Enter Leaving Address: ";
                   getline(cin, address);

                   Employee* emp3 = attendanceSystem.ApplyLeave(empID);
                   Leave* ol1 = new OfficialLeave(*emp3, "official", from, to, reason, address, days, "accepted");
                   attendanceSystem.ApplyLeave(empID, *ol1);
               }

               if (OutstandingLeaves.isClicked(mousePos))
               {
                   attendanceSystem.showOutstandingLeaves();
               }

               if (viewAttendanceLess80Button.isClicked(mousePos))
               {
                   cout << "Enter Month (MM): ";
                   int m, y;
                   cin >> m;
                   cout << "Enter Year (YYYY): ";
                   cin >> y;
                   cout << "Enter Threshold value: ";
                   int thr;
                   cin >> thr;
                   attendanceSystem.showEmployeesBelowAttendancePercentage(thr, m, y);
               }

               if (FileApplication.isClicked(mousePos))
               {
                   int empID;
                   cout << "Enter Employee ID: ";
                   cin >> empID;
                   attendanceSystem.fileApplication(empID);




               }
           }
       }


       window.clear();

       markAttendanceButton.draw(window);
       viewAttendanceButton.draw(window);
       viewAttendanceLess80Button.draw(window);
       ApplyCasualLeave.draw(window);
       ApplyUnpaidLeave.draw(window);
       ApplyEarnedLeave.draw(window);
       ApplyOfficialLeave.draw(window);
       OutstandingLeaves.draw(window);
       FileApplication.draw(window);
       window.display();
   }


   return 0;
}

