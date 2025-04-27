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
        users = {User(kEmptyUserId, 100, "Alice", "Smith", "X", false),
                 User(kEmptyUserId, 101, "Bob", "Jones", "", true)};
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
    auto result = repo->tryGetUserByTelegramId(100);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().first_name, "Alice");

    auto fail = repo->tryGetUserByTelegramId(123);
    EXPECT_FALSE(fail.has_value());
}

TEST_F(SqliteRepositoryTest, CanAddAndGetList) {
    List mylist(kEmptyListId, "testlist");
    auto add_res = repo->tryAddList(mylist);
    ASSERT_TRUE(add_res.has_value());

    auto get_res = repo->tryGetList("testlist");
    ASSERT_TRUE(get_res.has_value());
    EXPECT_EQ(get_res->name, "testlist");
}

TEST_F(SqliteRepositoryTest, GetAllListsEmptyInitially) {
    auto lists = repo->tryGetAllLists();
    ASSERT_FALSE(lists.has_value());
}
