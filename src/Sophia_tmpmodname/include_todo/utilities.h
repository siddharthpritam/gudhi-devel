#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>

#include "tower_converter.h"

namespace Gudhi {
namespace tmp_package_name {

template<class ComplexStructure>
Tower_converter<ComplexStructure>::type read_operation(std::string *line, std::vector<double> *vertices, double *timestamp)
{
    using TC = Tower_converter<ComplexStructure>;
    TC::type type;
    vertices->clear();
    double num;

    size_t next = line->find_first_not_of(' ', 0);
    size_t current = next;
    next = line->find_first_of(' ', current);
    if (next == std::string::npos) return TC::COMMENT;
    if (line->substr(current, next - current) == "i") type = TC::INCLUSION;
    else if (line->substr(current, next - current) == "c") type = TC::CONTRACTION;
    else if (line->substr(current, next - current) == "#") return TC::COMMENT;
    else {
        *timestamp = stod(line->substr(current, next - current));
        next = line->find_first_not_of(' ', next + 1);
        current = next;
        next = line->find_first_of(' ', current);
        if (next == std::string::npos) {
            std::cout << "Operation syntaxe error in file.\n";
            exit(0);
        }
        if (line->substr(current, next - current) == "i") type = TC::INCLUSION;
        else if (line->substr(current, next - current) == "c") type = TC::CONTRACTION;
        else if (line->substr(current, next - current) == "#") return TC::COMMENT;
        else {
            std::cout << "Operation syntaxe error in file.\n";
            exit(0);
        }
    }

    next = line->find_first_not_of(' ', next + 1);
    while (next != std::string::npos){
        current = next;
        next = line->find_first_of(' ', current);
        num = stod(line->substr(current, next - current));
        vertices->push_back(num);
        if (next != std::string::npos) next = line->find_first_not_of(' ', next + 1);
    }

    return type;
}

template<class ComplexStructure>
std::istream& operator>>(std::istream& file, Tower_converter<ComplexStructure>& tc)
{
    using TC = Tower_converter<ComplexStructure>;
    std::string line;

    if (file.is_open()){
        std::vector<double> vertices;
        double timestamp = -1;
        double defaultTimestamp = 0;
        while (getline(file, line, '\n')){
            TC::type type = read_operation(&line, &vertices, &timestamp);
            if (timestamp != -1) defaultTimestamp = timestamp;

            if (type == TC::INCLUSION){
                if (tc.add_insertion(&vertices, defaultTimestamp)) defaultTimestamp++;
            } else if (type == TC::CONTRACTION) {
                tc.add_contraction(vertices.at(0), vertices.at(1), defaultTimestamp);
                defaultTimestamp++;
            }

            timestamp = -1;
        }
        file.close();
    } else {
        std::cout << "Unable to open input file.\n";
        file.setstate(std::ios::failbit);
    }

    return file;
}

}
}

#endif // UTILITIES_H
