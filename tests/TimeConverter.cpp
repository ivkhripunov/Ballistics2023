//
// Created by ivankhripunov on 30.11.23.
//

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "time/TimeConverter.h"
#include "time/DutCorrection.h"
#include "../data/time_result.hpp"

using scalar = Ballistics::scalar;

template<typename RealType, typename DutContainer>
using TimeConverter = Ballistics::TimeModule::TimeConverter<RealType, DutContainer>;
using DutContainer = Ballistics::TimeModule::DutContainer;

template<typename RealType, Ballistics::TimeModule::TimeScale Scale>
using Time = Ballistics::TimeModule::Time<RealType, Scale>;

using TimeScale = Ballistics::TimeModule::TimeScale;

//TODO: add string to containers
//TODO: add parsers to another file

[[nodiscard]] std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) noexcept {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

[[nodiscard]] std::vector<std::string> split(const std::string &s, const std::string &delimiter) noexcept {
    Ballistics::indexType pos_start = 0, pos_end, delim_len = delimiter.length();
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        const std::string token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

TEST(CONVERT, SET1) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.0320059,
            -0.0330198,
            -0.0340678,
            -0.0351611,
            -0.0361345,
            -0.0370076,
            -0.0377478
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58486;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    const TimeConverter<scalar, DutContainer> timeConverter(dutContainer);

    const Time<scalar, TimeScale::UTC_SCALE> utc(Ballistics::timeResult[0][3], Ballistics::timeResult[0][4]);

    const Time<scalar, TimeScale::UT1_SCALE> ut1 = timeConverter.convertUTC_UT1(utc);
    const Time<scalar, TimeScale::TAI_SCALE> tai = timeConverter.convertUTC_TAI(utc);

    const Time<scalar, TimeScale::UT1_SCALE> ut1_reference(Ballistics::timeResult[0][1], Ballistics::timeResult[0][2]);
    const Time<scalar, TimeScale::TAI_SCALE> tai_reference(Ballistics::timeResult[0][5], Ballistics::timeResult[0][6]);

    ASSERT_DOUBLE_EQ(ut1.jdDayInt(), ut1_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(ut1.jdDayFrac(), ut1_reference.jdDayFrac());

    ASSERT_DOUBLE_EQ(tai.jdDayInt(), tai_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(tai.jdDayFrac(), tai_reference.jdDayFrac());

}

TEST(PARSER, SET1) {

    std::ifstream inputFile;
    inputFile.open("/home/ivankhripunov/CLionProjects/ballistics2023/data/eopc04_IAU2000.62-now");

    Ballistics::indexType counter = 0;
    std::string line, resultLine;
    if (inputFile.is_open()) {

        while(!inputFile.eof()) {
            getline(inputFile, line);

            if (counter > 13 && counter < 20) {
                resultLine = ReplaceAll(line, "    ", " ");
                resultLine = ReplaceAll(resultLine, "   ", " ");
                resultLine = ReplaceAll(resultLine, "  ", " ");

                const std::string delimiter = " ";
                const Ballistics::Containers::vector<std::string> stringVector = split(resultLine, delimiter);

                const auto mjd = static_cast<scalar>(std::stod(stringVector[0]));

                std::cout << mjd + 1 << std::endl;
            }

            counter++;
        }

        inputFile.close();
    }
}
