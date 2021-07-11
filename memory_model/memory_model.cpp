#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>
#include <vector>

namespace chr = std::chrono;

int main()
{
    double data_ratio = 0.5;
    double delta = 0.01;
    constexpr unsigned data_size = 1'000'000;

    std::map<unsigned, int> map;
    std::unordered_map<unsigned, int> unordered_map;
    std::vector<int> vector;

    std::random_device random_device;
    std::minstd_rand engine(random_device());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    chr::nanoseconds map_time(0);
    chr::nanoseconds unordered_map_time(0);
    chr::nanoseconds vector_time(0);
    for (; vector_time <= map_time && data_ratio < 1 && data_ratio > 0; data_ratio += delta) {
        map.clear();
        unordered_map.clear();
        vector.clear();

        map_time = chr::nanoseconds::zero();
        unordered_map_time = chr::nanoseconds::zero();
        vector_time = chr::nanoseconds::zero();

        for (unsigned i = 0; i < data_size; ++i) {
            if (distribution(engine) >= data_ratio) {
                auto map_start = chr::system_clock::now();
                map.insert({ i, 1 });
                auto map_finish = chr::system_clock::now();

                auto unordered_map_start = chr::system_clock::now();
                unordered_map.insert({ i, 1 });
                auto unordered_map_finish = chr::system_clock::now();

                auto vector_start = chr::system_clock::now();
                vector.push_back(1);
                auto vector_finish = chr::system_clock::now();

                map_time += chr::duration_cast<chr::nanoseconds>(map_finish - map_start);
                unordered_map_time += chr::duration_cast<chr::nanoseconds>(unordered_map_finish - unordered_map_start);
                vector_time += chr::duration_cast<chr::nanoseconds>(vector_finish - vector_start);
            }
            else {
                auto vector_start = chr::system_clock::now();
                vector.push_back(0);
                auto vector_finish = chr::system_clock::now();

                vector_time += chr::duration_cast<chr::nanoseconds>(vector_finish - vector_start);
            }
        }

        std::cout << std::setw(20) << "map_time:\t" << map_time.count() <<
            std::setw(10) << "delta: " << map_time.count() - vector_time.count() << std::endl <<
            std::setw(20) << "unordered_map_time:\t" << unordered_map_time.count() <<
            std::setw(10) << "delta: " << unordered_map_time.count() - vector_time.count() << std::endl <<
            std::setw(20) << "vector_time:\t" << vector_time.count() << std::endl <<
            std::setw(20) << "data_ratio:\t" << data_ratio << std::endl << std::endl;
    }

    std::cout << "\n\tResult: " << data_ratio - delta << std::endl;

    auto ptr1 = operator new(64);
    auto char_ptr1 = reinterpret_cast<char*>(ptr1);
    auto ptr2 = operator new(1024);
    auto char_ptr2 = reinterpret_cast<char*>(ptr2);

    auto l1_start = chr::system_clock::now();
    for (unsigned i = 0; i < 1'049'600; ++i) {
        char_ptr1[i % 64] = 1;
    }
    auto l1_finish = chr::system_clock::now();

    auto l2_start = chr::system_clock::now();
    for (unsigned i = 0; i < 64; ++i) {
        char_ptr2[i * 16] = 1;
    }
    auto l2_finish = chr::system_clock::now();

    std::cout << "L1 time: " << chr::duration_cast<chr::nanoseconds>(l1_finish - l1_start).count() << std::endl <<
        "L2 time: " << chr::duration_cast<chr::nanoseconds>(l2_finish - l2_start).count() << std::endl;

    return 0;
}