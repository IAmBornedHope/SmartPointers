#include <gtest/gtest.h>
#include "../unique_ptr/UniquePtr.hpp"

TEST(unique_ptr_constructor, default_constructor) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(unique_ptr_constructor, constructor_from_pointer) {
    UniquePtr<int> ptr(new int(42));
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(unique_ptr_constructor, constructor_from_pointer_nullptr) {
    UniquePtr<int> ptr(nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(unique_ptr_constructor, move_constructor) {
    UniquePtr<int> source(new int(7));
    int* raw = source.get();

    UniquePtr<int> moved(std::move(source));

    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(moved.get(), raw);
    EXPECT_EQ(*moved, 7);
}






TEST(unique_ptr_assignment, move_assignment) {
    UniquePtr<int> a(new int(1));
    UniquePtr<int> b(new int(2));

    int* raw_a = a.get();
    b = std::move(a);

    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(b.get(), raw_a);
    EXPECT_EQ(*b, 1);
}

TEST(unique_ptr_assignment, self_move_assignment) {
    UniquePtr<int> a(new int(5));
    int* raw = a.get();

    a = std::move(a);

    EXPECT_EQ(a.get(), raw);
    EXPECT_EQ(*a, 5);
}






TEST(unique_ptr_release, release_returns_pointer) {
    UniquePtr<int> ptr(new int(10));
    int* raw = ptr.get();

    int* released = ptr.release();

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(released, raw);
    delete released;
}

TEST(unique_ptr_release, release_on_empty) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.release(), nullptr);
}

TEST(unique_ptr_reset, reset_with_new_pointer) {
    UniquePtr<int> ptr(new int(1));
    ptr.reset(new int(2));

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 2);
}

TEST(unique_ptr_reset, reset_with_nullptr) {
    UniquePtr<int> ptr(new int(1));
    ptr.reset();

    EXPECT_EQ(ptr.get(), nullptr);
}

TEST(unique_ptr_reset, reset_with_same_pointer) {
    UniquePtr<int> ptr(new int(3));
    int* raw = ptr.get();

    ptr.reset(raw);

    EXPECT_EQ(ptr.get(), raw);
    EXPECT_EQ(*ptr, 3);
}

TEST(unique_ptr_reset, reset_on_empty) {
    UniquePtr<int> ptr;
    ptr.reset(new int(99));

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 99);
}






TEST(unique_ptr_access, dereference) {
    UniquePtr<int> ptr(new int(13));
    EXPECT_EQ(*ptr, 13);
}

TEST(unique_ptr_access, arrow_operator) {
    UniquePtr<std::string> ptr(new std::string("hello"));
    EXPECT_EQ(ptr->size(), 5);
    EXPECT_EQ(*ptr, "hello");
}






TEST(unique_ptr_array, index_operator) {
    UniquePtr<int[]> ptr(new int[3]{1, 2, 3});

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
}

TEST(unique_ptr_array, move_constructor_array) {
    UniquePtr<int[]> source(new int[2]{5, 6});
    int* raw = source.get();

    UniquePtr<int[]> moved(std::move(source));

    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(moved.get(), raw);
    EXPECT_EQ(moved[0], 5);
    EXPECT_EQ(moved[1], 6);
}






TEST(make_unique_test, single_object) {
    auto ptr = make_unique<int>(42);

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(make_unique_test, single_object_string) {
    auto ptr = make_unique<std::string>("MEPHI");

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, "MEPHI");
}

TEST(make_unique_test, array_zero_size) {
    auto ptr = make_unique<int[]>(0);
    EXPECT_NE(ptr.get(), nullptr);
}

TEST(make_unique_test, array_filled_with_zeros) {
    auto ptr = make_unique<int[]>(5);

    ASSERT_NE(ptr.get(), nullptr);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(ptr[i], 0);
    }
}

TEST(make_unique_test, array_custom_size) {
    auto ptr = make_unique<int[]>(3);
    ptr[0] = 10;
    ptr[1] = 20;
    ptr[2] = 30;

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}