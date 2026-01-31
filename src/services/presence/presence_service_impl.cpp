#include "services/presence_service_impl.h"
#include "common/logger.h"

namespace ourchat {

grpc::Status PresenceServiceImpl::SetOnline(grpc::ServerContext* context,
                                             const im::SetOnlineRequest* request,
                                             im::SetOnlineResponse* response) {
    LOG_INFO("SetOnline: user_id=" + std::to_string(request->user_id()));
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status PresenceServiceImpl::GetOnlineStatus(grpc::ServerContext* context,
                                                    const im::GetOnlineStatusRequest* request,
                                                    im::GetOnlineStatusResponse* response) {
    LOG_INFO("GetOnlineStatus: user_count=" + std::to_string(request->user_ids().size()));
    return grpc::Status::OK;
}

}
