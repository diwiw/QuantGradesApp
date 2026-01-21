#include "Grades.hpp"

#include <algorithm>
#include <iostream>

#include "core/Statistics.hpp"

namespace qga::core
{

    Grades::Grades() {}

    void Grades::add(int grade) { notes_.push_back(grade); }

    float Grades::getMean() const
    {
        if (notes_.empty())
            return 0.0f;

        auto mean_opt = qga::core::Statistics::calculateMean(notes_);

        return mean_opt ? static_cast<float>(*mean_opt) : 0.0f;
    }

    float Grades::getMedian() const
    {
        if (notes_.empty())
            return 0.0f;

        auto median_opt = qga::core::Statistics::calculateMedian(notes_);

        return median_opt ? static_cast<float>(*median_opt) : 0.0f;
    }

    float Grades::getStdDev() const
    {
        if (notes_.size() < 2)
            return 0.0f;

        auto stddev_opt = qga::core::Statistics::calculateStdDev(notes_);
        return stddev_opt ? static_cast<float>(*stddev_opt) : 0.0f;
    }

    void Grades::printGrades() const
    {
        for (int g : notes_)
            std::cout << g << " ";
        std::cout << "\n";
    }

    void Grades::printSummary() const
    {
        if (notes_.empty())
        {
            std::cout << "No grades available.\n";
            return;
        }

        std::cout << "\n=== Statistics ===\n";
        auto mean = qga::core::Statistics::calculateMean(notes_);
        auto median = qga::core::Statistics::calculateMedian(notes_);
        auto stddev = qga::core::Statistics::calculateStdDev(notes_);
        auto max_v = qga::core::Statistics::calculateMax(notes_);
        auto min_v = qga::core::Statistics::calculateMin(notes_);

        if (mean)
            std::cout << "Mean: " << mean.value() << "\n";
        if (median)
            std::cout << "Median: " << median.value() << "\n";
        if (stddev)
            std::cout << "StdDev: " << stddev.value() << "\n";
        if (max_v)
            std::cout << "Max: " << max_v.value() << "\n";
        if (min_v)
            std::cout << "Min: " << min_v.value() << "\n";
    }

    const std::vector<int>& Grades::getNotes() const { return notes_; }

    int Grades::getMax() const
    {
        if (notes_.empty())
            return 0;

        auto max_opt = qga::core::Statistics::calculateMax(notes_);
        return max_opt ? (*max_opt) : 0;
    }

    int Grades::getMin() const
    {
        if (notes_.empty())
            return 0;

        auto min_opt = qga::core::Statistics::calculateMin(notes_);
        return min_opt ? (*min_opt) : 0;
    }

} // namespace qga::core
