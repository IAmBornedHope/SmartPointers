#include <cstdio>
#include <chrono>
#include <memory>
#include <vector>
#include "unique_ptr/UniquePtr.hpp"
#include "shared_ptr/SharedPtr.hpp"

using Clock = std::chrono::steady_clock;

double get_ms(Clock::time_point t0) {
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

double unique_raw_create(std::size_t n) {
    auto t = Clock::now();
    {
        std::vector<int*> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(new int(i));
        }
        for (std::size_t i = 0; i < n; ++i)  {
            delete v[i]; v[i] = nullptr; 
        }
    }
    return get_ms(t);
}

double unique_std_create(std::size_t n) {
    auto t = Clock::now();
    {
        std::vector<std::unique_ptr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(std::make_unique<int>(i));
        }
    }
    return get_ms(t);
}

double unique_my_create(std::size_t n) {
    auto t = Clock::now();
    {
        std::vector<UniquePtr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(make_unique<int>(i));
        }
    }
    return get_ms(t);
}

double unique_std_move(std::size_t n) {
    std::vector<std::unique_ptr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(std::make_unique<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<std::unique_ptr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(std::move(src[i]));
        }
    }
    return get_ms(t);
}

double unique_my_move(std::size_t n) {
    std::vector<UniquePtr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(make_unique<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<UniquePtr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(std::move(src[i]));
        }
    }
    return get_ms(t);
}

double unique_std_reset(std::size_t n) {
    {
        std::vector<std::unique_ptr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(std::make_unique<int>(i));
        }

        auto t = Clock::now();
        for (std::size_t i = 0; i < n; ++i) {
            v[i].reset(new int(i));
        }
        return get_ms(t);
    }
}

double unique_my_reset(std::size_t n) {
    {
        std::vector<UniquePtr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(make_unique<int>(i));
        }

        auto t = Clock::now();
        for (std::size_t i = 0; i < n; ++i) {
            v[i].reset(new int(i));
        }
        return get_ms(t);
    }
}

double shared_std_create(std::size_t n) {
    auto t = Clock::now();
    {
        std::vector<std::shared_ptr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(std::make_shared<int>(i));
        }
    }
    return get_ms(t);
}

double shared_my_create(std::size_t n) {
    auto t = Clock::now();
    {
        std::vector<SharedPtr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(make_shared<int>(i));
        }
    }
    return get_ms(t);
}

double shared_std_copy(std::size_t n) {
    std::vector<std::shared_ptr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(std::make_shared<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<std::shared_ptr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(src[i]);
        }
    }
    return get_ms(t);
}

double shared_my_copy(std::size_t n) {
    std::vector<SharedPtr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(make_shared<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<SharedPtr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(src[i]);
        }
    }
    return get_ms(t);
}

double shared_std_move(std::size_t n) {
    std::vector<std::shared_ptr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(std::make_shared<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<std::shared_ptr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(std::move(src[i]));
        }
    }
    return get_ms(t);
}

double shared_my_move(std::size_t n) {
    std::vector<SharedPtr<int>> src;
    src.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        src.push_back(make_shared<int>(i));
    }

    auto t = Clock::now();
    {
        std::vector<SharedPtr<int>> dst;
        dst.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dst.push_back(std::move(src[i]));
        }
    }
    return get_ms(t);
}

double shared_std_reset(std::size_t n) {

        std::vector<std::shared_ptr<int>> v;
        v.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            v.push_back(std::make_shared<int>(i));
        }

        auto t = Clock::now();
        for (std::size_t i = 0; i < n; ++i) {
            v[i].reset(new int(i));
        }
        return get_ms(t);
}

double shared_my_reset(std::size_t n) {
    std::vector<SharedPtr<int>> v;
    v.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        v.push_back(make_shared<int>(i));
    }

    auto t_start = Clock::now();

    for (std::size_t i = 0; i < n; ++i) {
        int* new_val = new int(i);
        v[i].reset(new_val);
    }

    return get_ms(t_start);
}

void run_benchmarks() {
    for (std::size_t n : {1000, 1000000}) {
        std::printf("\n========== N = %zu ==========\n\n", n);
        std::printf("%-18s %10s %10s %10s\n", "Operation", "Raw", "std::", "My");
        std::printf("------------------------------------------------\n");

        std::printf("%-18s %10.3f %10.3f %10.3f\n", "unique create",
                    unique_raw_create(n), unique_std_create(n), unique_my_create(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "unique move",
                    "-", unique_std_move(n), unique_my_move(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "unique reset",
                    "-", unique_std_reset(n), unique_my_reset(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "shared create",
                    "-", shared_std_create(n), shared_my_create(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "shared copy",
                    "-", shared_std_copy(n), shared_my_copy(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "shared move",
                    "-", shared_std_move(n), shared_my_move(n));

        std::printf("%-18s %10s %10.3f %10.3f\n", "shared reset",
                    "-", shared_std_reset(n), shared_my_reset(n));
    }
}