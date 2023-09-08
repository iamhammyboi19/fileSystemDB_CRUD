#ifndef LOCAL_STORAGE_H_
#define LOCAL_STORAGE_H_

#include <string>
#include <vector>

class Local_Storage {
public:
  Local_Storage();
  ~Local_Storage(){};

private:
  void getStarted();
  void readStudentsData();
  void clearCIN();
  void createStudentData();
  void updateStudentData();
  bool findStudentByEmail(const std::string &email);
  void deleteStudentData();
  void collectStudentDetailsAndCreate(std::string studentName,
                                      std::string address, std::string phoneNo,
                                      std::string emailAddress,
                                      std::string math, std::string eng);
  bool isValidEmail(const std::string &email);
  bool isValidPhoneNo(const std::string &phoneno);
  std::vector<std::string> getAndDeleteStudent(const std::string &email);
};

#endif