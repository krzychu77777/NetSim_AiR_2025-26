#include "gtest/gtest.h"
#include "nodes.hxx"
#include "storage_types.hxx"
#include "package.hxx"

int main(int argc, char** argv) {
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();

    
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    printf("Buffer has value: %d\n", buffer.has_value() ? 1 : 0);

    //ASSERT_TRUE(buffer.has_value());
    //EXPECT_EQ(buffer.value().get_id(), 1);
    
}
