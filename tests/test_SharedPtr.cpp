#include <gtest/gtest.h>
#include "../shared_ptr/SharedPtr.hpp"


TEST(shared_ptr_constructor, default_constructor) {
    SharedPtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.get_counter(), 0);
}

TEST(shared_ptr_constructor, nullptr_constructor) {
    SharedPtr<int> ptr(nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.get_counter(), 0);
}

TEST(shared_ptr_constructor, constructor_from_pointer) {
    SharedPtr<int> ptr(new int(42));
    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(ptr.get_counter(), 1);
}

TEST(shared_ptr_constructor, constructor_from_pointer_nullptr) {
    SharedPtr<int> ptr(static_cast<int*>(nullptr));
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.get_counter(), 0);
}






TEST(shared_ptr_copy, copy_constructor_increments_counter) {
    SharedPtr<int> source(new int(7));
    ASSERT_EQ(source.get_counter(), 1);

    SharedPtr<int> copy(source);

    EXPECT_EQ(source.get(), copy.get());
    EXPECT_EQ(source.get_counter(), 2);
    EXPECT_EQ(copy.get_counter(), 2);
    EXPECT_EQ(*copy, 7);
}

TEST(shared_ptr_copy, copy_assignment_increments_counter) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(new int(2));
    ASSERT_EQ(a.get_counter(), 1);
    ASSERT_EQ(b.get_counter(), 1);

    b = a;

    EXPECT_EQ(a.get(), b.get());
    EXPECT_EQ(a.get_counter(), 2);
    EXPECT_EQ(b.get_counter(), 2);
    EXPECT_EQ(*b, 1);
}

TEST(shared_ptr_copy, self_copy_assignment) {
    SharedPtr<int> a(new int(5));
    int* raw = a.get();

    a = a;

    EXPECT_EQ(a.get(), raw);
    EXPECT_EQ(a.get_counter(), 1);
    EXPECT_EQ(*a, 5);
}

TEST(shared_ptr_copy, copy_of_empty) {
    SharedPtr<int> empty;
    SharedPtr<int> copy(empty);

    EXPECT_EQ(copy.get(), nullptr);
    EXPECT_EQ(copy.get_counter(), 0);
}

TEST(shared_ptr_copy, assign_empty_to_non_empty) {
    SharedPtr<int> a(new int(3));
    SharedPtr<int> empty;

    a = empty;

    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.get_counter(), 0);
}






TEST(shared_ptr_move, move_constructor) {
    SharedPtr<int> source(new int(9));
    int* raw = source.get();

    SharedPtr<int> moved(std::move(source));

    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(source.get_counter(), 0);
    EXPECT_EQ(moved.get(), raw);
    EXPECT_EQ(moved.get_counter(), 1);
    EXPECT_EQ(*moved, 9);
}

TEST(shared_ptr_move, move_assignment) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(new int(2));

    int* raw_a = a.get();
    b = std::move(a);

    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(a.get_counter(), 0);
    EXPECT_EQ(b.get(), raw_a);
    EXPECT_EQ(b.get_counter(), 1);
    EXPECT_EQ(*b, 1);
}

TEST(shared_ptr_move, self_move_assignment) {
    SharedPtr<int> a(new int(4));
    int* raw = a.get();

    a = std::move(a);

    EXPECT_EQ(a.get(), raw);
    EXPECT_EQ(a.get_counter(), 1);
    EXPECT_EQ(*a, 4);
}

TEST(shared_ptr_move, move_constructor_from_empty) {
    SharedPtr<int> source;
    SharedPtr<int> moved(std::move(source));

    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(moved.get(), nullptr);
}

TEST(shared_ptr_move, move_assignment_empty_to_non_empty) {
    SharedPtr<int> empty;
    SharedPtr<int> b(new int(5));

    b = std::move(empty);

    EXPECT_EQ(empty.get(), nullptr);
    EXPECT_EQ(b.get(), nullptr);
}






TEST(shared_ptr_counter, counter_decrements_on_destroy) {
    SharedPtr<int> a(new int(1));
    {
        SharedPtr<int> b(a);
        EXPECT_EQ(a.get_counter(), 2);
    }
    EXPECT_EQ(a.get_counter(), 1);
}

TEST(shared_ptr_counter, counter_zero_after_last_destroyed) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(a);
    EXPECT_EQ(a.get_counter(), 2);
}

TEST(shared_ptr_counter, counter_after_move_source_empty) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(std::move(a));

    EXPECT_EQ(a.get_counter(), 0);
    EXPECT_EQ(b.get_counter(), 1);
}






TEST(shared_ptr_access, dereference) {
    SharedPtr<int> ptr(new int(13));
    EXPECT_EQ(*ptr, 13);
}

TEST(shared_ptr_access, arrow_operator) {
    SharedPtr<std::string> ptr(new std::string("hello"));
    EXPECT_EQ(ptr->size(), 5);
    EXPECT_EQ(*ptr, "hello");
}

TEST(shared_ptr_access, modify_through_dereference) {
    SharedPtr<int> ptr(new int(1));
    *ptr = 100;
    EXPECT_EQ(*ptr, 100);
}

TEST(shared_ptr_access, modify_through_arrow) {
    SharedPtr<std::string> ptr(new std::string("hello"));
    ptr->append(" world");
    EXPECT_EQ(*ptr, "hello world");
}

TEST(shared_ptr_access, shared_modification) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(a);

    *b = 42;

    EXPECT_EQ(*a, 42);
}

TEST(shared_ptr_access, bool_operator_true) {
    SharedPtr<int> ptr(new int(1));
    EXPECT_TRUE(static_cast<bool>(ptr));
    if (ptr) { SUCCEED(); } else { FAIL(); }
}

TEST(shared_ptr_access, bool_operator_false) {
    SharedPtr<int> ptr;
    EXPECT_FALSE(static_cast<bool>(ptr));
}






TEST(shared_ptr_reset, reset_to_nullptr) {
    SharedPtr<int> ptr(new int(1));
    ptr.reset();

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.get_counter(), 0);
}

TEST(shared_ptr_reset, reset_explicit_nullptr) {
    SharedPtr<int> ptr(new int(1));
    ptr.reset(nullptr);

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.get_counter(), 0);
}

TEST(shared_ptr_reset, reset_with_new_pointer) {
    SharedPtr<int> ptr(new int(1));
    ptr.reset(new int(2));

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 2);
    EXPECT_EQ(ptr.get_counter(), 1);
}

TEST(shared_ptr_reset, reset_with_same_pointer) {
    SharedPtr<int> ptr(new int(3));
    int* raw = ptr.get();

    ptr.reset(raw);

    EXPECT_EQ(ptr.get(), raw);
    EXPECT_EQ(ptr.get_counter(), 1);
    EXPECT_EQ(*ptr, 3);
}

TEST(shared_ptr_reset, reset_decrements_shared_counter) {
    SharedPtr<int> a(new int(1));
    SharedPtr<int> b(a);
    ASSERT_EQ(a.get_counter(), 2);

    b.reset();

    EXPECT_EQ(a.get_counter(), 1);
    EXPECT_EQ(a.get(), b.get() == nullptr ? a.get() : nullptr);
    EXPECT_EQ(*a, 1);
}

TEST(shared_ptr_reset, reset_releases_shared_object) {
    SharedPtr<int> a(new int(1));
    {
        SharedPtr<int> b(a);
        EXPECT_EQ(a.get_counter(), 2);
        b.reset();
        EXPECT_EQ(a.get_counter(), 1);
    }
    EXPECT_EQ(a.get_counter(), 1);
}






TEST(shared_ptr_array, index_operator) {
    SharedPtr<int[]> ptr(new int[3]{1, 2, 3});

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
}

TEST(shared_ptr_array, copy_constructor_array) {
    SharedPtr<int[]> source(new int[2]{5, 6});
    SharedPtr<int[]> copy(source);

    EXPECT_EQ(source.get(), copy.get());
    EXPECT_EQ(source.get_counter(), 2);
    EXPECT_EQ(copy[0], 5);
    EXPECT_EQ(copy[1], 6);
}

TEST(shared_ptr_array, move_constructor_array) {
    SharedPtr<int[]> source(new int[2]{7, 8});
    int* raw = source.get();

    SharedPtr<int[]> moved(std::move(source));

    EXPECT_EQ(source.get(), nullptr);
    EXPECT_EQ(moved.get(), raw);
    EXPECT_EQ(moved[0], 7);
    EXPECT_EQ(moved[1], 8);
}

TEST(shared_ptr_array, reset_array) {
    SharedPtr<int[]> ptr(new int[2]{1, 2});
    ptr.reset(new int[3]{5, 6, 7});

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr[0], 5);
    EXPECT_EQ(ptr[1], 6);
    EXPECT_EQ(ptr[2], 7);
}






TEST(make_shared_test, single_object) {
    auto ptr = make_shared<int>(42);

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(ptr.get_counter(), 1);
}

TEST(make_shared_test, single_object_string) {
    auto ptr = make_shared<std::string>("abc");

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, "abc");
}

TEST(make_shared_test, multiple_args) {
    auto ptr = make_shared<std::string>(5, 'x');

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, "xxxxx");
}

TEST(make_shared_test, lvalue_argument) {
    std::string s = "test";
    auto ptr = make_shared<std::string>(s);

    EXPECT_EQ(*ptr, "test");
    EXPECT_EQ(s, "test");
}

TEST(make_shared_test, array_zero_size) {
    auto ptr = make_shared<int[]>(0);
    EXPECT_NE(ptr.get(), nullptr);
}

TEST(make_shared_test, array_filled_with_zeros) {
    auto ptr = make_shared<int[]>(5);

    ASSERT_NE(ptr.get(), nullptr);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(ptr[i], 0);
    }
}

TEST(make_shared_test, array_custom_size) {
    auto ptr = make_shared<int[]>(3);
    ptr[0] = 10;
    ptr[1] = 20;
    ptr[2] = 30;

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}

struct Base {
    int x = 25;
};

struct Derived : Base {
    int y = 511;
};

TEST(shared_ptr_subtyping, from_derived_pointer) {
    SharedPtr<Base> ptr(new Derived());

    ASSERT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr->x, 25);
}

TEST(shared_ptr_subtyping, copy_upcast) {
    SharedPtr<Derived> derived(new Derived());
    SharedPtr<Base> base(derived);

    EXPECT_EQ(base.get(), derived.get());
    EXPECT_EQ(base.get_counter(), 2);
    EXPECT_EQ(derived.get_counter(), 2);
}

TEST(shared_ptr_subtyping, move_upcast) {
    SharedPtr<Derived> derived(new Derived());
    Base* raw = derived.get();

    SharedPtr<Base> base(std::move(derived));

    EXPECT_EQ(derived.get(), nullptr);
    EXPECT_EQ(derived.get_counter(), 0);
    EXPECT_EQ(base.get(), raw);
    EXPECT_EQ(base.get_counter(), 1);
}