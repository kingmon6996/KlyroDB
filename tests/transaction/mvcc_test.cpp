#include <gtest/gtest.h>
#include "klyro/transaction/transaction_manager.hpp"
#include "klyro/transaction/visibility.hpp"

using namespace klyro::transaction;

TEST(MvccTest, BasicTransactionLifecycle) {
    TransactionManager tm;
    auto* txn = tm.begin(IsolationLevel::ReadCommitted);
    ASSERT_NE(txn, nullptr);
    EXPECT_EQ(txn->get_state(), TransactionState::Active);
    
    EXPECT_EQ(tm.commit(txn), klyro::core::Status::Success);
    EXPECT_EQ(txn->get_state(), TransactionState::Committed);
}

TEST(MvccTest, VisibilityManagerOwnWrites) {
    TransactionManager tm;
    VisibilityManager vm(tm.get_registry());
    
    auto* txn = tm.begin();
    Snapshot snap = tm.create_snapshot(txn);
    
    MVCCHeader hdr;
    hdr.xmin = txn->get_id(); // Own insert
    
    EXPECT_TRUE(vm.is_visible(hdr, snap, txn->get_id()));
    
    hdr.xmax = txn->get_id(); // Own delete
    EXPECT_FALSE(vm.is_visible(hdr, snap, txn->get_id())); // Now deleted
}

TEST(MvccTest, VisibilityManagerUncommittedWrites) {
    TransactionManager tm;
    VisibilityManager vm(tm.get_registry());
    
    auto* txn1 = tm.begin();
    auto* txn2 = tm.begin();
    
    Snapshot snap2 = tm.create_snapshot(txn2);
    
    MVCCHeader hdr;
    hdr.xmin = txn1->get_id(); // Written by active txn1
    
    EXPECT_FALSE(vm.is_visible(hdr, snap2, txn2->get_id())); // Not visible to txn2
}

TEST(MvccTest, VisibilityManagerCommittedWrites) {
    TransactionManager tm;
    VisibilityManager vm(tm.get_registry());
    
    auto* txn1 = tm.begin();
    MVCCHeader hdr;
    hdr.xmin = txn1->get_id();
    tm.commit(txn1);
    
    auto* txn2 = tm.begin(); // Starts AFTER txn1 commits
    Snapshot snap2 = tm.create_snapshot(txn2);
    
    EXPECT_TRUE(vm.is_visible(hdr, snap2, txn2->get_id()));
}
