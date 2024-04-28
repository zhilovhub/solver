#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;


class InstructionsExporter {
    public:
        virtual void write_text_to_temp_file(string text) {};

        virtual void export_instructions() {};
};


class InstructionsExporterImpl : public InstructionsExporter {
    private:
        string generate_file_name() {
            time_t current_time = time({});
            char current_time_string[size("yyyy_mm_ddThh_mm_ss")];
            strftime(data(current_time_string), size(current_time_string), "%Y_%m_%dT%H%M%S", gmtime(&current_time));

            return string(current_time_string) + ".txt";
        }

    public:
        const string instructions_directory = "./instructions";
        const string temp_filename = "cucu.txt";

        void write_text_to_temp_file(string text) override {
            if (!filesystem::exists(instructions_directory)) {
                filesystem::create_directory(instructions_directory);
            }
            
            ofstream file;
            file.open(instructions_directory + "/" + temp_filename);
            file << text;
            file.close();
        }

        void export_instructions() override {
            if (!filesystem::exists(instructions_directory)) {
                filesystem::create_directory(instructions_directory);
            }

            string instructions_filename = instructions_directory + "/" + generate_file_name();
            ifstream temp_file { instructions_directory + "/" + temp_filename };
            ofstream instructions_file { instructions_filename };

            if (temp_file && instructions_file) {
                string line;
                while (getline(temp_file, line)) {
                    instructions_file << line;
                }

                cout << "Export finished to " + instructions_filename << endl;
            } else {
                cout << "Can't read the files" << endl;
            }

            temp_file.close();
            instructions_file.close();
        }
};


class EquationSolver {
    private:
        int a, b, c;
        InstructionsExporter* exporter;
    public:
        EquationSolver(int a, int b, int c, InstructionsExporter* exporter) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->exporter = exporter;
        }

        map<string, int> solve() {
            char buff[100];
            if (a == 0 && b == 0 && c == 0) {
                sprintf(buff, "Your equation is: 0 = 0");
            } else if (a == 0 && b == 0) {
                sprintf(buff, "Your equation is: %d = 0", c);
            } else if (a == 0 && c == 0) {
                sprintf(buff, "Your equation is: %dx = 0", b);
            } else if (b == 0 && c == 0) {
                sprintf(buff, "Your equation is: %dx² = 0", a);
            } else if (a == 0) {
                sprintf(buff, "Your equation is: %dx + %d = 0", b, c);
            } else if (b == 0) {
                sprintf(buff, "Your equation is: %dx² + %d = 0", a, c);
            } else if (c == 0) {
                sprintf(buff, "Your equation is: %dx² + %dx = 0", a, b);
            } else  {
                sprintf(buff, "Your equation is: %dx² + %dx + %d = 0", a, b, c);
            }

            cout << buff << endl;
            exporter -> write_text_to_temp_file(buff);
            

            int D = pow(b, 2) - a * c;

            if (D > 0) {

            } else if (D == 0) {

            } else {

            }

            map<string, int> result;
            return result;      
        }
        
        void export_instructions() {
            exporter -> export_instructions();
        }
};



int main()
{
    int a, b, c;

    cout << "Enter the coefficients of the equation in format: a b c: ";
    cin >> a >> b >> c;


    InstructionsExporterImpl* exporter = new InstructionsExporterImpl();
    EquationSolver solver(a, b, c, exporter);

    map<string, int> solutions = solver.solve();
    solver.export_instructions();
}