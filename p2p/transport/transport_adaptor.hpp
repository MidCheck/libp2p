/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LIBP2P_TRANSPORT_ADAPTOR_HPP
#define LIBP2P_TRANSPORT_ADAPTOR_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <system_error>

#include <outcome/outcome.hpp>  // for outcome::result
#include "basic/adaptor.hpp"
#include "connection/capable_connection.hpp"
#include "event/emitter.hpp"
#include "multi/multiaddress.hpp"
#include "peer/peer_id.hpp"
#include "transport/transport_listener.hpp"

namespace libp2p::transport {

  /**
   * Allows to establish connections with other peers and react to received
   * attempts to do so; can be implemented, for example, as TCP, UDP etc
   */
 class TransportAdaptor : public basic::Adaptor {
   public:
    using ConnectionCallback =
        void(outcome::result<std::shared_ptr<connection::CapableConnection>>);
    using HandlerFunc = std::function<ConnectionCallback>;

    ~TransportAdaptor() override = default;

    /**
     * Try to establish connection with a peer
     * @param remoteId id of remote peer to dial
     * @param address of the peer
     * @param handler callback that will be executed on connection/error
     * @return connection in case of success, error otherwise
     */
    virtual void dial(const peer::PeerId &remoteId, multi::Multiaddress address,
                      HandlerFunc handler) = 0;

    /**
     * Create a listener for incoming connections of this Transport; in case
     * it was already created, return it
     * @param handler callback that will be executed on new connection
     * @return pointer to the created listener
     */
    virtual std::shared_ptr<TransportListener> createListener(
        TransportListener::HandlerFunc handler) = 0;

    /**
     * Check if this transport supports a given multiaddress
     * @param ma to be checked against
     * @return true, if transport supports that multiaddress, false otherwise
     * @note example: '/tcp/...' on tcp transport will return true
     */
    virtual bool canDial(const multi::Multiaddress &ma) const = 0;
  };
}  // namespace libp2p::transport

#endif  //LIBP2P_TRANSPORT_ADAPTOR_HPP