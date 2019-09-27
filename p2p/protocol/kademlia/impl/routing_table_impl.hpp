/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LIBP2P_KAD_ROUTING_TABLE_IMPL_HPP
#define LIBP2P_KAD_ROUTING_TABLE_IMPL_HPP

#include <deque>

#include <boost/assert.hpp>
#include "event/bus.hpp"
#include "peer/identity_manager.hpp"
#include "protocol/kademlia/bucket.hpp"
#include "protocol/kademlia/node_id.hpp"
#include "protocol/kademlia/routing_table.hpp"

namespace libp2p::protocol::kademlia {

  class RoutingTableImpl : public RoutingTable {
   public:
    ~RoutingTableImpl() override = default;

    enum class Error {
      SUCCESS = 0,
      PEER_REJECTED_HIGH_LATENCY = 1,
      PEER_REJECTED_NO_CAPACITY
    };

    RoutingTableImpl(std::shared_ptr<peer::IdentityManager> idmgr,
                     std::shared_ptr<event::Bus> bus,
                     RoutingTable::Config config = {});

    outcome::result<void> update(const peer::PeerId &pid) override;

    auto& getBuckets() {
      return buckets_;
    }

    void remove(const peer::PeerId &id) override;

    PeerIdVec getAllPeers() const override;

    PeerIdVec getNearestPeers(const NodeId &id, size_t count) override;

    size_t size() const override;

   private:
    std::vector<Bucket> buckets_;

    std::shared_ptr<peer::IdentityManager> idmgr_;
    NodeId local_;
    std::shared_ptr<event::Bus> bus_;
    size_t bucket_size_;

    void nextBucket();
  };

}  // namespace libp2p::protocol::kademlia

OUTCOME_HPP_DECLARE_ERROR(libp2p::protocol::kademlia, RoutingTableImpl::Error);

#endif  // LIBP2P_KAD_ROUTING_TABLE_IMPL_HPP