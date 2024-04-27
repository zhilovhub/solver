#include <iostream>
#include <string>
#include <ctime>

using namespace std;


class InstructionsExporter {
    public:
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

        void write_text_to_file() {
            cout << "wrote .... to " << generate_file_name() << endl;
        }

    public:
        void export_instructions() override {
            write_text_to_file();
        };
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

        void test_method() {
            exporter->export_instructions();
        }
};



int main()
{
    cout << __cplusplus << endl;
    InstructionsExporterImpl* exporter = new InstructionsExporterImpl();
    EquationSolver solver(1, 2, -3, exporter);
    solver.test_method();
}