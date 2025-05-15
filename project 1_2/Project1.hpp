#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

json loadJsonData(const string& filename);
void saveJsonData(const string& filename, json& data);
string unique_code(string name);
bool isUniqueID(json& data, string& id);

enum UserType {NONE, PROFESSOR, STUDENT};

typedef struct{
    string text;
    float ans;
}Question;

class Exam{
private:
    string name;
    string code;
public:
    Exam(){};
    ~Exam(){};
    void four_options(json &exam);
    void short_answer(json &exam);
    void long_answer(json &exam);
    void add(json &prof);
    void del(json &prof);
    json *findExamPro(json& professor, const string& examCode);
    json *findExamStu(json& student, const string& examCode);
    friend class Professor;
    friend class Student;
};

class User{
protected:
    string name;        
    string id;
    string password;
public:
    string set_name();
    string set_id();
    string set_password();
    void login(string &str, UserType &stat);
    void load_data(string id);
};

class Professor:public User{
private:
    vector <string> courses;
public:
    Professor(){};
    ~Professor(){};
    void signup();
    json *findProfessor(json& data, const string& professorID);
    int prof_menu();
    string create_exam(json &prof);
    bool view_exams(json &prof);
    void create_csv(string code, json &data);
    void view_students_scores(json &prof);
    void grade_long_questions(json &prof);
    bool prof_functionality();
};

class Student:public User{
private:
    string field;
    void random_options(vector <string>& options);
    vector <string> four_options_display(json &q);
public:
    Student(){};
    ~Student(){};
    void signup();
    json *findStudent(json& data, const string& studentID);
    int stu_menu();
    void reg_exam(json &student);
    void view_exams(json &student);
    string participate_exam(json &student);
    float calculate_score(json &exam, bool *is_answered, string *answers);
    void add_score_to_examJson(json &exam, float score, json &data, json &student);
    void exam_process(string code, json &student);
    void create_csv(string code, json &student);
    void view_scores(json &student);
    bool stu_functionality();
};
