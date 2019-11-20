// Copyright (c) 2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <interfaces/handler.h>

#include <util/memory.h>

#include <boost/signals2/connection.hpp>
#include <utility>

/**
 * hzx about boost::signals2::connection
 *  The signals2::connection class represents 
 * a connection between a Signal and a Slot. 
 * It is a lightweight object that has the ability 
 * to query whether the signal and slot are currently 
 * connected, and to disconnect the signal and slot. 
 * It is always safe to query or disconnect a connection.
 * 
 * 
 * The scoped_connection class is not copyable. 
 * It may only be copy constructed from an unscoped 
 * connection object.
 * 
*/

namespace interfaces {
namespace {

class HandlerImpl : public Handler
{
public:
    explicit HandlerImpl(boost::signals2::connection connection) : m_connection(std::move(connection)) {}

    void disconnect() override { m_connection.disconnect(); }

    boost::signals2::scoped_connection m_connection;
};

} // namespace

std::unique_ptr<Handler> MakeHandler(boost::signals2::connection connection)
{
    return MakeUnique<HandlerImpl>(std::move(connection));
}

} // namespace interfaces
