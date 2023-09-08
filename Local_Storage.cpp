#include "Local_Storage.h"
#include "CustomError.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

Local_Storage::Local_Storage() {
  try {
    getStarted();
  } catch (const CustomError &err) {
    std::cout << std::endl;
    std::cout << err << std::endl;
    std::cout << std::endl;
    getStarted();
  }
}

void Local_Storage::getStarted() {
  std::cout << "Welcome to this student file system database..." << std::endl;
  std::cout << "Press R to read all students data" << std::endl;
  std::cout << "Press C to create a new student data" << std::endl;
  std::cout << "Press U to update a student data" << std::endl;
  std::cout << "Press D to delete a student data" << std::endl;
  std::cout << "Press Q to quit/stop process\n" << std::endl;
  std::cout << "Enter one of the options you wish to perform: ";
  char input{};
  std::cin >> input;
  if (std::toupper(input) == 'R') {
    try {
      readStudentsData();
    } catch (const CustomError &err) {
      std::cout << err << std::endl;
    }

  } else if (std::toupper(input) == 'C') {
    clearCIN();
    try {
      createStudentData();
    } catch (const CustomError &err) {
      std::cout << err << std::endl;
      std::cout << std::endl;
      createStudentData();
    }
  } else if (std::toupper(input) == 'U') {
    try {
      updateStudentData();
    } catch (const CustomError &err) {
      std::cout << err << std::endl;
    }
  } else if (std::toupper(input) == 'D') {
    try {
      deleteStudentData();
    } catch (const CustomError &err) {
      std::cout << err << std::endl;
    }
  } else if (std::toupper(input) == 'Q') {
    std::cout << "Terminating process, closing application..." << std::endl;
  } else {
    throw CustomError("Invalid user input please try again", 400);
  }
}

// read all students or specific student
void Local_Storage::readStudentsData() {
  std::ifstream read{"studentdetails.csv"};

  if (!read) {
    throw CustomError("File not found", 404);
  }

  std::cout << "\nPress A to get print students data" << std::endl;
  std::cout << "Press B to get a specific student through email" << std::endl;
  std::cout << "Press Q to exit\n" << std::endl;

  char input{};
  std::cout << "Select an option: ";
  std::cin >> input;

  std::string header{};
  std::getline(read, header);
  std::istringstream headerGetter{header};
  std::string header2{};

  clearCIN();

  if (std::toupper(input) == 'A') {

    // show header
    std::cout << "\nReading students data..." << std::endl;
    while (std::getline(headerGetter, header2, ';')) {
      std::cout << std::left << std::setw(20) << header2 << " ";
    }
    std::cout << std::endl;

    // separate header
    std::string dash(120, '-');
    std::cout << dash << std::endl;

    // show all students
    std::string studs{};
    while (std::getline(read, studs, ';')) {
      std::cout << std::left << std::setw(20) << studs << " ";
    }
    std::cout << std::endl;
  } else if (std::toupper(input) == 'B') {

    // get student email
    std::cout << "Enter the student email address: ";
    std::string email{};
    std::cin >> email;
    std::string lines{};
    size_t count{0};

    while (std::getline(read, lines)) {
      if (lines.find(email) != std::string::npos) {
        //
        count++;

        std::cout << "\nReading students data..." << std::endl;
        while (std::getline(headerGetter, header2, ';')) {
          std::cout << std::left << std::setw(20) << header2 << " ";
        }
        std::cout << std::endl;

        std::string dash(120, '-');
        std::cout << dash << std::endl;

        std::istringstream found{lines};
        std::string showFound{};
        while (std::getline(found, showFound, ';')) {
          std::cout << std::left << std::setw(20) << showFound << " ";
        }
      }
    }
    if (count == 0) {
      throw CustomError(email + " not found", 404);
    }
  } else if (std::toupper(input) == 'Q') {
    std::cout << "Exiting..." << std::endl;
  } else {
    read.close();
    throw CustomError("Invalid user input please try again", 400);
  }

  std::cout << std::endl;

  read.close();
}

// create new student
void Local_Storage::createStudentData() {
  //
  std::string studentName{}, address{}, phoneNo{}, emailAddress{}, math{},
      eng{};

  std::cout << "Enter student name: ";
  std::getline(std::cin, studentName);
  if (studentName.length() < 1) {
    throw CustomError("Please enter your name", 400);
  }

  std::cout << "Enter student address: ";
  std::getline(std::cin, address);
  if (address.length() < 1) {
    throw CustomError("Please enter your address", 400);
  }

  std::cout << "Enter student phone number: ";
  std::getline(std::cin, phoneNo);
  if (!isValidPhoneNo(phoneNo)) {
    throw CustomError(
        phoneNo + " is not a valid phone number, please try again", 400);
  }

  std::cout << "Enter student email address: ";
  std::getline(std::cin, emailAddress);
  if (!isValidEmail(emailAddress)) {
    throw CustomError(
        emailAddress + " is not a valid email address, please try again.", 400);
  }
  if (findStudentByEmail(emailAddress)) {
    throw CustomError("Student with the email address " + emailAddress +
                          " already exist",
                      400);
  }

  std::cout << "Enter student your math grade: ";
  std::getline(std::cin, math);
  if (math.length() < 1 || math.length() > 3) {
    throw CustomError("Grades can only be from 0 - 100", 400);
  }

  std::cout << "Enter student your eng grade: ";
  std::getline(std::cin, eng);
  if (eng.length() < 1 || eng.length() > 3) {
    throw CustomError("Grades can only be from 0 - 100", 400);
  }

  std::cout << "\nCreating new student data..." << std::endl;

  try {
    collectStudentDetailsAndCreate(studentName, address, phoneNo, emailAddress,
                                   math, eng);
  } catch (const CustomError &err) {
    std::cout << err << std::endl;
  }
}

// collect student details and save to file
void Local_Storage::collectStudentDetailsAndCreate(
    std::string studentName, std::string address, std::string phoneNo,
    std::string emailAddress, std::string math, std::string eng) {
  //

  std::ifstream prevStudentsFile{"studentdetails.csv"};
  std::ofstream newStudentsFile{"tempOutputFile.csv"};
  std::string newStudent{};
  newStudent = studentName + ";" + address + ";" + phoneNo + ";" +
               emailAddress + ";" + math + ";" + eng;

  if (!prevStudentsFile || !newStudentsFile) {
    throw CustomError("Error opening or creating file", 400);
  }

  std::string studs{};

  while (std::getline(prevStudentsFile, studs)) {
    newStudentsFile << studs << "\n";
  }

  newStudentsFile << newStudent;

  prevStudentsFile.close();
  newStudentsFile.close();

  if (std::rename("tempOutputFile.csv", "studentdetails.csv") != 0) {
    throw CustomError("File error saving new student data", 500);
  } else {
    std::cout << "Congratulations new student data successfully created!"
              << std::endl;
  }
}

// update user data
void Local_Storage::updateStudentData() {
  //
  std::cout << "Enter student email address to find and update student data: ";
  std::string email{};
  std::cin >> email;

  if (!findStudentByEmail(email)) {
    throw CustomError(
        "Student with the email address `" + email + "` not found", 404);
  }

  // name;address;phoneNo;emailAddress;math;eng
  std::cout << "\nPress N to update student name";
  std::cout << "\nPress A to update student Address";
  std::cout << "\nPress P to update student Phone No";
  std::cout << "\nPress E to update student Email";
  std::cout << "\nPress M to update Math grade";
  std::cout << "\nPress L to update English grade";

  char option{};
  std::cout << "\nEnter an option to continue: ";
  std::cin >> option;
  clearCIN();

  std::string update{};
  std::vector<std::string> thestudent{};

  // update name
  if (std::toupper(option) == 'N') {
    //
    std::cout << "Enter new name: ";
    std::getline(std::cin, update);
    // if name not valid throw error
    if (update.length() < 1) {
      throw CustomError("Please enter your name", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[0] = update;
  }
  // update address
  else if (std::toupper(option) == 'A') {
    std::cout << "Enter new address: ";
    std::getline(std::cin, update);

    if (update.length() < 1) {
      throw CustomError("Please enter your address", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[1] = update;
  }
  // update phone number
  else if (std::toupper(option) == 'P') {
    std::cout << "Enter new phone number: ";
    std::getline(std::cin, update);

    if (!isValidPhoneNo(update)) {
      throw CustomError(
          update + " is not a valid phone number, please try again", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[2] = update;
  }
  // update email address
  else if (std::toupper(option) == 'E') {
    std::cout << "Enter new email address: ";
    std::getline(std::cin, update);

    if (!isValidEmail(update)) {
      throw CustomError(
          update + " is not a valid email address, please try again.", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[3] = update;
  }
  // update math
  else if (std::toupper(option) == 'M') {

    std::cout << "Enter new math grade: ";
    std::getline(std::cin, update);

    if (update.length() < 1 || update.length() > 3) {
      throw CustomError("Grades can only be from 0 - 100", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[4] = update;
  }
  // update english
  else if (std::toupper(option) == 'L') {
    std::cout << "Enter new english grade: ";
    std::getline(std::cin, update);

    if (update.length() < 1 || update.length() > 3) {
      throw CustomError("Grades can only be from 0 - 100", 400);
    }

    thestudent = getAndDeleteStudent(email);
    thestudent[5] = update;
  }
  // wrong option
  else {
    throw CustomError("Invalid option input", 400);
  }

  // convert from {"name", "address"} to name;address
  std::string str{};
  for (auto it = thestudent.begin(); it != thestudent.end(); it++) {
    for (auto it2 = it->begin(); it2 != it->end(); it2++) {
      str.push_back(*it2);
    }
    if (it != thestudent.end() - 1) {
      str.push_back(';');
    }
  }

  std::cout << "user: " << str << std::endl;

  std::ifstream prevStudentsFile{"studentdetails.csv"};
  std::ofstream newStudentsFile{"tempOutputFile.csv"};
  std::string collectStudent{};

  if (!prevStudentsFile || !newStudentsFile) {
    throw CustomError("Error opening or creating file", 500);
  }

  while (std::getline(prevStudentsFile, collectStudent)) {
    newStudentsFile << collectStudent << "\n";
  }
  newStudentsFile << str;

  prevStudentsFile.close();
  newStudentsFile.close();

  if (std::rename("tempOutputFile.csv", "studentdetails.csv") != 0) {
    throw CustomError("Error saving file", 500);
  } else {
    std::cout << "Congratulations new student data successfully created!"
              << std::endl;
  }
}

// get the student inside vector and delete from file;
std::vector<std::string>
Local_Storage::getAndDeleteStudent(const std::string &email) {
  std::ifstream studFile{"studentdetails.csv"};
  std::ofstream tempFile{"tempFile.csv"};

  if (!studFile || !tempFile) {
    throw CustomError("File not found", 404);
  }

  std::vector<std::string> studentDetails{};
  std::string student{};

  while (std::getline(studFile, student)) {
    // find student and push each details to vector separating with ';'
    if (student.find(email) != std::string::npos) {
      std::istringstream found{student};
      std::string strfound{};
      while (std::getline(found, strfound, ';')) {
        studentDetails.push_back(strfound);
      }
    } else {
      // store all students except the one that needs to get updated
      tempFile << student << "\n";
    }
  }

  studFile.close();
  tempFile.close();

  if (std::rename("tempFile.csv", "studentdetails.csv") != 0) {
    throw CustomError("Error saving file", 500);
  } else {
    std::cout << "Congratulations new student data successfully created!"
              << std::endl;
  }

  return studentDetails;
}

void Local_Storage::deleteStudentData() {
  //
  std::cout << "Enter your email address to delete your data: ";
  std::string email{};
  std::cin >> email;

  if (!findStudentByEmail(email)) {
    throw CustomError("Student with " + email + " does not exist", 404);
  }

  std::cout << "\nAre you sure you want to proceed to delete your student "
               "data? Y/N: ";
  char option{};
  std::cin >> option;

  if (std::toupper(option) == 'Y') {
    std::cout << "Deleting student data..." << std::endl;
    getAndDeleteStudent(email);
  } else if (std::toupper(option) == 'N') {
    std::cout << "Exiting..." << std::endl;
  } else {
    throw CustomError("Invalid option input", 400);
  }
}

// findStudentByEmailAddress
bool Local_Storage::findStudentByEmail(const std::string &email) {
  std::ifstream studentsFile{"studentdetails.csv"};

  if (!studentsFile) {
    throw CustomError("File not found!", 404);
  }

  std::string eachStudent{};

  if (!isValidEmail(email)) {
    throw CustomError(email + " is not a valid email address", 400);
  }

  while (std::getline(studentsFile, eachStudent)) {
    if (eachStudent.find(email) != std::string::npos) {
      studentsFile.close();
      return true;
    }
  }

  studentsFile.close();

  return false;
}

// flushes std::cin buffer
void Local_Storage::clearCIN() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// email validator
bool Local_Storage::isValidEmail(const std::string &email) {
  //
  const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

  return std::regex_match(email, pattern);
}

// phone number validator
bool Local_Storage::isValidPhoneNo(const std::string &phoneno) {

  const std::string numbers{"+0123456789"};

  for (auto it = phoneno.cbegin(); it != phoneno.cend(); it++) {
    if (std::find(numbers.cbegin(), numbers.cend(), *it) == numbers.cend())
      return false;
  }

  return true;
}
