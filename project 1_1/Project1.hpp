#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

enum UserType {PROFESSOR, STUDENT};
class User;
class Professor;
class Student;

string unique_code(string name){
    int n = name.size();
    for(int i = 0; i < n; i++)
        name[i] = (name[i] % 26) + 101;
    return name;
}

class General{
private:
    User **users;
    int users_len;
    int users_reg;
    int prof_num;
public:
    General();
    ~General();
    friend class Professor;
    friend class Student;
    friend class User;
};

class User{
protected:
    int user_index;
    string name;
    string id;
    string password;
public:
    UserType user_type;
    void set_name();
    bool set_id(General &general);
    void set_pass();
    virtual void signup(General &general){};
    User* login(General &general);
};
typedef struct{
    string text;
    float ans;
}Question;

class Exam{
private:
    string name;
    string code;
    Question *questions;
    int question_len;
    int question_size;
public:
    Exam();
    ~Exam();
    void add();
    void del();
    friend class Professor;
    friend class Student;
    friend string unique_code(string name);
};

class Professor: public User{
private:
    string *courses;
    int course_len;
    int size;
    Exam *exams;
    int exam_len;
    int exam_size;
public:
    Professor();
    ~Professor();
    void set_course();
    virtual void signup(General &general);
    int prof_menu();
    Exam& create_exam();
    bool view_exams();
    bool prof_functionality(General &general);
    friend class Student;
};

class Student: public User{
private:
    string field;
    Exam *exams;
    int exam_len;
    int exam_size;
public:
    Student();
    ~Student();
    void set_field();
    virtual void signup(General &general);
    int stu_menu();
    void reg_exam(General &general);
    void view_exams();
    string participate_exam();
    void exam_process(string code);
    bool stu_functionality(General &general);
};

    