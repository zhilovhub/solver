#include <iostream>
#include <string>
#include <chrono>

using namespace std;


class InstructionsExporter {
    public:
        virtual void export_instructions() {};
};


class InstructionsExporterImpl : public InstructionsExporter {
    private:
        string generate_file_name() {
            chrono::system_clock::time_point p = chrono::system_clock::now();
            time_t t = chrono::system_clock::to_time_t(p);
            char str[26];
            ctime_s(str, sizeof str, &t);
            return str;
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