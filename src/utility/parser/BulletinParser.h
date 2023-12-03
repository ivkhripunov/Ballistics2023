//
// Created by ivankhripunov on 02.12.23.
//

#ifndef BALLISTICS2023_BULLETINPARSER_H
#define BALLISTICS2023_BULLETINPARSER_H

#include <fstream>
#include "../types/BasicTypes.h"

namespace Ballistics::Utility {

    /**
     * Меняет подстроки в строке на заданные
     * @param str строка для изменения
     * @param from что меняем
     * @param to на что меняем
     * @return строка с замененными подстроками
     */
    [[nodiscard]] Containers::string ReplaceAll(Containers::string str, const Containers::string &from, const Containers::string &to) noexcept {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != Containers::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    /**
     * Аналог функции split() на питоне
     * @param s строка
     * @param delimiter разделитель
     * @return вектор разделенных строк
     */
    [[nodiscard]] Containers::vector<Containers::string>
    split(const Containers::string &s, const Containers::string &delimiter = " ") noexcept {
        Ballistics::indexType pos_start = 0, pos_end, delim_len = delimiter.length();
        Containers::vector<Containers::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != Containers::string::npos) {
            const Containers::string token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }

    struct MJD_DUT {
        Containers::vector<scalar> mjdVector;
        Containers::vector<scalar> dutVector;
    };

    /**
     * Парсит файлик бюллетени и возвращает значения дута в заданном отрезке (левая и правая границы входят)
     * @param path путь до файла eopc04_IAU2000.62-now
     * @param mjdBegin момент, с которого записываем данные в массив
     * @param mjdEnd момент, до которого записываем данные в массив
     * @return 2 массива: mjd и соответствующие dut
     */
    [[nodiscard]] MJD_DUT bulletinGetMJD_DUT(const Containers::string &path, const scalar mjdBegin, const scalar mjdEnd, bool CSV = false) noexcept {

        Containers::vector<scalar> mjdVector, dutVector;

        std::ifstream inputFile;
        inputFile.open(path);

        Ballistics::indexType counter = 1;
        Containers::string line, resultLine;
        if (inputFile.is_open()) {

            while (!inputFile.eof()) {
                std::getline(inputFile, line);

                //данные начинаются с 15 строчки (счетчик с 1)
                if (counter >= 15) {

                    resultLine = ReplaceAll(line, "    ", " ");
                    resultLine = ReplaceAll(resultLine, "   ", " ");
                    resultLine = ReplaceAll(resultLine, "  ", " ");

                    const Containers::string delimiter = " ";
                    const Ballistics::Containers::vector<Containers::string> stringVector = split(resultLine, delimiter);

                    //иначе последняя строчка имеет размер 1 и все падает
                    if (stringVector.size() == 16) {

                        const auto mjd = static_cast<scalar>(std::stod(stringVector[3]));
                        const auto dut = static_cast<scalar>(std::stod(stringVector[6]));

                        //добавляем только в нужных границах
                        if (mjd >= mjdBegin && mjd <= mjdEnd) {
                            mjdVector.push_back(mjd);
                            dutVector.push_back(dut);
                        }

                        if (mjd > mjdEnd) {
                            return {mjdVector, dutVector};
                        }
                    }
                }

                counter++;
            }

            inputFile.close();
        }

        return {mjdVector, dutVector};
    }



    /**
     * Парсит файлик СSV и возвращает значения дута в заданном отрезке (левая и правая границы входят)
     * @param path путь до файла CSV
     * @param mjdBegin момент, с которого записываем данные в массив
     * @param mjdEnd момент, до которого записываем данные в массив
     * @return 2 массива: mjd и соответствующие dut
     */
    [[nodiscard]] MJD_DUT CSVgetMJD_DUT(const Containers::string &path, const scalar mjdBegin, const scalar mjdEnd) noexcept {

        Containers::vector<scalar> mjdVector, dutVector;

        std::ifstream inputFile;
        inputFile.open(path);

        Ballistics::indexType counter = 1;
        Containers::string line, resultLine;
        if (inputFile.is_open()) {

            while (!inputFile.eof()) {
                std::getline(inputFile, line);

                //данные начинаются с 15 строчки (счетчик с 1)
                if (counter >= 2) {

                    const Containers::string delimiter = ",";
                    const Ballistics::Containers::vector<Containers::string> stringVector = split(resultLine, delimiter);

                    //иначе последняя строчка имеет размер 1 и все падает
                    if (stringVector.size() == 16) {

                        const auto mjd = static_cast<scalar>(std::stod(stringVector[3]));
                        const auto dut = static_cast<scalar>(std::stod(stringVector[6]));

                        //добавляем только в нужных границах
                        if (mjd >= mjdBegin && mjd <= mjdEnd) {
                            mjdVector.push_back(mjd);
                            dutVector.push_back(dut);
                        }

                        if (mjd > mjdEnd) {
                            return {mjdVector, dutVector};
                        }
                    }
                }

                counter++;
            }

            inputFile.close();
        }

        return {mjdVector, dutVector};
    }
}

#endif //BALLISTICS2023_BULLETINPARSER_H
