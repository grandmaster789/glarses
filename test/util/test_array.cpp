#include "../unittest.h"
#include "util/array.h"
#include "util/algorithm.h"

namespace test {
    TEST_CASE("push_pop", "util::Array") {
        using namespace glarses;

        util::Mallocator allocator;

        Array<int> arr(&allocator);

        REQUIRE(arr.capacity()          == 0);
        REQUIRE(arr.num_elements()      == 0);
        REQUIRE(arr.capacity_in_bytes() == 0);
        REQUIRE(arr.size_in_bytes()     == 0);

        arr.push(123);

        REQUIRE(arr.capacity()          >  0);
        REQUIRE(arr.num_elements()      == 1);
        REQUIRE(arr.capacity_in_bytes() >  0);
        REQUIRE(arr.size_in_bytes()     == sizeof(int));

        {
            int x = 0;
            REQUIRE(arr.pop(x));
            REQUIRE(x == 123);
        }

        arr
            .push(1)
            .push(2)
            .push(3);

        REQUIRE(arr[0] == 1);
        REQUIRE(arr[2] == 3);

        arr.swap_delete(0);

        REQUIRE(arr.num_elements() == 2);
        REQUIRE(arr.front() == 3);
        REQUIRE(arr.back() == 2);

        arr.clear();
        arr.push(1, 2, 3);

        REQUIRE(arr[0] == 1);
        REQUIRE(arr[2] == 3);
    }

    TEST_CASE("stl_range", "util::Array") {
        using namespace glarses;

        util::Mallocator allocator;

        Array<int> arr(&allocator);

        arr.push(1, 2, 3);

        int sum = 0;
        for (const auto& value: arr)
            sum += value;

        REQUIRE(sum == 1 + 2 + 3);

        arr.push(1, 2, 3);

        sort(arr);

        {
            Array<int> expected(&allocator);
            expected.push(1, 1, 2, 2, 3, 3);

            REQUIRE(arr == expected);
        }

        arr.stable_delete(2);
        arr.stable_delete(2);

        {
            Array<int> expected(&allocator);
            expected.push(1, 1, 3, 3);

            REQUIRE(arr == expected);
        }
    }

    TEST_CASE("array_transfer, pop_n", "util::Array") {
        using namespace glarses;

        util::Mallocator allocator;

        {
            Array<int> arr(&allocator);

            arr.push(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

            arr.pop_n(5);

            int sum = 0;
            for (const auto& value: arr)
                sum += value;

            REQUIRE(sum == 1 + 2 + 3 + 4 + 5);
        }

        {
            Array<int> arr(&allocator);
            Array<int> brr(&allocator);

            arr.push(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

            auto transferred = arr.pop_n(5, brr);

            REQUIRE(transferred == 5);
            REQUIRE(arr.num_elements() == 5);

            int sum = 0;
            for (const auto& value: brr)
                sum += value;

            REQUIRE(sum == 6 + 7 + 8 + 9 + 10);
        }

        {
            Array<int> arr(&allocator);
            Array<int> brr(&allocator);

            arr.push(1, 2, 3, 4);

            auto transferred = arr.pop_n(5, brr);

            REQUIRE(transferred == brr.num_elements());
            REQUIRE(arr.num_elements() == 0);

            int sum = 0;
            for (const auto& value: brr)
                sum += value;

            REQUIRE(sum == 1 + 2 + 3 + 4);
        }
    }

    TEST_CASE("view", "util::Array") {
        using namespace glarses;

        util::Mallocator allocator;

        Array<int> arr(&allocator);

        arr.push(1, 2, 3, 4, 5, 6);

        ArrayView<int> view(&arr[2], 4);

        REQUIRE(view[2] == 5);

        int sum = 0;
        for (const auto& value: view)
            sum += value;

        REQUIRE(sum == 3 + 4 + 5 + 6);
    }
}