#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <limits>
#include <cassert>
#include <cstdio>

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

        FILE* temp_file;

        InstructionsExporterImpl() {
            temp_file = tmpfile();
        }

        void write_text_to_temp_file(string text) override {
            fputs(text.c_str(), temp_file);
        }

        void export_instructions() override {
            if (!filesystem::exists(instructions_directory)) {
                filesystem::create_directory(instructions_directory);
            }

            string instructions_filename = instructions_directory + "/" + generate_file_name();
            ofstream instructions_file { instructions_filename };

            rewind(temp_file);
            char buffer[1024];
            if (temp_file && instructions_file) {
                while (fgets(buffer, sizeof buffer, temp_file) != nullptr) {
                    instructions_file << buffer;
                }
                cout << "Export finished to " + instructions_filename << endl;
            } else {
                cout << "Can't read the files" << endl;
            }

            instructions_file.close();
        }
};


class EquationSolver {
    private:
        float a, b, c;
        InstructionsExporter* exporter;
    public:
        EquationSolver(float a, float b, float c, InstructionsExporter* exporter) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->exporter = exporter;
        }

        map<string, float> solve() {
            char equation_string[100];
            if (a == 0 && b == 0 && c == 0) {
                sprintf(equation_string, "Your equation is: 0 = 0");
            } else if (a == 0 && b == 0) {
                sprintf(equation_string, "Your equation is: %.3f = 0", c);
            } else if (a == 0 && c == 0) {
                sprintf(equation_string, "Your equation is: %.3fx = 0", b);
            } else if (b == 0 && c == 0) {
                sprintf(equation_string, "Your equation is: %.3fx² = 0", a);
            } else if (a == 0) {
                sprintf(equation_string, "Your equation is: %.3fx + %.3f = 0", b, c);
            } else if (b == 0) {
                sprintf(equation_string, "Your equation is: %.3fx² + %.3f = 0", a, c);
            } else if (c == 0) {
                sprintf(equation_string, "Your equation is: %.3fx² + %.3fx = 0", a, b);
            } else  {
                sprintf(equation_string, "Your equation is: %.3fx² + %.3fx + %.3f = 0", a, b, c);
            }


            cout << equation_string << endl;
            exporter->write_text_to_temp_file(equation_string + string("\n\n"));

            map<string, float> solutions;

            if (a == 0 && b == 0 && c == 0) {
                solutions["solutions_count"] = numeric_limits<float>::infinity();
                exporter->write_text_to_temp_file("There are infinity solutions because 0 = 0 is always true\n");

                return solutions;
            } else if (a == 0) {
                if (b == 0) {
                    solutions["solutions_count"] = 0;
                    exporter->write_text_to_temp_file("There are no real solutions because non-zero doesn't equal zero\n");

                    return solutions;
                }
                
                float x = -c / b;

                char calculating_one_solution_step_string[300];
                sprintf(calculating_one_solution_step_string, "There is one solution that is calculated by x = -c / b\nx = -((%.3f) / (%.3f)) = %.3f\n\n", c, b, x);
                exporter->write_text_to_temp_file(calculating_one_solution_step_string);

                solutions["solutions_count"] = 1;
                solutions["x"] = x;

                return solutions;
            }
            

            float D = pow(b, 2) - 4 * a * c;
            solutions["D"] = D;

            char calculating_D_step_string[100];
            sprintf(calculating_D_step_string, "Calculating Discriminant by D = b² - 4ac:\nD = (%.3f)² - 4 * (%.3f) * (%.3f) = %.3f\n\n", b, a, c, D);

            exporter -> write_text_to_temp_file(calculating_D_step_string);

            string D_analysis_step_string;
            char calculation_solutions_step_string[300];

            if (D > 0) {
                D_analysis_step_string = "The D of the equation is greater than 0. There are two solutions";
                
                float D_sqrt = sqrt(D);

                float x1 = (-b + D_sqrt) / (2 * a);
                float x2 = (-b - D_sqrt) / (2 * a);

                solutions["solutions_count"] = 2;
                solutions["x1"] = x1;
                solutions["x2"] = x2;

                sprintf(calculation_solutions_step_string, "Calculating one solution by x = (-b + √D) / (2a):\nx = -(%.3f + √%.3f) / (2 * (%.3f)) = %.3f\n\nCalculating another solution by x = (-b - √D) / (2a):\nx = -(%.3f - √%.3f) / (2 * (%.3f)) = %.3f", b, D, a, x1, b, D, a, x2);

            } else if (D == 0) {
                D_analysis_step_string = "The D of the equation equals 0. There is one solution";

                float x = -b / (2 * a);

                solutions["solutions_count"] = 1;
                solutions["x"] = x;

                sprintf(calculation_solutions_step_string, "Calculating that one solution by x = (-b) / (2a):\nx = -(%.3f) / (2 * (%.3f))", b, a);

            } else {
                D_analysis_step_string = "The D of the equation is lower than 0. There are no real solutions";

                solutions["solutions_count"] = 0;

                sprintf(calculation_solutions_step_string, "");

            }

            exporter -> write_text_to_temp_file(D_analysis_step_string + string("\n\n"));
            exporter -> write_text_to_temp_file(calculation_solutions_step_string);

            
            return solutions;      
        }
        
        void export_instructions() {
            exporter -> export_instructions();
        }
};



class MockInstructionsExporterImpl : public InstructionsExporter {
    public:
        void write_text_to_temp_file(string text) override {
            cout << "write_text_to_temp_file with '" << text << "' called" << endl;
        }

        void export_instructions() override {}
};


void testEquationSolver() {
    MockInstructionsExporterImpl* mockExporter = new MockInstructionsExporterImpl();
    
    EquationSolver* solver;
    map<string, float> solutions;

    const string k = "solutions_count";
    const string x = "x";
    const string x1 = "x1";
    const string x2 = "x2";
    

    solver = new EquationSolver(0, 0, 0, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == numeric_limits<float>::infinity());

    solver = new EquationSolver(0, 0, 5, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 0);

    solver = new EquationSolver(0, 5, 0, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 1 && solutions[x] == 0);

    solver = new EquationSolver(0, 5, 10, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 1 && solutions[x] == -2);

    solver = new EquationSolver(0, 10, 5, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 1 && solutions[x] == -0.5);

    solver = new EquationSolver(4, 0, 0, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 1 && solutions[x] == 0);

    solver = new EquationSolver(10, 0, -100000, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 2 && solutions[x1] == 100 && solutions[x2] == -100);

    solver = new EquationSolver(10, 0, 100000, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 0);

    solver = new EquationSolver(1, -2, 1, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 1 && solutions[x] == 1);

    solver = new EquationSolver(1, -5.5, 2.5, mockExporter);
    solutions = solver->solve();
    assert(solutions[k] == 2 && solutions[x1] == 5 && solutions[x2] == 0.5);
}


int main()
{
    // uncomment it if you want to test solver
    // testEquationSolver();

    float a, b, c;
    
    while (true) {
        cout << "Enter the coefficients of the equation in format: a b c: ";

        cin >> a >> b >> c;
        if (cin.good() == false) {
            cout << "WRONG: you must enter three int or float numbers" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }
    


    InstructionsExporterImpl* exporter = new InstructionsExporterImpl();
    EquationSolver solver(a, b, c, exporter);

    map<string, float> solutions = solver.solve();

    float solutions_count = solutions["solutions_count"];
    const string x = "x";
    const string x1 = "x1";
    const string x2 = "x2";
    const string D = "D";

    if (solutions_count == 0) {
        cout << "There are no real solutions" << endl;
        if (solutions.find(D) != solutions.end()) {
            cout << "Discriminant was " << solutions[D] << endl;
        }
    } else if (solutions_count == 1) {
        cout << "x = " << solutions[x] << endl;
        if (solutions.find(D) != solutions.end()) {
            cout << "Discriminant was " << solutions[D] << endl;
        }
    } else if (solutions_count == 2) {
        cout << "x1 = " << solutions[x1] << endl;
        cout << "x2 = " << solutions[x2] << endl;
        cout << "Discriminant was " << solutions[D] << endl;
    } else if (solutions_count == numeric_limits<float>::infinity()) {
        cout << "There are infinity solutions" << endl;
    }


    bool need_calculation;
    cout << "\nDo you need calculation details? [1/0]: ";
    cin >> need_calculation;

    if (need_calculation == true) {
        solver.export_instructions();
    }

    return 0;
}
