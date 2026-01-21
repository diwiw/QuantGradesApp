/**
 * @file Grades.hpp
 * @brief A simple class to manage and analyze a collection of integer grades.
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/ILogger.hpp" // ✅ pełny include, zapewnia definicję qga::utils::ILogger

namespace qga::core
{

    /**
     * @class Grades
     * @brief Stores a list of integer grades and provides basic statistics.
     *
     * This class allows the user to add grades, retrieve them, and compute
     * various statistical metrics like mean, median, standard deviation,
     * maximum, and minimum. It also provides simple printing utilities.
     */
    class Grades
    {
      private:
        std::vector<int> notes_; ///< Container holding all grades.

      public:
        Grades();            ///< Default constructor
        void add(int grade); ///< Adds a new grade
        const std::vector<int>& getNotes() const;
        float getMean() const;     ///< Arithmetic mean
        float getMedian() const;   ///< Median
        float getStdDev() const;   ///< Standard deviation
        int getMax() const;        ///< Maximum
        int getMin() const;        ///< Minimum
        void printSummary() const; ///< Print summary
        void printGrades() const;  ///< Print all grades
    };

} // namespace qga::core
