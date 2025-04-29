#include <SQLiteCpp/SQLiteCpp.h>
#include <gtest/gtest.h>

#include <memory>

#include "list.h"
#include "sqlite_repository.h"
#include "user.h"

std::unique_ptr<SQLite::Database> createInMemoryDb() {
    return std::make_unique<SQLite::Database>(
        ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}

class SqliteRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override {
        users = {User(100, "Alice", "Smith", "X", false),
                 User(101, "Bob", "Jones", "", true)};
        db = createInMemoryDb();
        repo = std::make_unique<SqliteRepository>(std::move(db), users);
        ASSERT_TRUE(repo->init());
    }

    std::vector<User> users;
    std::unique_ptr<SqliteRepository> repo;
    std::unique_ptr<SQLite::Database> db;
};

TEST_F(SqliteRepositoryTest, CanGetAllUsers) {
    auto result = repo->tryGetAllUsers();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), users.size());
}

TEST_F(SqliteRepositoryTest, CanGetUserByTelegramId) {
    auto result = repo->tryGetUser(100);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().first_name, "Alice");

    auto fail = repo->tryGetUser(123);
    EXPECT_FALSE(fail.has_value());
}

TEST_F(SqliteRepositoryTest, CanAddAndGetList) {
    List mylist("testlist");
    auto add_res = repo->tryAddList(mylist);
    ASSERT_TRUE(add_res.has_value());

    auto get_res = repo->tryGetList("testlist");
    ASSERT_TRUE(get_res.has_value());
    EXPECT_EQ(get_res->name, "testlist");
}

TEST_F(SqliteRepositoryTest, CanRemoveList) {
    List mylist("testlist");
    auto add_res = repo->tryAddList(mylist);
    ASSERT_TRUE(add_res.has_value());

    auto delete_res = repo->tryRemoveList("testlist");
    ASSERT_TRUE(delete_res.has_value());

    auto get_res = repo->tryGetList("testlist");
    ASSERT_FALSE(get_res.has_value());
}

TEST_F(SqliteRepositoryTest, GetAllListsEmptyInitially) {
    auto lists = repo->tryGetAllLists();
    ASSERT_FALSE(lists.has_value());
}

TEST_F(SqliteRepositoryTest, SwapUsers) {
    List mylist("testlist");
    auto add_res = repo->tryAddList(mylist);

    auto result = repo->trySwapUsers("testlist", 100, 101);

    ASSERT_TRUE(result.has_value());

    auto user1 = repo->tryGetUser(100);
    auto user2 = repo->tryGetUser(101);

    auto list_user_1 = repo->tryGetListUser(mylist.list_id, user1.value().user_id);
    auto list_user_2 = repo->tryGetListUser(mylist.list_id, user2.value().user_id);

    ASSERT_TRUE(list_user_1.has_value());
    ASSERT_TRUE(list_user_2.has_value());

    ASSERT_EQ(list_user_1.value().list_user_order, 2);
    ASSERT_EQ(list_user_2.value().list_user_order, 1);
}
