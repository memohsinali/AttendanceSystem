# AttendanceSystem
Consider an attendance and leave management system for a commercial organisation. The
system keeps record of employees’ attendance and leaves, and generates several reports.
The length of the working week is 40 hours: 8 hours a day and 5 days a week. The employees are
required to remain in the premises for this period. Attendance is marked by the guards on duty
standing at the gates. The employees can view their attendance details any time.
The organisation provides different kinds of leaves to its employee
• Casual leave: This leave can be availed in case of short-term sickness or emergency. The leave
does not require a prior approval. Each employee gets 15 casual leaves for a year. However she
cannot avail more than 4 leaves at a time
• Earned leave: This leave is used when employee wants to take more than 4 days off. The leav
requires prior approval. An employee can take 21 earned leaves in a year
• Official leave: If an employee goes to some official visit, she can file official leave. Full (credit
hours) is awarded in this case
• Unpaid leave: An employee can avail unpaid leave for a quite longer period of time e.g. study
leave for a few years. However prior approval is required, and the employee shall not get any
salary during this period
For each leave application, different data items are recorded: employee name and id, type o
leave, leave period (from, to), leave address (e.g. ex-Pakistan leave), reason, date of application
etc. The system forwards leave application to the applicant’s supervisor. When the supervisor
approves application, the system notifies the employee and updates records. The system record
date of approval as well. In case of long leave such as Earned leave, the system also requires
approval of the company director.
The system generates the following reports:
• Show employees whose attendance for a month is less than a given percentage (e.g. 80%)
• Show employees with any outstanding leaves (e.g. earned leaves without approval, or casual
leaves which are not filed
• Show attendance details for a given employee
• Show leave details for a given employee (including leave balance)
