#pragma once
#include "session.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class SessionService {
public:
    virtual ~SessionService() = default;

    virtual grpc::Status GetFriends(grpc::ServerContext* context,
                                    const GetFriendsRequest* request,
                                    GetFriendsResponse* response) = 0;

    virtual grpc::Status AddFriend(grpc::ServerContext* context,
                                   const AddFriendRequest* request,
                                   AddFriendResponse* response) = 0;
};

} // namespace im
