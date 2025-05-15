#include <bits/stdc++.h>
#include "Project1.hpp"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

//---------------------------------------
json loadJsonData(const string& filename){
    ifstream file(filename);
    json data;
    if(file){
        file >> data;
    }
    else{
        data = {{"professors", json::array()}, {"students", json::array()}, {"numOfProfessors", 0}};
    }
    file.close();
    return data;
}

void saveJsonData(const string& filename, json& data){
    ofstream file(filename);
    file << data.dump(4);
    file.close();
}

string unique_code(string name){
    int n = name.size();
    for(int i = 0; i < n; i++)
        name[i] = (name[i] % 26) + 101;
    return name;
}

bool isUniqueID(json& data, string& id){
    if(data.contains("professors")){
        for(auto& prof : data["professors"]){
            if(prof["id"] == id) 
                return false;
        }
    }
    if(data.contains("students")){
        for(auto& student : data["students"]){
            if(student["id"] == id) 
                return false;
        }
    }
    return true;
}

//--------------------------------------
string User::set_name(){
    string name;
    cout << "Enter your name: ";
    getline(cin >> ws, name);
    cout <<"###############################################################" << endl;
    return name;
}

string User::set_id(){
    json data = loadJsonData("users.json");
    string id;
    cout << "Enter your ID: ";
    getline(cin >> ws, id);
    while(!isUniqueID(data, id)){
        cout << "Oops! ID is taken. Try different one." << endl;
        cout <<"###############################################################" << endl;
        cout << "Enter your ID: ";
        getline(cin >> ws, id);
    }
    cout <<"###############################################################" << endl;
    return id;
}

string User::set_password(){
    string password;
    cout << "Enter your password: ";
    getline(cin >> ws, password);
    cout <<"###############################################################" << endl;
    return password;
}

void User::login(string &str, UserType &stat){
    json data = loadJsonData("users.json");
    string id, password;
    cout << "Enter your id: ";
    getline(cin >> ws, id);
    cout << "Enter your password: ";
    getline(cin >> ws, password);
    cout <<"###############################################################" << endl;

    if(data.contains("professors")){
        for(auto& prof : data["professors"]){
            if(prof["id"] == id && prof["password"] == password){
                cout << "Login successful! Welcome, " << prof["name"] << "!" << endl;
                cout <<"###############################################################" << endl;
                str = id;
                stat = PROFESSOR;
                return;
            }
        }
    }
    if(data.contains("students")){
        for(auto& student : data["students"]){
            if(student["id"] == id && student["password"] == password){
                cout << "Login successful! Welcome, " << student["name"] << "!" << endl;
                cout <<"###############################################################" << endl;
                str = id;
                stat = STUDENT;
                return;
            }
        }
    }
    cout << "Login failed! Invalid ID or password." << endl;
    cout <<"###############################################################" << endl;
}

void User::load_data(string id){
    this->id = id;
}

//----------------------------------------------------
void Exam::four_options(json &exam){
    string text, opt1, opt2, opt3, opt4, correct_ans;
    int neg_score, pos_score, correct_index;
    cout << "Text of the question?" << endl;
    getline(cin >> ws, text);
    cout << "The options?" << endl;
    cin >> opt1 >> opt2 >> opt3 >> opt4;
    cout << "The answer (1-4): "; cin >> correct_index;
    cout << "The negative point: "; cin >> neg_score;
    cout << "The positive point: "; cin >> pos_score;
    if(correct_index == 1) correct_ans = opt1;
    else if(correct_index == 2) correct_ans = opt2;
    else if(correct_index == 3) correct_ans = opt3;
    else if(correct_index == 4) correct_ans = opt4;
    exam["questions"].push_back({
    {"type", "four options"},
    {"text", text},
    {"options", {opt1, opt2, opt3, opt4}},
    {"answer", {{"text", correct_ans}, {"index", correct_index}}},
    {"negativePoint", neg_score},
    {"positivePoint", pos_score},
    });
    cout << "Question created successfully!" << endl;
}
void Exam::short_answer(json &exam){
    string text, correct_ans;
    int neg_score, pos_score;
    cout << "Text of the question?" << endl;
    getline(cin >> ws, text);
    cout << "The answer: "; cin >> correct_ans;
    cout << "The negative point: "; cin >> neg_score;
    cout << "The positive point: "; cin >> pos_score;
    exam["questions"].push_back({
        {"type", "short answer"},
        {"text", text},
        {"answer", correct_ans},
        {"negativePoint", neg_score},
        {"positivePoint", pos_score},
    });
    cout << "Question created successfully!" << endl;
}
void Exam::long_answer(json &exam){
    string text, correct_ans;
    int pos_score;
    cout << "Text of the question?" << endl;
    getline(cin >> ws, text);
    cout << "Answer of the question: ";
    getline(cin >> ws, correct_ans);
    cout << "The positive point: "; cin >> pos_score;
    exam["questions"].push_back({
        {"type", "long answer"},
        {"text", text},
        {"answer", correct_ans},
        {"positivePoint", pos_score},
    });
    cout << "Question created successfully!" << endl;
}
void Exam::add(json &prof){
    json data = loadJsonData("users.json");
    json *exam = findExamPro(prof, code);
    cout << "1.Four options\t2.Short answer\t3.Long answer" << endl;
    cout << "Type of question? ";
    int choice;
    cin >> choice;
    cout <<"###############################################################" << endl;
    switch(choice){
        case 1:
            four_options(*exam);
            break;
        case 2:
            short_answer(*exam);
            break;
        case 3:
            long_answer(*exam);
            break;
        default:
            cout << "Wrong choice :| Try again." << endl;
    }
    cout <<"###############################################################" << endl;
    (*exam)["NumOfQuestions"] = (*exam)["questions"].size();
    for(auto& p : data["professors"]){
        if(p["id"] == prof["id"]){
            p = prof;
            break;
        }
    }
    saveJsonData("users.json", data);
}
void Exam::del(json &prof){
    json data = loadJsonData("users.json");
    json *exam = findExamPro(prof, code);
    if(!exam){
        cout << "Exam not found!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    if(!(*exam).contains("NumOfQuestions") || (*exam)["NumOfQuestions"].get<int>() == 0){
        cout << "No question to remove!!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    cout << "Which Question?";
    int index;
    cin >> index;
    cout <<"###############################################################" << endl;
    if(index > (*exam)["NumOfQuestions"].get<int>() || index < 1){
        cout << "Question not found" << endl;
        cout <<"###############################################################" << endl;
    }
    else{  
        (*exam)["questions"].erase((*exam)["questions"].begin() + index);
        (*exam)["NumOfQuestions"] = (*exam)["questions"].size();
        cout << "Question number " << index << " successfully is removed" << endl;
        cout <<"###############################################################" << endl;
        for(auto& p : data["professors"]){
            if(p["id"] == prof["id"]){
                p = prof;
                break;
            }
        }
        saveJsonData("users.json", data);
    }
}
json *Exam::findExamPro(json& professor, const string& examCode){
    if(professor.contains("exams")){
        for(auto& exam : professor["exams"]){
            if(exam["code"] == examCode)
                return &exam;
        }
    }
    return nullptr;
}
json *Exam::findExamStu(json& student, const string& examCode){
    if(student.contains("exams_reg")){
        for(auto& exam : student["exams_reg"]){
            if(exam["code"] == examCode)
                return &exam;
        }
    }
    return nullptr;
}
//----------------------------------------------------
void Professor::signup(){
    
    json data = loadJsonData("users.json");
    name = set_name();
    int num;
    cout << "Enter number of courses: ";
    cin >> num;
    cout << "Enter your courses: ";
    cin.ignore();
    for(int i = 0; i < num; i++){
        string course;
        cin >> course;
        courses.push_back(course);
    }
    cout <<"###############################################################" << endl;
    id = set_id();
    password = set_password();
    data["professors"].push_back({
        {"name", name},
        {"id", id},
        {"password", password},
        {"courses", courses},
        {"numOfExams", 0}
    });
    data["numOfProfessors"] = data["professors"].size();
    cout << "Congrats:) You're successfully Signed Up as a professor" << endl;
    cout <<"###############################################################" << endl;
    saveJsonData("users.json", data);
}

json *Professor::findProfessor(json& data, const string& professorID){
    if(data.contains("professors")){
        for(auto& prof : data["professors"]){
            if(prof["id"] == professorID)
                return &prof;
        }
    }
    return nullptr;
}

int Professor::prof_menu(){
    cout << "1.Create an exam\t2.My exams\n3.View students scores\t4.Grade long questions\n5.Log Out" << endl;
    int choice;
    cin >> choice;
    while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5){
        cout << "Wrong choice :| Try again." << endl;
        cout <<"###############################################################" << endl;
        cin >> choice;
    }
    cout <<"###############################################################" << endl;
    return choice;
}

string Professor::create_exam(json &prof){
    json data = loadJsonData("users.json");
    string name, code;
    if(!prof.contains("exams")){
        prof["exams"] = json::array();
    }
    while(true){
        cout << "Name of the exam: ";
        cin >> name;
        bool is_unique = true;
        for(auto &ex : prof["exams"]){
            if(ex["name"] == name){
                cout << "Exam name already taken!! Try again" << endl;
                cout <<"###############################################################" << endl;
                is_unique = false;
                break;
            }
        }
        if(is_unique)
            break;
    }
    string tmp = unique_code(name); 
    char index = data["numOfProfessors"].get<int>() + 48;
    code = tmp + index;
    cout << "The exam code is: " << code << endl;
    cout <<"###############################################################" << endl;
    prof["exams"].push_back({
        {"name", name},
        {"code", code},
        {"questions", json::array()},
        {"NumOfQuestions", 0}
    });
    prof["numOfExams"] = prof["exams"].size();
    
    if(!data.contains("exams")){
        data["exams"] = json::array();
    }
    data["exams"].push_back({
        {"code", code},
        {"name", name},
        {"scores", json::array()},
        {"numOfScores", 0}
    });
    
    for(auto& p : data["professors"]){
        if(p["id"] == prof["id"]){
            p = prof;
            break;
        }
    }
    saveJsonData("users.json", data);
    return code;
}
bool Professor::view_exams(json &prof){
    json data = loadJsonData("users.json");
    if(prof["numOfExams"] == 0){
        cout << "No exam to show!" << endl;
        cout <<"###############################################################" << endl;
        return true;
    }
    for(auto &exam : prof["exams"]){
        cout << "-" << exam["name"].get<string>() << " : " << exam["code"].get<string>() << endl;
    }
    string code;
    cout << "Enter the code of the exam you want: ";
    cin >> code;
    Exam tmp;
    json *exam = tmp.findExamPro(prof, code);
    if(exam){
        cout << "The name of the exam: " << (*exam)["name"].get<string>() << endl;
        if((*exam)["NumOfQuestions"] == 0)
            cout << "Zero question" << endl;
        else{
            int count = 1;
            for(auto &q : (*exam)["questions"]){
                if(q["type"] == "four options"){
                    cout << count++ << "." <<  q["text"].get<string>() << endl;
                    int tmp = 1;
                    for(auto &opt : q["options"])
                        cout << tmp++ << ")" << opt.get<string>() << "\t";
                    cout << endl;
                    cout << "\tCorrect answer is: " << q["answer"]["index"].get<int>() << ")" << q["answer"]["text"].get<string>() << endl;
                }
                else if(q["type"] == "short answer"){
                    cout << count++ << "." <<  q["text"].get<string>() << endl;
                    cout << "\tthe short answer is: " << q["answer"].get<string>() << endl;
                }
                else if(q["type"] == "long answer"){
                    cout << count++ << "." <<  q["text"].get<string>() << endl;
                    cout << "\tthe long answer is: " << q["answer"].get<string>() << endl;
                }
            }
        }
        cout <<"###############################################################" << endl;
        return true;
    }
    return false;
}
void Professor::create_csv(string code, json &data){
    json exam;
    for(auto &ex : data["exams"]){
        if(ex["code"] == code){
            exam = ex;
            break;
        }
    }
    if(exam["numOfScores"] == 0){
        cout << "No scores to show!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    ofstream csv_file(exam["name"].get<string>() + "_scores.csv");
    csv_file << "Student Name, Score" << endl;
    vector<pair<float, string>> scores;
    for(auto &score : exam["scores"])
        scores.push_back({score["score"].get<float>(), score["student_name"].get<string>()});
    sort(scores.rbegin(), scores.rend());
    for(auto &score : scores) {
        csv_file << score.second << ", " << score.first << endl;
    }
    csv_file << "----------------------------" << endl;
    float total_score = 0;
    int num_students = exam["scores"].size();
    for(auto& s : exam["scores"]) {
        total_score += s["score"].get<float>();
    }
    float average_score = num_students > 0 ? total_score / num_students : 0;
    
    float max_score = 0;
    for(auto& s : exam["scores"]) {
        if(s["score"].get<float>() > max_score) {
            max_score = s["score"].get<float>();
        }
    }
    csv_file << "Average score: " << average_score << endl;
    csv_file << "Maximum score: " << max_score << endl;
    csv_file.close();
    cout << "Scores saved to " << exam["name"].get<string>() + "_scores.csv successfully" << endl;
    cout <<"###############################################################" << endl;
}
void Professor::view_students_scores(json &prof){
    json data = loadJsonData("users.json");
    if(prof["numOfExams"] == 0){
        cout << "No exam to show!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    string code;
    for(auto &ex : prof["exams"]){
        cout << ex["name"].get<string>() << " : " << ex["code"].get<string>() << endl;
    }
    bool flag = false;
    while(!flag){
        cout << "Enter the code of the exam: ";
        cin >> code;
        for(auto &ex : data["exams"]){
            if(ex["code"] == code){
                create_csv(code, data);
                flag = true;
                break;
            }
        }
        if(!flag){
            cout << "wrong code! try again" << endl;
            cout <<"###############################################################" << endl;
        }
    }
}
void Professor::grade_long_questions(json &prof){
    json data = loadJsonData("users.json");
    if(prof["numOfExams"] == 0){
        cout << "No exam to show!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    for(auto &ex : prof["exams"]){
        cout << ex["name"].get<string>() << " : " << ex["code"].get<string>() << endl;
    }
    string code;
    bool flag = false;
    json exam;
    while(!flag){
        cout << "Enter the code of the exam: ";
        cin >> code;
        for(auto &ex : data["exams"]){
            if(ex["code"] == code){
                exam = ex;
                flag = true;
                break;
            }
        }
        if(!flag){
            cout << "wrong code! try again" << endl;
            cout <<"###############################################################" << endl;
        }
    }
    for(auto &score : exam["scores"]){
        cout << "-" << score["student_name"].get<string>()<< endl;
    }
    cout << "Enter the name of the student you want to grade: ";
    string student_name;
    cin >> student_name;
    cout <<"###############################################################" << endl;
    string student_id;
    for(auto &stu : data["students"]){
        if(stu["name"] == student_name){
            student_id = stu["id"];
            break;
        }
    }
    Student tmp_stu;
    json *student = tmp_stu.findStudent(data, student_id);
    if(!student){
        cout << "Student not found!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }
    Exam tmp_ex;
    json *exam_stu = tmp_ex.findExamStu(*student, code);
    if(!exam_stu){
        cout << "Exam not found for this student!" << endl;
        cout <<"###############################################################" << endl;
        return;
    }

    float current_score = (*exam_stu)["received points"].get<float>();
    int max_score = (*exam_stu)["maximum_score"].get<int>();

    for(auto &q : (*exam_stu)["questions"]){
        if(q["type"] == "long answer"){
            cout << q["text"].get<string>() << endl;
            if(q.contains("student answer")){
                cout << "Student answer: " << q["student answer"].get<string>() << endl;
            }
            else{
                cout << "Student answer: " << "Not answered" << endl;
            }
            cout << "The real answer: " << q["answer"].get<string>() << endl;
            cout << "The maximum score: " << q["positivePoint"].get<int>() << endl;
            cout << "Enter received score (0-" << q["positivePoint"].get<int>() << "): ";
            float point;
            cin >> point;
            
            if(point < 0 || point > q["positivePoint"].get<int>()){
                cout << "Invalid point! Please enter a value between 0 and " << q["positivePoint"].get<int>() << endl;
                continue;
            }
            
            if(!q.contains("received_score")) {
                q["received_score"] = point;
            }
            if(!q.contains("is_correct")) {
                q["is_correct"] = (point == q["positivePoint"].get<int>());
            }
            
            max_score += q["positivePoint"].get<int>();
            current_score += point;
            
            cout << "--------------------------------" << endl;
        }
    }
    (*exam_stu)["received points"] = current_score;
    (*exam_stu)["maximum_score"] = max_score;
    current_score = current_score / max_score * 20;
    for(auto &ex : (*student)["exams_reg"]){
        if(ex["code"] == code){
            ex = *exam_stu;
            break;
        }
    }

    for(auto &ex : data["exams"]){
        if(ex["code"] == code){
            for(auto &score : ex["scores"]){
                if(score["student_name"] == student_name){
                    score["score"] = round(current_score * 100.0) / 100.0;
                    break;
                }
            }
            break;
        }
    }

    saveJsonData("users.json", data);
    cout << "Grades saved successfully for " << student_name << endl;
    cout <<"###############################################################" << endl;
}
bool Professor::prof_functionality(){
    json data = loadJsonData("users.json");
    do{
        json *prof = findProfessor(data, id);
        int func = prof_menu();
        if(func == 1){
            string code = create_exam(*prof);
            data = loadJsonData("users.json");
            prof = findProfessor(data, id);
            Exam exam; 
            exam.code = code;
            do{
                cout << "1.Add Question\t2.Remove Question\t3.Return to menu" << endl;
                int method;
                cin >> method;
                cout <<"###############################################################" << endl;
                if(method == 1){
                    exam.add(*prof); 
                }
                else if(method == 2){
                    exam.del(*prof);
                }
                else if(method == 3){
                    break;
                    cout <<"###############################################################" << endl;
                }
                else{
                    cout << "Wrong choice :| Try again." << endl;
                    cout <<"###############################################################" << endl;
                }
                data = loadJsonData("users.json");
                prof = findProfessor(data, id);
                if(!prof){
                    cout << "Professor not found after operation!" << endl;
                    cout <<"###############################################################" << endl;
                    return false;
                }
            }while(true);
        }
        else if(func == 2){
            bool flag = view_exams(*prof);
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
            view_students_scores(*prof);
        }
        else if(func == 4){
            grade_long_questions(*prof);
        }
        else if(func == 5){
            return true;
        }
    }while(true);
}

//-------------------------------------------------------------
void Student::signup(){
    json data = loadJsonData("users.json");
    name = set_name();
    cout << "Enter your field of study: ";
    getline(cin >> ws, field);
    cout <<"###############################################################" << endl;
    id = set_id();
    password = set_password();
    data["students"].push_back({
        {"name", name},
        {"id", id},
        {"password", password},
        {"field", field},
        {"numOfExams", 0},
        {"numsOfScores", 0}
    });
    saveJsonData("users.json", data);
    cout << "Congrats:) You're successfully Signed Up as a student" << endl;
    cout <<"###############################################################" << endl;
}
int Student::stu_menu(){
    cout << "1.Register for a new exam\n2.View registered exams\n3.Participate in new exam\n4.View my scores\n5.Log Out" << endl;
    int choice;
    cin >> choice;
    while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5){
        cout << "Wrong choice :| Try again." << endl;
        cout << "###############################################################" << endl;
        cin >> choice;
    }
    cout << "###############################################################" << endl;
    return choice;
}
json *Student::findStudent(json& data, const string& studentID){
    if(data.contains("students")){
        for(auto& stu : data["students"]){
            if(stu["id"] == studentID)
                return &stu;
        }
    }
    return nullptr;
}
void Student::reg_exam(json &student){
    json data = loadJsonData("users.json");
    cout << "Enter the exam code: ";
    string code;
    cin >> code;
    if(!student.contains("exams_reg")){
        student["exams_reg"] = json::array();
        student["numOfExams"] = 0;
    }
    if(student["numOfExams"] > 0){
        for(auto &ex : student["exams_reg"]){
            if(code == ex["code"]){
                cout << "Exam already registered!!" << endl;
                cout << "###############################################################" << endl;
                return;
            }
        }
    }
    for(auto &prof : data["professors"]){
        if(prof["numOfExams"] > 0){
            for(auto ex : prof["exams"]){
                if(code == ex["code"]){
                    student["exams_reg"].push_back({
                        {"name", ex["name"]},
                        {"code", ex["code"]},
                        {"questions", ex["questions"]},
                        {"NumOfQuestions", ex["NumOfQuestions"]}
                    });
                    student["numOfExams"] = student["exams_reg"].size();
                    for(auto &s : data["students"]){
                        if(s["id"] == student["id"]){
                            s = student;
                            break;
                        }
                    }
                    saveJsonData("users.json", data);
                    cout << "Registered successfully" << endl;
                    cout << "###############################################################" << endl;
                    return;
                }
            }
        }
    }
    cout << "Exam not found!! Returning to menu" << endl;
    cout << "###############################################################" << endl;
}
void Student::view_exams(json &student){
    if(student["numOfExams"] == 0)
        cout << "No registered exam!!" << endl;
    else{
        for(auto ex : student["exams_reg"]){
            cout << "-" << ex["name"].get<string>() << " : " << ex["code"].get<string>() << endl;
        }
    }
    cout << "###############################################################" << endl;

}
string Student::participate_exam(json &student){
    if(student["numOfExams"] == 0){
        cout << "No exam to participate in!!" << endl;
        cout << "###############################################################" << endl;
        return"";
    }
    cout << "Enter the exam code: ";
    string code;
    cin >> code;
    for(auto ex : student["exams_reg"]){
        if(code == ex["code"])
            return code;
    }
    cout << "Wrong code!! Try again" << endl;
    cout << "###############################################################" << endl;
    return "";
}
void Student::random_options(vector <string>& options){
    srand(static_cast<unsigned>(time(0)));
    for(size_t i = 0; i < 4; i++)
    {
        size_t randomIndex = rand() % 4;
        swap(options[i], options[randomIndex]);
    }
}
vector <string> Student::four_options_display(json &q){
    vector <string> options;
    for(auto &opt : q["options"])
        options.push_back(opt);
    random_options(options);
    int tmp = 1;
    for(auto &opt : options)
        cout << tmp++ << ")" << opt << "\t";
    cout << endl;
    return options;
}
float Student::calculate_score(json &exam, bool *is_answered, string *answers){
    int counter = 0;
    float received_points = 0;
    int maximum_score = 0;
    for(auto &q : exam["questions"]){
        if(is_answered[counter] == true){
            if(q["type"] == "four options"){
                if(q["answer"]["text"] == answers[counter]){
                    received_points += q["positivePoint"].get<int>(); 
                    if(q.contains("received_score"))
                        q["received_score"] = q["positivePoint"].get<int>();
                    else
                        q.push_back({"received_score", q["positivePoint"].get<int>()});
                    if(q.contains("is_correct"))
                        q["is_correct"] = true;
                    else
                        q.push_back({"is_correct", true});
                }
                else{
                    received_points += q["negativePoint"].get<int>();
                    if(q.contains("received_score"))
                        q["received_score"] = q["negativePoint"].get<int>();
                    else
                        q.push_back({"received_score", q["negativePoint"].get<int>()});
                    if(q.contains("is_correct"))
                        q["is_correct"] = false;
                    else
                        q.push_back({"is_correct", false});
                }

            }
            else if(q["type"] == "short answer"){
                if(q["answer"] == answers[counter]){
                    received_points += q["positivePoint"].get<int>(); 
                    if(q.contains("received_score"))
                        q["received_score"] = q["positivePoint"].get<int>();
                    else
                        q.push_back({"received_score", q["positivePoint"].get<int>()});
                    if(q.contains("is_correct"))
                        q["is_correct"] = true;
                    else
                        q.push_back({"is_correct", true});
                }
                else{
                    received_points += q["negativePoint"].get<int>();
                    if(q.contains("received_score"))
                        q["received_score"] = q["negativePoint"].get<int>();
                    else
                        q.push_back({"received_score", q["negativePoint"].get<int>()});
                    if(q.contains("is_correct"))
                        q["is_correct"] = false;
                    else
                        q.push_back({"is_correct", false});
                }
            }
            else if(q["type"] == "long answer"){
                //do nothing
            }
        }
        else{
            if(q["type"] != "long answer"){
                if(q.contains("received_score"))
                    q["received_score"] = 0;
                else
                    q.push_back({"received_score", 0});
                if(q.contains("is_correct"))
                    q["is_correct"] = false;
                else
                    q.push_back({"is_correct", false});
            }
        }
        if(q["type"] == "long answer"){
            //do nothing
        }
        else
            maximum_score += q["positivePoint"].get<int>();
        counter++;
    }
    exam.push_back({"received points", received_points});
    exam.push_back({"maximum_score", maximum_score});
    float score = received_points / maximum_score * 20;
    return score;
}
void Student::add_score_to_examJson(json &exam, float score, json &data, json &student){
    for(auto& ex : data["exams"]){
        if(ex["code"] == exam["code"]){
            if(!ex.contains("scores")){
                ex["scores"] = json::array();
            }
            name = student["name"];
            ex["scores"].push_back({
                {"student_name", name},
                {"score", round(score * 100) / 100.0}
            });
            ex["numOfScores"] = ex["scores"].size();
            break;
        }
    }
}
void Student::exam_process(string examCode, json &student){
    json data = loadJsonData("users.json");
    Exam tmp;
    json *exam = tmp.findExamStu(student, examCode);
    if((*exam)["NumOfQuestions"] < 3){
        cout << "Exam is not created or not enough questions to begin!!" << endl;
        cout << "###############################################################" << endl;
        return;
    }
    cout << "----------------------- Exam is started -----------------------" << endl;
    int len = (*exam)["NumOfQuestions"];
    string answers[len];
    bool is_answered[len] = {false};
    int ans_num = 0;
    int current_q = 0;
    vector <string> options;
    while(true){
        if(current_q == 0){
            auto q = (*exam)["questions"].begin();
            cout << current_q+1 << ")" << (*q)["text"].get<string>() << " (" << (*q)["type"].get<string>() << ")" << endl;
            if((*q)["type"] == "four options")
                options = four_options_display(*q);
            cout << "1.Answer\t2.Next" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                if((*q)["type"] == "four options"){
                    int index;
                    cin >> index;
                    answers[current_q] = options[index-1];
                }
                else
                    getline(cin >> ws, answers[current_q]);
                if((*q)["type"] == "long answer"){
                    (*q)["student answer"] = answers[current_q];
                }
                is_answered[current_q] = true;
                current_q++;
            }
            else if(choice == 2){
                current_q++;
            }
            else{
                cout << "Wrong choice!! Try again" << endl;
            }
        }
        else if(current_q == len-1){
            auto q = (*exam)["questions"].end() - 1;
            cout << current_q+1 << ")" << (*q)["text"].get<string>() << " (" << (*q)["type"].get<string>() << ")"  << endl;
            if((*q)["type"] == "four options")
                options = four_options_display(*q);
            cout << "1.Answer\t2.Previous\t3.Finish" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                if((*q)["type"] == "four options"){
                    int index;
                    cin >> index;
                    answers[current_q] = options[index-1];
                }
                else
                    getline(cin >> ws, answers[current_q]);
                if((*q)["type"] == "long answer"){
                    (*q)["student answer"] = answers[current_q];
                }
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
            auto q = (*exam)["questions"].begin() + current_q;
            cout << current_q+1 << ")" << (*q)["text"].get<string>() << " (" << (*q)["type"].get<string>() << ")"  <<  endl;
            if((*q)["type"] == "four options")
                options = four_options_display(*q);
            cout << "1.Answer\t2.Next\t3.Previous" << endl;
            int choice;
            cin >> choice;
            if(choice == 1){
                cout << "Your answer: ";
                if((*q)["type"] == "four options"){
                    int index;
                    cin >> index;
                    answers[current_q] = options[index-1];
                }
                else
                    getline(cin >> ws, answers[current_q]);
                if((*q)["type"] == "long answer"){
                    (*q)["student answer"] = answers[current_q];
                }
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
    float score = calculate_score(*exam, is_answered, answers);
    printf("Your score is: %.2f (without long questions)\n", score);
    cout << "###############################################################" << endl;
    score = round(score * 100) / 100.0;
    add_score_to_examJson(*exam, score, data, student);
    if(!student.contains("exams_participated")){
        student["exams_participated"] = json::array();
    }
    student["exams_participated"].push_back({
        {"name", (*exam)["name"]},
        {"code", (*exam)["code"]},
    });
    student["numsOfScores"] = student["exams_participated"].size();
    for(auto& s : data["students"]){
        if(s["id"] == student["id"]){
            s = student;
            break;
        }
    }
    saveJsonData("users.json", data);
}
void Student::create_csv(string code, json &student) {
    json data = loadJsonData("users.json");
    
    float score;
    string exam_name;
    Exam tmp;
    json* exam = tmp.findExamStu(student, code);
    json exam_score;
    for(auto& ex : data["exams"]) {
        if(ex["code"] == code) {
            exam_score = ex;
            exam_name = ex["name"];
            for(auto& s : ex["scores"]) {
                if(s["student_name"] == student["name"]) {
                    score = s["score"];
                    break;
                }
            }
            break;
        }
    }
    
    string filename = student["name"].get<string>() + "_" + code + ".csv";
    ofstream csv_file(filename);
    

    csv_file << "Exam Name, Your Score, received Scores (JSON), Is Correct (JSON), Average Score, Maximum Score, Your Position, Number of Students\n";

    csv_file << exam_name << ", " << score << ", ";
    csv_file << "[";
    bool first = true;
    for(auto& q : (*exam)["questions"]){
        if(q.contains("received_score")) {
            if(!first) csv_file << "; ";
            csv_file << "(Q:" << q["text"].get<string>() << " You got " << q["received_score"].get<int>() << " points)";
            first = false;
        }
    }
    csv_file << "], ";
    
    csv_file << "[";
    first = true;
    for(auto& q : (*exam)["questions"]){
        if(q.contains("is_correct")) {
            if(!first) csv_file << "; ";
            csv_file << "(Q:" << q["text"].get<string>() << ":" << (q["is_correct"].get<bool>() ? "true" : "false") << ")";
            first = false;
        }
    }
    csv_file << "], ";
    
    float total_score = 0;
    int num_students = 0;
    for(auto& s : exam_score["scores"]) {
        total_score += s["score"].get<float>();
        num_students++;
    }
    float average_score = num_students > 0 ? total_score / num_students : 0;
    
    float max_score = 0;
    for(auto& s : exam_score["scores"]) {
        if(s["score"].get<float>() > max_score) {
            max_score = s["score"].get<float>();
        }
    }
    
    int position = 1;
    for(auto& s : exam_score["scores"]) {
        if(s["score"].get<float>() > score) {
            position++;
        }
    }
    csv_file << average_score << ", "
             << max_score << ", "
             << position << ", "
             << num_students << "\n";
    
    csv_file.close();
    cout << "The CSV file is created successfully for your score(in your directory)" << endl;
    cout << "File name: " << filename << endl;
    cout << "###############################################################" << endl;
}
void Student::view_scores(json &student){
    if(student["numsOfScores"] == 0){
        cout << "No scores to show!!" << endl;
        cout << "###############################################################" << endl;
        return;
    }
    for(auto& ex : student["exams_participated"]){
        cout << ex["name"].get<string>() << " : " << ex["code"].get<string>() << endl;
    }
    cout << "###############################################################" << endl;
    bool flag = false;
    while(!flag){
        cout << "Enter the exam code: ";
        string code;
        cin >> code;
        for(auto& ex : student["exams_participated"]){
            if(code == ex["code"]){
                create_csv(code, student);
                flag = true;
                break;
            }
        }
        if(flag == false){
            cout << "Wrong code!! Try again" << endl;
            cout << "###############################################################" << endl;
        }
    }
}
bool Student::stu_functionality(){
    while(true){
        json data = loadJsonData("users.json");
        json *student = findStudent(data, id);
        int func = stu_menu();
        if(func == 1){
            reg_exam(*student);
        }
        if(func == 2){
            view_exams(*student);
        }
        if(func == 3){
            string examCode = participate_exam(*student);
            if(examCode != "")
                exam_process(examCode, *student);
        }
        else if(func == 4){
            view_scores(*student);
        }
        else if(func == 5){
            return true;
        }
    }
}

//--------------------------------------
int main(){
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
                Professor P;
                P.signup();
                bool Loggedout = P.prof_functionality();
                if(Loggedout){
                    cout << "Logged out successfully" << endl;
                    continue;
                }
            }
            else if(choice == 2){
                Student S;
                S.signup();
                bool Loggedout = S.stu_functionality();
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
            User U;
            string id;
            UserType stat = NONE;
            U.login(id, stat);
            if(stat == PROFESSOR){
                Professor P;
                P.load_data(id);
                bool Loggedout = P.prof_functionality();
                if(Loggedout){
                    cout << "Logged out successfully" << endl;
                    continue;
                }
            }
            else if(stat == STUDENT){
                Student S;
                S.load_data(id);
                bool Loggedout = S.stu_functionality();
                if(Loggedout){
                    cout << "Logged out successfully" << endl;
                    continue;
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