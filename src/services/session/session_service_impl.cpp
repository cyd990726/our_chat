#include "services/session_service_impl.h"
#include "common/logger.h"

namespace ourchat {

grpc::Status SessionServiceImpl::GetFriends(grpc::ServerContext* context,
                                             const im::GetFriendsRequest* request,
                                             im::GetFriendsResponse* response) {
    LOG_INFO("GetFriends: user_id=" + std::to_string(request->user_id()));
    return grpc::Status::OK;
}

grpc::Status SessionServiceImpl::AddFriend(grpc::ServerContext* context,
                                            const im::AddFriendRequest* request,
                                            im::AddFriendResponse* response) {
    LOG_INFO("AddFriend: user_id=" + std::to_string(request->user_id()));
    response->set_success(true);
    return grpc::Status::OK;
}

}
