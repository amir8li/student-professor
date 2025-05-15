#include <iostream>
#include <stdlib.h>
#include <string>
#include "Project1.hpp"
using namespace std;
//--------------------------------------
General::General(){
    users = new User*[10];
    users_len = 10;
    users_reg = 0;
    prof_num = 0;
}
General::~General() {
    for (int i = 0; i < users_reg; i++) {
        delete users[i];
    }
    delete[] users;  
}
void User::set_name(){
    cout << "Enter your name: ";
    getline(cin >> ws, name);
    cout <<"###############################################################" << endl;
}
bool User::set_id(General &general){
    cout << "Enter your ID: ";
    getline(cin >> ws, id);
    cout <<"###############################################################" << endl;
    int tmp = 0;
    while(tmp < general.users_reg){
        if(general.users[tmp++]->id== this->id)
            return false;
    }
    return true;
}
void User::set_pass(){
    cout << "Enter your password: ";
    getline(cin >> ws, password);
    cout <<"###############################################################" << endl;
}
User* User::login(General &general){
    string id, password;
    cout << "Enter your id: ";
    getline(cin >> ws, id);
    cout << "Enter your password: ";
    getline(cin >> ws, password);
    cout <<"###############################################################" << endl;
    for(int i = 0; i < general.users_reg; i++){
        if(general.users[i]->id == id
            && general.users[i]->password == password){
            cout << "Logged In successfully" << endl;
            cout <<"###############################################################" << endl;
            return general.users[i]; 
        }
    }
    return nullptr;
}
//--------------------------------------
Exam::Exam(){
    questions = new Question[10];
    question_len = 10;
    question_size = 0;
}
Exam::~Exam(){
    delete[] questions;
}
void Exam::add(){
    if(question_size == question_len){
        question_len *= 2;
        Question* temp = new Question[question_len];
        for(int i = 0; i < question_size; i++) {
            temp[i] = questions[i]; 
        }
        delete[] questions; 
        questions = temp;
    }
    Question question;
    cout << "Text of the question?" << endl;
    getline(cin >> ws, question.text);
    cout << "The answer: ";
    cin >> question.ans;
    questions[question_size++] = question;
    cout << "Question created successfully!" << endl;
    cout <<"###############################################################" << endl;
}
void Exam::del(){
    if(question_size == 0){
        cout << "No question to remove!!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    cout << "Which Question? ";
    int index;
    cin >> index;
    cout <<"###############################################################" << endl;
    if(index > question_size || index < 1){
        cout << "Question not found" << endl;
        cout <<"###############################################################" << endl;
    }
    else{  
        for(int i = index-1; i < question_size - 1; i++)
            questions[i] = questions[i+1];
        question_size--;
        cout << "Question number " << index << " successfully is removed" << endl;
        cout <<"###############################################################" << endl;
    }
}
Professor::Professor(){
    user_type = PROFESSOR;
    courses = new string[10];
    exams = new Exam[10];
    course_len = 10;
    size = 0;
    exam_len = 10;
    exam_size = 0;
}
Professor::~Professor(){
    delete[] courses;
    delete[] exams;
}
void Professor::set_course(){
    int num;
    cout << "Enter number of courses: ";
    cin >> num;
    cout << "Enter your courses: ";
    for(int i = 0; i < num; i++){
        cin >> courses[i];
        size++;
        if(size == course_len) {
            course_len *= 2;
            string* temp = new string[course_len];
            for(int i = 0; i < size; i++) {
                temp[i] = courses[i]; 
            }
            delete[] courses; 
            courses = temp;
        }
    }
    cout <<"###############################################################" << endl;
}
void Professor::signup(General &general){
    set_name();
    set_course();
    while(!set_id(general)){
        cout << "Oops! ID is taken. Try different one." << endl;
        cout <<"###############################################################" << endl;
    }
    set_pass();
    cout << "Congrats:) You're successfully Signed Up as a professor"<< endl;
    cout <<"###############################################################" << endl;
    user_index = general.users_reg;
    general.users_reg++;
    general.prof_num++;
    if(general.users_reg == general.users_len) {
        general.users_len *= 2;
        User** temp = new User*[general.users_len];
        for(int i = 0; i < general.users_reg; i++) {
            temp[i] = general.users[i];
        }
        delete[] general.users;
        general.users = temp;
    }    
}
int Professor::prof_menu(){
    cout << "1.Create an exam\t2.My exams\t3.Log Out" << endl;
    int choice;
    cin >> choice;
    while(choice != 1 && choice != 2 && choice != 3){
        cout << "Wrong choice :| Try again." << endl;
        cout <<"###############################################################" << endl;
        cin >> choice;
    }
    cout <<"###############################################################" << endl;
    return choice;
}
Exam& Professor::create_exam(){
    if(exam_size == exam_len) {
        exam_len *= 2;
        Exam* temp = new Exam[exam_len];
        for(int i = 0; i < exam_size; i++) {
            temp[i] = exams[i]; 
        }
        delete[] exams; 
        exams = temp;
    }
    Exam& exam = exams[exam_size++];
    while(true){
        bool flag = true;
        cout << "Name of the exam: ";
        cin >> exam.name;
        for(int i = 0; i < exam_size-1; i++){
            if(exams[i].name == exam.name){
                cout << "Exam name already token!! Try again" << endl;
                cout <<"###############################################################" << endl;
                flag = false;
                break;
            }
        }
        if(flag)
            break;
    }
    string tmp = unique_code(exam.name);
    char index = user_index + 48;
    exam.code = tmp + index;
    cout << "The exam code is: " << exam.code << endl;
    cout <<"###############################################################" << endl;
    return exam;
}
bool Professor::view_exams(){
    if(exam_size == 0){
        cout << "No exam to show!" << endl;
        cout <<"###############################################################" << endl;
        return true;
    }
    for(int i = 0; i < exam_size; i++){
        cout << "-" << exams[i].name << " : " << exams[i].code << endl;
    }
    string code;
    cout << "Enter the code of the exam you want: ";
    cin >> code;
    for(int i = 0; i < exam_size; i++){
        if(exams[i].code == code){
            cout << "The name of the exam: " << exams[i].name << endl;
            if(exams[i].question_size == 0)
                cout << "Zero question" << endl;
            else{
                for(int j = 0; j < exams[i].question_size; j++){
                    cout << j+1 << "." << exams[i].questions[j].text << endl;
                    cout << "\tthe answer is: " << exams[i].questions[j].ans << endl;
                }
            }
            cout <<"###############################################################" << endl;
            return true;
        }
    }
    return false;
}
bool Professor::prof_functionality(General &general){
    do{
        int func = prof_menu();
        if(func == 1){
            Exam &exam = create_exam();
            do{
            cout << "1.Add Question\t2.Remove Question\t3.Return to menu" << endl;
            int method;
            cin >> method;
            cout <<"###############################################################" << endl;
            if(method == 1){
                exam.add(); 
            }
            else if(method == 2){
                exam.del();
            }
            else if(method == 3){
                break;
                cout <<"###############################################################" << endl;
            }
            else{
                cout << "Wrong choice :| Try again." << endl;
                cout <<"###############################################################" << endl;
            }
            }while(true);
        }
        else if(func == 2){
            bool flag = view_exams();
            if(flag){
                while(true){
                cout << "1.Return to menu" << endl;
                int choice;
                cin >> choice;
                if(choice == 1){
                cout <<"###############################################################" << endl;
                    break;
                }
                else
                    cout << "Wrong choice :| Try again." << endl;
                cout <<"###############################################################" << endl;
                }
            }
            else{
                cout << "Wrong code!! Returning to menu." << endl;
                cout <<"###############################################################" << endl;
            }
        }
        else if(func == 3){
            general.users[user_index] = new Professor(*this);
            return true;
        }
    }while(true);
}
//--------------------------------------
Student::Student(){
    user_type = STUDENT;
    exams = new Exam[10];
    exam_len = 10;
    exam_size = 0;
}
Student::~Student(){
    delete[]exams;
}
void Student::set_field(){
    cout << "Enter your field of study: ";
    getline(cin >> ws, field);
    cout <<"###############################################################" << endl;
}
void Student::signup(General &general){
    set_name();
    set_field();
    while(!set_id(general)){
        cout << "Oops! ID is taken. Try different one." << endl;
        cout <<"###############################################################" << endl;
    }
    set_pass();
    cout << "Congrats:) You're successfully Signed Up as a student" << endl;
    cout <<"###############################################################" << endl;
    user_index = general.users_reg;
    general.users_reg++;
    if(general.users_reg == general.users_len) {
        general.users_len *= 2;
        User** temp = new User*[general.users_len];
        for(int i = 0; i < general.users_reg; i++) {
            temp[i] = general.users[i];
        }
        delete[] general.users;
        general.users = temp;
    }    
}
int Student::stu_menu(){
    cout << "1.Register for a new exam\n2.View registered exams\n3.Participate in new exam\n4.Log Out" << endl;
    int choice;
    cin >> choice;
    while(choice != 1 && choice != 2 && choice != 3 && choice != 4){
        cout << "Wrong choice :| Try again." << endl;
        cout << "###############################################################" << endl;
        cin >> choice;
    }
    cout << "###############################################################" << endl;
    return choice;
}
void Student::reg_exam(General &general){
    if(exam_size == exam_len){
        exam_len *= 2;
        Exam *temp = new Exam[exam_len];
        for(int z = 0; z < exam_size; z++)
            temp[z] = exams[z];
        delete []exams;
        exams = temp;
    }
    cout << "Enter the exam code: ";
    string code;
    cin >> code;
    for(int j = 0; j < exam_size; j++){
        if(code == exams[j].code){
            cout << "Exam already registered!!" << endl;
            cout << "###############################################################" << endl;
            return;
        }
    }
    for(int i = 0; i < general.users_reg; i++){
        if(i == user_index)
            continue;
        Professor* P = dynamic_cast<Professor*>(general.users[i]);
        if(P == nullptr){
            continue;
        }
        for(int j = 0; j < P->exam_size; j++){
            if(code == P->exams[j].code){
                cout << "Registered successfully" << endl;
                cout << "###############################################################" << endl;
                exams[exam_size++] = P->exams[j];
                return;
            }
        }
    }
    cout << "Exam not found!! Returning to menu" << endl;
    cout << "###############################################################" << endl;
}
void Student::view_exams(){
    if(exam_size == 0)
        cout << "No registered exam!!" << endl;
    else{
        for(int i = 0; i < exam_size; i++){
            cout << "-" << exams[i].name << " : " << exams[i].code  << endl;
        }
    }
    cout << "###############################################################" << endl;
}
string Student::participate_exam(){
    if(exam_size == 0){
        cout << "No exam to participate in!!" << endl;
        cout << "###############################################################" << endl;
        return"";
    }
    cout << "Enter the exam code: ";
    string code;
    cin >> code;
    for(int i = 0; i < exam_size; i++){
        if(code == exams[i].code)
            return code;
    }
    cout << "Wrong code!! Try again" << endl;
    cout << "###############################################################" << endl;
    return "";
}
void Student::exam_process(string code){
    int exam_ind = -1;
    for(int i = 0; i < exam_size; i++){
        if(exams[i].code == code){
            exam_ind = i;
            break;
        }
    }
    if(exams[exam_ind].question_size == 0){
        cout << "Exam is not created yet!!" << endl;
        cout << "###############################################################" << endl;
        return;
    }
    cout << "----------------------- Exam is started -----------------------" << endl;
    int len = exams[exam_ind].question_size;
    float answers[len];
    bool is_answered[len] = {false};
    int ans_num = 0;
    int current_q = 0;
    while(true){
        if(current_q == 0){
            cout << current_q+1 << ")" << exams[exam_ind].questions[current_q].text << endl;
            cout << "1.Answer\t2.Next" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                cin >> answers[current_q];
                is_answered[current_q] = true;
                current_q++;
            }
            else if(choice == 2)
                current_q++;
            else{
                cout << "Wrong choice!! Try again" << endl;
            }
        }
        else if(current_q == len-1){
            cout << current_q+1 << ")" << exams[exam_ind].questions[current_q].text << endl;
            cout << "1.Answer\t2.Previous\t3.Finish" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                cin >> answers[current_q];
                is_answered[current_q] = true;
            }
            else if(choice == 2){
                current_q--;
            }
            else if(choice == 3){
                break;
            }
            else{
                cout << "Wrong choice!! Try again" << endl;
            }
        }
        else{
            cout << current_q+1 << ")" << exams[exam_ind].questions[current_q].text << endl;
            cout << "1.Answer\t2.Next\t3.Previous" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                cin >> answers[current_q];
                is_answered[current_q] = true;
                current_q++;
            }
            else if(choice == 2)
                current_q++;
            else if(choice == 3)
                current_q--;
            else{
                cout << "Wrong choice!! Try again" << endl;
            }
        }
        cout << "---------------------------------------------------------------" << endl;
        cout << "Remaining questions: ";
        for(int i = 0; i < len; i++){
            if(is_answered[i] == false)
                cout << i+1 << "/";
        }
        cout << endl;
        cout << "---------------------------------------------------------------" << endl;
    }
    cout << "----------------------- Exam is finished ----------------------" << endl;
    int correct = 0;
    for(int i = 0; i < len; i++){
        if(is_answered[i] == true)
            if(exams[exam_ind].questions[i].ans == answers[i])
                correct++;
    }
    float score = 20 * ((float)correct / (float)len);
    printf("Your score is: %.2f\n", score);
    cout << "###############################################################" << endl;
}
bool Student::stu_functionality(General &general){
    while(true){
        int func = stu_menu();
        if(func == 1){
            reg_exam(general);
        }
        if(func == 2){
            view_exams();
        }
        if(func == 3){
            string code = participate_exam();
            if(code != "")
                exam_process(code);
        }
        else if(func == 4){
            general.users[user_index] = new Student(*this);
            return true;
        }
    }
}
//--------------------------------------
int main(){
    General general;
    do{
    cout <<"########################### WELCOME ###########################" << endl;
    int start;
    cout << "1.Sign UP   2.Login   3.Exit The Program" << endl;
    cin >> start;
    if(start == 1){
        cout <<"###############################################################" << endl;
        int choice;
        cout << "Sign Up as a\n1.Professor\t2.Student" << endl;
        cin >> choice;
        cout <<"###############################################################" << endl;
        if(choice == 1){
            Professor *P = new Professor();
            P->signup(general);
            bool Loggedout = P->prof_functionality(general);
            if(Loggedout){
                cout << "Logged out successfully" << endl;
                continue;
            }
        }
        else if(choice == 2){
            Student *S = new Student();
            S->signup(general);
            bool Loggedout = S->stu_functionality(general);
            if(Loggedout){
                cout << "Logged out successfully" << endl;
                continue;
            }
        }
        else{
            cout << "Wrong choice :| Try again." << endl;
            cout <<"###############################################################" << endl;
        }
    }
    else if(start == 2){
        cout <<"###############################################################" << endl;
        User U;
        User *user = U.login(general);
        if(user == nullptr){
            cout << "User not found. Try again" << endl;
            cout <<"###############################################################" << endl;
        }
        else{
            if(user->user_type == STUDENT){
                Student* S = dynamic_cast<Student*>(user);
                bool Loggedout = S->stu_functionality(general);
                if(Loggedout){
                    cout << "Logged out successfully" << endl;
                    continue;
                }
            }
            else{
                Professor* P = dynamic_cast<Professor*>(user);
                
                bool Loggedout = P->prof_functionality(general);
                if(Loggedout){
                    cout << "Logged out successfully" << endl;
                    continue;
                }
            }
        }
    }
    else if(start == 3){
        cout <<"######################## SEE YOU SOON #########################" << endl;
        break;
    }
    else{
        cout << "Wrong choice :| Try again." << endl; 
        cout <<"###############################################################" << endl;
    }
    }while(true);
    return 0;
}