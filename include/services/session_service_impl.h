#ifndef SESSION_SERVICE_IMPL_H
#define SESSION_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include "session.grpc.pb.h"

namespace ourchat {

class SessionServiceImpl : public im::SessionService, public grpc::Service {
public:
    grpc::Status GetFriends(grpc::ServerContext* context,
                             const im::GetFriendsRequest* request,
                             im::GetFriendsResponse* response) override;
    
    grpc::Status AddFriend(grpc::ServerContext* context,
                           const im::AddFriendRequest* request,
                           im::AddFriendResponse* response) override;
};

}

#endif
